// Dependência: QTRSensors by Pololu v4.0.0
// Repositório oficial: https://github.com/pololu/qtr-sensors-arduino

#include <QTRSensors.h>

QTRSensors moduloSensores;

const uint8_t NUM_SENSORES = 8;
const uint8_t TEMPO_CALIBRAGEM_S = 10;
uint16_t valorDosSensores[NUM_SENSORES];

// const uint8_t PIN_IR = 4;
const uint8_t PIN_D1 = 34;
const uint8_t PIN_D2 = 35;
const uint8_t PIN_D3 = 32;
const uint8_t PIN_D4 = 33;
const uint8_t PIN_D5 = 25;
const uint8_t PIN_D6 = 26;
const uint8_t PIN_D7 = 27;
const uint8_t PIN_D8 = 13;
const uint8_t PIN_LED_ESP32 = 2;

uint16_t iteracoesCalibragem() {
  const double TEMPO_ADC_S = 40e-6;
  const uint8_t AMOSTRAS = 4;
  const uint8_t LEITURAS = 10;
  const double TEMPO_POR_ITERACAO_S = TEMPO_ADC_S * AMOSTRAS * NUM_SENSORES * LEITURAS;

  if (TEMPO_CALIBRAGEM_S == 0) {
    return 1;
  } else {
    return static_cast<uint16_t>(TEMPO_CALIBRAGEM_S / TEMPO_POR_ITERACAO_S);
  }
}

void setup() {
  // Configuração dos sensores
  moduloSensores.setTypeAnalog();
  moduloSensores.setSensorPins((const uint8_t[]){ PIN_D8, PIN_D7, PIN_D6, PIN_D5, PIN_D4, PIN_D3, PIN_D2, PIN_D1 }, NUM_SENSORES);
  // moduloSensores.setEmitterPin(PIN_IR);

  // Calibragem
  delay(500);
  pinMode(PIN_LED_ESP32, OUTPUT);
  digitalWrite(PIN_LED_ESP32, HIGH);

  for (uint16_t i = 0; i < iteracoesCalibragem(); i++) {
    moduloSensores.calibrate();
  }

  digitalWrite(PIN_LED_ESP32, LOW);

  Serial.begin(115200);
  Serial.println("Valores mínimos e máximos medidos durante a calibragem:");

  for (uint8_t i = 0; i < NUM_SENSORES; i++) {
    Serial.print(moduloSensores.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();

  for (uint8_t i = 0; i < NUM_SENSORES; i++) {
    Serial.print(moduloSensores.calibrationOn.maximum[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);
}

void loop() {
  uint16_t posicao = moduloSensores.readLineWhite(valorDosSensores);

  // Printa o valor dos sensores (0 é o máximo de refletância e 1000 o
  // mínimo) e na última coluna a posição da linha.
  for (uint8_t i = 0; i < NUM_SENSORES; i++) {
    Serial.print(valorDosSensores[i]);
    Serial.print('\t');
  }
  Serial.println(posicao);

  // delay(100);
}
