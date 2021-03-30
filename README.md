# Penugasan-Arduino-NCC
## Ultrasonic Sensor Glasses

Untuk penugasann ketiga sekaligus yang terakhir ini, saya memilih project IoT yaitu Ultrasonic Sensor Glasses. Ultrasonic glasses ini adalah sebuah kacamata yang dimodifikasi dengan sensor ultrasonic untuk mendeteksi hal hal yang terdapat di depan pengguna.
Hal ini mirip dengan kemampuan "Echolocation" yang dimiliki beberapa hewan sepeti kelelawar, lumba-lumba, dan beberapa ikan paus. Walaupun tidak se-sempurna echolocation milik hewan-hewan tersebut, namun dengan menerapkan prinsip dasar yang sama, kita dapat meniru dengan memanfaatkan teknologi yang ada saat ini.
Kacamata ini cocok digunakan untuk seorang tuna netra untuk membantu berjalan dan mendeteksi keadaan disekitar.

![alt text](https://github.com/rikiachmad/Arduino-NCC/blob/main/Ilustrasi.jpg?raw=true "Ilustrasi" )

Pada umumnya seorang tuna netra menggunakan sebuah tongkat bantu untuk mendeteksi segala sesuatu yang berada di depannya. Namun, hal ini kurang efektif untuk mendeteksi sebuah objek-objek yang berada di posisi yang pas dengan kepala.
Oleh karena itu, kacamata ini dibuat untuk membantu mereka agar dapat lebih berhati-hati dalam menjalani kehidupan sehari-hari.

## Komponen Penyusun
* arduino pro mini 328 - 5v/16mhz
* DFPlayer Mini
* Custom Print Circuit board
* Ultrasonic Sensor HC-SR04
* Kabel Jack 3.6mm

## Gambar Rangkaian
![alt text](https://github.com/rikiachmad/Arduino-NCC/blob/main/CircuitDesign.png?raw=true "Desain Circuit" )

Desain rangkaian diatas masih menggunakan breadboard dan speaker hanya untuk pembuatan desain circuit. Untuk pengaplikasiannya digunakan Custom Print Circuit Board yang lebih praktis dan efisien untuk memudahkan penggunaan.

## Penjelasan Source Code

```C

#include <Arduino.h>
#include <NewPing.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#define MAX_DISTANCE 400

const int trigPin = 3;
const int echoPin = 2;
int minDistance   = 100;

static unsigned long timer = millis();

int waitTime = 3500;

NewPing sonar(trigPin, echoPin ,MAX_DISTANCE);

SoftwareSerial mp3SoftwareSerial(8, 9); // RX, TX
DFRobotDFPlayerMini DFPlayer;

```
Bagian source code diatas digunakan untuk penggunaan library dan inisialisasi variable-variable yang akan digunakan.

```C

void alert(int distance) {
  if (millis() - timer > waitTime) {
    timer = millis();
    DFPlayer.play(distance+1);
  } 
}
```
Fungsi "alert()" digunakan untuk mentrigger sebuah pesan berupa voiceline dari audio yang telah disiapkan dengan menggunakan DFPlayer berdasarkan jarak dari objek tersebut. 
```C
void setupDFPlayer() {
  mp3SoftwareSerial.begin(9600);
  Serial.println();
  Serial.println(F("Blind assistancs smart glass..."));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!DFPlayer.begin(mp3SoftwareSerial)){
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  
  DFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
  DFPlayer.volume(29);  //Set volume value (0~30)
  DFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  DFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
}
```
Fungsi "setupDFPlayer()" digunakan sebagai user interface settings sekaligus error handling apabila terjadi kesalahan pada DFPlayer.
```C
void setup() {
  Serial.begin(9600);
  setupDFPlayer();
}

void loop() {
  delay(30);
  int distance  = sonar.ping_cm();

  if(distance < minDistance && distance > 1) {
    Serial.print("Obstacle ");
    Serial.print(distance);
    Serial.println(" CM ahead");

    alert(distance);
  }
  else {
  }
}
```
Dan dua fungsi terakhir merupakan fungsi standart untuk arduino. Pada fungsi loop, jarak antara sensor dengan benda disimpan kedalam variable dan di cek apakah terdapat sebuah objek yang berada pada jarak minimal untuk sensor dapat mendeteksi objek tersebut. Jika iya maka fungsi alert akan dipanggil dengan passing argumen berupa jarak objek.


