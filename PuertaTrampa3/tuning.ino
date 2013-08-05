#ifdef DEBUG
void update_variables() {
    carrito.setMaxSpeed(VELOCIDAD_ABRIENDO);
    carrito.setAcceleration(ACELERACION_ABRIENDO);
    carrito.delay_before_fullstop = DELAY_BEFORE_FULLSTOP;

    espejo1.setMaxSpeed(VELOCIDAD_ESPEJO1);
    espejo1.setAcceleration(ACELERACION_ESPEJO1);
    espejo1.delay_before_fullstop = DELAY_BEFORE_FULLSTOP;

    espejo2.setMaxSpeed(VELOCIDAD_ESPEJO2);
    espejo2.setAcceleration(ACELERACION_ESPEJO2);
    espejo2.delay_before_fullstop = DELAY_BEFORE_FULLSTOP;

    espejo3.setMaxSpeed(VELOCIDAD_ESPEJO3);
    espejo3.setAcceleration(ACELERACION_ESPEJO3);
    espejo3.delay_before_fullstop = DELAY_BEFORE_FULLSTOP;
}

String inputString = "";

void parseLine() {
  if (inputString.startsWith("set ")) {
    String sIndex = inputString.substring(inputString.indexOf(" ")+1);
    int index = sIndex.toInt();
    String sValue = sIndex.substring(sIndex.indexOf(" ")+1);
    int value = sValue.toInt();
    *variables[index] = value;
    /*
    Serial.print("setting ");
    Serial.print(index);
    Serial.print("=");
    Serial.println(value);
    */
    update_variables();
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read(); 
    if (inChar == '\n') {
      parseLine();
      inputString = "";
    } else {
      inputString += inChar;
    }
  }
}
#endif


