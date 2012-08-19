#define number_of_74hc595s 3 
#define SER_Pin            8   //pin 14 on the 75HC595
#define RCLK_Pin           9   //pin 12 on the 75HC595
#define SRCLK_Pin          7   //pin 11 on the 75HC595
#define STATUS_LED         2

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


void setup() {
    pinMode(SER_Pin, OUTPUT);
    pinMode(RCLK_Pin, OUTPUT);
    pinMode(SRCLK_Pin, OUTPUT);
    pinMode(STATUS_LED, OUTPUT);
    digitalWrite(STATUS_LED, HIGH);

    clearRegisters();
    writeRegisters();
}

int n = 0;


void loop() {
    int value;
    for (int i=0; i<numOfRegisterPins; i++) {
      value = (((n+i) % 3) == 0) ? HIGH : LOW;
      setRegisterPin(i, value);
    }
    n++;
    writeRegisters();
    delay(200);
}
