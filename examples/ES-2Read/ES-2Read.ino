//ES-2 DDI serial Read
/*
注）電源制御用にLow Activeデバイス(PNPトランジスタ)を接続した場合のサンプルです。
電源制御用のトランジスタの種類に応じて書き換えてください。
Arduinoをセンサの電源ピンに直接接続することは推奨しません。

Note: This sample is when a Low Active transistor is connected.
Rewrite it according to the type of transistor for power supply control. 
It is not recommended to connect the Arduino directly to the power pins of the sensor.
*/
#include "ES-2_DDISerial.h"

#define DATA_PIN  2
#define POWER_PIN  12

ES2 ECSensor;

void setup() {
  Serial.begin(9600);
  ECSensor.begin(DATA_PIN,POWER_PIN,POWER_LOWACTIVE);
}


void loop() {

  if(ECSensor.Read())
  {
  Serial.print("EC(dS/m):");
  Serial.println(ECSensor.GetEC_dSM());
  Serial.print("WaterTemp(C):");
  Serial.println(ECSensor.GetWaterTemp());
  }
else
  {
  Serial.println("Error");
  }
    delay(1000);

}

