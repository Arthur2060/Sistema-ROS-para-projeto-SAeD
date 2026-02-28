#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <cmath>
#include <vector>
#include <Wire.h>

Adafruit_MPU6050 mpu;
sensors_event_t a, g, temp;

const float DISTANCIA_MEDIDA = 4.0;
const float DIMENSAO_DE_QUADRO = 1.0;

float calcularDistanciaX(float distancia);
float calcularDistanciaY(float angulo);
std::vector<int> calcularCelula(float distanciaX, float distanciaY);

void configurarAcelerometro();
void configurarGyroscopio();
void configurarTermometro();

bool tabuleiro[26][26] = {}

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10);

  Serial.println("Adafruit MPU6050 teste!");

  if (!mpu.begin()) {
    Serial.println("Acelerometro nao encontrado!");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 pronto!");

  configurarAcelerometro();
  configurarGyroscopio();
  configurarTermometro();
  
  Serial.println("");
  delay(100);
}

void loop() {
  mpu.getEvent(&a, &g, &temp);

  float angulo = a.acceleration.y * 10;

  float distanciaX = calcularDistanciaX(angulo);
  float distanciaY = calcularDistanciaY(distanciaX);

  int TAMANHO_MAPA_VERTICAL = 26;
  int TAMANHO_MAPA_HORIZONTAL = 26;
  
  for (int linha = 0 ; linha <= TAMANHO_MAPA_VERTICAL ; linha++) {
    for (int coluna = 0 ; coluna <= TAMANHO_MAPA_HORIZONTAL ; coluna++) {
      int coordenadas[] = {coluna, linha};
      if (calcularCelula(coluna, linha)[0] == coordenadas[0] &&
          calcularCelula(coluna, linha)[1] == coordenadas[1]
        ) {
        tabuleiro[linha][coluna] = false;
      } else {
        tabuleiro[linha][coluna] = true;
      }
    }
  }

  for (int linha = 0 ; linha <= TAMANHO_MAPA_VERTICAL ; linha++) {
    for (int coluna = 0 ; coluna <= TAMANHO_MAPA_HORIZONTAL ; coluna++) {
      if (tabuleiro[linha][coluna]) {
        Serial.print("[ ]");
      } else {
        Serial.print("[X]");
      }
      Serial.println("");
    }
  }
  delay(1000);
}

float calcularDistanciaY(float angulo) {
  return sin(angulo) * DISTANCIA_MEDIDA;
}

float calcularDistanciaX(float distanciaX) {
  float cateto_adjacente = pow(distanciaX, 2);
float cateto_oposto = pow(DISTANCIA_MEDIDA, 2) - pow(distanciaX, 2);
return sqrt(cateto_oposto);
}

std::vector<int> calcularCelula(float distanciaX, float distanciaY)
{
  int X = distanciaX / DIMENSAO_DE_QUADRO;
  int Y = distanciaY / DIMENSAO_DE_QUADRO;
  
  return {Y, X};
}

void configurarAcelerometro() {
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
}

void configurarGyroscopio() {
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }
}

void configurarTermometro() {
    mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
}