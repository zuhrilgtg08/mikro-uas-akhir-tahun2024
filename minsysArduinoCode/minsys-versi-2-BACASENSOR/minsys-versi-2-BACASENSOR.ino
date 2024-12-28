#include <DHT.h>
#define DHTPIN 4    
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);
#define SOIL_PIN A0
#define LDR A1
#define TRIGGPIN 7
#define ECHOPIN 8

void setup() {
  // Serial.begin(115200);
  Serial.begin(9600);

  dht.begin();
  pinMode(TRIGGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
}

void loop() {
  float temp = dht.readTemperature() - 2.0;
  float humi = dht.readHumidity();

  int lightValue =  analogRead(LDR);

  int bacaSoil = analogRead(SOIL_PIN);
  float dataSoil = (100 - ((bacaSoil/1023.00) * 100));

  float dataLevelAir = pembacaanJarak();
  double ketinggianAir = 0.30 - dataLevelAir; //nilai 0.30 dlm meter yaitu ukuran tanki yaitu 0.30 meter

  if(ketinggianAir <= 0 || dataLevelAir <= 0 || dataLevelAir > 0.30 )  {
    ketinggianAir = 0;
    dataLevelAir = 0;
    // Serial.println("Error: Jarak tidak valid");
    // Serial.println("------------------------------------------");
  }

  // Serial.print("Jarak ke Permukaan Air: ");
  // Serial.print(dataLevelAir, 2); 
  // Serial.println(" m");
  // Serial.println("------------------------------------------");
  // delay(1000);

  // Serial.print("Level Air: ");
  // Serial.print(ketinggianAir, 2); 
  // Serial.println(" m");
  // Serial.println("------------------------------------------");
  // delay(1000);

  // Serial.print("Data Soil Moisture: ");
  // Serial.print(dataSoil); 
  // Serial.println(" %");
  // Serial.println("------------------------------------------");
  // delay(1000);

  // Serial.print("Data LDR: ");
  // Serial.print(lightValue); 
  // Serial.println(" LUX");
  // Serial.println("------------------------------------------");
  // delay(2000);

  if (isnan(humi) || isnan(temp) ) {   
    Serial.println("DHT11 ERROR");
    return 0;
  } 
  kirimDatatoMcu8266(lightValue, ketinggianAir, dataSoil, temp);

  //else {
  //   Serial.print("Temperature: ");
  //   Serial.print(temp);
  //   Serial.println(" °C");
  //   Serial.println("------------------------------------------");
  //   delay(1000);

  //   Serial.print("Humidity: ");
  //   Serial.print(humi);
  //   Serial.println(" %");
  //   Serial.println("------------------------------------------");
  // }
    delay(300);
}

// sudah terkonvert ke meter buat ketinggan dari permukaan air
float pembacaanJarak() {
  long duration;
  double meter;
  digitalWrite(TRIGGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGPIN, LOW);

  duration = pulseIn(ECHOPIN, HIGH);
  meter = (duration * 0.034 / 2) / 100;
  return meter;
}

// Kirim Data ke NodeMCU8266
void kirimDatatoMcu8266(int ldr, double ultrasonic, float soil, float temp) {
  String kirimData = String(ldr) + "_" + String(ultrasonic) + "_" + String(soil) + "_" + String(temp);
  Serial.println(kirimData);
}