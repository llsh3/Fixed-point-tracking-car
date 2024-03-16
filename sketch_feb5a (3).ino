#include <Arduino.h>
#include"BluetoothSerial.h"
BluetoothSerial SerialBT;
const int Motor1_1 = 25;
const int Motor1_2 = 26;//right front
const int Motor2_1 = 18;
const int Motor2_2 = 19;//left front
const int Motor3_1 = 22;
const int Motor3_2 = 23;//left back
const int Motor4_1 = 32;
const int Motor4_2 = 33;//right back
const int track1 = 13;
const int track2 = 12;
const int track3 = 34;
const int track4 = 35;
int a,b=0,Sensor[4]={0},Mode,X,Y,x=0,y=0;
char BluetoothData[11];
void Tracking();


void Sensor_Read()
{
  Sensor[0] = digitalRead(track1); //Black line detected as high (1) and white line as low (0) 
  Sensor[1] = digitalRead(track2);
  Sensor[2] = digitalRead(track3);
  Sensor[3] = digitalRead(track4);
}

void Track_Init()
{
//循迹模块D0引脚初始化,设置为输入模式
  pinMode(track1, INPUT);
  pinMode(track2, INPUT);
  pinMode(track3, INPUT);
  pinMode(track4,INPUT);
}

void Motor_Init()
{
//Tracking module D0 pin initialization, set to input mode
  pinMode(Motor1_1, OUTPUT);
  pinMode(Motor1_2, OUTPUT);
  pinMode(Motor2_1, OUTPUT);
  pinMode(Motor2_2, OUTPUT);
  pinMode(Motor3_1, OUTPUT);
  pinMode(Motor3_2, OUTPUT);
  pinMode(Motor4_1, OUTPUT);
  pinMode(Motor4_2, OUTPUT);
}

void Motor_Control(int M1_1,int M1_2,int M2_1,int M2_2,int M3_1,int M3_2,int M4_1,int M4_2)
{
  digitalWrite(Motor1_1, M1_1); 
  digitalWrite(Motor1_2, M1_2);

  digitalWrite(Motor2_1, M2_1); 
  digitalWrite(Motor2_2, M2_2);

  digitalWrite(Motor3_1, M3_1); 
  digitalWrite(Motor3_2, M3_2);

  digitalWrite(Motor4_1, M4_1); 
  digitalWrite(Motor4_2, M4_2);
}

void STOP()
{
  Motor_Control(0,0,0,0,0,0,0,0);
}
void MOVE_FORWARD()
{
  Motor_Control(1,0,1,0,1,0,1,0);
}
void MOVE_BACKWARD()
{
  Motor_Control(0,1,0,1,0,1,0,1);
}
void SHIFT_RIGHT()
{
  Motor_Control(0,1,1,0,0,1,1,0);
}
void SHIFT_LEFT()
{
 Motor_Control(1,0,0,1,1,0,0,1);
}void ROTATE_ANTICLOCKWISE()
{
 Motor_Control(1,0,0,1,0,1,1,0);
}
void ROTATE_CLOCKWISE() 
{
  Motor_Control(0,1,1,0,1,0,0,1);
}
void Tracking()
{
   if (Sensor[0]==0&&Sensor[1]==1&&Sensor[2]==0) 
   {
    MOVE_FORWARD();

  }
  if (Sensor[0]==1&&Sensor[1]==0&&Sensor[2]==0) {
    SHIFT_RIGHT();

  }
   if (Sensor[0]==0&&Sensor[1]==0&&Sensor[2]==1) {
    SHIFT_LEFT();

 }
}
void DRIVETO(int a,int b,int A,int B)
{ if(a<=A) 
      if(a<A)
             {while(a<A)
               { Sensor_Read();
                 Tracking();
               if(Sensor[0]==1)//track1 on the black line
                 MOVE_FORWARD();
               if(Sensor[1]==1&&Sensor[0]==1)//Passing right Angle
                a++;
               } STOP;
             }
      else  STOP();
     
  else
        {while(a>A)
             { Sensor_Read();
               Tracking();
               if(Sensor[1]==1&&Sensor[0]==0&&Sensor[2]==0)//track2 on the black line
                   MOVE_BACKWARD();
                if(Sensor[1]==1&&Sensor[0]==1)//Passing right Angle
                  a--;
             } STOP();
        }
if(b<=B) 
       if(b<B)
            {
             while(b<B)
               { Sensor_Read();
                 Tracking();
                 if(Sensor[1]==1)
                 SHIFT_LEFT();
                if(Sensor[1]==1&&Sensor[0]==1)//Passing right Angle
                    b++;
               } STOP;
            }
      else  STOP();
     
else
      {while(b>B)
             { Sensor_Read();
               Tracking();
               if(Sensor[1]==1&&Sensor[0]==0&&Sensor[2]==0)
               SHIFT_LEFT();
               if(Sensor[1]==1&&Sensor[0]==1)
                    b--;
             } STOP;
      }
   
}

