#include <WaveHC.h>
#include <WaveUtil.h>
#include <Wire.h>


String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the volumes root directory
FatReader file;   // This object represent the WAV file for a pi digit or period
WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time
/*
 * Define macro to put error messages in flash memory
 */
#define error(msg) error_P(PSTR(msg))

//////////////////////////////////// SETUP

void setup() {
  Serial.begin(115200);
  inputString.reserve(20);

  Wire.begin(4);
  Wire.onReceive(receiveEvent);
  
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  
  PgmPrintln("Wav player");
  
  if (!card.init()) {
    error("Card init. failed!");
  }
  if (!vol.init(card)) {
    error("No partition!");
  }
  if (!root.openRoot(vol)) {
    error("Couldn't open dir");
  }

  PgmPrintln("Files found:");
  root.ls();
  digitalWrite(13, LOW);
}

/////////////////////////////////// LOOP

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read(); 
    if (inChar == '\n') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }
}

void loop() {
  if (stringComplete) {
    if (inputString.startsWith("play:")) {
      char c_filename[13];
      String filename = inputString.substring(5) + ".wav";
      filename.toCharArray(c_filename, 13);
      play(c_filename);
    } else if (inputString == "stop") {
      if (wave.isplaying) {
        wave.stop();
      }
    }
    inputString = "";
    stringComplete = false;
  }
}

/////////////////////////////////// HELPERS

void error_P(const char *str) {
  PgmPrint("Error: ");
  SerialPrint_P(str);
  sdErrorCheck();
  while(1);
}
/*
 * print error message and halt if SD I/O error
 */
void sdErrorCheck(void) {
  if (!card.errorCode()) return;
  PgmPrint("\r\nSD I/O error: ");
  Serial.print(card.errorCode(), HEX);
  PgmPrint(", ");
  Serial.println(card.errorData(), HEX);
  while(1);
}
/*
 * Open and start playing a WAV file
 */
void play(char *name) {
  if (wave.isplaying) {// already playing something, so stop it!
    wave.stop(); // stop it
  }
  digitalWrite(13, HIGH);

  if (!file.open(root, name)) {
    PgmPrint("Couldn't open file ");
    Serial.print(name); 
    return;
  }
  if (!wave.create(file)) {
    PgmPrintln("Not a valid WAV");
    return;
  }
  // ok time to play!
  digitalWrite(13, LOW);
  wave.play();
}

void receiveEvent(int howMany)
{
  while (Wire.available()) {
    inputString += (char)Wire.read();
  }
  stringComplete = true;
}
