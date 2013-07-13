#define DATA          11
#define ENABLE        12
#define CLOCK         13
#define STATUS_LED    13

const int MAX_DISPLAY_DELAY = 150;
const int MIN_DISPLAY_DELAY = 80;
const int DISPLAY_ACCEL = 20;
const int BLINK_SPEED = 300;
const int DISPLAY_LEN = 5;

const int LEFT_KEY = 1;
const int RIGHT_KEY = 2;
const int SPACE_KEY = 4;

unsigned int display[DISPLAY_LEN];

unsigned int characters[] = {
  40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 0, 40101, 40101, 65535, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 59226, 1792, 33402, 1246, 10277, 4474, 24958, 33862, 38106, 42054, 40101, 40101, 1920, 40101, 40101, 40101, 40101, 25446, 37754, 24666, 2907, 24702, 24678, 24954, 25380, 2139, 17176, 25732, 24600, 30464, 29568, 25434, 25190, 25562, 25318, 8574, 2115, 25368, 58368, 60288, 38016, 5121, 33882, 2129, 40101, 2059, 40101, 24, 40101, 18447, 24589, 16396, 18445, 26638, 2149, 10255, 24581, 29, 18441, 3201, 24584, 16677, 16389, 16397, 26630, 10247, 16388, 8207, 2101, 16393, 49152, 49536, 38016, 10253, 32780, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101, 40101,
};

//const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ *-#1234567890<";
//const int charset_space = 26;
//const int charset_backspace = 40;
//const int charset_initial = 0;

const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-#1234567890* <";
const int charset_space = sizeof(charset) - 3;
const int charset_backspace = sizeof(charset) - 2;
const int charset_initial = sizeof(charset) - 15;


class HiScoreDisplay {
  unsigned long last_change_time;
  int cursor;
  int initials[DISPLAY_LEN];
  char keystatus;
  char prev_keystatus;
  boolean space_pressed;
  int display_delay;
  
  inline boolean left_key() {
    return keystatus & LEFT_KEY;
  }

  inline boolean right_key() {
    return keystatus & RIGHT_KEY;
  }

  inline boolean space_key() {
    return keystatus & SPACE_KEY;
  }

  void update_display() {
    bool blinking = ((millis() - last_change_time) / BLINK_SPEED) % 2;
    for (int n=0; n<DISPLAY_LEN; n++) {
      char c = charset[initials[n]];
      if (n == cursor && blinking) {
         c = '_';
      }
      display[n] = characters[c];
    }
  }

  void change_display(int d) {
    initials[cursor] = initials[cursor] + d;
    if (initials[cursor] < 0) {
      initials[cursor] = sizeof(charset) - 2;
    }
    if (initials[cursor] == sizeof(charset) - 1) {
      initials[cursor] = 0;
    }
  }

public:
  HiScoreDisplay () {
    for (int n=0; n<DISPLAY_LEN; n++) {
      initials[n] = charset_initial;
    }
    last_change_time = millis();
    cursor = 0;
    prev_keystatus = 0;
    display_delay = MAX_DISPLAY_DELAY;
    space_pressed = false;
    update_display();
  }

  inline void setKeystatus(char c) {
    keystatus = c;
  }

  void loop() {
    if (keystatus != prev_keystatus) {
      display_delay = MAX_DISPLAY_DELAY;
    }
    
    if (space_key()) {
      if (!space_pressed) {
        if (charset[initials[cursor]] == '<') {
          initials[cursor] = charset_space;
          cursor = max(0, cursor - 1);
          initials[cursor] = charset_backspace;
        } else {
          cursor = (cursor + 1) % DISPLAY_LEN;
        }
        space_pressed = true;
      }
    } else {
      space_pressed = false;
    }

    if (left_key() || right_key()) {
      unsigned long now = millis();
      if (now > (last_change_time + display_delay)) {
        last_change_time = now;
        if (left_key()) {
          change_display(-1);
        }
        if (right_key()) {
          change_display(1);
        }
        display_delay = max(display_delay - DISPLAY_ACCEL, MIN_DISPLAY_DELAY);
      }
    }
    
    
    update_display();
    prev_keystatus = keystatus;
  }
  
};

HiScoreDisplay hds;

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

    pinMode(DATA, OUTPUT);
    pinMode(ENABLE, OUTPUT);
    pinMode(CLOCK, OUTPUT);
    pinMode(STATUS_LED, OUTPUT);
    digitalWrite(STATUS_LED, HIGH);

    clearRegisters();
    writeRegisters();
}

void loop() {
    hds.loop();
    writeRegisters();
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    hds.setKeystatus(inChar);
    Serial.println((int)inChar);
  }
}

