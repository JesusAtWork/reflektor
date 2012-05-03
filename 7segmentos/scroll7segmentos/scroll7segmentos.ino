long n = 0;
int x = 1;
int del = 1;

byte segments[128] = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 64, 128, 255, 63, 6, 91, 79, 102, 109, 125, 7, 127, 103, 255, 255, 255, 255, 255, 255, 255, 119, 124, 57, 94, 121, 113, 125, 118, 48, 30, 117, 56, 55, 84, 92, 115, 191, 80, 109, 120, 28, 62, 126, 118, 110, 91, 255, 255, 255, 255, 8, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
int pinout[8] = {5, 7, 4, 11, 10, 3, 6, 2};
char message[] = "  REFLECTOR Q1 -  ABCDEFGHIJKLMNOPQRSTUVWXYZ 1234567890  ";
int msg_len = sizeof(message) - 1;
int pos = 0;
int scroll_delay = 2000;
int delayed = 0;

void setup(){
  for (int i=1; i<=13; i++) {
    pinMode(i, OUTPUT);
  }
}
 
void loop(){
  for (int digit = 1; digit<=4; digit++) {
    clearLEDs();
    pickDigit(digit);
    pickChar(message[(pos+digit-1)%msg_len]);
    delayed++;
    if (delayed > scroll_delay) {
      pos++;
      delayed = 0;
    }
    delayMicroseconds(del);  
  }
}
 
void pickDigit(int x){
  digitalWrite(13, LOW); // dig 1
  digitalWrite(12, LOW); // dig 2
  digitalWrite( 9, LOW); // dig 3
  digitalWrite( 8, LOW); // dig 4
 
  switch(x){
    case 1: digitalWrite(13, HIGH); break;
    case 2: digitalWrite(12, HIGH); break;
    case 3: digitalWrite(9, HIGH); break;
    case 4: digitalWrite(8, HIGH); break;    
  }
}

void clearLEDs() {
  pickChar(' ');
}

void pickChar(char c){
  byte b = segments[c];
  for (int n=0; n<8; n++) {
    int pin = pinout[n];
    
    if (b & 1 << n) {
      digitalWrite(pin, LOW);
    } else {
      digitalWrite(pin, HIGH);
    }
  }
}

