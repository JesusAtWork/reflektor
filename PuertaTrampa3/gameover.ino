
const char* GameoverState::MESSAGE = "    GAME OVER     ";

void GameoverState::setup() {
    carrito.stop();
    espejo1.stop();
    espejo2.stop();
    espejo3.stop();
    digitalWrite(LED_GANASTE, HIGH);
    start_time = millis();
    last_change = start_time - 1000;
    pos = 0;
}


void GameoverState::loop() {

#ifdef DEBUG
    if (digitalRead(DEBUG_PIN) == LOW) {
        change_state(setup_state);
    }
#endif
    digitalWrite(LED_GANASTE, (((millis() - start_time) / 100) % 2)?HIGH:LOW);
    
    if (millis() > last_change + 200) {
        last_change = millis();
        display.show(&MESSAGE[pos]);
        pos ++;
        if (MESSAGE[pos + DISPLAY_LEN] == '\0') {
          pos = 0;
        }
    }
}


