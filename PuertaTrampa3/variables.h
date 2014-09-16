#define DEBUG

int PUERTA_CERRADA = 1000;
int PUERTA_ABIERTA = 0;
int POCA_LUZ = 180;
int MUCHA_LUZ = 250;
int DELAY_BEFORE_FULLSTOP = 2000;
int VELOCIDAD_ABRIENDO = 5000;
int ACELERACION_ABRIENDO = 1000;
int VELOCIDAD_CERRANDO = 100;
int ACELERACION_CERRANDO = 20;
int VELOCIDAD_ESPEJO1 = 50;
int ACELERACION_ESPEJO1 = 30;
int VELOCIDAD_ESPEJO2 = 50;
int ACELERACION_ESPEJO2 = 30;
int VELOCIDAD_ESPEJO3 = 50;
int ACELERACION_ESPEJO3 = 30;
int MOVIMIENTO_ESPEJO = 200;
int DELAY_LOOP = 0;

int* variables[] = {
    &PUERTA_CERRADA,
    &PUERTA_ABIERTA,
    &POCA_LUZ,
    &MUCHA_LUZ,
    &DELAY_BEFORE_FULLSTOP,
    &VELOCIDAD_ABRIENDO,
    &ACELERACION_ABRIENDO,
    &VELOCIDAD_CERRANDO,
    &ACELERACION_CERRANDO,
    &VELOCIDAD_ESPEJO1,
    &ACELERACION_ESPEJO1,
    &VELOCIDAD_ESPEJO2,
    &ACELERACION_ESPEJO2,
    &VELOCIDAD_ESPEJO3,
    &ACELERACION_ESPEJO3,
    &MOVIMIENTO_ESPEJO,
    &DELAY_LOOP,
    0
};
