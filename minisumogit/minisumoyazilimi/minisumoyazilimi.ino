#include <QTRSensors.h>

// Çizgi sensörleri için QTR kütüphanesi
QTRSensors qtr;
const uint8_t SensorCount = 6; // 6 sensör
uint16_t sensorValues[SensorCount];

// MZ80 IR Sensör Pinleri
const int irLeft = A0;    // Sol MZ80 sensörü
const int irCenter = A1;  // Orta MZ80 sensörü
const int irRight = A3;   // Sağ MZ80 sensörü (A2 yerine A3, çakışmayı önlemek için)

// Motor Pinleri (Motor sürücü kartına uygun)
const int motorLeftForward = 5;   // Sol motor ileri
const int motorLeftBackward = 6;  // Sol motor geri
const int motorRightForward = 9;  // Sağ motor ileri
const int motorRightBackward = 10;// Sağ motor geri

// Ayarlar
const int threshold = 500;        // Çizgi sensör eşik değeri
const int baseSpeed = 150;        // Temel hız (6V motorlara uygun)
const int maxSpeed = 255;         // Maksimum PWM değeri
unsigned long lastUpdate = 0;     // Zamanlama için
const long interval = 10;         // Güncellenme aralığı (ms)

// Değişkenler
int lastPosition = 0;

void setup() {
  // Motor pinlerini çıkış olarak ayarla
  pinMode(motorLeftForward, OUTPUT);
  pinMode(motorLeftBackward, OUTPUT);
  pinMode(motorRightForward, OUTPUT);
  pinMode(motorRightBackward, OUTPUT);

  // MZ80 IR sensör pinlerini giriş olarak ayarla
  pinMode(irLeft, INPUT);
  pinMode(irCenter, INPUT);
  pinMode(irRight, INPUT);

  // QTR sensör başlatma
  qtr.setTypeRC(); // RC mod
  qtr.setSensorPins((const uint8_t[]){A4, A5, A6, A7, A8, A9}, SensorCount); // A2 çakışmasını önlemek için güncellendi
  qtr.setEmitterPin(2); // IR LED kontrol pini

  Serial.begin(9600); // Hata ayıklam için
  delay(500); // Sensörlerin ısınma süresi
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdate >= interval) {
    lastUpdate = currentMillis;

    qtr.read(sensorValues); // Çizgi sensörlerinden değerleri oku

    // MZ80 IR sensörlerini oku
    bool leftDetected = digitalRead(irLeft);
    bool centerDetected = digitalRead(irCenter);
    bool rightDetected = digitalRead(irRight);

    // Çizgi pozisyonunu hesapla
    int position = readLineSensor();

    // Karar verme
    if (position == 0 && lastPosition == 0) { // Çizgi dışına çıktıysa
      moveBackward();
    } else if (centerDetected) { // Rakip önde
      moveForward(baseSpeed + 50);
    } else if (leftDetected) { // Rakip solda
      turnLeft(baseSpeed + 50);
    } else if (rightDetected) { // Rakip sağda
      turnRight(baseSpeed + 50);
    } else { // Rakip algılanmadıysa
      searchForOpponent();
    }

    lastPosition = position;
    debugOutput(position, leftDetected, centerDetected, rightDetected); // Hata ayıklam için
  }
}

// Ağırlıklı ortalama ile çizgi pozisyon hesaplama
int readLineSensor() {
  unsigned int sum = 0;
  int weightedSum = 0;
  for (int i = 0; i < SensorCount; i++) {
    if (sensorValues[i] > threshold) {
      sum += sensorValues[i];
      weightedSum += sensorValues[i] * (i - SensorCount / 2);
    }
  }
  return (sum > 0) ? (weightedSum / sum) : 0;
}

// Rakip arama modu
void searchForOpponent() {
  static bool turnLeft = true;
  static unsigned long turnStart = 0;
  if (millis() - turnStart < 300) {
    motorControl(turnLeft ? -baseSpeed : baseSpeed, turnLeft ? baseSpeed : -baseSpeed);
  } else {
    turnLeft = !turnLeft;
    turnStart = millis();
  }
}

// İleri hareket
void moveForward(int speed) {
  motorControl(speed, speed);
}

// Geri hareket
void moveBackward() {
  motorControl(-baseSpeed, -baseSpeed);
  delay(200); // Kısa süre geri git
  searchForOpponent(); // Arama moduna dön
}

// Sola dönüş
void turnLeft(int speed) {
  motorControl(-baseSpeed, baseSpeed);
}

// Sağa dönüş
void turnRight(int speed) {
  motorControl(baseSpeed, -baseSpeed);
}

// Motor kontrol fonksiyonu
void motorControl(int leftSpeed, int rightSpeed) {
  leftSpeed = constrain(leftSpeed, -maxSpeed, maxSpeed);
  rightSpeed = constrain(rightSpeed, -maxSpeed, maxSpeed);

  if (leftSpeed >= 0) {
    analogWrite(motorLeftForward, leftSpeed);
    analogWrite(motorLeftBackward, 0);
  } else {
    analogWrite(motorLeftForward, 0);
    analogWrite(motorLeftBackward, -leftSpeed);
  }

  if (rightSpeed >= 0) {
    analogWrite(motorRightForward, rightSpeed);
    analogWrite(motorRightBackward, 0);
  } else {
    analogWrite(motorRightForward, 0);
    analogWrite(motorRightBackward, -rightSpeed);
  }
}

// Hata ayıklam için seri çıktı
void debugOutput(int position, bool left, bool center, bool right) {
  Serial.print("Pos: "); Serial.print(position);
  Serial.print(" | IR: L"); Serial.print(left);
  Serial.print(" C"); Serial.print(center);
  Serial.print(" R"); Serial.println(right);
}