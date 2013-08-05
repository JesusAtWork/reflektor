void PlayState::verificar_sensor1() {
    int sensor1 = analogRead(SENSOR1);
    if (estado_puerta == ABRIENDO) {
        if(fin_de_carrera_activado()) {
            estado_puerta = CERRANDO;
            carrito.setMaxSpeed(VELOCIDAD_CERRANDO);
            carrito.setAcceleration(ACELERACION_CERRANDO);
            carrito.setCurrentPosition(PUERTA_ABIERTA);
        }
    } else {
        if (sensor1 < POCA_LUZ) {
            carrito.moveTo(PUERTA_CERRADA);
        }

        if (sensor1 > MUCHA_LUZ && !fin_de_carrera_activado()) {
            if (estado_puerta == CERRADA) {
                Serial.println("track:puerta");
            }
            estado_puerta = ABRIENDO;
            carrito.setMaxSpeed(VELOCIDAD_ABRIENDO);
            carrito.setAcceleration(ACELERACION_ABRIENDO);
            carrito.move(-PUERTA_CERRADA * 2);
        }

        if (estado_puerta == CERRANDO && carrito.distanceToGo() == 0 ) {
            estado_puerta = CERRADA;
            Serial.println("track:juego");
        }
    }
}

void PlayState::verificar_sensor2() {
    int sensor2 = analogRead(SENSOR0);
    if (sensor2 > MUCHA_LUZ) {
        // ganaste!
        Serial.println("track:gameover");
        change_state (gameover_state);
    }
}

void PlayState::mover_espejo(int espejo, int direccion) {
    if (espejo == 0) {
        espejo1.move(direccion);
    } else if (espejo == 1) {
        espejo2.move(direccion);
    } else {
        espejo3.move(direccion);
    }
}

void PlayState::procesar_botones_izquierda_derecha() {
    boolean izquierdo = digitalRead(BOTON_IZQUIERDO) == HIGH;
    boolean derecho = digitalRead(BOTON_DERECHO) == HIGH;

    if (izquierdo == true && derecho == false) {
        mover_espejo(espejo_activo, -MOVIMIENTO_ESPEJO);
    }

    if (derecho == true && izquierdo == false) {
        mover_espejo(espejo_activo, MOVIMIENTO_ESPEJO);
    }

    if (izquierdo == false && derecho == false) {
        mover_espejo(espejo_activo, 0);
    }
}

void PlayState::mostrar_espejo_activo () {
    for (int k=0; k<3; k++) {
        digitalWrite(LED_ESPEJO1+k, espejo_activo == k ? HIGH : LOW);
    }
}

void PlayState::procesar_boton_cambio_espejo() {
    static boolean estado_anterior = false;
    boolean estado_actual = digitalRead(BOTON_CAMBIO) == HIGH;

    if ( estado_actual == true && estado_anterior == false) {
        Serial.println("reset");
        espejo_activo = (espejo_activo + 1) % 3;
        mostrar_espejo_activo();
    }
    estado_anterior = estado_actual;
}

void PlayState::procesar_botonera() {
    procesar_botones_izquierda_derecha();
    procesar_boton_cambio_espejo();
}

void PlayState::scrollear_texto() {
    if (millis() > last_change + 200) {
        display.show(&texto[n]);
        n++;
        last_change = millis();
        if (texto[n+DISPLAY_LEN-1] == '\0') {
          n = 0;
        }
    }
}

void PlayState::setup() {
    espejo_activo = 0;
    mostrar_espejo_activo();
    digitalWrite(LED_GANASTE, LOW);
    Serial.println("track:juego");
}

void PlayState::loop()
{
    verificar_sensor2();
    verificar_sensor1();
    procesar_botonera();
    scrollear_texto();

#ifdef DEBUG
    if (digitalRead(DEBUG_PIN) == LOW) {
        change_state(gameover_state);    
    }
/*
    static int l = 0;
    if ((l++)%30 == 0) {
        Serial.print("debug:");
        //Serial.print(sensor1);
        Serial.print(" - ");
        //Serial.println(sensor2);
    }
*/
#endif
}


