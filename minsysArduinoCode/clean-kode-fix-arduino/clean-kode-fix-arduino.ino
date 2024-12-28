// #include <Adafruit_Sensor.h>
#include <NewPing.h>
#include <DHT.h>
#include <Servo.h>

// pin arduino inisial
#define DHTPIN 4
#define LDR A1
#define LED 6
#define DHTTYPE DHT22
#define TRIGPIN 9
#define ECHOPIN 10
#define SENSOR_MOISTURE A0
#define BUZZER 5
#define RELAY_PIN 11

// Konstan Variable global
const float tankiEmptyLevel = 5.0;
const float tankiFullLevel = 20.0;
const float batasKelembapanTanah = 70.00; //Batas nilai ADC kelembapan tanah
bool emergency = false;

// object deklarasi
DHT dht(DHTPIN, DHTTYPE);
Servo servoVentilasi;
Servo servoKatupAir;
NewPing sonar(TRIGPIN, ECHOPIN, 30);

void setup() {
  Serial.begin(9600);
  dht.begin();

  // pinMode Aktuator
  servoVentilasi.attach(8);
  servoKatupAir.attach(3);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED,LOW);
  digitalWrite(BUZZER, LOW);
  servoVentilasi.write(0);
  servoKatupAir.write(0);
}

void loop() {
  int lightValue = analogRead(LDR);
  int valMoisture = analogRead(SENSOR_MOISTURE);
  float persentaseKelembapan = (100 - ((valMoisture/1023.00) * 100));
  float temp = dht.readTemperature() - 2.0;
  float humi = dht.readHumidity();
  int levelAir = sonar.ping_cm();

  if(!isnan(temp) && !isnan(humi)) {
    displaySensorData(temp, humi, levelAir, persentaseKelembapan, lightValue);
    kontrolVentilasi(temp);
    kontrolLed(lightValue);
    kontrolPump(levelAir);
    kontrolIrigasi(persentaseKelembapan);
    cekEmergency(temp, persentaseKelembapan, levelAir);
  } else {
    Serial.println("Sensor Error!");
  }
  delay(2000);
}

// void displaySensorData
void displaySensorData(float t, float h, float w, int adc, int l) {
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" °C");
  Serial.println("------------------------------------------");
  delay(1000);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %");
  Serial.println("------------------------------------------");
  delay(1000);

  Serial.print("Water Level: ");
  Serial.print(w);
  Serial.println(" cm");
  Serial.println("------------------------------------------");
  delay(1000);

  Serial.print("Nilai ADC: ");
  Serial.print(adc);
  Serial.println(" #");
  Serial.println("------------------------------------------");
  delay(1000);

  Serial.print("LUX LDR: ");
  Serial.print(l);
  Serial.println(" Lux");
  Serial.println("------------------------------------------");
  delay(1000);
}

// void kontrolVentilasi
void kontrolVentilasi(float t) {
  if(t > 35.0) {
    servoVentilasi.write(90);
    Serial.println("Ventilasi Open");
    Serial.println("------------------------------------------");
  } else {
    servoVentilasi.write(0);
    Serial.println("Ventilasi Close");
    Serial.println("------------------------------------------");
  }
}

// void kontrolLed
void kontrolLed(int cahaya) {
  if(cahaya < 500) {
    digitalWrite(LED, HIGH);
    Serial.println("LED: ON");
    Serial.println("------------------------------------------");
  } else {
    digitalWrite(LED, LOW);
    Serial.println("LED: OFF");
    Serial.println("------------------------------------------");
  }
}

// void kontrolPump
void kontrolPump(float w) {
  if(w < tankiEmptyLevel) {
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("Pump ON");
    Serial.println("------------------------------------------");
  } else if(w > tankiFullLevel) {
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("Pump OFF");
    Serial.println("------------------------------------------");
  }
}

// void kontrolIrigasi
void kontrolIrigasi(float adc) {
  if(adc < 30.00) {
    servoKatupAir.write(90);
    Serial.println("Irigasi: ON");
    Serial.println("------------------------------------------");
  } else if(adc > batasKelembapanTanah) {
    servoKatupAir.write(0);
    Serial.println("Irigasi :OFF");
    Serial.println("------------------------------------------");
  }
}

// void cekEmergency
void cekEmergency(float t, int adc, float w) { 
  if(t > 38.00 || adc > 200 || w > tankiFullLevel || w < tankiEmptyLevel) {
    digitalWrite(BUZZER, HIGH);
    emergency = true;

    if(t > 38.00) Serial.println("SUHU PANAS");
    if(adc > 700) Serial.println("TANAH KERING");
    if(w > tankiFullLevel) Serial.println("TANKI LUBER");
    if(w < tankiEmptyLevel) Serial.println("TANKI EMPTY");
  } else {
    digitalWrite(BUZZER, LOW);
    if(emergency) emergency = false;
  }
}