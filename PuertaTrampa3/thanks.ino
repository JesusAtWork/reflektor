prog_char agradecimientos[] PROGMEM = ""
  
  "     PE()EKTOP [reflektor] *#*#*#*# "
  "bits & volts: alecu "
  "ondas sonoras: cristian martinez "
  " - GRACIAS ===== "
  "* Noemi y Loli de Casa Abasto "
  "* Luli, Ame y Nico "
  "* Walter Tournier "
  "* Bernardo, Merlina y todo el Club de Jaqueo "
  "* vanlit que ayudo un monton cuando quemaban las papas "
  "* y especialmente a Jorge Crowe "
  "*#*#*#*#* 2013 Club de Jaqueo  *#*#*#*#*    \0";


boolean ThanksState::algun_boton() {
  return digitalRead(BOTON_IZQUIERDO) == LOW || digitalRead(BOTON_DERECHO) == LOW || digitalRead(BOTON_CAMBIO) == LOW;
}

void ThanksState::setup() {
    carga0.reset();
    carga1.reset();
    while(algun_boton()) {
      delay(100);
    }
    play_track("alarma");
}

void ThanksState::scrollear_texto() {
    if (millis() > last_change + SCROLL_INTERVAL) {
        n++;
        last_change = millis();
        if (pgm_read_byte_near(agradecimientos+n+DISPLAY_LEN-1) == '\0') {
          n = 0;
        }
    }
    for (int j=0; j<5; j++) {
      buffer[j] = pgm_read_byte_near(agradecimientos+n+j);
    }
    buffer[5] = 0;
    //Serial.println(mostrar);
    display.show(buffer);

}

void ThanksState::revisar_botones() {   
    if (algun_boton()) {
        change_state(&attract_state);
    }
}

void ThanksState::loop() {
    scrollear_texto();
    revisar_botones();
}

