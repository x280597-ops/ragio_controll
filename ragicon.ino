#include "BluetoothSerial.h"


BluetoothSerial SerialBT;

String buttonState = "OFF";
unsigned long lastReceiveTime = 0;

#define MOTOR_TIMEOUT 100
// DRV8833
#define BIN1 32
#define BIN2 33
#define AIN1 26
#define AIN2 27
#define STBY 25

void left_Motor(int speed) {

  if (speed > 0) {
    // 正転
    analogWrite(AIN1, speed);
    analogWrite(AIN2, 0);
  }
  else if (speed < 0) {
    // 逆転
    analogWrite(AIN1, 0);
    analogWrite(AIN2, -speed);
  }
  else {
    // 停止
    analogWrite(AIN1, 0);
    analogWrite(AIN2, 0);
  }
}


void right_Motor(int speed) {

  if (speed > 0) {
    // 正転
    analogWrite(BIN1, speed);
    analogWrite(BIN2, 0);
  }
  else if (speed < 0) {
    // 逆転
    analogWrite(BIN1, 0);
    analogWrite(BIN2, -speed);
  }
  else {
    // 停止
    analogWrite(BIN1, 0);
    analogWrite(BIN2, 0);
  }
}

void left_MotorStop() {
  analogWrite(AIN1, 0);
  analogWrite(AIN2, 0);
}

void Go_Front(int speed){
  left_Motor(speed);
  right_Motor(speed);
}
void Go_Back(int speed){
  left_Motor(-speed);
  right_Motor(-speed);
}
void Go_Left(int speed){
  int half_speed=speed/2;
  left_Motor(half_speed);
  right_Motor(speed);
}
void Go_Right(int speed){
  int half_speed=speed/2;
  left_Motor(speed);
  right_Motor(half_speed);
}
void right_MotorStop() {
  analogWrite(BIN1, 0);
  analogWrite(BIN2, 0);
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
  right_MotorStop();
  left_MotorStop();


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
    lastReceiveTime = millis();
    String data = SerialBT.readStringUntil('\n');
    data.trim();

    Serial.print("受信: ");
    Serial.println(data);


    // =====================
    // 1なら正転
    // 0なら停止
    // =====================

    if (data == "F") {
      buttonState = "ON";
      // 正転
      Go_Front(200);
    }
    else if (data == "B"){
      buttonState = "ON";
      // 正転
      Go_Back(200);
    }
    else if (data == "L"){
      buttonState = "ON";
      // 正転
      Go_Left(200);
    }
    else if (data == "R"){
      buttonState = "ON";
      // 正転
      Go_Right(200);
    }
    else {

      buttonState = "OFF";

      // 停止
      right_MotorStop();
      left_MotorStop();


    }
  }
  if (millis() - lastReceiveTime > MOTOR_TIMEOUT) {
  right_MotorStop();
  left_MotorStop();
  }
  delay(10);
}