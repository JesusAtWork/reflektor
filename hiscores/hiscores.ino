#define DATA           5
#define ENABLE         6
#define CLOCK          7
#define STATUS_LED    13
#define DISPLAY_LEN    5

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

unsigned int display[DISPLAY_LEN];

unsigned int characters[] = {
    40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 0, 40101, 40101, 65535, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 59226, 1792, 33402, 1246, 10277, 4474, 24958, 33862, 38106, 42054, 40101, 40101, 1920, 40101, 40101, 40101, 40101, 25446, 37754, 24666, 2907, 24702, 24678, 24954, 25380, 2139, 17176, 25732, 24600, 30464, 29568, 25434, 25190, 25562, 25318, 8574, 2115, 25368, 58368, 60288, 38016, 5121, 33882, 2129, 40101, 2059, 40101, 24, 40101, 18447, 24589, 16396, 18445, 26638, 2149, 10255, 24581, 29, 18507, 25732, 2065, 16677, 16389, 16397, 26630, 10247, 16388, 8207, 2101, 16393, 49152, 49536, 38016, 10253, 32780, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101,
};

void clearRegisters(){
  for(int i = 0; i < DISPLAY_LEN; i++){
     display[i] = 0;
  }
} 


void writeRegisters(){
  digitalWrite(CLOCK, LOW);
  digitalWrite(ENABLE, HIGH);
  delayMicroseconds(10);
  for(int i = DISPLAY_LEN; i >=  0; i--){
    for (int j = 0; j < 16; j++) {
      digitalWrite(CLOCK, LOW);
      delayMicroseconds(10);
      unsigned int val = (display[i] >> j);
      digitalWrite(DATA, val & 1 ? HIGH : LOW);
      digitalWrite(CLOCK, HIGH);
      delayMicroseconds(10);
    }
  }
  digitalWrite(CLOCK, LOW);
  delayMicroseconds(10);
  digitalWrite(ENABLE, LOW);
}

void setup() {
    Serial.begin(115200);
    inputString.reserve(200);  

    pinMode(DATA, OUTPUT);
    pinMode(ENABLE, OUTPUT);
    pinMode(CLOCK, OUTPUT);
    pinMode(STATUS_LED, OUTPUT);
    digitalWrite(STATUS_LED, HIGH);

    clearRegisters();
    writeRegisters();
}

void loop() {
    if (stringComplete) {
        Serial.println(inputString); 
        for (int j=0; j<DISPLAY_LEN; j++) {
            char c = inputString.charAt(j);
            display[j] = characters[c];
        }
        writeRegisters();
        inputString = "";
        stringComplete = false;
    }

}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read(); 
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}

