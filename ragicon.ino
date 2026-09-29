#include "BluetoothSerial.h"


BluetoothSerial SerialBT;

String buttonState = "OFF";

// DRV8833
#define AIN1 26
#define AIN2 27
#define STBY 25

void motorForward(int speed) {
  analogWrite(AIN1, speed);
  analogWrite(AIN2, 0);
}

void motorStop() {
  analogWrite(AIN1, 0);
  analogWrite(AIN2, 0);
}

void setup() {

  Serial.begin(115200);

  // =====================
  // モーター
  // =====================

  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  motorStop();



  // =====================
  // Bluetooth
  // =====================

  if (!SerialBT.begin("RC_CAR")) {
    Serial.println("Bluetooth開始失敗");
    return;
  }

  Serial.println("Bluetooth Server Started");

  Serial.print("Bluetooth MAC: ");
  Serial.println(SerialBT.getBtAddressString());
}


void loop() {

  if (SerialBT.available()) {

    String data = SerialBT.readStringUntil('\n');
    data.trim();

    Serial.print("受信: ");
    Serial.println(data);


    // =====================
    // 1なら正転
    // 0なら停止
    // =====================

    if (data == "1") {

      buttonState = "ON";

      // 正転
      motorForward(200);

    }
    else if (data == "0") {

      buttonState = "OFF";

      // 停止
      motorStop();

    }


    // =====================
    // TFT更新
    // =====================
  }

  delay(10);
}