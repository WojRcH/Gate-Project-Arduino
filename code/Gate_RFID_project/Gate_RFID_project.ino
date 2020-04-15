#include <Servo.h>
#include <MFRC522.h>

const int SS_pin = 10;
const int RST_pin = 9;
const int buzzer = 6;
const int redPin = 5;
const int greenPin = 4;
const int bluePin = 3;
const int servoPin = 2;

MFRC522 mfrc522(SS_pin, RST_pin);
Servo servo;
int servoAngle = 180;
String code;
String RFID_code;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  servo.attach(servoPin);
  servo.write(servoAngle);
}


void loop() {
  // put your main code here, to run repeatedly:
  RGBColor(255, 0, 0);

  //Get RFID_code // source:https://create.arduino.cc/projecthub/Aritro/security-access-using-rfid-reader-f7c746
  RFID_code = "";
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    RFID_code.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    RFID_code.concat(String(mfrc522.uid.uidByte[i], HEX));
    RFID_code.toUpperCase();
  }
  code = RFID_code.substring(1);

  //code = Serial.readStringUntil('\n');

  //Display actual card code in Serial
  Serial.println(code);

  //Check if code is correct to open the gate
  if (code.equals("67 E4 32 63")) {
    RGBColor(0, 255, 0);
    Serial.println("Access granted!");
    tone(buzzer, 1000);
    delay(500);
    noTone(buzzer);
    for (servoAngle = 180; servoAngle >= 90; servoAngle--) {
      servo.write(servoAngle);
      delay(30);
    }
    delay(3000);
    tone(buzzer, 1000);
    RGBColor(255, 0, 0);
    delay(500);
    noTone(buzzer);
    RGBColor(0, 0, 0);
    delay(500);
    tone(buzzer, 1000);
    RGBColor(255, 0, 0);
    delay(500);
    noTone(buzzer);
    for (servoAngle = 90; servoAngle <= 180; servoAngle++) {
      servo.write(servoAngle);
      delay(30);
    }
  }
  else {
    Serial.println("Access denied!");
    tone(buzzer, 500);
    delay(500);
    noTone(buzzer);
    RGBColor(0, 0, 0);
    delay(500);
    RGBColor(255, 0, 0);
    tone(buzzer, 500);
    delay(500);
    noTone(buzzer);
    RGBColor(0, 0, 0);
    delay(500);
    RGBColor(255, 0, 0);
  }
}

//RGB LED control
void RGBColor(int redVal, int greenVal, int blueVal) {
  analogWrite(redPin, redVal);
  analogWrite(greenPin, greenVal);
  analogWrite(bluePin, blueVal);
}
