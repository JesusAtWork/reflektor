#define DATA          11
#define ENABLE        12
#define CLOCK         13

#define DATA_HIGH     digitalWrite(DATA, HIGH)
#define DATA_LOW      digitalWrite(DATA, LOW)
#define CLOCK_HIGH    digitalWrite(CLOCK, HIGH)
#define CLOCK_LOW     digitalWrite(CLOCK, LOW)


/*
#define DATA_HIGH   PORTB |= _BV (3); // digitalWrite (11, HIGH);
#define DATA_LOW    PORTB &= ~_BV (3); // digitalWrite (11, LOW);
#define CLOCK_HIGH  PORTB |= _BV (5); // digitalWrite (13, HIGH);
#define CLOCK_LOW   PORTB &= ~_BV (5); // digitalWrite (13, LOW);
*/

#define STATUS_LED    13
#define DISPLAY_LEN   5


unsigned int display[DISPLAY_LEN];

unsigned int characters[] = {
40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 0, 40101, 40101, 65535, 40101, 40101, 27679, 40101, 40101, 40101, 40101, 40101, 40101, 36, 8, 40101, 59226, 1792, 33402, 1246, 10277, 4474, 24958, 33862, 38106, 42054, 40101, 40101, 1920, 60, 40101, 40101, 17247, 25446, 37754, 24666, 2907, 24702, 24678, 24954, 25380, 2139, 17176, 25732, 24600, 30464, 29568, 25434, 25190, 25562, 25318, 8574, 2115, 25368, 58368, 60288, 38016, 5121, 33882, 2129, 40101, 2059, 40101, 24, 40101, 18447, 24589, 16396, 18445, 26638, 2149, 10255, 24581, 29, 18441, 3201, 24584, 16677, 16389, 16397, 26630, 10247, 16388, 8207, 2101, 16393, 49152, 49536, 38016, 10253, 32780, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101,
};

void clearRegisters(){
  for(int i = 0; i < DISPLAY_LEN; i++){
     display[i] = 0;
  }
} 

void writeRegisters(){
  digitalWrite(ENABLE, HIGH);
  
  for(int i = DISPLAY_LEN-1; i >= 0; i--){
    for (int jay = 0; jay < 16; jay++) {
      CLOCK_LOW;
      unsigned int val = (display[i] >> jay);
      if (val & 1) {
        DATA_HIGH;
      } else {
        DATA_LOW;
      }
      CLOCK_HIGH;
    }
  }
  digitalWrite(ENABLE, LOW);
}

void setup() {
    pinMode(DATA, OUTPUT);
    pinMode(ENABLE, OUTPUT);
    pinMode(CLOCK, OUTPUT);
    pinMode(STATUS_LED, OUTPUT);
    digitalWrite(STATUS_LED, HIGH);
    clearRegisters();
}

int n = 0;
//char *texto="    0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz Hola Mundo - bienvenidos al club de jaqueo - REFLEKTOR [c] 2013 - CLUB DE JAQUEO - GPL        \0"; 

//AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz
char *texto = "     REFLEKTOR [c] 2013 *#O CLUB DE JAQUEO *#O GPL - reflektor@protocultura.net       \0";
//char *texto=" *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O *#O\0";

//char *texto = "R [c] ";

unsigned long last_change = 0;

void loop() {
    
    for (int j=0; j<DISPLAY_LEN; j++) {
      display[j] = characters[texto[j+n]];
    }
    writeRegisters();
    
    if (millis() > last_change + 200) {
        n++;
        last_change = millis();
    }
    if (texto[n+DISPLAY_LEN-1] == '\0') {
      n = 0;
    }
    //display[4] = characters['a' + (n%DISPLAY_LEN)];
    //clearRegisters();
    //writeRegisters();
    
/*
    display[0] = 1<<0;
    writeRegisters();
    delay(500);

    display[0] = 1<<15;
    writeRegisters();
    delay(500);

    display[0] = 1<<3;
    writeRegisters();
    delay(500);
*/
}
