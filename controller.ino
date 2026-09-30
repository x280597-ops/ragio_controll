#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// =====================
// ボタン
// =====================

#define BUTTON_FORWARD   27
#define BUTTON_BACK      26
#define BUTTON_LEFT      25
#define BUTTON_RIGHT     32



// RC_CARのMAC
uint8_t receiverAddress[] = {
  0x30, 0x76, 0xF5, 0xAF, 0xFC, 0x72
};


// ギミック状態
bool gimmickState = false;


// 前回のギミックボタン状態
bool oldGimmickOn = HIGH;
bool oldGimmickOff = HIGH;


void setup() {

  Serial.begin(115200);

  // INPUT_PULLUP
  pinMode(BUTTON_FORWARD, INPUT_PULLUP);
  pinMode(BUTTON_BACK, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);



  // =====================
  // Bluetooth
  // =====================

  if (!SerialBT.begin("RC_CONTROLLER", true)) {
    Serial.println("Bluetooth開始失敗");
    return;
  }

  Serial.println("Bluetooth Classic 送信側 起動");

  delay(1000);

  Serial.println("RC_CARへ接続中...");

  bool result = SerialBT.connect(receiverAddress);

  if (result) {
    Serial.println("接続成功！");
  }
  else {
    Serial.println("接続失敗！");
  }
}


void loop() {

  if (!SerialBT.connected()) {

    Serial.println("Bluetooth未接続");
    delay(500);
    return;
  }

  // =====================
  // 移動
  // =====================

  if (digitalRead(BUTTON_FORWARD) == LOW) {

    SerialBT.println("F");
    Serial.println("前進");

  }
  else if (digitalRead(BUTTON_BACK) == LOW) {

    SerialBT.println("B");
    Serial.println("後退");

  }
  else if (digitalRead(BUTTON_LEFT) == LOW) {

    SerialBT.println("L");
    Serial.println("左");

  }
  else if (digitalRead(BUTTON_RIGHT) == LOW) {

    SerialBT.println("R");
    Serial.println("右");

  }
  else {

    SerialBT.println("S");
    Serial.println("停止");
  }


  delay(50);
}