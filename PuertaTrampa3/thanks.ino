/*
const int SCROLL_INTERVAL = 100;

boolean ThanksState::algun_boton() {
  return digitalRead(BOTON_IZQUIERDO) == LOW || digitalRead(BOTON_DERECHO) == LOW || digitalRead(BOTON_CAMBIO) == LOW;
}

void ThanksState::setup() {
    carga0.reset();
    carga1.reset();
    while(algun_boton()) {
    }
    play_track("alarma");
}

const char *agradecimientos = ""
  
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


void ThanksState::scrollear_texto() {
    if (millis() > last_change + DISPLAY_UPDATE_INTERVAL) {
        n++;
        last_change = millis();
        if (agradecimientos[n+DISPLAY_LEN-1] == '\0') {
          n = 0;
        }
    }
    display.show(&agradecimientos[n]);
}

void ThanksState::revisar_botones() {   
    if (algun_boton()) {
        change_state(&reset_state);
    }
}

void ThanksState::loop() {
    scrollear_texto();
    revisar_botones();
}
*/
