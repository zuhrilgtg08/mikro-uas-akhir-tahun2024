#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <Servo.h>

ESP8266WebServer server(80);

Servo servo1;
Servo servo2;
const char* ssid = "Yolo";
const char* password = "Yowai#$2024";

const int servoAir = D1; 
const int servoVent = D2; 
const int relay = D5;
const int buzzer = D7;
const int led = D8;

bool stateServoAir = false;
bool stateServoVent = false;
bool stateRelay = false;
bool stateLed = false;

void setup() {
  // Serial.begin(115200);
  Serial.begin(9600);

  pinMode(relay, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(relay, LOW);
  digitalWrite(buzzer, LOW);
  digitalWrite(led, LOW);

  servo1.attach(servoAir);
  servo2.attach(servoVent);
  servo1.write(0); 
  servo2.write(0); 

  // Debug Servo1
  if(!servo1.attached()) Serial.println("Servo Irigasi Gagal inisialisasi");

  // Koneksi WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    Serial.println("Menghubungkan ke WiFi...");
  }
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("");
  Serial.print("WiFi Terhubung! ");
  Serial.println(ssid);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // setup server
  server.on("/irigasi", HTTP_GET, getServoIrigasi);
  server.on("/ventilasi", HTTP_GET, getServoVentilasi);
  server.on("/relayPompa", HTTP_GET, getRelayPump);
  server.on("/kontrolLed", HTTP_GET, getLedControl);

  server.on("/irigasi", HTTP_POST, setServoIrigasi);
  server.on("/ventilasi", HTTP_POST, setServoVentilasi);
  server.on("/relayPompa", HTTP_POST, setRelayPump);
  server.on("/kontrolLed", HTTP_POST, setLedControl);

  // Endpoint untuk mengirim data dari arduino ke halaman web
  server.on("/sendDataArduino", HTTP_GET, kirimDatatoWeb);

  server.begin();
  Serial.print("Server is Running....");
}

void split(String data, char delimiter, String parts[], int maxParts) {
  int currentIndex = 0;
  int startIndex = 0;

  for(int i = 0; i < data.length(); i++) {
    if(data[i] == delimiter) {
      if(currentIndex < maxParts) {
        parts[currentIndex] = data.substring(startIndex, i);
        currentIndex++;
        startIndex = i + 1;
      }
    }
  }

  if(currentIndex < maxParts) {
    parts[currentIndex] = data.substring(startIndex);
  }
}

void loop() {
  server.handleClient();
  String dataArduino = " ";

  while(Serial.available() > 0) {
    // Baca data dari arduino
    dataArduino += char(Serial.read());
  }

  Serial.print("Data Kiriman arduino: ");
  dataArduino.trim();
  Serial.println(dataArduino);
  delay(500);

  if(dataArduino.length() > 0) {
    const int banyaknyaData = 4;
    String arrayParts[banyaknyaData];

    split(dataArduino, '_', arrayParts, banyaknyaData);

    int nilaiLdr = arrayParts[0].toInt();
    double nilaiHcsr04 = arrayParts[1].toDouble();
    float nilaiSoil = arrayParts[2].toFloat();
    float nilaiDht = arrayParts[3].toFloat();
    int konvertUltrasonic = nilaiHcsr04 * 100;

    // Debug output
    Serial.println("Nilai LDR: " + String(nilaiLdr));
    Serial.println("Nilai Ultrasonic: " + String(nilaiHcsr04));
    Serial.println("Nilai Soil Moisture: " + String(nilaiSoil));
    Serial.println("Nilai DHT: " + String(nilaiDht));
    Serial.println("Nilai Konvert HC-SR04: " + String(konvertUltrasonic));


    // ldr < 300 lux maka led nyala
    if (nilaiLdr < 300) {
      digitalWrite(led, HIGH);
    } else {
      digitalWrite(led, LOW);
    }

    // jika temperature > 35.0
    if(nilaiDht > 35.0) {
      servo2.write(90);
      delay(4000);
    } else {
      servo2.write(0);
    }

    // jika tanki kosong untuk kontrol pump air
    if(konvertUltrasonic < 50) {
      digitalWrite(relay, HIGH);
      Serial.println("Pump ON");
    } else if(konvertUltrasonic >= 100) {
      digitalWrite(relay, LOW);
      Serial.println("Pump OFF");
    }

    // kontrol irigasi jika nilai adc
    if(nilaiSoil < 30.00) {
      servo1.write(90);
      delay(4000);
    } else if(nilaiSoil >= 70.00) {
      servo1.write(0);
    }

    // buzzer emergency
    // if(nilaiDht > 36.00 || nilaiSoil > 73.00 || konvertUltrasonic > 100 || konvertUltrasonic < 50 || nilaiSoil < 28.00){
    //   digitalWrite(buzzer, HIGH);
    
    //   if(nilaiDht > 36.00) Serial.println("SUHU PANAS");
    //   if(nilaiSoil < 28.00) Serial.println("TANAH KERING");
    //   if(konvertUltrasonic > 100) Serial.println("TANKI LUBER");
    //   if(konvertUltrasonic < 50) Serial.println("TANKI EMPTY");
    // } else {
    //   digitalWrite(buzzer, LOW);
    // }

    // buzzer emergency opsi 2
    if(nilaiDht > 36.00 || nilaiSoil < 28.00){
      digitalWrite(buzzer, HIGH);
    
      if(nilaiDht > 36.00) Serial.println("SUHU PANAS");
      if(nilaiSoil < 28.00) Serial.println("TANAH KERING");
    } else {
      digitalWrite(buzzer, LOW);
    }
  }

  delay(2500);

  // Debug untuk belajar ambil data index pertama dari data sing dikirim
  // if(dataArduino.length() > 0) {
  //   int nilaiKirimanLdr = dataArduino.toInt();
  //   Serial.println("Nilai LDR: " + String(nilaiKirimanLdr));
  //   delay(3000);
  // }

}

// Untuk Monitoring Sensor ke Halaman WEB
void  kirimDatatoWeb() {
  String dt = Serial.readStringUntil('\n');
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", dt);
}

// Untuk Kontrol Manual Aktuator
void setServoIrigasi() {
  stateServoAir = !stateServoAir;
  servo1.write(stateServoAir ? 180 : 0);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", stateServoAir ? "ON" : "OFF");
}

void setServoVentilasi() {
  stateServoVent = !stateServoVent;
  servo2.write(stateServoVent ? 180 : 0);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", stateServoVent ? "ON" : "OFF");
}

void setRelayPump() {
  stateRelay = !stateRelay;
  digitalWrite(relay, stateRelay ? HIGH : LOW);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", stateRelay ? "ON" : "OFF");
}

void setLedControl() {
  stateLed = !stateLed;
  digitalWrite(led, stateLed ? HIGH : LOW);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", stateLed ? "ON" : "OFF");
}

void getServoIrigasi() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", stateServoAir ? "ON" : "OFF");
}

void getServoVentilasi() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", stateServoVent ? "ON" : "OFF");
}

void getRelayPump() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", stateRelay ? "ON" : "OFF");
}

void getLedControl() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", stateLed ? "ON" : "OFF");
}