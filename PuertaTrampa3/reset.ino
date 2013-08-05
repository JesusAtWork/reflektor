void ResetState::ir_al_fin_carrera() {
    estado_puerta = ABRIENDO;
    carrito.setCurrentPosition(PUERTA_CERRADA + 800);
    espejo1.setCurrentPosition(MOVIMIENTO_ESPEJO * 2);
    espejo2.setCurrentPosition(MOVIMIENTO_ESPEJO * 2);
    espejo3.setCurrentPosition(MOVIMIENTO_ESPEJO * 2);
    carrito.moveTo(PUERTA_ABIERTA);
    espejo1.moveTo(0);
    espejo2.moveTo(0);
    espejo3.moveTo(0);
}

void ResetState::cerrar_puerta() {
    estado_puerta = CERRANDO;
    carrito.setCurrentPosition(PUERTA_ABIERTA);
    espejo1.setCurrentPosition(0);
    espejo2.setCurrentPosition(0);
    espejo3.setCurrentPosition(0);
    carrito.moveTo(PUERTA_CERRADA);
    espejo1.moveTo(MOVIMIENTO_ESPEJO);
    espejo2.moveTo(MOVIMIENTO_ESPEJO);
    espejo3.moveTo(MOVIMIENTO_ESPEJO);
}

void ResetState::setup() {
    digitalWrite(LED_GANASTE, LOW);
    Serial.println("reset");
    ir_al_fin_carrera();
}

void ResetState::loop() {
#ifdef DEBUG
    if (digitalRead(DEBUG_PIN) == LOW) {
        change_state(play_state);    
    }
#endif
    if (estado_puerta == ABRIENDO) {
        if (fin_de_carrera_activado() || carrito.distanceToGo() == 0) {
            cerrar_puerta();
        }
    } else { // estado_puerta == CERRANDO
        if (carrito.distanceToGo() == 0) {
            estado_puerta = CERRADA;
            change_state(play_state);
        }
    }
}