#include <Wire.h>

void setup()
{
  Wire.begin();
  pinMode(13, OUTPUT);
}

void play(char* trackname)
{
  digitalWrite(13, HIGH);
  Wire.beginTransmission(4);
  Wire.write(trackname);
  Wire.endTransmission();
  digitalWrite(13, LOW);
}

void loop()
{
  play("gameover.wav");
  delay(3000);
  play("game1.wav");
  delay(3000);
  play("alarma.wav");
  delay(3000);
  play("game2.wav");
  delay(3000);
}
