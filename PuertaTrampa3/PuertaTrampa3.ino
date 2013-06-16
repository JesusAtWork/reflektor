#include <AccelStepper.h>
#include <Stepper595.h>

//#define DEBUG

// delay antes de apagar el motor [./]
// frenar en seco cuando se suelta el boton 
// debounce del boton de seleccion de espejo
// maquina de estado para fin de juego [./]
#define FIN_CARRERA_PIN    3
#define BOTON_1            4
#define BOTON_2            5
#define BOTON_3            6
#define SENSOR0           A5
#define SENSOR1           A4
#define LED_ESPEJO0       10
#define LED_GANASTE       13

#define SER_Pin            8   //pin 14 on the 75HC595   
#define RCLK_Pin           9   //pin 12 on the 75HC595   
#define SRCLK_Pin          7   //pin 11 on the 75HC595   

#define PUERTA_CERRADA  1400
#define PUERTA_ABIERTA     0

#define POCA_LUZ         180
#define MUCHA_LUZ        250

Stepper stepper(4, 4, 6, 5, 7);
Stepper stepper2(4, 0, 1, 2, 3);
Stepper stepper3(4, 8, 9, 10, 11);
Stepper stepper4(4, 12, 13, 14, 15);

class State {
  public:
    virtual void setup() = 0;
    virtual void loop() {}
};

State *current_state;
void change_state (State &new_state); // stupidest (but needed!) forward declaration ever

void change_state (State &new_state) {
  current_state = &new_state;
  current_state->setup();
}

void steppers_go () {
    unsigned long now = millis();
    stepper.go(now);
    stepper2.go(now);
    stepper3.go(now);
    stepper4.go(now);
    writeRegisters();
}

boolean inline fin_de_carrera_activado() {
  return digitalRead(FIN_CARRERA_PIN) == HIGH;
}

boolean abriendo_puerta = false;
boolean puerta_cerrada = true;
boolean estado_boton1 = false;
boolean estado_boton2 = false;
boolean estado_boton3 = false;
int espejo_activo = 0;
int count = 0;

void mover_espejo(int espejo, int direccion) {
    if (espejo == 0) {
        stepper2.move(direccion);
    } else if (espejo == 1) {
        stepper3.move(direccion);
    } else {
        stepper4.move(direccion);
    }
}

class PlayState : public State {
  public:
    void setup() {
        Serial.println("track:juego");
    }
    
    void loop()
    {
        
      int sensor1 = analogRead(SENSOR1); 
      
      if (abriendo_puerta) {
        if(fin_de_carrera_activado()) {
          abriendo_puerta = false;
          stepper.setMaxSpeed(100);
          stepper.setAcceleration(20);
          stepper.setCurrentPosition(PUERTA_ABIERTA);
        }
        
      } else {
  
        if (sensor1 > MUCHA_LUZ && !fin_de_carrera_activado()) {
          stepper.setMaxSpeed(5000);
          stepper.setAcceleration(1000);
          if (!abriendo_puerta) {
            Serial.println("track:puerta");
            puerta_cerrada = false;
          }
          abriendo_puerta = true;
          stepper.move(-PUERTA_CERRADA * 2);
        }
        
        if (sensor1 < POCA_LUZ) {
          stepper.moveTo(PUERTA_CERRADA);
          if (stepper.distanceToGo() < 10 && !puerta_cerrada) {
            puerta_cerrada = true;
            Serial.println("track:juego");
          } 
        }
      }
      
      int sensor2 = analogRead(SENSOR0);
      if (sensor2 > MUCHA_LUZ) {
         // ganaste!
      } else {
          setRegisterPin(LED_GANASTE, LOW);
      }  
   
      if (digitalRead(BOTON_1) == HIGH) {
          estado_boton1 = true;
      }
  
      if (digitalRead(BOTON_2) == HIGH) {
          estado_boton2 = true;
      }
  
      if (digitalRead(BOTON_3) == HIGH) {
          estado_boton3 = true;
      }
   
      if (estado_boton1) {
        Serial.println("reset");
        espejo_activo = (espejo_activo + 1) % 3;
        for (int k=0; k<3; k++) {
           digitalWrite(LED_ESPEJO0+k, espejo_activo == k ? HIGH : LOW);
        }  
      }
      
      if (estado_boton2 && !estado_boton3) {
        mover_espejo(espejo_activo, -4);
      }
  
      if (estado_boton3 && !estado_boton2) {
        mover_espejo(espejo_activo, 4);
        Serial.println("reset");
      }
      
      estado_boton1 = false;
      estado_boton2 = false;
      estado_boton3 = false;
  
      static int l = 0;
      #ifdef DEBUG
        if ((l++)%30 == 0) {
          Serial.print("debug:");
          Serial.print(sensor1);
          Serial.print(" - ");
          Serial.println(sensor2);
        }
      #endif
    }
} play_state;

