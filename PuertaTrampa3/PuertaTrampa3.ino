#include <AccelStepper.h>

#define number_of_74hc595s 3 

#define SER_Pin            8   //pin 14 on the 75HC595
#define RCLK_Pin           9   //pin 12 on the 75HC595
#define SRCLK_Pin          7   //pin 11 on the 75HC595
#define FIN_CARRERA_PIN    3
#define BOTON_1            4
#define BOTON_2            5
#define BOTON_3            6
#define SENSOR0           A5
#define SENSOR1           A4
#define PUERTA_CERRADA  1400
#define PUERTA_ABIERTA     0

#define POCA_LUZ         100
#define MUCHA_LUZ        150


#define numOfRegisterPins number_of_74hc595s * 8
#define setRegisterPin(index,value) registers[index] = value

boolean registers[numOfRegisterPins];

void clearRegisters(){
  for(int i = numOfRegisterPins - 1; i >=  0; i--){
     registers[i] = LOW;
  }
} 

void writeRegisters(){

  digitalWrite(RCLK_Pin, LOW);

  for(int i = numOfRegisterPins - 1; i >=  0; i--){
    digitalWrite(SRCLK_Pin, LOW);

    int val = registers[i];

    digitalWrite(SER_Pin, val);
    digitalWrite(SRCLK_Pin, HIGH);

  }
  digitalWrite(RCLK_Pin, HIGH);

}



class MyStepper: public AccelStepper {
  protected:
    uint8_t lpin1;
    uint8_t lpin2;
    uint8_t lpin3;
    uint8_t lpin4;
    
  public:
    MyStepper(uint8_t pins, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4) : AccelStepper(pins, pin1, pin2, pin3, pin4) {
      lpin1 = pin1;
      lpin2 = pin2;
      lpin3 = pin3;
      lpin4 = pin4;
    }
   void go()
   {
     if (distanceToGo() != 0) {
       run();
     } else {
       setRegisterPin(lpin1, LOW);
       setRegisterPin(lpin2, LOW);
       setRegisterPin(lpin3, LOW);
       setRegisterPin(lpin4, LOW);
     }
   }
   void step(uint8_t step)
   {
    switch (step & 0x3)
    {
    case 0:    // 1010
        setRegisterPin(lpin1, HIGH);
        setRegisterPin(lpin2, LOW);
        setRegisterPin(lpin3, HIGH);
        setRegisterPin(lpin4, LOW);
        break;

    case 1:    // 0110
        setRegisterPin(lpin1, LOW);
        setRegisterPin(lpin2, HIGH);
        setRegisterPin(lpin3, HIGH);
        setRegisterPin(lpin4, LOW);
        break;

    case 2:    //0101
        setRegisterPin(lpin1, LOW);
        setRegisterPin(lpin2, HIGH);
        setRegisterPin(lpin3, LOW);
        setRegisterPin(lpin4, HIGH);
        break;


    case 3:    //1001
        setRegisterPin(lpin1, HIGH);
        setRegisterPin(lpin2, LOW);
        setRegisterPin(lpin3, LOW);
        setRegisterPin(lpin4, HIGH);
        break;
    }
  }

};

MyStepper stepper(4, 4, 6, 5, 7);
MyStepper stepper2(4, 0, 1, 2, 3);
MyStepper stepper3(4, 8, 9, 10, 11);
MyStepper stepper4(4, 12, 13, 14, 15);

const int LED_GANASTE = 16;

void finDeCarrera() {

// Ir al fin de carrera
    stepper.setCurrentPosition(PUERTA_CERRADA + 300);
    stepper2.setCurrentPosition(200);
    stepper3.setCurrentPosition(200);
    stepper4.setCurrentPosition(200);
    stepper.moveTo(PUERTA_ABIERTA);
    stepper2.moveTo(0);
    stepper3.moveTo(0);
    stepper4.moveTo(0);
    
    while(digitalRead(FIN_CARRERA_PIN) == LOW) {
      stepper.go();
      stepper2.go();
      stepper3.go();
      stepper4.go();
      writeRegisters();    
    }
    stepper.setCurrentPosition(PUERTA_ABIERTA);
    stepper2.setCurrentPosition(0);
    stepper3.setCurrentPosition(0);
    stepper4.setCurrentPosition(0);

// Ir a la posicion inicial
    stepper.moveTo(PUERTA_CERRADA);
    stepper2.moveTo(50);
    stepper3.moveTo(50);
    stepper4.moveTo(50);

    while (stepper.distanceToGo() > 0) {
      stepper.go();
      stepper2.go();
      stepper3.go();
      stepper4.go();
      writeRegisters();    
    }
}

void setup() {
//    Serial.begin(9600);
    pinMode(SER_Pin, OUTPUT);
    pinMode(RCLK_Pin, OUTPUT);
    pinMode(SRCLK_Pin, OUTPUT);
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

    finDeCarrera();
}

boolean abriendo_puerta = false;
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

void loop() {  
    int sensor1 = analogRead(SENSOR1); 
    
    if (abriendo_puerta) {
      if(digitalRead(FIN_CARRERA_PIN) == HIGH) {
        // llegamos al fin de la carrera
        abriendo_puerta = false;
        stepper.setMaxSpeed(100);
        stepper.setAcceleration(20);
        stepper.setCurrentPosition(PUERTA_ABIERTA);
      }
      
    } else {

      if (sensor1 > MUCHA_LUZ && digitalRead(FIN_CARRERA_PIN) == LOW) {
        stepper.setMaxSpeed(5000);
        stepper.setAcceleration(500);
        abriendo_puerta = true;
        stepper.move(-PUERTA_CERRADA * 2);
      }
      
      if (sensor1 < POCA_LUZ) {
        stepper.moveTo(PUERTA_CERRADA);      
      }
    }
    
    int sensor2 = analogRead(SENSOR0);
    if (sensor2 > MUCHA_LUZ) {
        setRegisterPin(LED_GANASTE, HIGH);      
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
 
    count += 1;
    if (true || count % 1) {

      if (estado_boton1) {
        espejo_activo = (espejo_activo + 1) % 3;
      }
      
      if (estado_boton2 && !estado_boton3) {
        mover_espejo(espejo_activo, -4);
      }

      if (estado_boton3 && !estado_boton2) {
        mover_espejo(espejo_activo, 4);
      }
      
      estado_boton1 = false;
      estado_boton2 = false;
      estado_boton3 = false;
    }
    
    stepper.go();
    stepper2.go();
    stepper3.go();
    stepper4.go();
    writeRegisters();
}
