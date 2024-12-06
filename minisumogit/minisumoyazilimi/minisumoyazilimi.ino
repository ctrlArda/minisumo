#include <QTRSensors.h>

// Çizgi sensörleri için QTR kütüphanesi
QTRSensors qtr;
const uint8_t SensorCount = 6; // Arduino Nano için 6 sensör kullanımı
uint16_t sensorValues[SensorCount]; // Sensör değerlerini saklayan dizi

// MZ80 IR Sensör Pinleri
const int irLeft = A0;   // Sol MZ80 sensörü
const int irCenter = A1; // Orta MZ80 sensörü
const int irRight = A2;  // Sağ MZ80 sensörü

// Motor Pinleri
const int motorLeftForward = 5;   // Sol motor ileri
const int motorLeftBackward = 6; // Sol motor geri
const int motorRightForward = 9; // Sağ motor ileri
const int motorRightBackward = 10; // Sağ motor geri

// Threshold ve Hız Ayarları
const int threshold = 500;  // Çizgi sensör threshold değeri
const int baseSpeed = 200;  // Robotun temel hızı
const int maxSpeed = 255;   // Maksimum hız

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

  // QTR sensör başlatma (6 sensör)
  qtr.setTypeRC(); // Sensörlerin RC modunda çalışmasını ayarla
  qtr.setSensorPins((const uint8_t[]){A3, A4, A5, A6, A7, A2}, SensorCount); 
  qtr.setEmitterPin(2); // IR LED kontrol pini

  Serial.begin(9600); // Seri iletişim başlatılır (test amaçlı)
}

void loop() {
  qtr.read(sensorValues); // Çizgi sensörlerinden değerleri oku

  // MZ80 IR sensörlerini oku
  bool leftDetected = digitalRead(irLeft);     // Sol MZ80 sensör
  bool centerDetected = digitalRead(irCenter); // Orta MZ80 sensör
  bool rightDetected = digitalRead(irRight);   // Sağ MZ80 sensör

  // Çizgi sensör pozisyonunu hesapla
  int position = readLineSensor();

  // Karar verme süreci
  if (position == 0) { 
    // Çizgi dışına çıktıysa geri git
    moveBackward();
  } else if (centerDetected) { 
    // Rakip önde → İleri saldır
    moveForward(baseSpeed + 50);
  } else if (leftDetected) { 
    // Rakip solda → Sola saldır
    turnLeft(baseSpeed + 50);
  } else if (rightDetected) { 
    // Rakip sağda → Sağa saldır
    turnRight(baseSpeed + 50);
  } else { 
    // Rakip algılanmadıysa arama modu
    searchForOpponent();
  }

  delay(10); // Döngü gecikmesi
}

// Çizgi sensörlerinden pozisyon hesaplama
int readLineSensor() {
  int position = 0; // Varsayılan pozisyon
  for (int i = 0; i < SensorCount; i++) {
    if (sensorValues[i] > threshold) {
      position = i - SensorCount / 2; // Merkezden sapma pozisyonu
      break;
    }
  }
  return position; // Pozisyonu döndür
}

// Rakip arama modu
void searchForOpponent() {
  static bool turnLeft = true; // Sola başlayarak döngü
  if (turnLeft) {
    motorControl(-baseSpeed, baseSpeed); // Sola dön
  } else {
    motorControl(baseSpeed, -baseSpeed); // Sağa dön
  }
  delay(500); // Dönüş süresi
  turnLeft = !turnLeft; // Yön değiştir
}

// İleri hareket
void moveForward(int speed) {
  motorControl(speed, speed); // Her iki motoru ileri çalıştır
}

// Geri hareket
void moveBackward() {
  motorControl(-baseSpeed, -baseSpeed); // Her iki motoru geri çalıştır
}

// Sola dönüş
void turnLeft(int speed) {
  motorControl(-baseSpeed, baseSpeed); // Sol motor geri, sağ motor ileri
}

// Sağa dönüş
void turnRight(int speed) {
  motorControl(baseSpeed, -baseSpeed); // Sağ motor geri, sol motor ileri
}

// Motor kontrol fonksiyonu
void motorControl(int leftSpeed, int rightSpeed) {
  if (leftSpeed > 0) {
    analogWrite(motorLeftForward, leftSpeed); // Sol motor ileri
    analogWrite(motorLeftBackward, 0);       // Sol motor geri kapalı
  } else {
    analogWrite(motorLeftForward, 0);        // Sol motor ileri kapalı
    analogWrite(motorLeftBackward, -leftSpeed); // Sol motor geri
  }

  if (rightSpeed > 0) {
    analogWrite(motorRightForward, rightSpeed); // Sağ motor ileri
    analogWrite(motorRightBackward, 0);         // Sağ motor geri kapalı
  } else {
    analogWrite(motorRightForward, 0);          // Sağ motor ileri kapalı
    analogWrite(motorRightBackward, -rightSpeed); // Sağ motor geri
  }
}
