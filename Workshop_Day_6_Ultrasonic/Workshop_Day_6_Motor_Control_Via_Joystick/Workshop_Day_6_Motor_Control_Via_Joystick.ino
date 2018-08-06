/***************************************************************************************************************************************************************************************************************************************************************************
Name: Arduion Workshop Day #5
Purpose: Motor control using Arduino code for SN754410 H-bridgemotor driver control.
Control the speed of the motor using using A JOYSTICK!!
***************************************************************************************************************************************************************************************************************************************************************************/

//Joystick Pin information
int xPin = A1;//initialize  the X-axis of the joystick to an anolog pin
int yPin = A0;//initialize  the Y-axis of the joystick to an anolog pin
int buttonPin = 2;//initialize  the button of the joystick to an digitalpin

//H-Bridge Pin Information to control Left motor
int motorL_speedPin = 5; // Speed(PWM) control H-bridge output 
int motorL_Forward = 6; // H-bridge direction control 
int motorL_Backward = 7; // H-bridge direction control 

//H-Bridge Pin Information to control Right motor
int motorR_speedPin = 11; // Speed(PWM) control H-bridge output 
int motorR_Forward = 12; // H-bridge direction control 
int motorR_Backward = 13; // H-bridge direction control 

//Variables needed to control motor
float left_motor_speed=0; // value for motor speed
float right_motor_speed=0; // value for motor speed

//variables to read in joystick data
int xPosition = 0;// set initial value of x-axis to 0.
int yPosition = 0;// set initial value of y-axis to 0.
int buttonState = 0;// set initial value of button to 0.

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
  
  //set joystick interface pins as inputs
  pinMode(xPin, INPUT);// set x axis as input. 
  pinMode(yPin, INPUT);//set y axis as input
  pinMode(buttonPin, INPUT); 
  
  //set left motor interface pins as outputs
   pinMode(motorL_speedPin, OUTPUT);// set Speed(PWM) control H-bridge as output 
   pinMode(motorL_Forward, OUTPUT);// H-bridge direction control as output
   pinMode(motorL_Backward, OUTPUT);// H-bridge direction control as output

//set right motor interface pins as outputs
   pinMode(motorR_speedPin, OUTPUT);// set Speed(PWM) control H-bridge as output 
   pinMode(motorR_Forward, OUTPUT);// H-bridge direction control as output
   pinMode(motorR_Backward, OUTPUT);// H-bridge direction control as output
  
//initializing left motor speed and direction
  analogWrite(motorL_speedPin, 0);//set initial speed of left to 0
  digitalWrite(motorL_Forward, HIGH);//have left motor drive forward.
  digitalWrite(motorL_Backward, LOW);//have left motor drive forward. 
//initializing right motor speed and direction  
  analogWrite(motorR_speedPin, 0);//set initial speed of right to 0
  digitalWrite(motorR_Forward, HIGH);//have right motor drive forward.
  digitalWrite(motorR_Backward, LOW);//have right motor drive forward. 
}

void loop() {

  left_motor_speed=0; // calculate speed to send to left motor
  right_motor_speed= 0;//calculate speed to send to right motor

//Gather Joystick and Button Push Values
    xPosition = analogRead(xPin);//read the x axis value
    delay(10);//this is required inbetween analog reads. 
    yPosition = analogRead(yPin);//read the y axis value
    buttonState = digitalRead(buttonPin);//read the button push value

//Print out Joystick Data which was read
    Serial.print("X: ");
    Serial.print(xPosition);
    Serial.print(" | Y: ");
    Serial.print(yPosition);
    Serial.print(" | Button: ");
    Serial.print(buttonState);
        
//CALCULATE SPEED OF MOTORS This is done using the idea that the formula
// (speed- turn) will return a value inbetween -1 and 1, and that opposite formula
// (speed + turn) will also return a value inbetween -1 and 1. This acts as a percentage
//control. 
  int turnI;//will hold the value of the X-axis of the joystick position.
  turnI=map(xPosition, 0, 1023, 100, -100);//Convert X-axis of joystick into value between -100 & 100.
  int speedI;//will hold the value of the Y-axis of the joystick position.
  speedI=map(yPosition, 0, 1023, 100, -100);//Convert Y-axis of joystick into value between -100 & 100.
  
  //printing out integer values of turn and speed
    Serial.print("   TurnI: ");
    Serial.print(turnI);
    Serial.print(" | speedI: ");
    Serial.print(speedI);
        
  //turning turn and speed into floating points inbetween -1 and 1. 
  float turnF= (float)turnI/100; // turn the -100 to 100 range to -1 to 1
  float speedF= (float)speedI/100; // turn the -100 to 100 range to -1 to 1.
  
  //printing floating point turn and speed values
   Serial.print("   TurnF: ");
    Serial.print(turnF);
    Serial.print(" | speedF: ");
    Serial.println(speedF);
  
  left_motor_speed=254*(speedF+turnF); // calculate speed to send to left motor
  right_motor_speed= 254*(speedF-turnF);//calculate speed to send to right motor
  
//double check that each motor speed is not larger than 255 or less than -255
  if(right_motor_speed > 255){right_motor_speed=254;}
  else if(right_motor_speed < -255){right_motor_speed=-254;}
  if(left_motor_speed > 255){left_motor_speed=254;}
  else if(left_motor_speed < -255){left_motor_speed=-254;}
 
//write the speeds to the motors!
  sendMotorSpeed(left_motor_speed, right_motor_speed);
  
  delay(10); // add some delay between reads
}


void sendMotorSpeed(float lSpeed, float rSpeed)//change direction of motors depending on input value
{
  //saving values to send to speed as integers, instead of floats
  int leftSpeed= (int)lSpeed;
  int rightSpeed= (int)rSpeed;
  
  if( leftSpeed < 0 )//if left motor needs to go backward
  {
       digitalWrite(motorL_Forward, LOW);//have left motor drive backward.
       digitalWrite(motorL_Backward, HIGH);//have left motor drive backward. 
       analogWrite(motorL_speedPin, abs(leftSpeed));//change speed of left motor
       Serial.print("   LMS BACKWARD: ");//print left motor speed
       Serial.print(left_motor_speed);
  }
  else //if left motor needs to go forward
  {
        digitalWrite(motorL_Forward, HIGH);//have left motor drive forward.
       digitalWrite(motorL_Backward, LOW);//have left motor drive forward. 
       analogWrite(motorL_speedPin, abs(leftSpeed));//change speed of left motor
       Serial.print("   LMS FORWARD: ");//print left motor speed
       Serial.print(left_motor_speed);
  }
  if( rightSpeed < 0 )//if right motor needs to go backward
  {
       digitalWrite(motorR_Forward, LOW);//have right motor drive backward.
       digitalWrite(motorR_Backward, HIGH);//have right motor drive backward. 
       analogWrite(motorR_speedPin, abs(rightSpeed));//change speed of right motor
       Serial.print("   RMS BACKWARD: ");//print left motor speed
       Serial.println(right_motor_speed);
  }
  else//if left motor needs to go forward 
  {
        digitalWrite(motorR_Forward, HIGH);//have right motor drive forward.
       digitalWrite(motorR_Backward, LOW);//have right motor drive forward. 
       analogWrite(motorR_speedPin, abs(rightSpeed));//change speed of right motor
       Serial.print("   RMS FORWARD: ");//print left motor speed
       Serial.println(right_motor_speed);
  }
  return;
}
