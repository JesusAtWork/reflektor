const char* GAMEOVER_MESSAGE = "    GAME OVER     ";
const int GAMEOVER_DELAY = 8000;

void GameoverState::apagar_todo() {
    carrito.stop();
    espejo1.stop();
    espejo2.stop();
    espejo3.stop();
    carrito.setMaxSpeed(VELOCIDAD_ABRIENDO);
    carrito.setAcceleration(ACELERACION_ABRIENDO);
    carrito.move(0);

    start_time = millis();
    last_change = start_time - 1000;
    pos = 0;

    Stepper::train.setRegisterPin(LED_GANASTE, LOW);
    Stepper::train.setRegisterPin(LED_SENSOR0, LOW);
    Stepper::train.setRegisterPin(LED_SENSOR1, LOW);
    Stepper::train.setRegisterPin(LED_ESPEJO1, LOW);
    Stepper::train.setRegisterPin(LED_ESPEJO1+1, LOW);
    Stepper::train.setRegisterPin(LED_ESPEJO1+2, LOW);
}

void GameoverState::setup() {
    play_track("gameover");

    apagar_todo();
}


void GameoverState::loop() {
#ifdef DEBUG
//    if (digitalRead(DEBUG_PIN) == LOW) {
//        change_state(reset_state);
//    }
#endif
    display.show(&GAMEOVER_MESSAGE[pos]);

    if (millis() > last_change + 200) {
        last_change = millis();
        pos ++;
        if (GAMEOVER_MESSAGE[pos + DISPLAY_LEN] == '\0') {
          pos = 0;
        }
    }
    
    if (millis() < start_time + GAMEOVER_DELAY/2) {
        Stepper::train.setRegisterPin(ENABLE_LASER, (millis()/100)%2);
    } else {
        Stepper::train.setRegisterPin(ENABLE_LASER, LOW);
    }
    
    if (millis() > start_time + GAMEOVER_DELAY) {
        change_state(&reset_state);
    }
}


