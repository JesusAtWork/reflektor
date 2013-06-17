#include <AccelStepper.h>
#include <Stepper595.h>
#include "variables.h"

// PENDIENTES:
// demorar un ratito en ganar [  ]
// usar variables en todos lados [./]

#define DEBUG_PIN          2

#define FIN_CARRERA_PIN    3
#define BOTON_CAMBIO       4
#define BOTON_IZQUIERDO    5
#define BOTON_DERECHO      6

#define SER_Pin            8   //pin 14 on the 75HC595   
#define RCLK_Pin           9   //pin 12 on the 75HC595   
#define SRCLK_Pin          7   //pin 11 on the 75HC595
#define LED_ESPEJO1       10   // 11 y 12 son los otros espejos
#define LED_GANASTE       13

#define SENSOR0           A5
#define SENSOR1           A4

Stepper carrito(4, 4, 6, 5, 7);
Stepper espejo1(4, 0, 1, 2, 3);
Stepper espejo2(4, 8, 9, 10, 11);
Stepper espejo3(4, 12, 13, 14, 15);

class State {
public:
    virtual void setup() = 0;
    virtual char* name() = 0;
    virtual void loop() {}
};

State *current_state;
void change_state (State &new_state);  // stupidest (but needed!) forward declaration ever

void change_state (State &new_state) {
#ifdef DEBUG
    Serial.println(new_state.name());
    while (digitalRead(DEBUG_PIN) == LOW) {
      delay(50);
    }
#endif
    current_state = &new_state;
    current_state->setup();
}

void steppers_go () {
    unsigned long now = millis();
    carrito.go(now);
    espejo1.go(now);
    espejo2.go(now);
    espejo3.go(now);
    writeRegisters();
}

boolean inline fin_de_carrera_activado() {
    return digitalRead(FIN_CARRERA_PIN) == HIGH;
}

typedef enum {ABRIENDO, CERRANDO, CERRADA} EstadosPuerta;
EstadosPuerta estado_puerta;
int espejo_activo;

class GameoverState : public State {
public:
    void setup() {
        carrito.stop();
        espejo1.stop();
        espejo2.stop();
        espejo3.stop();
        digitalWrite(LED_GANASTE, HIGH);
    }
    char* name() {
      return "Game Over";
    }
#ifdef DEBUG
    void loop();
#endif

} gameover_state;


class PlayState : public State {
    void verificar_sensor1() {
        int sensor1 = analogRead(SENSOR1);
        if (estado_puerta == ABRIENDO) {
            if(fin_de_carrera_activado()) {
                estado_puerta = CERRANDO;
                carrito.setMaxSpeed(VELOCIDAD_CERRANDO);
                carrito.setAcceleration(ACELERACION_CERRANDO);
                carrito.setCurrentPosition(PUERTA_ABIERTA);
            }
        } else {
            if (sensor1 < POCA_LUZ) {
                carrito.moveTo(PUERTA_CERRADA);
            }

            if (sensor1 > MUCHA_LUZ && !fin_de_carrera_activado()) {
                if (estado_puerta == CERRADA) {
                    Serial.println("track:puerta");
                }
                estado_puerta = ABRIENDO;
                carrito.setMaxSpeed(VELOCIDAD_ABRIENDO);
                carrito.setAcceleration(ACELERACION_ABRIENDO);
                carrito.move(-PUERTA_CERRADA * 2);
            }

            if (estado_puerta == CERRANDO && carrito.distanceToGo() == 0 ) {
                estado_puerta = CERRADA;
                Serial.println("track:juego");
            }
        }
    }

    void verificar_sensor2() {
        int sensor2 = analogRead(SENSOR0);
        if (sensor2 > MUCHA_LUZ) {
            // ganaste!
            Serial.println("track:gameover");
            change_state (gameover_state);
        }
    }

    void mover_espejo(int espejo, int direccion) {
        if (espejo == 0) {
            espejo1.move(direccion);
        } else if (espejo == 1) {
            espejo2.move(direccion);
        } else {
            espejo3.move(direccion);
        }
    }

    void procesar_botones_izquierda_derecha() {
        boolean izquierdo = digitalRead(BOTON_IZQUIERDO) == HIGH;
        boolean derecho = digitalRead(BOTON_DERECHO) == HIGH;

        if (izquierdo == true && derecho == false) {
            mover_espejo(espejo_activo, -MOVIMIENTO_ESPEJO);
        }

        if (derecho == true && izquierdo == false) {
            mover_espejo(espejo_activo, MOVIMIENTO_ESPEJO);
        }

        if (izquierdo == false && derecho == false) {
            mover_espejo(espejo_activo, 0);
        }
    }

    void mostrar_espejo_activo () {
        for (int k=0; k<3; k++) {
            digitalWrite(LED_ESPEJO1+k, espejo_activo == k ? HIGH : LOW);
        }
    }

    void procesar_boton_cambio_espejo() {
        static boolean estado_anterior = false;
        boolean estado_actual = digitalRead(BOTON_CAMBIO) == HIGH;

        if ( estado_actual == true && estado_anterior == false) {
            Serial.println("reset");
            espejo_activo = (espejo_activo + 1) % 3;
            mostrar_espejo_activo();
        }
        estado_anterior = estado_actual;
    }

    void procesar_botonera() {
        procesar_botones_izquierda_derecha();
        procesar_boton_cambio_espejo();
    }

public:
    void setup() {
        espejo_activo = 0;
        mostrar_espejo_activo();
        digitalWrite(LED_GANASTE, LOW);
        Serial.println("track:juego");
    }

    char* name() {
      return "Running Game";
    }

