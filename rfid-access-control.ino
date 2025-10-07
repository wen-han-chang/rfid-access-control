#include <Wire.h>
#include <SeeedOLED.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>

#define SS_PIN     5     // RC522 SS (SDA)
#define RST_PIN    27    // RC522 RST
#define SERVO_PIN  16    // 伺服訊號腳

MFRC522 rfid(SS_PIN, RST_PIN);
Servo servo;

// -------- 白名單卡片 UID --------
byte allowedUIDs[][10] = {
  {0x85, 0xD2, 0x88, 0x5E},  
  {0xBB, 0xBD, 0x4B, 0x63},  
  {0x55, 0x66, 0x77, 0x88}   
};
byte allowedLens[] = {4, 4, 4};
const size_t NUM_UIDS = sizeof(allowedLens);

// -------- OLED 畫面 --------
void showIdle() {
  SeeedOled.clearDisplay();
  SeeedOled.setTextXY(0, 0);
  SeeedOled.putString("PLEASE SCAN");
}

void showWelcome() {
  SeeedOled.clearDisplay();
  SeeedOled.setTextXY(0, 0);
  SeeedOled.putString("SUCCESS");
  SeeedOled.setTextXY(1, 0);
  SeeedOled.putString("WELCOME");
}

void showFailure() {
  SeeedOled.clearDisplay();
  SeeedOled.setTextXY(0, 0);
  SeeedOled.putString("FAILURE");
}

// -------- UID 比對 --------
bool isAllowed(const MFRC522::Uid &uid) {
  for (size_t i = 0; i < NUM_UIDS; i++) {
    if (allowedLens[i] != uid.size) continue;
    bool match = true;
    for (byte j = 0; j < uid.size; j++) {
      if (allowedUIDs[i][j] != uid.uidByte[j]) {
        match = false;
        break;
      }
    }
    if (match) return true;
  }
  return false;
}

void setup() {
  Serial.begin(115200);

  Wire.begin(21, 22);          // SDA=21, SCL=22
  SeeedOled.init();
  SeeedOled.deactivateScroll();
  SeeedOled.setPageMode();
  showIdle();

  SPI.begin();                 // SCK=18, MISO=19, MOSI=23
  rfid.PCD_Init();
  delay(50);

  servo.setPeriodHertz(50);
  servo.attach(SERVO_PIN, 500, 2400);
  servo.write(0);

  Serial.println("請將卡片靠近 RC522 感應區");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  Serial.print("UID:");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();

  if (isAllowed(rfid.uid)) {
    showWelcome();
    servo.write(90);
    delay(2000);
    servo.write(0);
  } else {
    showFailure();
    delay(1500);
  }

  showIdle();
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
