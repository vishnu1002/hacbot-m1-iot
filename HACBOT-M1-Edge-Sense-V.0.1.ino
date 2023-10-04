// HACBOT-M1-Edge-Sense-V.0.1

#include <AFMotor.h>  
#include <NewPing.h>
#include <Servo.h> 

////////////////////////////////////////////////////////////////////////

#define TRIG_PIN 3 
#define ECHO_PIN 2
#define SENS1_PIN A1
#define SENS2_PIN A2

#define MAX_DISTANCE 200 
#define MAX_SPEED 75
#define MAX_SPEED_OFFSET 20

////////////////////////////////////////////////////////////////////////

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 

AF_DCMotor motor1(1);   // motor 1
AF_DCMotor motor2(2);   // motor 2
AF_DCMotor motor3(3);   // motor 3
AF_DCMotor motor4(4);   // motor 4

Servo myservo;   

boolean goesForward = false;
int distance = 100;
int speedSet = 0;

int SENS1_val;
int SENS2_val;

void setup() 
{
  pinMode(SENS1_PIN,INPUT);
  pinMode(SENS2_PIN,INPUT);
  myservo.attach(10);  
  myservo.write(115); 
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);

}

void loop() 
{
  int distanceR = 0;
  int distanceL =  0;
  delay(40);

  SENS1_val=digitalRead(SENS1_PIN);
  SENS2_val=digitalRead(SENS2_PIN);

  if(distance<=20 || SENS1_val==1 || SENS2_val==1)
  {
    moveStop();
    delay(50);
    moveBackward();
    delay(200);
    moveStop();
    delay(200);
    distanceR = lookRight();
    delay(200);
    distanceL = lookLeft();
    delay(200);

    if(distanceR>=distanceL)
    {
      turnRight();
      moveStop();
    }
    else
    {
      turnLeft();
      moveStop();
    }
  }
  else
  {
    moveForward();
  }

  distance = readPing();
 
}

int lookRight()
{
  myservo.write(50); 
  delay(500);
  int distance = readPing();
  delay(100);
  myservo.write(115); 
  return distance;
}

int lookLeft()
{
  myservo.write(170); 
  delay(500);
  int distance = readPing();
  delay(100);
  myservo.write(115); 
  return distance;
  delay(100);
}

int readPing() 
{ 
  delay(70);
  int cm = sonar.ping_cm();
  if(cm==0)
  {
    cm = 250;
  }
  return cm;
}

void moveStop() 
{
  motor1.run(RELEASE); 
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
} 
  
void moveForward() 
{
 if(!goesForward)
  {
    goesForward=true;
    motor1.run(FORWARD);      
    motor2.run(FORWARD);
    motor3.run(FORWARD); 
    motor4.run(FORWARD);     
   for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
   {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet);
    motor3.setSpeed(speedSet);
    motor4.setSpeed(speedSet);
    delay(5);
   }
  }
}

void moveBackward() 
{
  goesForward=false;
  motor1.run(BACKWARD);      
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}  

void turnRight() 
{
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);     
  delay(500);
  motor1.run(FORWARD);      
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);      
} 
 
void turnLeft() 
{
  motor1.run(BACKWARD);     
  motor2.run(BACKWARD);  
  motor3.run(FORWARD);
  motor4.run(FORWARD);   
  delay(500);
  motor1.run(FORWARD);     
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}  
