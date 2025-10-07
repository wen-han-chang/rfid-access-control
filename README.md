# rfid-access-control 🔐

ESP32 + RC522 + OLED + Servo 的 RFID 智慧門鎖，支援白名單驗證與 OLED 狀態顯示，成功卡片會觸發伺服馬達模擬開鎖，失敗卡片則拒絕。

---

## ✨ Features
- ✅ RFID 白名單驗證  
- ✅ OLED 顯示狀態 (Scan / Success / Failure)  
- ✅ 伺服馬達控制門鎖  
- ❌ 未授權卡片 → 拒絕進入  

---

## 🛠 Hardware Requirements
- ESP32 Dev Board  
- RC522 RFID Reader  
- Grove OLED Display  
- Servo Motor (SG90/MG90S 等皆可)  
- RFID 卡片 / Tag  

---

## 🔌 Wiring
| ESP32 Pin | Module      | Pin   |
|-----------|-------------|-------|
| 21        | OLED SDA    | SDA   |
| 22        | OLED SCL    | SCL   |
| 18        | RC522 SCK   | SCK   |
| 19        | RC522 MISO  | MISO  |
| 23        | RC522 MOSI  | MOSI  |
| 5         | RC522 SDA   | SS    |
| 27        | RC522 RST   | RST   |
| 16        | Servo       | SIG   |
| GND/3.3V  | 共用電源與地線 | VCC/GND |

---

## 🚀 Usage
1. 將程式碼 (`main.cpp` 或 `rfid-access-control.ino`) 上傳至 ESP32  
2. 修改 `allowedUIDs` 陣列，新增允許的 RFID 卡片 UID  
   ```cpp
   byte allowedUIDs[][10] = {
     {0x85, 0xD2, 0x88, 0x5E},  
     {0xBB, 0xBD, 0x4B, 0x63},  
     {0x55, 0x66, 0x77, 0x88}   
   };