void setup() 
{
SerialBT.begin("LSHandCCJ");
Serial.println("Hello,I have started");
Motor_Init();
Track_Init();
}

void loop() 
{
while (SerialBT.available()>0)
{
for(a=0;a<11;a++)
  BluetoothData[a]=SerialBT.read();
}
Mode=BluetoothData[0];
if(Mode==1)
DRIVETO(0,0,BluetoothData[1],BluetoothData[2]);delay(2000);
DRIVETO(BluetoothData[1],BluetoothData[2],0,0);
DRIVETO(0,0,BluetoothData[1],BluetoothData[2]);delay(2000);
DRIVETO(BluetoothData[1],BluetoothData[2],0,0);
DRIVETO(0,0,BluetoothData[1],BluetoothData[2]);delay(2000);
DRIVETO(BluetoothData[1],BluetoothData[2],0,0);

if(Mode==2)
{
  int min;//Define a variable to store the minimum subscript
 for(int i=0;i<6;i++)//The external loop controls the number of comparisons, and if there are n numbers, compare n-1 times
 {
   min=1;// Let's say the minimum value is data[i], which is data[0]
    for(int j=i+1;j<12;j+=2)//Compare the minimum value with the number following the minimum value: data[i+1]~data[n-1]
   {
//The smallest of these numbers is found by comparing them with the specified minimum data[min]
      if(BluetoothData[i]*BluetoothData[i]+BluetoothData[i+1]*BluetoothData[i+1]<BluetoothData[min]*BluetoothData[min]+BluetoothData[min+1]*BluetoothData[min+1])
      {
      min=j;//Record the subscript of the minimum value
      } 
     if(BluetoothData[i]!=BluetoothData[min])
    {
//There is no way to borrow third-party variable exchange this
      BluetoothData[i]=BluetoothData[min]+BluetoothData[i];
      BluetoothData[min]=BluetoothData[i]-BluetoothData[min];
      BluetoothData[i]=BluetoothData[i]-BluetoothData[min];

      BluetoothData[i+1]=BluetoothData[min+1]+BluetoothData[i+1];
      BluetoothData[min+1]=BluetoothData[i+1]-BluetoothData[min+1];
      BluetoothData[i+1]=BluetoothData[i+1]-BluetoothData[min+1];
    }
   }
  DRIVETO(0,0,BluetoothData[1],BluetoothData[2]);delay(2000);
  DRIVETO(BluetoothData[1],BluetoothData[2],BluetoothData[3],BluetoothData[4]);delay(2000);
  DRIVETO(BluetoothData[3],BluetoothData[4],BluetoothData[5],BluetoothData[6]);delay(2000);
  DRIVETO(BluetoothData[5],BluetoothData[6],BluetoothData[7],BluetoothData[8]);delay(2000);
  DRIVETO(BluetoothData[7],BluetoothData[8],BluetoothData[9],BluetoothData[10]);
  
 }
}
if(Mode==3)
{
 DRIVETO(0,0,3,0);
 DRIVETO(3,0,3,2);
 DRIVETO(3,2,1,2);
 DRIVETO(1,2,1,5);
 DRIVETO(1,5,2,5);
 DRIVETO(2,5,2,3);
 DRIVETO(2,3,3,3);
 DRIVETO(3,3,3,5);
 DRIVETO(4,5,4,3);
 DRIVETO(5,3,5,2);
 DRIVETO(5,2,4,2);
 DRIVETO(4,2,4,0);
 DRIVETO(4,0,0,0);
}
if(Mode==4)
{
 DRIVETO(0,0,4,0);
 DRIVETO(4,0,4,5);
 DRIVETO(4,5,1,5);
 DRIVETO(1,5,1,4);
 DRIVETO(1,4,3,4);
 DRIVETO(3,4,3,3);
 DRIVETO(3,3,1,3);
 DRIVETO(1,3,1,2);
 DRIVETO(1,2,3,2);
 DRIVETO(3,2,3,1);
 DRIVETO(3,1,1,1);
 DRIVETO(1,1,1,0);
 DRIVETO(1,0,0,0);

}
if(Mode==5)
{
 DRIVETO(0,0,5,0);
 DRIVETO(5,0,5,1);
 DRIVETO(5,1,3,1);
 DRIVETO(3,1,3,2);
 DRIVETO(3,2,4,2);
 DRIVETO(4,2,4,3);
 DRIVETO(4,3,3,3);
 DRIVETO(3,3,3,4);
 DRIVETO(3,4,5,4);
 DRIVETO(5,4,5,5);
 DRIVETO(5,5,0,5);
 DRIVETO(0,4,2,4);
 DRIVETO(2,3,1,3);
 DRIVETO(1,3,2,2);
 DRIVETO(2,2,2,1);
 DRIVETO(2,1,0,1);
 DRIVETO(0,1,0,0);
}
}