    void loop()
    {
        verificar_sensor2();
        verificar_sensor1();
        procesar_botonera();

#ifdef DEBUG
        if (digitalRead(DEBUG_PIN) == LOW) {
            change_state(gameover_state);    
        }
/*
        static int l = 0;
        if ((l++)%30 == 0) {
            Serial.print("debug:");
            //Serial.print(sensor1);
            Serial.print(" - ");
            //Serial.println(sensor2);
        }
*/
#endif
    }
} play_state;

class SetupState : public State {

    void ir_al_fin_carrera() {
        estado_puerta = ABRIENDO;
        carrito.setCurrentPosition(PUERTA_CERRADA + 800);
        espejo1.setCurrentPosition(MOVIMIENTO_ESPEJO * 2);
        espejo2.setCurrentPosition(MOVIMIENTO_ESPEJO * 2);
        espejo3.setCurrentPosition(MOVIMIENTO_ESPEJO * 2);
        carrito.moveTo(PUERTA_ABIERTA);
        espejo1.moveTo(0);
        espejo2.moveTo(0);
        espejo3.moveTo(0);
    }

    void cerrar_puerta() {
        estado_puerta = CERRANDO;
        carrito.setCurrentPosition(PUERTA_ABIERTA);
        espejo1.setCurrentPosition(0);
        espejo2.setCurrentPosition(0);
        espejo3.setCurrentPosition(0);
        carrito.moveTo(PUERTA_CERRADA);
        espejo1.moveTo(MOVIMIENTO_ESPEJO);
        espejo2.moveTo(MOVIMIENTO_ESPEJO);
        espejo3.moveTo(MOVIMIENTO_ESPEJO);
    }

public:
    void setup() {
        digitalWrite(LED_GANASTE, LOW);
        ir_al_fin_carrera();
    }

    char* name() {
      return "Initial Setup";
    }

    void loop() {
#ifdef DEBUG
        if (digitalRead(DEBUG_PIN) == LOW) {
            change_state(play_state);    
        }
#endif
        if (estado_puerta == ABRIENDO) {
            if (fin_de_carrera_activado() || carrito.distanceToGo() == 0) {
                cerrar_puerta();
            }
        } else { // estado_puerta == CERRANDO
            if (carrito.distanceToGo() == 0) {
                estado_puerta = CERRADA;
                change_state(play_state);
            }
        }
    }
} setup_state;

#ifdef DEBUG
void GameoverState::loop() {
    static boolean state = true;
    
    state = !state;
    if (digitalRead(DEBUG_PIN) == LOW) {
        change_state(setup_state);
    }
    digitalWrite(LED_GANASTE, state?HIGH:LOW);
    delay(VELOCIDAD_CERRANDO);
}
#endif DEBUG


void update_variables() {
    carrito.setMaxSpeed(VELOCIDAD_ABRIENDO);
    carrito.setAcceleration(ACELERACION_ABRIENDO);
    carrito.delay_before_fullstop = DELAY_BEFORE_FULLSTOP;

    espejo1.setMaxSpeed(VELOCIDAD_ESPEJO1);
    espejo1.setAcceleration(ACELERACION_ESPEJO1);
    espejo1.delay_before_fullstop = DELAY_BEFORE_FULLSTOP;

    espejo2.setMaxSpeed(VELOCIDAD_ESPEJO2);
    espejo2.setAcceleration(ACELERACION_ESPEJO2);
    espejo2.delay_before_fullstop = DELAY_BEFORE_FULLSTOP;

    espejo3.setMaxSpeed(VELOCIDAD_ESPEJO3);
    espejo3.setAcceleration(ACELERACION_ESPEJO3);
    espejo3.delay_before_fullstop = DELAY_BEFORE_FULLSTOP;
}

#ifdef DEBUG
String inputString = "";

void parseLine() {
  if (inputString.startsWith("set ")) {
    String sIndex = inputString.substring(inputString.indexOf(" ")+1);
    int index = sIndex.toInt();
    String sValue = sIndex.substring(sIndex.indexOf(" ")+1);
    int value = sValue.toInt();
    *variables[index] = value;
    /*
    Serial.print("setting ");
    Serial.print(index);
    Serial.print("=");
    Serial.println(value);
    */
    update_variables();
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read(); 
    if (inChar == '\n') {
      parseLine();
      inputString = "";
    } else {
      inputString += inChar;
    }
  }
}
#endif

void setup() {
    Serial.begin(115200);
    
#ifdef DEBUG
    Serial.println("hello board");
    inputString.reserve(200);
#endif
    
    Serial.println("-------------------------");
    Serial.println("reset");

    pinMode(SER_Pin, OUTPUT);
    pinMode(RCLK_Pin, OUTPUT);
    pinMode(SRCLK_Pin, OUTPUT);
    pinMode(LED_ESPEJO1 + 0, OUTPUT);
    pinMode(LED_ESPEJO1 + 1, OUTPUT);
    pinMode(LED_ESPEJO1 + 2, OUTPUT);
    pinMode(LED_GANASTE, OUTPUT);

    pinMode(FIN_CARRERA_PIN, INPUT);
    pinMode(BOTON_CAMBIO, INPUT);
    pinMode(BOTON_IZQUIERDO, INPUT);
    pinMode(BOTON_DERECHO, INPUT);
    
    pinMode(DEBUG_PIN, INPUT);
    digitalWrite(DEBUG_PIN, HIGH);

    clearRegisters();
    writeRegisters();

    update_variables();

    change_state (setup_state);
}

void loop() {
    current_state->loop();
    steppers_go();
    if (DELAY_LOOP > 0) {
        delay(DELAY_LOOP);
    }
}
