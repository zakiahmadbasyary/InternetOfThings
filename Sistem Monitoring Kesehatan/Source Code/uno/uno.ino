#include <SoftwareSerial.h>
SoftwareSerial SMESerial(6, 7);
#include <PulseSensorPlayground.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pulse Sensor Setup
#define USE_ARDUINO_INTERRUPTS true
const int PulseWire = A0;
int Threshold = 550;
PulseSensorPlayground pulseSensor;

// DS18B20 Setup
const int ONE_WIRE_BUS = 2; // Pin data untuk sensor DS18B20
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// LCD I2C Setup
LiquidCrystal_I2C lcd(0x27, 16, 2); // Address 0x27, 16 chars, 2 lines
int myBPMFiks = 0; // Variabel untuk menyimpan nilai BPM
bool beatDetected = false; // Indikator apakah BPM terdeteksi

void setup() {
  // Serial Communication Setup
  Serial.begin(9600);
  SMESerial.begin(9600);
  Serial.println("Serial Begin");

  // Pulse Sensor Setup
  pulseSensor.analogInput(PulseWire);
  pulseSensor.setThreshold(Threshold);
  delay(2000);

  if (pulseSensor.begin()) {
    Serial.println("Pulse Sensor Initialized");
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Pulse Sensor OK");
  } else {
    Serial.println("Pulse Sensor Error");
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Pulse Sensor Err");
  }

  // DS18B20 Setup
  sensors.begin();
  lcd.setCursor(0, 1);
  lcd.print("Temp Sensor Init");
  Serial.println("Temperature Sensor Initialized");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Reset indikator deteksi denyut pada setiap loop
  beatDetected = false;
  myBPMFiks=0;

  // Pulse Sensor BPM Measurement
  int myBPM = pulseSensor.getBeatsPerMinute();
  if (pulseSensor.sawStartOfBeat()) {
    beatDetected = true; // Set indikator deteksi denyut

    // // Display BPM on LCD
    // lcd.setCursor(0, 0);
    // lcd.print("BPM: ");
    // lcd.print(myBPM);
    // lcd.print("   "); // Clear trailing characters

    // // Send BPM to Serial Monitor and SMESerial
    // Serial.print("BPM: ");
    // Serial.println(myBPM);
    // SMESerial.print(myBPM);
    // SMESerial.print('|');

      // // Jika tidak ada deteksi denyut, set nilai BPM ke 0
    if (myBPM > 0) {
      myBPMFiks=myBPM;

    }

  }
  lcd.setCursor(0, 0);
  lcd.print("BPM: ");
  lcd.print(myBPMFiks);
  lcd.print("   "); // Clear trailing characters

  Serial.println("No BPM detected, BPM set to 0");
  SMESerial.print(myBPMFiks);
  SMESerial.print('|');


  // DS18B20 Temperature Measurement
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);

  if (temperatureC != DEVICE_DISCONNECTED_C) {
    // Display temperature on LCD
    lcd.setCursor(0, 1);
    lcd.print("Temp: ");
    lcd.print(temperatureC);
    lcd.print((char)223);
    lcd.print("C   "); // Clear trailing characters

    // Send Temperature to Serial Monitor and SMESerial
    Serial.print("Temp: ");
    Serial.print(temperatureC);
    Serial.println(" C");
    SMESerial.print(temperatureC);
    SMESerial.print('|');
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Temp Sensor Err");
    Serial.println("Temperature Sensor Error");
  }

  delay(2000);
}
