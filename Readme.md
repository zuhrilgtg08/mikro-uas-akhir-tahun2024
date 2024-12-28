# Proyek Monitoring Data Sensor ke NodeMCU8266

Program ini dirancang untuk membaca dan mengirimkan data dari berbagai sensor ke NodeMCU8266. Sistem ini memungkinkan pemantauan data seperti suhu, kelembapan, intensitas cahaya, kelembapan tanah, dan level ketinggian air. Data dari setiap sensor dikirimkan melalui protokol serial dalam format yang mudah diproses.

---

## Fitur Utama

### 1. Pemantauan Suhu dan Kelembapan
- Membaca suhu dan kelembapan menggunakan sensor **DHT11**.
- Menyesuaikan pembacaan suhu dengan **offset** untuk meningkatkan akurasi pengukuran.

### 2. Pemantauan Intensitas Cahaya
- Membaca data dari **sensor LDR** dalam satuan **LUX**.

### 3. Pemantauan Kelembapan Tanah
- Membaca data dari **Soil Moisture Sensor**.
- Mengkonversi data kelembapan tanah menjadi nilai dalam persentase (%).

### 4. Pemantauan Level Air
- Membaca jarak ke permukaan air menggunakan **sensor ultrasonik HC-SR04**.
- Menghitung ketinggian air dalam tangki berdasarkan dimensi tangki.

### 5. Pengiriman Data ke NodeMCU8266
- Mengirimkan data dari semua sensor dalam format string dengan delimiter `_` untuk memudahkan parsing oleh NodeMCU8266.

---

## Peralatan yang Dibutuhkan

1. **DHT11** - Sensor Suhu dan Kelembapan.
2. **LDR** - Sensor Cahaya.
3. **Soil Moisture Sensor** - Sensor Kelembapan Tanah.
4. **HC-SR04** - Sensor Ultrasonik untuk membaca jarak.
5. **Arduino Board** - Untuk membaca data sensor.
6. **NodeMCU8266** - Untuk menerima data melalui komunikasi serial.

---

## Rangkaian

### 1. **DHT11**
   - Data pin terhubung ke **pin digital 4** pada Arduino.
### 2. **LDR**
   - Terhubung ke **pin analog A1**.
### 3. **Soil Moisture Sensor**
   - Terhubung ke **pin analog A0**.
### 4. **HC-SR04 (Ultrasonic Sensor)**
   - **Trig pin** → Terhubung ke **pin digital 7**.
   - **Echo pin** → Terhubung ke **pin digital 8**.

---

## Format Pengiriman Data
Data yang dikirimkan dari Arduino ke NodeMCU8266 memiliki format sebagai berikut:


### Contoh Data

- **28.5**: Suhu dalam derajat Celcius.
- **65.2**: Kelembapan dalam persen (%).
- **450**: Intensitas cahaya dalam satuan LUX.
- **30**: Kelembapan tanah dalam persen (%).
- **120**: Ketinggian air dalam satuan cm.

---

## Cara Kerja
1. **Pembacaan Sensor**:
   - Arduino membaca data dari setiap sensor sesuai pin yang telah ditentukan.
   - Data diolah untuk memastikan akurasi dan relevansi.

2. **Pengiriman Data**:
   - Data sensor digabungkan menjadi satu string dengan `_` sebagai delimiter.
   - String ini dikirim melalui komunikasi serial ke NodeMCU8266.

3. **NodeMCU8266**:
   - NodeMCU menerima data serial dan memprosesnya untuk digunakan lebih lanjut (misalnya, ditampilkan pada web server atau aplikasi IoT).

---



# Kontrol Aktuator dengan ESP8266

Proyek ini menggunakan ESP8266 untuk memonitor sensor dan mengontrol berbagai aktuator seperti servo, relay, LED, dan buzzer melalui jaringan WiFi. Sistem ini dirancang untuk mengontrol irigasi, ventilasi, dan pompa air, serta menyediakan fitur monitoring sensor melalui halaman web.

## Fitur
1. **Monitoring Sensor**:
   - Sensor cahaya (LDR)
   - Sensor kelembapan tanah (Soil Moisture)
   - Sensor suhu (DHT11)
   - Sensor ultrasonik (HC-SR04)

2. **Kontrol Aktuator**:
   - **Servo**:
     - Irigasi (Servo1)
     - Ventilasi (Servo2)
   - **Relay**:
     - Mengontrol pompa air
   - **LED**:
     - Indikator pencahayaan
   - **Buzzer**:
     - Alarm keadaan darurat

3. **Endpoint HTTP**:
   - **GET**:
     - `/irigasi`: Status servo irigasi
     - `/ventilasi`: Status servo ventilasi
     - `/relayPompa`: Status relay pompa air
     - `/kontrolLed`: Status LED
     - `/sendDataArduino`: Data sensor dari Arduino
   - **POST**:
     - `/irigasi`: Mengontrol servo irigasi
     - `/ventilasi`: Mengontrol servo ventilasi
     - `/relayPompa`: Mengontrol relay pompa air
     - `/kontrolLed`: Mengontrol LED

## Kebutuhan Perangkat
- **Hardware**:
  - ESP8266
  - Servo motor (2 unit)
  - Relay modul
  - LED
  - Buzzer
  - Sensor DHT11
  - Sensor LDR
  - Sensor kelembapan tanah
  - Sensor ultrasonik HC-SR04
- **Software**:
  - Arduino IDE
  - Library:
    - `ESP8266WiFi`
    - `ESP8266WebServer`
    - `ArduinoJson`
    - `Servo`

## Instalasi dan Penggunaan

### 1. **Persiapan Hardware**
- Sambungkan perangkat sesuai skema berikut:
  - Servo irigasi ke pin `D1`
  - Servo ventilasi ke pin `D2`
  - Relay ke pin `D5`
  - Buzzer ke pin `D7`
  - LED ke pin `D8`
- Sambungkan sensor sesuai konfigurasi.

### 2. **Kode Program**
- Buka file di Arduino IDE.
- Sesuaikan nama dan password WiFi pada baris:
```cpp
  const char* ssid = "Yolo";
  const char* password = "Yowai#$2024";