void display_show_energia(int valor) {
    char str_energia[10];
    snprintf(str_energia, 6, "%05d", valor);
    display.show(str_energia);
}

void play_track(char* trackname)
{
  digitalWrite(13, HIGH);
  Wire.beginTransmission(4);
  Wire.write("play:");
  Wire.write(trackname);
  Wire.endTransmission();
  digitalWrite(13, LOW);
}

void stop_audio()
{
  Wire.beginTransmission(4);
  Wire.write("stop");
  Wire.endTransmission();  
}
