// HACBOT-M1-Obstacle-Avoid-V.0.1

#include <AFMotor.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

////////////////////////////////////////////////////////////////////////

Servo servoLook;

byte trig = 3;
byte echo = 2;

byte maxDist = 150;
byte stopDist = 30;
float timeOut = 2*(maxDist+10)/100/340*1000000;

byte motorSpeed = 75;
int motorOffset = 10;
int turnSpeed = 50;

AF_DCMotor motor1(1);    // motor 1
AF_DCMotor motor2(2);    // motor 2
AF_DCMotor motor3(3);    // motor 3
AF_DCMotor motor4(4);    // motor 4

void setup() 
{
  motor1.setSpeed(motorSpeed);
  motor4.setSpeed(motorSpeed);
  motor3.setSpeed(motorSpeed+motorOffset);
  motor2.setSpeed(motorSpeed+motorOffset);
  motor1.run(RELEASE);
  motor4.run(RELEASE);
  motor3.run(RELEASE);
  motor2.run(RELEASE);
  servoLook.attach(10);
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
}

void loop() 
{
  servoLook.write(100);
  delay(750);
  int distance = getDistance();
  if(distance >= stopDist)
  {
    moveForward();
  }
  while(distance >= stopDist)
  {
    distance = getDistance();
    delay(250);
  }

  stopMove();
  int turnDir = checkDirection();
  Serial.print(turnDir);
  switch (turnDir)
  {
    case 0:                                       //Turn left
      turnLeft (400);
      break;
    case 1:                                       //Turn around
      turnLeft (700);
      break;
    case 2:                                       //Turn right
      turnRight (400);
      break;
  }
}

void accelerate()
{
  for (int i=0; i<motorSpeed; i++)
  {
    motor1.setSpeed(i);
    motor2.setSpeed(i+motorOffset);
    motor3.setSpeed(i+motorOffset);
    motor4.setSpeed(i);
    delay(10);
  }
}

void decelerate()
{
  for (int i=motorSpeed; i!=0; i--)
  {
    motor1.setSpeed(i);
    motor2.setSpeed(i+motorOffset); 
    motor3.setSpeed(i+motorOffset);
    motor4.setSpeed(i);
    delay(10);
  }
}

void moveForward()
{
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void stopMove()
{
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void turnLeft(int duration)
{
  motor1.setSpeed(motorSpeed+turnSpeed);
  motor2.setSpeed(motorSpeed+motorOffset+turnSpeed);
  motor3.setSpeed(motorSpeed+motorOffset+turnSpeed);
  motor4.setSpeed(motorSpeed+turnSpeed);
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(FORWARD);
  
  delay(duration);

  motor1.setSpeed(motorSpeed);
  motor2.setSpeed(motorSpeed+motorOffset);
  motor3.setSpeed(motorSpeed+motorOffset);
  motor4.setSpeed(motorSpeed);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  
}

void turnRight(int duration)
{
  motor1.setSpeed(motorSpeed+turnSpeed);
  motor2.setSpeed(motorSpeed+motorOffset+turnSpeed);
  motor3.setSpeed(motorSpeed+motorOffset+turnSpeed);
  motor4.setSpeed(motorSpeed+turnSpeed);
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);
  delay(duration);
  motor1.setSpeed(motorSpeed);
  motor2.setSpeed(motorSpeed+motorOffset);
  motor3.setSpeed(motorSpeed+motorOffset);
  motor4.setSpeed(motorSpeed);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

int getDistance()
{
  unsigned long pulseTime;
  int distance;
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  pulseTime = pulseIn(echo, HIGH, timeOut);
  distance = (float)pulseTime * 340 / 2 / 10000;
  return distance;
}

int checkDirection()
{
  int distances [2] = {0,0};                                    //Left and right distances
  int turnDir = 1;                                              //Direction to turn, 0 left, 1 reverse, 2 right
  servoLook.write(180);                                         //Turn servo to look left
  delay(500);
  distances [0] = getDistance();                                //Get the left object distance
  servoLook.write(10);                                           //Turn servo to look right
  delay(1000);
  distances [1] = getDistance();                                //Get the right object distance
  if (distances[0]>=200 && distances[1]>=200)                   //If both directions are clear, turn left
    turnDir = 0;
  else if (distances[0]<=stopDist && distances[1]<=stopDist)    //If both directions are blocked, turn around
    turnDir = 1;
  else if (distances[0]>=distances[1])                          //If left has more space, turn left
    turnDir = 0;
  else if (distances[0]<distances[1])                           //If right has more space, turn right
    turnDir = 2;
  return turnDir;
}





