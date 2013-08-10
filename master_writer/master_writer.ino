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
  Wire.write("play:");
  Wire.write(trackname);
  Wire.endTransmission();
  digitalWrite(13, LOW);
}

void stop()
{
  digitalWrite(13, HIGH);
  Wire.beginTransmission(4);
  Wire.write("stop");
  Wire.endTransmission();
  digitalWrite(13, LOW);
}

void loop()
{
  play("gameover");
  delay(3000);
  play("game1");
  delay(3000);
  play("alarma");
  delay(3000);
  play("game2");
  delay(3000);
  stop();
  delay(1000);  
}
