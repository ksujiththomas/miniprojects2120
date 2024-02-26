//// It is a fire fighting robot. 3 flame sensors are connected. vehicle will move in the direction where the intensity of fire is high.
///// If fire is detected the vehicle will spray water using a mini water pump..the pump is attached on a servomotor..






/*------ Arduino Fire Fighting Robot Code----- */
 #include<SoftwareSerial.h>
  SoftwareSerial mySerial(9,10);
  
# include <Servo.h>
Servo myservo;
 
int pos = 0;    

boolean fire = false;
 
/*-------defining Inputs------*/
 
/*-------defining Outputs------*/
#define LM1 7      // left motor
#define LM2 8       // left motor
#define RM1 4       // right motor
#define RM2 2       // right motor
#define pump 6
 
void setup()
{
  pinMode(LM1, OUTPUT);
  pinMode(LM2, OUTPUT);
  pinMode(RM1, OUTPUT);
  pinMode(RM2, OUTPUT);
  pinMode(pump, OUTPUT);

mySerial.begin(9600);
 delay(100);
  myservo.attach(3);
  myservo.write(90); 
}  
void send_message()
{
  mySerial.println("AT+CMGF=1");
  
  delay(500);
  mySerial.println("AT+CMGS=\"8281675450\"\r");
  
  delay(500);       
  mySerial.println("fire detected");
 
  delay(100);
  mySerial.println((char)26);
 delay(500);
} 
void put_off_fire()
{
    delay (300);
 
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, HIGH);
    
   digitalWrite(pump, HIGH); 
   delay(500);
    
    for (pos = 50; pos <= 130; pos += 1) 
    { 
      myservo.write(pos); 
      delay(10);  
    }
    for (pos = 130; pos >= 50; pos -= 1) 
    { 
      myservo.write(pos); 
      delay(10);
    }
  
    digitalWrite(pump,LOW);
    myservo.write(90);
  
    fire=false;
    send_message();
}
 
void loop()
{
   myservo.write(90); //Sweep_Servo();  
   int sensorValue1 = analogRead(A0);
   int sensorValue2 = analogRead(A1);
   int sensorValue3 = analogRead(A2);
   if(sensorValue1>100&&sensorValue2>100&&sensorValue3>100) //If Fire not detected all sensors are zero
   {
           //Do not move the robot
            digitalWrite(LM1, HIGH);
            digitalWrite(LM2, HIGH);
             digitalWrite(RM1, HIGH);
             digitalWrite(RM2, HIGH);
             
    }
    else
    {
       if(sensorValue1<sensorValue2 &&sensorValue1<sensorValue3)  //If Fire is to the right
       {
       //Move the robot right
           
            fire = true;
             digitalWrite(LM1, LOW);
        digitalWrite(LM2, HIGH);
        digitalWrite(RM1, HIGH);
        digitalWrite(RM2, HIGH);
    
      }
    
       else if(sensorValue2<sensorValue1 &&sensorValue2<sensorValue3) //If Fire is straight ahead
      {
       //Move the robot forward
              digitalWrite(LM1, LOW);
            digitalWrite(LM2, HIGH);
            digitalWrite(RM1, LOW);
            digitalWrite(RM2, HIGH);
          fire = true;
      }
    
      else //If Fire is to the left
      {
        //Move the robot left
        digitalWrite(LM1, HIGH);
            digitalWrite(LM2, HIGH);
            digitalWrite(RM1, LOW);
            digitalWrite(RM2, HIGH);
        fire = true;
      }
    }
    
//delay(300); //Slow down the speed of robot
 
     while (fire == true)
     {
      put_off_fire();
     }
} 
