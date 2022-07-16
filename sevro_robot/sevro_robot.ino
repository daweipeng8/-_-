#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SoftwareSerial.h>

#define Relay1 4       //realy1----------4
#define Relay2 5       //realy2----------5
#define Speed 5

SoftwareSerial mySerial(2,3);//rx为2号端口,tx为3号端口 
//BT07 RX--------3
//BT07 TX--------2
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

bool flag = false;
String comdata = "";

int j = 0;
int Val[61] = {0};
int pro_angle[5] = {102,102,102,102,102};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(Relay1,OUTPUT);
  pinMode(Relay2,OUTPUT);
  digitalWrite(Relay1,LOW);
  digitalWrite(Relay2,LOW);
  pwm.begin();
  pwm.setPWMFreq(50); //freq  0--60
  //  HOME
  for(int i=0;i < 5;i++)
  {
    pwm.setPWM(i, 0, 102);
    }
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
    
   int pulse[j/6][5];
   for(int i=0;i < j/6;i++)
   {
     for(int k=0;k < 5;k++)
     {
       //pulse[i][k] = (int)(double(Val[k+1]*4096/1800)+102.4);
       pulse[i][k] = (int)(double(Val[i*6+k+1]*4096.0/1800.0)+102.4);
      }
    }
    for(int i=0;i < j/6;i++)
   {
     for(int k=0;k < 5;k++)
     {
       Serial.print(pulse[i][k]);
       Serial.print(",");
      }
    if(i%1 == 0)
    {
      Serial.println();
      }
    }
  for(int i=0;i < j/6;i++)
  {
    if(Val[(i+1)*6] == 0)
    {
      digitalWrite(Relay1,LOW);
      digitalWrite(Relay2,HIGH);
      }
    else if(Val[(i+1)*6] == 1)
    {
      digitalWrite(Relay1,HIGH);
      digitalWrite(Relay2,LOW);
      }
    while(!((pro_angle[0] == pulse[i][0])&&(pro_angle[1] == pulse[i][1])&&(pro_angle[2] == pulse[i][2])&&
          (pro_angle[3] == pulse[i][3])&&(pro_angle[4] == pulse[i][4])))
    {
      for(int k = 0 ;k < 5;k++)
      {
        if(pro_angle[k] > pulse[i][k])
        {
          pro_angle[k]--;
          }
        else if(pro_angle[k] < pulse[i][k])
        {
          pro_angle[k]++;
          }
        pwm.setPWM(k, 0, pro_angle[k]);
      }
      delay(Speed);
     }
    }

    j = 0;
    for(int i=0;i < 61;i++)
    {
      Val[i] = 0;
      }
  }
}
