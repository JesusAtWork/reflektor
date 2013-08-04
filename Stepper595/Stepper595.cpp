#include <Stepper595.h>

void StepperTrain::clearRegisters() {
    for(int i = numOfRegisterPins - 1; i >=  0; i--) {
        registers[i] = LOW;
    }
}

void StepperTrain::writeRegisters() {
    digitalWrite(rclk_pin, LOW);

    for(int i = numOfRegisterPins - 1; i >=  0; i--) {
        digitalWrite(srclk_pin, LOW);
        int val = registers[i];
        digitalWrite(ser_pin, val);
        digitalWrite(srclk_pin, HIGH);
    }
    digitalWrite(rclk_pin, HIGH);
}

Stepper::Stepper(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4) : AccelStepper(4, pin1, pin2, pin3, pin4) {
    lpin1 = pin1;
    lpin2 = pin2;
    lpin3 = pin3;
    lpin4 = pin4;
    stop_time = millis();
    delay_before_fullstop = DEFAULT_STOP_DELAY;
}

void Stepper::go(unsigned long current_time)
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

void Stepper::stop() {
    move(0);
}

void Stepper::fullstop() {
    train.setRegisterPin(lpin1, LOW);
    train.setRegisterPin(lpin2, LOW);
    train.setRegisterPin(lpin3, LOW);
    train.setRegisterPin(lpin4, LOW);
}

void Stepper::step(uint8_t step)
{
    switch (step & 0x3)
    {
    case 0:    // 1010
        train.setRegisterPin(lpin1, HIGH);
        train.setRegisterPin(lpin2, LOW);
        train.setRegisterPin(lpin3, HIGH);
        train.setRegisterPin(lpin4, LOW);
        break;

    case 1:    // 0110
        train.setRegisterPin(lpin1, LOW);
        train.setRegisterPin(lpin2, HIGH);
        train.setRegisterPin(lpin3, HIGH);
        train.setRegisterPin(lpin4, LOW);
        break;

    case 2:    //0101
        train.setRegisterPin(lpin1, LOW);
        train.setRegisterPin(lpin2, HIGH);
        train.setRegisterPin(lpin3, LOW);
        train.setRegisterPin(lpin4, HIGH);
        break;


    case 3:    //1001
        train.setRegisterPin(lpin1, HIGH);
        train.setRegisterPin(lpin2, LOW);
        train.setRegisterPin(lpin3, LOW);
        train.setRegisterPin(lpin4, HIGH);
        break;
    }
}
