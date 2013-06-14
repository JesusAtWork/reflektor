#include <AccelStepper.h>

#define SER_Pin            8   //pin 14 on the 75HC595
#define RCLK_Pin           9   //pin 12 on the 75HC595
#define SRCLK_Pin          7   //pin 11 on the 75HC595

#define number_of_74hc595s 3 
#define numOfRegisterPins number_of_74hc595s * 8
#define setRegisterPin(index,value) registers[index] = value

#define DEFAULT_STOP_DELAY 2000

extern boolean registers[];

void clearRegisters();
void writeRegisters();

class MyStepper: public AccelStepper {
  private:
    void fullstop();
    unsigned long stop_time;

  protected:
    uint8_t lpin1;
    uint8_t lpin2;
    uint8_t lpin3;
    uint8_t lpin4;
    
  public:
    int delay_before_fullstop;
    MyStepper(uint8_t pins, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4);
    void go(unsigned long current_time);
    void step(uint8_t step);
    void stop();
};
