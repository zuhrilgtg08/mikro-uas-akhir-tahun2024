#include <Adafruit_Sensor.h>
#include <NewPing.h>
#include <DHT.h>
#include <Servo.h>

#define DHTPIN 4
#define LDR A1
#define LED 6
#define DHTTYPE DHT22
#define TRIGPIN 12
#define ECHOPIN 13
#define SENSOR_MOISTURE A0
#define BUZZER 5
#define RELAY_PIN 10

DHT dht(DHTPIN, DHTTYPE);
Servo servoVentilasi;
Servo servoKatupAir;
NewPing sonar(TRIGPIN, ECHOPIN, 100);

const float tankiEmptyLevel = 10.0;
const float tankiFullLevel = 100.0;
const int batasKelembapanTanah = 700; //Batas nilai ADC kelembapan tanah
bool emergency = false;

void setup() { 
  Serial.begin(9600);
  dht.begin();

  // pinMode Aktuator
  servoVentilasi.attach(8);
  servoVentilasi.write(0);
  servoKatupAir.attach(9);
  servoKatupAir.write(0);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  pinMode(LED, OUTPUT);
  digitalWrite(LED,LOW);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
}

void loop() {
  delay(500);
  int lightValue = analogRead(LDR);
  int valMoisture = analogRead(SENSOR_MOISTURE);
  float temp = dht.readTemperature() - 2.0;
  float humi = dht.readHumidity();
  float levelAir = bacaUltrasonic();

  if(!isnan(temp) && !isnan(humi)) {
    // Tampilkan hasil Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println("°C");
   
    Serial.print("Kelembapan: ");
    Serial.print(humi);
    Serial.println("%");
    Serial.println("--------------------------");

    Serial.print("Intesitas Cahaya: ");
    Serial.println(lightValue);

    Serial.print("Nilai ADC: ");
    Serial.println(valMoisture);

    Serial.print("Water Level: ");
    Serial.print(levelAir);
    Serial.println(" cm");
    Serial.println("--------------------------");
  } else {
    Serial.println("Gagal Baca DHT sensor!");
  }

  // Buka Jendela JIka Panas
  if(temp > 35.00) {
    servoVentilasi.write(90);
    Serial.println("servo VENTILASI on");
  } else {
    servoVentilasi.write(0);
    Serial.println("servo VENTILASI off");
  } 

  // Jika Pada malam hari < 500 lux valuenya
  if (lightValue < 500) {
    digitalWrite(LED, HIGH); // LED menyala jika malam
    Serial.println("LED: ON");
  } else {
    digitalWrite(LED, LOW);  // LED mati jika siang
    Serial.println("LED: OFF");
  }

  // Kontrol pompa air berdasarkan level air
  if (levelAir < tankiEmptyLevel) {
    digitalWrite(RELAY_PIN, HIGH); // Pompa menyala
    Serial.println("Pompa: ON");
  } else if (levelAir > tankiFullLevel) {
    digitalWrite(RELAY_PIN, LOW); // Pompa mati
    Serial.println("Pompa: OFF");
  }

  // Kontrol servo katup air untuk irigasi berdasarkan kelembapan tanah
  if (valMoisture > batasKelembapanTanah) { // Jika tanah kering
    servoKatupAir.write(90); //servo terbuka 
    Serial.println("Katup: Buka");
  } else {
    servoKatupAir.write(0); // servo tertutup
    Serial.println("Katup: Tutup");
  }

  // Kondisi ketika emergency buzzer menyala
  if(temp > 38.0 || valMoisture > 700 || levelAir > tankiFullLevel || levelAir < tankiEmptyLevel) {
    digitalWrite(BUZZER, HIGH); //Aktifkan BUzzer
    emergency = true;

    // Tampilkan semua status bagian mana saja yg emergency
    Serial.print("Darurat!!!");
    if(temp > 38.00) Serial.print("Suhu udara sangat panas!!");
    if(valMoisture > 700) Serial.print("Tanah Sangat Kering!!");
    if(levelAir > tankiFullLevel) Serial.print("Air Luber!!");
    if(levelAir < tankiEmptyLevel) Serial.print("Tanki Kosong!!");
    Serial.println("------------------------------------------");
  } else {
    digitalWrite(BUZZER, LOW);
  }
  emergency = false;
  delay(2000);
}

// Function ultrasonik baca level air 
float bacaUltrasonic() {
  int jarak = sonar.ping_cm();
  return jarak;
}