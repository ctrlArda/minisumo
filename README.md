mz80 çalışma kontrol

int sensorPin = 2;  // Sensörün OUT pinini bağladığınız pin
int sensorState = 0;  // Sensörün durumunu saklamak için değişken

void setup() {
  pinMode(sensorPin, INPUT);  // Sensör pinini giriş olarak ayarlayın
  Serial.begin(9600);         // Seri haberleşmeyi başlatın
}

void loop() {
  sensorState = digitalRead(sensorPin);  // Sensör çıkışını oku
  if (sensorState == HIGH) {
    Serial.println("Kızılötesi ışık algılandı!");
  } else {
    Serial.println("Kızılötesi ışık algılanmadı.");
  }
  delay(1000);  // 1 saniye bekle
}

QTR8-a için

int sensorPin = A0;  // QTR-8A sensörünün OUT pinini bağladığınız pin (A0)
int sensorValue = 0; // Sensörden alınan değeri saklamak için değişken

void setup() {
  Serial.begin(9600);  // Seri haberleşmeyi başlatın
}

void loop() {
  sensorValue = analogRead(sensorPin);  // Sensörden analog değeri oku

  // Sensör değerini seri monitöre yazdır
  Serial.print("Sensör değeri: ");
  Serial.println(sensorValue);
  
  delay(1000);  // 1 saniye bekle
}



Adım 1: Gerekli Donanımı Bağlama
L298 motor sürücü kartının bağlantıları şu şekildedir:

VCC – Arduino'nun 5V pinine bağlanır (L298 kartının kontrol devresi için).
GND – Arduino'nun GND pinine bağlanır.
IN1, IN2, IN3, IN4 – Motorları kontrol etmek için Arduino'nun dijital pinlerine bağlanır (örneğin IN1 → D3, IN2 → D4, IN3 → D5, IN4 → D6).
OUT1, OUT2 – Motorun birinci çıkışı (DC motor 1 için).
OUT3, OUT4 – Motorun ikinci çıkışı (DC motor 2 için).
+12V – Motorun güç kaynağı (genellikle motorlar için ayrı bir güç kaynağı gereklidir).
Bağlantılarınızı şu şekilde yapabilirsiniz:

VCC → 5V (Arduino)
GND → GND (Arduino)
IN1 → D3 (Arduino)
IN2 → D4 (Arduino)
IN3 → D5 (Arduino)
IN4 → D6 (Arduino)
OUT1 → Motor 1
OUT2 → Motor 1
OUT3 → Motor 2
OUT4 → Motor 2
+12V → Motorların güç kaynağı (örneğin, 12V pil veya adaptör).
Adım 2: Arduino IDE'yi Açma
Arduino IDE'yi açın.
Bağlantı ve kart modelini doğru seçtiğinizden emin olun (Tools > Board > Arduino Uno, Tools > Port'tan doğru portu seçin).
Adım 3: Kodu Yazma
L298 ile motorları test etmek için aşağıdaki basit kodu kullanabilirsiniz. Bu kod, motorları döndürerek L298'in çalışıp çalışmadığını kontrol eder.

cpp
Kodu kopyala
// Motor sürücü pinleri
int in1 = 3;
int in2 = 4;
int in3 = 5;
int in4 = 6;

void setup() {
  // Motor kontrol pinlerini çıkış olarak ayarla
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void loop() {
  // Motor 1'i döndür
  digitalWrite(in1, HIGH);  // Motor 1 yönü
  digitalWrite(in2, LOW);   // Motor 1 yönü
  delay(1000);  // 1 saniye bekle

  // Motor 1'i durdur
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  delay(1000);  // 1 saniye bekle

  // Motor 2'yi döndür
  digitalWrite(in3, HIGH);  // Motor 2 yönü
  digitalWrite(in4, LOW);   // Motor 2 yönü
  delay(1000);  // 1 saniye bekle

  // Motor 2'yi durdur
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(1000);  // 1 saniye bekle
}
Adım 4: Kodu Yükleme
Kodu Arduino'ya yüklemek için:

Arduino'nuzu bilgisayarınıza bağlayın.
"Yükle" butonuna tıklayın (ekranda ok işareti bulunan buton).
Kodu yükledikten sonra, motorların düzgün bir şekilde döndüğünden emin olun. Seri monitör kullanmanıza gerek yoktur, çünkü motorları görsel olarak test edebilirsiniz.

Adım 5: Test Etme
Arduino'yu çalıştırdıktan sonra motorların belirli bir yönde döndüğünü ve ardından durduğunu gözlemlemeniz gerekir. Kodda her bir motor için 1 saniye döndürme ve durdurma işlemi yapılır.
Motorlar, L298 sürücü kartı ile doğru şekilde yönlendirilmelidir.
Ekstra Notlar:
Eğer motorunuz dönmüyorsa:
Bağlantıları ve motorun doğru şekilde güç aldığından emin olun.
Motorun güç kaynağının yeterli voltajda olduğuna dikkat edin.
L298 kartının yön pinlerinin doğru bağlandığından emin olun.
Bu adımlarla, L298 motor sürücü kartını Arduino ile test edebilirsiniz.
