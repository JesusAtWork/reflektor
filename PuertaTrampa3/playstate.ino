const int DISPLAY_UPDATE_INTERVAL = 100;

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
                play_track("alarma");
            }
            estado_puerta = ABRIENDO;
            carrito.setMaxSpeed(VELOCIDAD_ABRIENDO);
            carrito.setAcceleration(ACELERACION_ABRIENDO);
            carrito.move(-PUERTA_CERRADA * 2);
        }

        if (estado_puerta == CERRANDO && carrito.distanceToGo() == 0 ) {
            estado_puerta = CERRADA;
            play_track("game2");
        }
    }
}

void PlayState::verificar_sensor2() {
    int sensor2 = analogRead(SENSOR0);
    if (sensor2 > MUCHA_LUZ) {
        // ganaste!
        play_track("youwin");
        change_state (&gameover_state);
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
    boolean izquierdo = digitalRead(BOTON_IZQUIERDO) == LOW;
    boolean derecho = digitalRead(BOTON_DERECHO) == LOW;

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
        Stepper::train.setRegisterPin(LED_ESPEJO1+k, espejo_activo == k ? HIGH : LOW);
    }
}

void PlayState::procesar_boton_cambio_espejo() {
    static boolean estado_anterior = false;
    boolean estado_actual = digitalRead(BOTON_CAMBIO) == LOW;

    if ( estado_actual == true && estado_anterior == false) {
        espejo_activo = (espejo_activo + 1) % 3;
        mostrar_espejo_activo();
    }
    estado_anterior = estado_actual;
}

void PlayState::procesar_botonera() {
    procesar_botones_izquierda_derecha();
    procesar_boton_cambio_espejo();
}

const char *texto = "     REFLEKTOR [c] 2013 *#O CLUB DE JAQUEO *#O GPL - reflektor@protocultura.net       \0";

void PlayState::scrollear_texto() {
    if (millis() > last_change + DISPLAY_UPDATE_INTERVAL) {
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
    play_track("game1");
    tiempo_inicial = millis();
}

void PlayState::mostrar_energia() {
    if (millis() > last_change + DISPLAY_UPDATE_INTERVAL) {
        int energia = ENERGIA_INICIAL - (millis() - tiempo_inicial) / 16;
        display_show_energia(energia);
        last_change = millis();
        if (energia < 0) {
            play_track("gameover");
            change_state(&gameover_state);    
        }
    }
}

void titilar_led_ldr() {
  boolean estado = (millis() / 100)%2;
  
  if (estado_puerta == CERRADA) {
    Stepper::train.setRegisterPin(LED_SENSOR0, LOW);
    Stepper::train.setRegisterPin(LED_SENSOR1, estado);
  } else {
    Stepper::train.setRegisterPin(LED_SENSOR1, LOW);
    Stepper::train.setRegisterPin(LED_SENSOR0, estado);
  }
}

void PlayState::loop()
{
    Stepper::train.setRegisterPin(ENABLE_LASER, HIGH);
    verificar_sensor2();
    verificar_sensor1();
    procesar_botonera();
    titilar_led_ldr();
    //scrollear_texto();
    mostrar_energia();
    
#ifdef DEBUG
//    if (digitalRead(DEBUG_PIN) == LOW) {
//        change_state(gameover_state);    
//    }

    static int l = 0;
    if ((l++)%30 == 0) {
        int sensor0 = analogRead(SENSOR0);
        int sensor1 = analogRead(SENSOR1);
        Serial.print("debug:");
        Serial.print(sensor0);
        Serial.print(" - ");
        Serial.println(sensor1);
    }

#endif
}


