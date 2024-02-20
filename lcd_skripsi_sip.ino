#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


byte suhuIcon[8] = {
    0b11000,
    0b11000,
    0b00111,
    0b00100,
    0b00100,
    0b00100,
    0b00111,
    0b00000}; // icon suhu

byte tikIcon[8] = {
    0b00000,
    0b00000,
    0b00100,
    0b00000,
    0b00000,
    0b00100,
    0b00000,
    0b00000}; // icon titik

byte barisIcon[8] = {
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100}; // icon baris

    
int logam = 0;
int led_hijau = 6;
int led_merah = 7;
int led_kuning = 8;
int led_biru = 9;
int sensorPin = A0;             // Sambungkan pin OUT sensor ke pin A0 Arduino.
int sensorValue = 0;            // Variabel untuk menyimpan nilai yang diterima dari sensor
int roundedTemperature;         // Deklarasi variabel global
bool lcdHold = false;
bool holdButtonPressed = false; // apakah tombol "hold" sedang ditekan
const int normalMin = 0;
const int normalMax = 20;
const int panasMin = 21;
const int panasMax = 110;
const int proximityPin = 1;
const int holdButtonPin = 2;    // Pin untuk tombol menahan LCD
const int resetButtonPin = 3;   // Pin untuk tombol reset
const int buzzerPin = 5;        // Pin buzzer

float hasil_akhir;
float kalibrasi_panas, kalibrasi_dingin;

