#include <Servo.h>

void safety();
void hazard();
void windows_up();
void windows_down();

Servo myservo;
int pos = 0;
int temp=A0;
int smoke=A1;
int flame=8;
int horn=9;
int indicator=10;
int extinguish=7; // relay connecting the MCU to the extinguisher setup

float temp_reading;
int smoke_reading,flag=0,flag1=0,flag2=0,once=0;

void setup() {
  myservo.attach(3);
  pinMode(extinguish, OUTPUT);
 pinMode(indicator, OUTPUT);  //for flashing indicators the pins are set to output
  pinMode(horn, OUTPUT);    //for switching on the horn the pins are set to output
    digitalWrite(indicator, LOW); //initial condition of horn as well as indicators are set low
  digitalWrite(horn, LOW);
  digitalWrite(extinguish, LOW);
  Serial.begin(9600);   // for testing the sensors the data has to be transmitted 
  windows_up();
}

void loop() {

temp_reading = analogRead(temp);  // taking reading from the temp sensor LM35
temp_reading=temp_reading/2.048;  //in order to covert it into celsius the adc reading is divided by 2.048
smoke_reading = analogRead(smoke); // taking reading from the smoke sensor MQ3 or MQ2

if(digitalRead(flame)==0)    //flame sensor provides a digital output. normally it produces a high voltage level and on 
{                            //detection of IR radiation from flame it switches off the module producing a low value at output
hazard();
flag=1;
}
if((digitalRead(flame)==1) && (flag==1))
{
windows_up();
flag=0;  
}

if(temp_reading>50)
{
  hazard();
  flag1=1;
  }
  if((temp_reading<50) && (flag1==1))
  {
   windows_up();
   flag1=0; 
  }

if(smoke_reading>800)
{
  hazard();
  flag2=1;
  }
  if((smoke_reading<800) && (flag2==1))
  {
   windows_up();
   flag2=0; 
  }

  Serial.print("temp = ");
  Serial.print(temp_reading);
  Serial.print("    MQ3 = ");
  Serial.print(smoke_reading);
  Serial.print("\r\n");
}

  

void hazard()           // this function produces warnings for the driver
{ 
  if(once==0)
  {
    windows_down();
    once=1;
    }
  digitalWrite(extinguish, HIGH); 
  digitalWrite(indicator, HIGH);
  digitalWrite(horn, HIGH);
  delay(500);
  digitalWrite(indicator, LOW);
  digitalWrite(horn, LOW);
  delay(500);
  
}


  void windows_down()
  {
    for (pos = 0; pos <= 180; pos += 1) 
    { 
    // in steps of 1 degree
    myservo.write(pos);             
    delay(15);                      
  }
 }
  void windows_up()
  {
    for (pos = 180; pos >=0; pos -= 1) 
    { 
    // in steps of 1 degree
    myservo.write(pos);             
    delay(15);                      
  }
      
      once=0;
      digitalWrite(extinguish, LOW);
 }
