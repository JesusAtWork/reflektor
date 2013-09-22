const char* GANASTE_MESSAGE = "    GANASTE     ";
const int GANASTE_DELAY = 8000;

void GanasteState::setup() {
    play_track("youwin");
    apagar_todo();
    espejo1.setMaxSpeed(VELOCIDAD_ESPEJO1*2);
    espejo1.setAcceleration(ACELERACION_ESPEJO1*2);
    espejo1.move(3000);
    espejo2.setMaxSpeed(VELOCIDAD_ESPEJO2*2);
    espejo2.setAcceleration(ACELERACION_ESPEJO2*2);
    espejo2.move(-3000);
    espejo3.setMaxSpeed(VELOCIDAD_ESPEJO3*2);
    espejo3.setAcceleration(ACELERACION_ESPEJO3*2);
    espejo3.move(3000);
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


