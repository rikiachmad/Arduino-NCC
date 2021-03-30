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

void alert(int distance) {
  if (millis() - timer > waitTime) {
    timer = millis();
    DFPlayer.play(distance+1);
  } 
}


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