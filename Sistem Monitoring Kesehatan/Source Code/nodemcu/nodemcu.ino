#define BLYNK_TEMPLATE_ID "TMPL6IurRfGpH"
#define BLYNK_TEMPLATE_NAME "kesehatan"
#define BLYNK_AUTH_TOKEN "1gqXTM2NqMvOAoNiicShuJdBVsgnObxN"

#define BLYNK_PRINT Serial

#include <SoftwareSerial.h>
#include <BlynkSimpleEsp8266.h>  // Library untuk NodeMCU ESP8266
#include <ESP8266WiFi.h>



// Pin untuk komunikasi serial dengan Arduino
SoftwareSerial SMESerial(D6, D7); // Pin RX dan TX untuk komunikasi dengan Arduino Uno

// Koneksi WiFi dan Blynk
char auth[] = BLYNK_AUTH_TOKEN;  // Ganti dengan Blynk Auth Token Anda
char ssid[] = "Nimo";         // Ganti dengan SSID WiFi Anda
char pass[] = "05072004";     // Ganti dengan password WiFi Anda
BlynkTimer timer;
int bpmFiks=0;

void setup() {
  // Mulai komunikasi serial dengan PC dan Arduino
  Serial.begin(9600);
  SMESerial.begin(9600);  // Untuk komunikasi dengan Arduino Uno
  delay(100);


  // Mulai koneksi Blynk
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  timer.setInterval(1000L, sensor);
}

void loop() {
  // Menjalankan loop Blynk
  Blynk.run();
  timer.run();
}

void sensor() {
  bpmFiks=0;
  // Cek apakah ada data dari Arduino Uno
  if (SMESerial.available() < 1) return;

  // Membaca data yang diterima dari Arduino Uno
  String sensorData = "";
  while (SMESerial.available()) {
    sensorData += (char)SMESerial.read();
  }

  // Pisahkan data BPM dan suhu berdasarkan tanda '|'
  int separatorIndex = sensorData.indexOf('|');
  if (separatorIndex > 0) {
    String bpmStr = sensorData.substring(0, separatorIndex);
    String tempStr = sensorData.substring(separatorIndex + 1);

    // Konversi ke integer dan float
    int bpm = bpmStr.toInt();
    float temperatureC = tempStr.toFloat();
    if(bpm > 0){
      bpmFiks=bpm;
    }

    // Kirim data BPM dan suhu ke Blynk
    Blynk.virtualWrite(V0, bpmFiks);         // Kirim data BPM ke Blynk (virtual pin V0)
    Blynk.virtualWrite(V1, temperatureC); // Kirim data suhu ke Blynk (virtual pin V1)

    // Tampilkan data di Serial Monitor untuk debug
    Serial.print("BPM: ");
    Serial.println(bpmFiks);
    Serial.print("Temp: ");
    Serial.println(temperatureC);
  }
}