class GameoverState : public State {
  public:
    void setup() {
      stepper.stop();
      stepper2.stop();
      stepper3.stop();
      stepper4.stop();
      digitalWrite(LED_GANASTE, HIGH);
    }
} gameover_state;

class SetupState : public State {
    enum {ABRIENDO, CERRANDO} estado_puerta;

    void ir_al_fin_carrera() {
      estado_puerta = ABRIENDO;
      stepper.setCurrentPosition(PUERTA_CERRADA + 800);
      stepper2.setCurrentPosition(200);
      stepper3.setCurrentPosition(200);
      stepper4.setCurrentPosition(200);
      stepper.moveTo(PUERTA_ABIERTA);
      stepper2.moveTo(0);
      stepper3.moveTo(0);
      stepper4.moveTo(0);
    }

    void cerrar_puerta() {    
      estado_puerta = CERRANDO;
      stepper.setCurrentPosition(PUERTA_ABIERTA);
      stepper2.setCurrentPosition(0);
      stepper3.setCurrentPosition(0);
      stepper4.setCurrentPosition(0);
      stepper.moveTo(PUERTA_CERRADA);
      stepper2.moveTo(50);
      stepper3.moveTo(50);
      stepper4.moveTo(50);
    }
    
  public:
    void setup() {
      ir_al_fin_carrera();
    }
    void loop() {
      if (estado_puerta == ABRIENDO) {
        if (fin_de_carrera_activado() || stepper.distanceToGo() == 0) {
          cerrar_puerta();
        }
      } else { // estado_puerta == CERRANDO
        if (stepper.distanceToGo() == 0) {
          change_state(play_state);
        }
      }
    }
} setup_state;



void setup() {
    Serial.begin(115200);
    Serial.println("-------------------------");
    Serial.println("reset");
    
    pinMode(SER_Pin, OUTPUT);
    pinMode(RCLK_Pin, OUTPUT);
    pinMode(SRCLK_Pin, OUTPUT);
    pinMode(LED_ESPEJO0 + 0, OUTPUT);
    pinMode(LED_ESPEJO0 + 1, OUTPUT);
    pinMode(LED_ESPEJO0 + 2, OUTPUT);
    pinMode(LED_GANASTE, OUTPUT);

    pinMode(FIN_CARRERA_PIN, INPUT);
    pinMode(BOTON_1, INPUT);
    pinMode(BOTON_2, INPUT);
    pinMode(BOTON_3, INPUT);
    
    clearRegisters();
    writeRegisters();

    stepper.setMaxSpeed(5000);
    stepper.setAcceleration(500);

    stepper2.setMaxSpeed(50);
    stepper2.setAcceleration(30);

    stepper3.setMaxSpeed(50);
    stepper3.setAcceleration(30);

    stepper4.setMaxSpeed(50);
    stepper4.setAcceleration(30);
  
    change_state (setup_state);
}

void loop() {
    current_state->loop();
    steppers_go();
}
