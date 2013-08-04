#include <AccelStepper.h>

const byte number_of_74hc595s = 3;
const byte numOfRegisterPins = number_of_74hc595s * 8;

#define DEFAULT_STOP_DELAY 2000
#define NELEMS(x)  (sizeof(x) / sizeof(x[0]))

class Stepper;

class StepperTrain {
    boolean registers[numOfRegisterPins];
    byte ser_pin, rclk_pin, srclk_pin;
public:
    void setup(byte ser_pin, byte rclk_pin, byte srclk_pin) {
        this->ser_pin = ser_pin;
        this->rclk_pin = rclk_pin;
        this->srclk_pin = srclk_pin;
        pinMode(ser_pin, OUTPUT);
        pinMode(rclk_pin, OUTPUT);
        pinMode(srclk_pin, OUTPUT);
    }
    void clearRegisters();
    void writeRegisters();
    inline void setRegisterPin(byte index, boolean value) {
        registers[index] = value;
    }
};

class Stepper: public AccelStepper {
private:
    void fullstop();
    unsigned long stop_time;

protected:
    uint8_t lpin1;
    uint8_t lpin2;
    uint8_t lpin3;
    uint8_t lpin4;

public:
    static StepperTrain train;
    int delay_before_fullstop;
    Stepper(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4);
    void go(unsigned long current_time);
    void step(uint8_t step);
    void stop();
};
