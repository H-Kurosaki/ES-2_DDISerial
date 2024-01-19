METER社製(旧社名:Decagon) 
ES-2 電気伝導度センサ
DDI Serial Libraly for Arduino
対応機種:
  Arduino UNO R3(R4ではない)
  Arduino Mega 2560 R3 など
その他の5V 16MHzで駆動するAVRマイコンを搭載したArduinoのみ対応します。

一切割り込みを使用しない代わりに遅いです。read実行にはおよそ220msecかかります。
ES-2は電力が供給された瞬間に1回だけシリアル通信を行う。したがって電源の制御が必須。
毎回読むたびに電源を入れたり切ったりしています。
Arduinoのバスでは5Vの機種でも電力が微妙に足りないので通信が不安定になります。
必ずトランジスタなど50mA以上の電力を供給すること(マニュアル通り20mAだと足りない)。
数値の単位はuS/cmですが一般的なdS/mに直すには1000で割ります。
対応する関数を2つ用意してあります。
double GetEC_uScM(void);    uS/cm単位でのECを返す
double GetEC_dSM(void);     dS/m単位でのECを返す

ES-2の接続は

Ground - ArduinoのGNDピン

DATA - 任意のArduinoのデジタル入力ピンに直接接続

Power - PNPトランジスタのコレクタ出力など5V出力可能なデバイス

トランジスタのbaseピン - 抵抗 - Arduinoのデジタル出力ピン


Setup関数内でbegin(int datapin,int powerpin,bool powermode)関数を実行します。
datapinにDATAを接続したピン番号
powerpinには電源制御トランジスタのbaseピンに接続したピン番号
powermodeはトランジスタの種類によって決まります

  NPNトランジスタはLOW Activeなので"POWER_LOWACTIVE"
  
  PNPトランジスタやフォトリレーではHIGH Activeなので"POWER_HIGHACTIVE"
  
  を指定します。

Loop関数内で
Read()を実行します。実行中は約220msぐらい待ち時間が発生します。
戻り値はTrueが計測成功、Falseが失敗です。
計測に成功した場合、その後に

GetEC_uScM(void);

GetEC_dSM(void);

GetWaterTemp(void);

を実行することでECと水温を取得できます。
測定に失敗していた場合、過去に測定に成功したときの値が残っています。
一度も測定に成功していない場合、負数が返されます。
連続した実行はできません。
Read()再実行には1秒以上待ってください。

-------------------------------------------------------English

METER(Former Company Name:Decagon) 
ES-2 Electrical Conductivity Sensor
DDI Serial Libraly for Arduino
Supported platforms:
  Arduino UNO R3(Not R4)
  Arduino Mega 2560 R3 etc.
Only Arduino with AVR microcontroller driven by other 5V 16MHz is supported.

It is slow instead of using any interrupts; it takes approximately 220 msec to execute a read.
ES-2 performs serial communication only once at the moment power is supplied. Therefore, control of the power supply is essential.
The power is turned on and off each time a read is performed.
The Arduino bus does not have enough power even for 5V models slightly, so communication becomes unstable.
Be sure to supply more than 50mA of power, such as transistors (20mA as per manual is not enough).
The unit of the number is uS/cm, but to convert it to the common dS/m, divide by 1000.
Two corresponding functions are provided.
double GetEC_uScM(void); returns EC in uS/cm
double GetEC_dSM(void); returns EC in dS/m

How to use
The ES-2 connections are
Ground - Arduino GND pin
DATA - Direct connection to any Arduino digital input pin
Power - 5V output device such as collector output of PNP transistor
Transistor base pin - Resistor - Arduino digital output pin

Execute the begin(int datapin,int powerpin,bool powermode) function in the Setup function.
Pin number with DATA connected to datapin
powerpin is the pin number connected to the base pin of the power control transistor
powermode is determined by the type of transistor
  NPN transistors are LOW Active, so "POWER_LOWACTIVE".
  PNP transistors and photorelays are HIGH active, so "POWER_HIGHACTIVE" is used for PNP transistors and photorelays.

In the Loop function, execute
Read() is executed. While executing the Read() function, a wait time of about 220 ms is generated.
The return value is True for a successful measurement and False for a failed measurement.
If the measurement succeeds, the following functions can be used.
GetEC_uScM(void);
GetEC_dSM(void);
GetWaterTemp(void);
can be executed to obtain the EC and water temperature.
If the measurement has failed, the values from a previous successful measurement remain.
If the measurement has never succeeded, a negative number is returned.
Consecutive runs are not allowed.
Wait at least 1 second for Read() reexecution.
