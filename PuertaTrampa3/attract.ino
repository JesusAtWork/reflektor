const int DISPLAY_UPDATE_INTERVAL = 100;

void AttractState::setup() {
    last_change = 0;
    n = 0;
    stop_audio();
    carga0.reset();
    carga1.reset();
}

const char *texto = "     PE()EKTOP [reflektor] - presione los dos botones rojos para comenzar      \0";

void AttractState::scrollear_texto() {
    if (millis() > last_change + DISPLAY_UPDATE_INTERVAL) {
        display.show(&texto[n]);
        n++;
        last_change = millis();
        if (texto[n+DISPLAY_LEN-1] == '\0') {
          n = 0;
        }
    }
}

void AttractState::revisar_botones() {   
    boolean izquierdo = digitalRead(BOTON_IZQUIERDO) == LOW;
    boolean derecho = digitalRead(BOTON_DERECHO) == LOW;

    if (izquierdo == true && derecho == true) {
        change_state(&reset_state);
    }
}

void AttractState::loop() {
//    play_track("gameover");
    scrollear_texto();
    revisar_botones();
}
