const int MAX_DISPLAY_DELAY = 150;
const int MIN_DISPLAY_DELAY = 80;
const int DISPLAY_ACCEL = 20;
const int BLINK_SPEED = 300;

const int LEFT_KEY = 1;
const int RIGHT_KEY = 2;
const int SPACE_KEY = 4;

#define IS_LEFT_PRESSED (keystatus & LEFT_KEY)
#define IS_RIGHT_PRESSED (keystatus & RIGHT_KEY)
#define IS_SPACE_PRESSED (keystatus & SPACE_KEY)

const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-=@*#< ";
const int charset_space = 42;
const int charset_backspace = 41;
const int charset_initial = 0;

//const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-#1234567890* <";
//const int charset_space = sizeof(charset) - 3;
//const int charset_backspace = sizeof(charset) - 2;
//const int charset_initial = sizeof(charset) - 15;

void InputInitialsState::convert_buffer() {
  for (int n=0; n<DISPLAY_LEN; n++) {
    byte c = charset[initials[n]];
    buffer[n] = c;
  }
}
 
boolean InputInitialsState::is_blinking() {
  return ((millis() - last_change_time) / BLINK_SPEED) % 2;
}

void InputInitialsState::display_cursor() {
  if (is_blinking()) {
    char* c = &buffer[cursor];
    if (*c == ' ') {
      *c = '_';
    } else {
      *c = ' ';
    }
  }
}

void InputInitialsState::update_display() {
  convert_buffer();
  display_cursor();
  display.show(buffer);
}

void InputInitialsState::change_display(int d) {
  initials[cursor] = initials[cursor] + d;
  if (initials[cursor] < 0) {
    initials[cursor] = sizeof(charset) - 2;
  }
  if (initials[cursor] == sizeof(charset) - 1) {
    initials[cursor] = 0;
  }
}

void InputInitialsState::setup () {
  for (int n=0; n<DISPLAY_LEN; n++) {
    initials[n] = charset_initial;
  }
  last_change_time = millis();
  cursor = 0;
  prev_keystatus = 0;
  display_delay = MAX_DISPLAY_DELAY;
  space_was_pressed = false;
  update_display();
}

inline byte InputInitialsState::getKeystatus() {
  byte ret = 0;
  
  if (digitalRead(BOTON_CAMBIO)) {
    ret |= SPACE_KEY;
  }
  if (digitalRead(BOTON_IZQUIERDO)) {
    ret |= LEFT_KEY;
  }
  if (digitalRead(BOTON_DERECHO)) {
    ret |= RIGHT_KEY;
  }
  return ret;
}

void save_initials() {
  // TODO
}

void InputInitialsState::loop() {
  byte keystatus = getKeystatus();

  if (keystatus != prev_keystatus) {
    display_delay = MAX_DISPLAY_DELAY;
  }
  
  if (IS_SPACE_PRESSED) {
    if (!space_was_pressed) {
      if (charset[initials[cursor]] == '<') {
        initials[cursor] = charset_space;
        cursor = max(0, cursor - 1);
        initials[cursor] = charset_backspace;
      } else {
        if (++cursor == DISPLAY_LEN) {
          save_initials();
          change_state(reset_state);
        }
      }
      space_was_pressed = true;
    }
  } else {
    space_was_pressed = false;
  }

  if (IS_LEFT_PRESSED || IS_RIGHT_PRESSED) {
    unsigned long now = millis();
    if (now > (last_change_time + display_delay)) {
      last_change_time = now;
      if (IS_LEFT_PRESSED) {
        change_display(-1);
      }
      if (IS_RIGHT_PRESSED) {
        change_display(1);
      }
      display_delay = max(display_delay - DISPLAY_ACCEL, MIN_DISPLAY_DELAY);
    }
  }
  
  update_display();
  prev_keystatus = keystatus;
}

/*
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
*/
/*
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
*/




