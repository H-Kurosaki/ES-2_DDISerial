#include "ES-2_DDISerial.h"


//--------------------------------------------------------------
void ES2::begin(int datapin,int powerpin,bool powermode)
{
dpin=datapin;
ppin=powerpin;
pmode=powermode;
  pinMode(ppin,OUTPUT);
  digitalWrite(ppin,!pmode);//OFF
  pinMode(dpin,INPUT);
  delay(200);
  LastEC_uScm=-1;
  LastWaterTemp=-999;
}
//--------------------------------------------------------------------
double ES2::GetEC_uScM(void)
{
  return LastEC_uScm;
}
//--------------------------------------------------------------------
double ES2::GetEC_dSM(void)
{
  return LastEC_uScm/1000;
}
//--------------------------------------------------------------------
double ES2::GetWaterTemp(void)
{
  return LastWaterTemp;
}
//--------------------------------------------------------------------
bool ES2::Read(void)
{
bool bit[MAX_BITS];
char res[MAX_STR_BUFFER];
  if (millis()>(4294967295-300))
    {delay(310);}

  int i;
  unsigned long timecount=millis();
  digitalWrite(ppin,pmode);//Power ON
  delay(1);
  //センサーの応答を待つ
  while(digitalRead(dpin))
  {
      //wait
      if(timecount+150>millis()||timecount>millis())
        {digitalWrite(ppin,!pmode);return false;}//time out
  }
	timecount=millis();

delayMicroseconds(150);//パルスが上がりきるのを待つ(インピーダンスにより最適値が変化します)
//パルスを記録する
for(i=0;i<MAX_BITS;i++)
  {
    bit[i]=digitalRead(dpin);
    delayMicroseconds(828);//1200bps bit width(正確には833だが4単位でしか計測できないので)
  }
  digitalWrite(ppin,!pmode);//Power OFF
//ヘッダのbit000000000000を飛ばす
i=0;
for(i;i<MAX_BITS;i++)
  {
    if(bit[i]==true){break;}
    if(i>100){return false;}//データのヘッダが見つからない
  }
//スタートビットとストップビットの対応を確認して頭出しする
for(i;i<MAX_BITS-20;i++)
  {
    if(bit[i]==false && bit[i+9]==true &&bit[i+10]==false && bit[i+19]==true){break;}    
    if(i>100){return false;}//データの先頭が見つからない
  }

//Serial.println(i);
int bitcount=0;
int value=0;
//10bit(スタートビット1,本体8,ストップビット1)ずつ復号
int bintable[]={0,1,2,4,8,16,32,64,128,0};//10bitの解釈(最初と最後は加算しない)
int wp=0;
int lastCR=0;
int lastSPC=0;
int lastTAB=0;
//10bit単位で復号する
for(i;i<MAX_BITS;i++)
  {
  if(bit[i])
    {value+=bintable[bitcount];}
  //Serial.print(bit[i]);
  bitcount++;
  if(bitcount==10)
      {
      if(value==0x09){Serial.print("<TAB>");}
      else if(value==0x0D){Serial.print("<CR>");}
      else{Serial.print((char)value);}
      if(value=='\t'){lastTAB=wp;}//TABはデータ先頭
      if(value==0x20){lastSPC=wp;}//スペースはECの末端(温度の先頭)
      if(value=='\r'){lastCR=wp;}//\rは2回出てくるが後のがデータ末端
      res[wp]=(char)value;
      bitcount=0;
      value=0;
      wp++;
      }
  }
  res[MAX_STR_BUFFER-1]=0;//突き抜けないようにバッファの末端を0で止める
  
  if(lastSPC==0||lastCR==0){return false;}//フォーマットに従った文字列が見つからない
  //チェックサム確認
  if(TestChecksum(res))
      {
        //Serial.println("SUM OK");
      }
  else
      {
        return false;//チェックサム照合に失敗
      }

    ////----------------------値の換算
    //復号された通信文の例    <TAB>946 19.2<CR>q4<CR>
    //                      <TAB><EC(uS/cm)> <Temp(C)><CR><SensorID><SUM><CR>
    
    
    LastEC_uScm=(double)atol((char *)(res+lastTAB+1));
    LastWaterTemp=(double)atof((char *)(res+lastSPC+1));
    return true;

}
//--------------------------------------------------------------------
bool ES2::TestChecksum(char *response)
{
  uint16_t length;
  uint16_t i;
  uint16_t sum=0;

  length=strlen(response);
  for(i=0;i<length;i++)
  {
  sum+=response[i];
  if(response[i]=='\r')
    {break;}
  }

  sum+=response[++i];
  sum=sum%64+32;

  //Serial.println(sum);
  //Serial.println((int)response[++i]);
  if(sum==(int)response[++i])
        {return true;}//SUM一致

  return false;
}


/*
通信文の例(1)
10bit区切り(スタートビット1(常にL),本体8bit,ストップビット1(常にH))
00000000000011111101001000010000011001000000100100100110010010011001001110100101001110010101100001010001110100100001010101100001001010000100000000000000000000000000000000000000000000000000000000000000E

000000000000111111 <HEADER>
0100100001	<TAB>
0000011001	0
0000001001	 
0010011001	2
0010011001	2
0011101001	.
0100111001	9
0101100001	<CR>
0100011101	q
0010000101	B
0101100001	<CR>
0010100001	<終わり>
0000000000  
0000000000
0000000000
0000000000
0000000000
0000000000

通信文の例(2)
00000000000001111110100100001011001100101000110010000111001000000100101000110010010011001001110100101001110010101100001010001110101011010010101100001001010000100000000000000000000000000000000000000000E
0000000000000111111 <HEADER>
0100100001  <TAB>	
0110011001  3
0100011001  1
0000111001  8
0000001001   
0100011001  1
0010011001  2
0011101001  .
0100111001  9
0101100001  <CR>
0100011101  q
0101101001  -
0101100001  <CR>
0010100001  <終わり>
0000000000
0000000000
0000000000
0000000000
*/
