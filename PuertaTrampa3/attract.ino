const int DISPLAY_UPDATE_INTERVAL = 100;
const int SCROLL_INTERVAL = 200;

void AttractState::setup() {
    last_change = 0;
    n = 0;
    stop_audio();
    carga0.reset();
    carga1.reset();
}

const char *texto = "     PE()EKTOP [REFLEKTOR] - PRESIONE LOS DOS BOTONES ROJOS      \0";

void AttractState::scrollear_texto() {
    display.show(&texto[n]);
    if (millis() > last_change + SCROLL_INTERVAL) {
        last_change = millis();
        n++;
        if (texto[n+DISPLAY_LEN-1] == '\0') {
          n = 0;
        }
    }
}

void AttractState::revisar_botones() {   
    boolean izquierdo = digitalRead(BOTON_IZQUIERDO) == LOW;
    boolean derecho = digitalRead(BOTON_DERECHO) == LOW;
    boolean cambio = digitalRead(BOTON_CAMBIO) == LOW;

    if (izquierdo && derecho && cambio) {
        change_state(&reset_state);
    }
    if (izquierdo && derecho) {
        change_state(&reset_state);
    }
}

void AttractState::loop() {
//    play_track("gameover");
    scrollear_texto();
    revisar_botones();
}

