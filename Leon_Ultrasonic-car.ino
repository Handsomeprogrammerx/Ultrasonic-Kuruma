#include <ESP8266WiFi.h>
#include <Servo.h>

Servo servo; //pin D4
#define trig D6 //30deg, 2cm-400cm, 10microsec +-0.3cm
#define echo D5
#define ENA D1
#define ENB D7
#define IN1 D2
#define IN2 D3                                         
#define IN3 D8
#define IN4 D0
int motor_speed = 80; //75-255, 83.25cm/s
int reverse_speed = 85;
int rotation_speed = 90;



//When will it boot?
void setup() 
{
  Serial.begin(115200);
  servo.attach(D4);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  servo.write(90);
  delay(300);

}

void loop() 
{
  //Measure the distance front if pass go forward
  //stop();
  //delay(1000);
  int front = measure();
  if (front > 6) //I must account doppler shift
  {
    forward();
    while (front > 20)
    {
      Serial.print("Front distance: ");
      Serial.print(front);
      Serial.println("cm");
      front = measure();
      Serial.print("Updated front = ");
      Serial.println(front);
    }
    stop();
    delay(1000);
  }
  //else //If cut this out it'll be faster
  
    servo.write(180);
    delay(500);
    int left = measure();
    Serial.print("Lefthand distance: ");
    Serial.print(left);
    Serial.println("cm");
    servo.write(90);
    delay(300);
    servo.write(0);
    delay(500);
    int right = measure();
    Serial.print("Righthand distance: ");
    Serial.print(right);
    Serial.println("cm");
    delay(5);
    servo.write(90);
    delay(500);
    if (left > 6 && right > 6)
    {
      if (left > right || left == right)
      {
        turnleft();
        delay(170);
      }
      else
      {
        turnright();
        delay(170);
      }      
    }
    else //Prevent from strucking in long straight alley
    { 
      while (left <= 6 && right <= 6)
      {
        backward();
        delay(1000);
        stop();
        servo.write(180);
        delay(500);
        left = measure();
        Serial.print("Lefthand distance: ");
        Serial.print(left);
        Serial.println("cm");
        servo.write(90);
        delay(300);
        servo.write(0);
        delay(500);
        right = measure();
        Serial.print("Righthand distance: ");
        Serial.print(right);
        Serial.println("cm");
        delay(5);
        servo.write(90);
        delay(500);
      }
    }
    if (left > right || left == right)
    {
      turnleft();
      delay(170);     
    }
    else
    {
      turnright();
      delay(170);
    }      

  
   
  //else if measure left and right if left is safer go left vice versa
  //else go backward for amount
  //measure the distance in front again 
}

int measure()
{
  while (digitalRead(echo) == HIGH);
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(11);
  digitalWrite(trig, LOW);
  int distance = (int)(((343.0 * pulseIn(echo, HIGH, 30000) * 0.0001) / 2.0) + 0.5);
  delay(100);
  if (distance == 0) return 999;
  return distance;
}

void backward()
{
  analogWrite(ENA, reverse_speed);
  analogWrite(ENB, reverse_speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Backward");
}
void forward()
{
  analogWrite(ENA, motor_speed);
  analogWrite(ENB, motor_speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Forward");
}
void turnright()
{
  analogWrite(ENA, rotation_speed);
  analogWrite(ENB, rotation_speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Turn Left");
}
void turnleft()
{
  analogWrite(ENA, rotation_speed);
  analogWrite(ENB, rotation_speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Turn Right");
}
void stop()
{
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  Serial.println("Stop!");
}
//5/6/2025 the robot does not stop in time when go forward