/* Stub for CDC.cpp */

#include <CDC.cpp>
// Motor pins
// Connect the motor pins by looking at the move functions
const int motor2Left = 5;
const int motor2Right = 4;
const int motor1Right =6;
const int motor1Left = 7;

// Ultrasonic sensor pins
const int trigPin = 9;
const int echoPin = 10;

float measuredDistance;
float userDistance;
float moveBackDistance;

void setup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(motor1Right, OUTPUT);
  pinMode(motor2Right, OUTPUT);
  pinMode(motor1Left, OUTPUT);
  pinMode(motor2Left, OUTPUT);

  // Setting all the pins to low at first
  stopMoving();
  Serial.begin(9600);
}

void loop()
{
//======================= Getting user guessed value ========================

  Serial.println("\n\n Guess the distance to the obstacle ( in cm )");
  while(Serial.available() == 0);
  if(Serial.available() > 0)
  {
    userDistance = (float)Serial.parseInt();
    while(Serial.available()>0){
      Serial.read();
    }
  }
  if(userDistance>0){
    Serial.print("User guess = ");
  Serial.println(userDistance);

  //===================================================================

  //========================== Moving the kart ========================

  Serial.print("Moving the kart for ");
  Serial.print(userDistance);
  Serial.println(" cm ...");
  getDistance();
  Serial.println(measuredDistance);
  if(measuredDistance < userDistance+1.0) // The +1 for userDistance is an offset for the min distance the sensor can measure
  {
    moveBackDistance = measuredDistance - 1.0;
    while(measuredDistance > 4.0)
    {
      Serial.println(measuredDistance);
      moveForward();
      getDistance();
    }
    delay(3000);
    stopMoving();


    Serial.println("Crashed!!!");
    Serial.println("User guess was wrong");
  }
  else
  {
    moveBackDistance = userDistance;
    float correctPercent = (userDistance/measuredDistance)*100;
    float temp = measuredDistance - userDistance;
    Serial.println(temp);
    Serial.println(measuredDistance);
    while(measuredDistance > temp)
    {
      Serial.println(measuredDistance);
      Serial.println(temp);
      moveForward();
      getDistance();
    }
    stopMoving();
    Serial.println("Moved");
    Serial.print("The user is ");
    Serial.print(correctPercent);
    Serial.println("% correct");
  }

  //==================================================================

  //========================== Moving back ===========================

  Serial.println("\nMoving back in 5 seconds...");
  for(int i=1; i<=5; i++)
  {
    Serial.print("  ");
    Serial.print(i);
    delay(1000);
  }
  Serial.println("\n\nMoving back...");
  getDistance();
  float temp = measuredDistance + moveBackDistance;
  while(measuredDistance < temp)
  {
    moveBackward();
    getDistance();
  }
  stopMoving();

  //==================================================================

  delay(3000);
  }
  Serial.println("Do you want the kart to calculate the distnc. and move ?(y/n)");
  while(Serial.available()==0);
  if(Serial.read()=='y'){
     AI();
  }

  /*delay(3000);
  serial.printIn("Move the kart back? (y/n)";
  while(Serial.available()==0);
  if(Serial.read()=='y')
  { Serial.println("\nMoving back in 5 seconds...");
  for(int i=1; i<=5; i++)
  {
    Serial.print("  ");
    Serial.print(i);
    delay(1000);
  }

  Serial.println("\n\nMoving back...");
  getDistance();
  float temp = measuredDistance + moveBackDistance;
  while(measuredDistance < temp)
  {
    moveBackward();
    getDistance();
  }
 stopMoving();*/





}

void getDistance()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);

  // Calculating distance
  measuredDistance = duration*(0.034/2);
}

//====================== Kart Moving Functions ============================

int power=150;

void stopMoving()
{
  digitalWrite(motor1Left, LOW);
  digitalWrite(motor2Right, LOW);
  digitalWrite(motor1Right, LOW);
  digitalWrite(motor2Left, LOW);
}

void moveForward()
{
  analogWrite(motor1Left, power);
  analogWrite(motor2Right, power);
  digitalWrite(motor1Right, LOW);
  digitalWrite(motor2Left, LOW);
}

void moveBackward()
{
  digitalWrite(motor1Left, LOW);
  digitalWrite(motor2Right, LOW);
  analogWrite(motor1Right, power);
  analogWrite(motor2Left, power);
}

void AI(){
  //moveBackDistance = measuredDistance - 5.0;
  getDistance();
    while(measuredDistance > 10.5)
    {
      Serial.println("AI:"+String(measuredDistance));
      moveForward();
      getDistance();
    }
    stopMoving();
}
