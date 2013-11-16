const char* GANASTE_MESSAGE = "    GANASTE     ";
const int GANASTE_DELAY = 8000;

void GanasteState::setup() {
    carrito.move(0);
    espejo1.move(0);
    espejo2.move(0);
    espejo3.move(0);
    apagar_todo();
    play_track("youwin");
    carga0.llenar();
}

void GanasteState::loop() {
    boolean state = (millis()/100)%2;
    Stepper::train.setRegisterPin(LED_GANASTE, state);
    Stepper::train.setRegisterPin(ENABLE_LASER, state);
    Stepper::train.setRegisterPin(LED_GANASTE, state);
    Stepper::train.setRegisterPin(LED_SENSOR0, state);
    Stepper::train.setRegisterPin(LED_SENSOR1, state);
    Stepper::train.setRegisterPin(LED_ESPEJO1, state);
    Stepper::train.setRegisterPin(LED_ESPEJO1+1, state);
    Stepper::train.setRegisterPin(LED_ESPEJO1+2, state);

    display.show(&GANASTE_MESSAGE[pos]);
    if (millis() > last_change + 200) {
        last_change = millis();
        pos ++;
        if (GANASTE_MESSAGE[pos + DISPLAY_LEN] == '\0') {
          pos = 0;
        }
    }
    
    if (millis() > start_time + GAMEOVER_DELAY) {
        change_state(&inputinitials_state);
    }
}


