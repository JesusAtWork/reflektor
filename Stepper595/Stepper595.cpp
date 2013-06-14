#include <Stepper595.h>

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

MyStepper::MyStepper(uint8_t pins, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4) : AccelStepper(pins, pin1, pin2, pin3, pin4) {
    lpin1 = pin1;
    lpin2 = pin2;
    lpin3 = pin3;
    lpin4 = pin4;
    stop_time = millis();
    delay_before_fullstop = DEFAULT_STOP_DELAY;
}

void MyStepper::go(unsigned long current_time)
{
  if (distanceToGo() != 0) {
    stop_time = current_time + delay_before_fullstop;
    run();
  } else {
    if (current_time >= stop_time) {
      fullstop();
    }
  }
}

void MyStepper::fullstop() {
   setRegisterPin(lpin1, LOW);
   setRegisterPin(lpin2, LOW);
   setRegisterPin(lpin3, LOW);
   setRegisterPin(lpin4, LOW);
}

void MyStepper::step(uint8_t step)
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
