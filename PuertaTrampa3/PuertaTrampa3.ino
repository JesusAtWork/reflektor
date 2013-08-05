#include <AccelStepper.h>
#include <Stepper595.h>
#include <display16.h>
#include "variables.h"

// PENDIENTES:
// demorar un ratito en ganar [  ]
// usar variables en todos lados [./]

#undef DEBUG

const byte FIN_CARRERA_PIN = 3;
const byte BOTON_CAMBIO = 4;
const byte BOTON_IZQUIERDO = 5;
const byte BOTON_DERECHO = 6;

const byte DISPLAY_LEN = 5;

const byte ENABLE_5484 = 2;
const byte CLOCK_5484  = 7;
const byte DATA_5484   = 8;

const byte SER_Pin   = 8;   //pin 14 on the 75HC595   
const byte RCLK_Pin  = 9;   //pin 12 on the 75HC595   
const byte SRCLK_Pin = 7;   //pin 11 on the 75HC595

const byte LED_ESPEJO1 = 10; // 11 y 12 son los otros espejos
const byte LED_GANASTE = 13;

const byte SENSOR0 = A5;
const byte SENSOR1 = A4;

const char *texto = "     REFLEKTOR [c] 2013 *#O CLUB DE JAQUEO *#O GPL - reflektor@protocultura.net       \0";

Display16 display(DISPLAY_LEN, ENABLE_5484, CLOCK_5484, DATA_5484);
StepperTrain Stepper::train = StepperTrain();
Stepper carrito(4, 6, 5, 7);
Stepper espejo1(0, 1, 2, 3);
Stepper espejo2(8, 9, 10, 11);  
Stepper espejo3(12, 13, 14, 15);

typedef enum {ABRIENDO, CERRANDO, CERRADA} EstadosPuerta;
EstadosPuerta estado_puerta;
int espejo_activo;

class State {
public:
    virtual const char* name() = 0;
    virtual void setup() = 0;
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
    Stepper::train.writeRegisters();
}

boolean inline fin_de_carrera_activado() {
    return digitalRead(FIN_CARRERA_PIN) == HIGH;
}


class GameoverState : public State {
    unsigned long start_time;
    unsigned long last_change;
    static const char* MESSAGE;
    int pos;
public:
    const char* name() {
      return "Game Over";
    }
    void setup();
    void loop();
} gameover_state;

class PlayState : public State {
    unsigned long last_change;
    int n;
    void verificar_sensor1();
    void verificar_sensor2();
    void mover_espejo(int espejo, int direccion);
    void procesar_botones_izquierda_derecha();
    void mostrar_espejo_activo();
    void procesar_boton_cambio_espejo();
    void procesar_botonera();
    void scrollear_texto();
public:
    const char* name() {
      return "Running Game";
    }
    void setup();
    void loop();
} play_state;

class SetupState : public State {
    void ir_al_fin_carrera();
    void cerrar_puerta();
public:
    const char* name() {
      return "Initial Setup";
    }
    void setup();
    void loop();
} setup_state;


void setup() {
    Serial.begin(115200);
    
#ifdef DEBUG
    Serial.println("hello board");
    inputString.reserve(200);
    update_variables();
#endif
    
    Serial.println("-------------------------");
    Serial.println("reset");

    pinMode(LED_ESPEJO1 + 0, OUTPUT);
    pinMode(LED_ESPEJO1 + 1, OUTPUT);
    pinMode(LED_ESPEJO1 + 2, OUTPUT);
    pinMode(LED_GANASTE, OUTPUT);

    pinMode(FIN_CARRERA_PIN, INPUT);
    pinMode(BOTON_CAMBIO, INPUT);
    pinMode(BOTON_IZQUIERDO, INPUT);
    pinMode(BOTON_DERECHO, INPUT);
    
    Stepper::train.setup(SER_Pin, RCLK_Pin, SRCLK_Pin);
    
    //pinMode(DEBUG_PIN, OUTPUT);
    //digitalWrite(DEBUG_PIN, HIGH);

    Stepper::train.clearRegisters();
    Stepper::train.writeRegisters();

    change_state (setup_state);
}

void loop() {
    current_state->loop();
    steppers_go();
    if (DELAY_LOOP > 0) {
        delay(DELAY_LOOP);
    }
}
