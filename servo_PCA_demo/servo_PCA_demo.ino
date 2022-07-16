#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SoftwareSerial.h>

#define Relay 4
SoftwareSerial mySerial(2,3);//rx为2号端口,tx为3号端口 
//BT07 RX--------3
//BT07 TX--------2
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

bool flag = false;
String comdata = "";

int j = 0;
int Val[61] = {0};
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(Relay,OUTPUT);
  pwm.begin();
  pwm.setPWMFreq(50); //freq  0--60

}

void loop() {
  while(mySerial.available() > 0)
  {
    comdata += char(mySerial.read());
    delay(2);
    flag = true;
    }
  if(flag == true)
  {
    Serial.println(comdata);
//    Serial.println(comdata[1]-'0');
//    Serial.println((int)comdata[1]);
    Serial.println(comdata.length());
    
//    Serial.println(comdata[0]);
//    Serial.println(comdata[comdata.length()-1]);
//    Serial.println("***************************");
    for(int i = 0; i < comdata.length()-1 ; i++)//(123,0,1,2,3,1)
    {
      if(comdata[i] == ','||comdata[i] == '(')
      {
        j++;
        }
     else
     {
        Val[j] = Val[j]*10 + comdata[i] -'0';
      }
    }
    comdata = String("");
    flag = false;
    Serial.print("J = ");
    Serial.println(j);

    for(int i=0;i < j;i++)
  {
    Serial.print("Val[i] = ");
    Serial.print(Val[i+1]);
    //pulse[i] = ((double)(Val[i]/90)+0.5)*4096/20;   //angle---pulse
    //pulse[i] = (int)(102.4 + (float)(Val[i]*4096/1800));
    }
   Serial.println();
   int pluse[j/6][5];
   for(int i=0;i < j/6;i++)
   {
     for(int k=0;k < 5;k++)
     {
       //pluse[i][k] = (int)(double(Val[k+1]*4096/1800)+102.4);
       pluse[i][k] = (int)(double(Val[k+1]*4096.0/1800.0)+102.4);
      }
    }
    for(int i=0;i < j/6;i++)
   {
     for(int k=0;k < 5;k++)
     {
       Serial.print(pluse[i][k]);
       Serial.print(",");
      }
    if(i%1 == 0)
    {
      Serial.println();
      }
    }

    j = 0;
    for(int i=0;i < 61;i++)
    {
      Val[i] = 0;
      }
  }
}
