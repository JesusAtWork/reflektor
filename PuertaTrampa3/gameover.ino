const char* GAMEOVER_MESSAGE = "    GAME OVER     ";
const int GAMEOVER_DELAY = 5000;

void GameoverState::setup() {
    carrito.stop();
    espejo1.stop();
    espejo2.stop();
    espejo3.stop();
    Stepper::train.setRegisterPin(LED_GANASTE, HIGH);
    start_time = millis();
    last_change = start_time - 1000;
    pos = 0;
    Stepper::train.setRegisterPin(ENABLE_LASER, LOW);
}


void GameoverState::loop() {
#ifdef DEBUG
//    if (digitalRead(DEBUG_PIN) == LOW) {
//        change_state(reset_state);
//    }
#endif
    Stepper::train.setRegisterPin(LED_GANASTE, (((millis() - start_time) / 100) % 2)?HIGH:LOW);
    
    if (millis() > last_change + 200) {
        last_change = millis();
        display.show(&GAMEOVER_MESSAGE[pos]);
        pos ++;
        if (GAMEOVER_MESSAGE[pos + DISPLAY_LEN] == '\0') {
          pos = 0;
        }
    }
    
    if (millis() > start_time + GAMEOVER_DELAY) {
        change_state(&inputinitials_state);
    }
}