void setup()
{
  lcd.begin();
  lcd.createChar(0, suhuIcon);
  lcd.createChar(1, tikIcon);
  lcd.createChar(2, barisIcon);
  lcd.backlight();
  Serial.begin(9600);
  sensors.begin();
  
  // Inisialisasi pin mode sebagai input output
  pinMode(holdButtonPin, INPUT_PULLUP);
  pinMode(resetButtonPin, INPUT_PULLUP);
  pinMode(proximityPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(led_merah, OUTPUT);
  pinMode(led_hijau, OUTPUT);
  pinMode(led_kuning, OUTPUT);
  pinMode(led_biru, OUTPUT);
}

void loop()
{
// call sensors.requestTemperatures() to issue a global temperature
    // request to all devices on the bus
    sensors.requestTemperatures();
    // Get the temperature in Celsius and round it to the nearest integer
    float temperatureC = sensors.getTempCByIndex(0);
    roundedTemperature = round(temperatureC); // Mengisi nilai variabel global dan di bulatkan menggunakan kan round

  
  digitalWrite(buzzerPin, LOW);
//=================sensor logam===========================
  logam = digitalRead(proximityPin);
  
  digitalWrite(buzzerPin, LOW);
  if (logam == HIGH)
  {
//    Serial.println(0);            // Cetak "0" jika tidak ada objek
    digitalWrite(buzzerPin, LOW); // Matikan buzzer jika tidak ada objek
    digitalWrite(led_kuning, HIGH);
  }
  else
  {
    logam = analogRead(proximityPin); // Membaca nilai analog dari sensor logam
//    Serial.print("Object Detected - Nilai Sensor: ");
//    Serial.println(logam);
    if (logam > 100)
    {                                
      digitalWrite(buzzerPin, HIGH); // Aktifkan buzzer jika nilai sensor logam melebihi 100
      digitalWrite(led_biru, HIGH); //  Aktif jika logam terdeteksi
      digitalWrite(led_hijau, LOW);
      digitalWrite(led_kuning, LOW);
    }
    else
    {
      digitalWrite(buzzerPin, LOW); // Matikan buzzer jika nilai sensor logam tidak melebihi 100
      digitalWrite(led_merah, LOW);
      digitalWrite(led_kuning, HIGH); // Aktif jika logam tidak terdeteksi
    }
  }
  delay(150);

//===============akhir sensor logam========================
 
 sensorValue = analogRead(sensorPin);
if(sensorValue > 14 &&round(temperatureC)>21)
{
  Serial.println("Suhu");
  Serial.println(round(temperatureC));
  Serial.println();
  kalibrasi_dingin= sensorValue/sensorValue;
  hasil_akhir =  kalibrasi_dingin*sensorValue;
  

  
  
  digitalWrite(buzzerPin, LOW);
  digitalWrite(led_merah, LOW);
  digitalWrite(led_hijau, HIGH);
}
 else
{
  digitalWrite(buzzerPin, LOW);
  digitalWrite(led_merah, LOW);
  digitalWrite(led_hijau, LOW);
}

//kondisi jika oli jelek
if(round(temperatureC) > 30 && sensorValue > 800) {
    kalibrasi_panas = 95.5/sensorValue;
    hasil_akhir =  kalibrasi_panas*sensorValue;
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(led_merah, HIGH);
  digitalWrite(led_hijau, LOW);
 
 }else if(round(temperatureC) > 21 && round(temperatureC) < 30 && sensorValue > 800){
  kalibrasi_dingin = 155.5/sensorValue;
    hasil_akhir =  kalibrasi_dingin*sensorValue;
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(led_merah, HIGH);
  digitalWrite(led_hijau, LOW);
  
}else if(round(temperatureC)> 21 && round(temperatureC) < 30 && sensorValue > 32){
  kalibrasi_dingin = 232.5/sensorValue;
  hasil_akhir =  kalibrasi_dingin*sensorValue;
  digitalWrite(buzzerPin, LOW);
  digitalWrite(led_merah, LOW);
  digitalWrite(led_hijau, HIGH);

 }else if(round(temperatureC) > 30 && sensorValue > 32){
  kalibrasi_panas = 121.5/sensorValue;
  hasil_akhir =  kalibrasi_panas*sensorValue;
  digitalWrite(buzzerPin, LOW);
  digitalWrite(led_merah, LOW);
  digitalWrite(led_hijau, HIGH);
  
  }else{
  digitalWrite(buzzerPin, LOW);
  digitalWrite(led_merah, LOW);
  digitalWrite(led_hijau, LOW);
}
//===============akhir====================


  // Mengecek status tombol menahan LCD
  bool holdButtonState = digitalRead(holdButtonPin);
  if (holdButtonState == LOW && !holdButtonPressed)
  {
    lcdHold = !lcdHold;
    holdButtonPressed = true;
    digitalWrite(buzzerPin, LOW);
    delay(120);
    digitalWrite(buzzerPin, HIGH);
    Serial.println("LCD Hold");
  }
  else if (holdButtonState == HIGH)
  {
    holdButtonPressed = false;
  }

  if (!lcdHold)
  {
    
    // print nilai ke serial monitor
//    Serial.print("Temperature is: ");
//    Serial.print(roundedTemperature);
//    Serial.println();
//
//    Serial.print("Viskositas: ");
//    Serial.print(hasil_akhir);
//    Serial.println();
//
  Serial.print("Fe: ");
  Serial.println(logam);

    if (roundedTemperature >= normalMin && roundedTemperature <= normalMax)
    {
      dingin();
    }
    else if (roundedTemperature >= panasMin && roundedTemperature <= panasMax)
    {
      panas();
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      Serial.println("Oli Terlau Panas");
      digitalWrite(buzzerPin, HIGH); // Aktifkan buzzer jika nilai sensor logam melebihi 100
      digitalWrite(led_merah, HIGH);
      digitalWrite(led_hijau, LOW);
    }
  }
  // Tombol reset
  if (digitalRead(resetButtonPin) == LOW)
  {
    resetLCD();
    lcdHold = false;
    // kembalikan status lcdHold ke false saat tombol reset ditekan
  }
  delay(150);
}

//====================Kode_untuk_saat_suhu_dingin=================================
 void dingin()
{
  lcd.setCursor(1, 0);
  lcd.print("mPa.s");

  lcd.setCursor(7, 0);
  lcd.write((byte)2);

  lcd.setCursor(8, 0);
  lcd.print("Fe");

  lcd.setCursor(10, 0);
  lcd.write((byte)1);


  if (logam == 1)
  {
  lcd.setCursor(12, 0); // Atur posisi kursor LCD
  lcd.print(0);         // Tampilkan angka 0 di layar LCD
  }
  else if(logam > 251)
  {
  lcd.setCursor(12, 0); // Atur posisi kursor LCD
  lcd.print(100);     // Tampilkan angka  di layar LCD
  }else{
  float percentage = logam * 0.4; // 2.5%
  lcd.setCursor(12, 0); // Atur posisi kursor LCD
  lcd.print(percentage);     // Tampilkan angka  di layar LCD
  }

  lcd.setCursor(15, 0);
  lcd.print("%");




  lcd.setCursor(1, 1);
  lcd.print(hasil_akhir,1);

  lcd.setCursor(15, 1);
  lcd.write((byte)0);

  lcd.setCursor(7, 1);
  lcd.write((byte)2);

  lcd.setCursor(8, 1);
  lcd.print("SH");

  lcd.setCursor(12, 1);
  lcd.print(roundedTemperature);

  lcd.setCursor(10, 1);
  lcd.write((byte)1);
}
//====================akhir_Kode_untuk_saat_suhu_dingin==========================


//====================Kode_untuk_saat_suhu_panas=================================
 void panas()
{
  lcd.setCursor(1, 0);
  lcd.print("mPa.s");

  lcd.setCursor(7, 0);
  lcd.write((byte)2);

  lcd.setCursor(8, 0);
  lcd.print("Fe");

  lcd.setCursor(10, 0);
  lcd.write((byte)1);


  if (logam == 1)
  {
  lcd.setCursor(12, 0); // Atur posisi kursor LCD
  lcd.print(0);         // Tampilkan angka 0 di layar LCD
  }
  else if(logam > 251)
  {
  lcd.setCursor(12, 0); // Atur posisi kursor LCD
  lcd.print(100);     // Tampilkan angka  di layar LCD
  }else{
  float percentage = logam * 0.4; // 2.5%
  lcd.setCursor(12, 0); // Atur posisi kursor LCD
  lcd.print(percentage);     // Tampilkan angka  di layar LCD
  }
  
  

  lcd.setCursor(15, 0);
  lcd.print("%");

  lcd.setCursor(1, 1);
  lcd.print(hasil_akhir,1);

  lcd.setCursor(15, 1);
  lcd.write((byte)0);

  lcd.setCursor(7, 1);
  lcd.write((byte)2);

  lcd.setCursor(8, 1);
  lcd.print("SH");

  lcd.setCursor(12, 1);
  lcd.print(roundedTemperature);

  lcd.setCursor(10, 1);
  lcd.write((byte)1);
}
//====================akhir_Kode_untuk_saat_suhu_panas=======


//=====================fungsi_reset==========================
 void resetLCD()
{
  lcdHold = false;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Reset.....");
  Serial.println("LCD Reset");
  digitalWrite(led_biru, LOW);
  digitalWrite(buzzerPin, LOW);
  delay(150);
  digitalWrite(led_biru, HIGH);
  digitalWrite(buzzerPin, HIGH);
  delay(150);
  digitalWrite(led_biru, LOW);
  digitalWrite(buzzerPin, LOW);
  delay(150);
  digitalWrite(led_kuning, HIGH);
  digitalWrite(buzzerPin, HIGH);
  delay(150);
  digitalWrite(led_kuning, LOW);
  digitalWrite(buzzerPin, LOW);
  delay(150);
   digitalWrite(led_merah, HIGH);
  digitalWrite(buzzerPin, HIGH);
  delay(150);
  digitalWrite(led_merah, LOW);
  digitalWrite(buzzerPin, LOW);
  delay(150);
   digitalWrite(led_hijau, HIGH);
  digitalWrite(buzzerPin, HIGH);
  delay(150);
  digitalWrite(led_hijau, LOW);
  digitalWrite(buzzerPin, LOW);
  delay(150);
  lcd.clear();
}

//=====================akhir_fungsi_reset==========================
