#include <AccelStepper.h>

#define number_of_74hc595s 3 
#define SER_Pin            8   //pin 14 on the 75HC595
#define RCLK_Pin           9   //pin 12 on the 75HC595
#define SRCLK_Pin         10   //pin 11 on the 75HC595
#define STATUS_LED         2
#define FIN_CARRERA_PIN    3

#define numOfRegisterPins number_of_74hc595s * 8
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

#define setRegisterPin(index,value) registers[index] = value


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
   void stop()
   {
     setRegisterPin(lpin1, LOW);
     setRegisterPin(lpin2, LOW);
     setRegisterPin(lpin3, LOW);
     setRegisterPin(lpin4, LOW);
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

MyStepper stepper(4, 0, 1, 2, 3);
MyStepper stepper2(4, 9, 10, 11, 12);
int INACTIVO = 400;
int ACTIVO = 800;

void finDeCarrera() {
    stepper.enableOutputs();
    stepper.setMaxSpeed(500);
    stepper.setAcceleration(500);

    stepper2.enableOutputs();
    stepper2.setMaxSpeed(100);
    stepper2.setAcceleration(30);


// Ir al fin de carrera
    stepper.setCurrentPosition(2000);
    stepper2.setCurrentPosition(200);
    stepper.moveTo(0);
    stepper2.moveTo(0);
    while(digitalRead(FIN_CARRERA_PIN) == LOW) {
      stepper.run();
      stepper2.run();
      writeRegisters();    
   //   clearRegisters();
   //   writeRegisters();
    }
    stepper.setCurrentPosition(0);
    stepper2.setCurrentPosition(0);

// Ir a la posicion inicial
    stepper.moveTo(1300);
    stepper2.moveTo(500);

    while (stepper.distanceToGo() > 0) {
      stepper.run();
      stepper2.run();
      writeRegisters();    
    }

    stepper.disableOutputs();
}

void setup() {
    Serial.begin(9600);
    pinMode(SER_Pin, OUTPUT);
    pinMode(RCLK_Pin, OUTPUT);
    pinMode(SRCLK_Pin, OUTPUT);
    pinMode(FIN_CARRERA_PIN, INPUT);
    pinMode(STATUS_LED, OUTPUT);
    digitalWrite(STATUS_LED, HIGH);

    clearRegisters();
    writeRegisters();

//    setRegisterPin(0, HIGH);
//    setRegisterPin(8, HIGH);
    stepper.setEnablePin(12);
    finDeCarrera();
//    clearRegisters();
    stepper.stop();
    setRegisterPin(8, HIGH);
    writeRegisters();
}


void loop() {  
    int ldr = analogRead(A5); 
    
    if (ldr > ACTIVO) {
        
        stepper.disableOutputs();
    }
    
    if (ldr < INACTIVO) {
      
    }
    //int sensorValue = digitalRead(3);
    //Serial.println(sensorValue);

    if (stepper2.distanceToGo() == 0) {
     //stepper2.setCurrentPosition(200);
     //stepper2.moveTo(0);
     //setRegisterPin(8, LOW);
     stepper2.stop();
    }

    stepper2.run();
    writeRegisters();
}
