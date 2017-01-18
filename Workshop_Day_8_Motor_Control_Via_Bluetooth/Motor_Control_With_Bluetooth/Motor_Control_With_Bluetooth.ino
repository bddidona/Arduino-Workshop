/*
* Arduino code for SN754410 H-bridge
* motor driver control using HC-06 Bluetooth communication Module!
*Control the speed of the motor using A bluetooth app!
*Created by: Michael Bowyer 11/23/2016
*/

//H-Bridge Pin Information to control Left motor
int motorL_speedPin = 11; // Speed(PWM) control H-bridge output 
int motorL_Forward = 12; // H-bridge direction control 
int motorL_Backward = 13; // H-bridge direction control 

//H-Bridge Pin Information to control Right motor
int motorR_speedPin = 5; // Speed(PWM) control H-bridge output 
int motorR_Forward = 6; // H-bridge direction control 
int motorR_Backward = 7; // H-bridge direction control 

//Variables needed to control motor
float left_motor_speed=0; // value for motor speed
float right_motor_speed=0; // value for motor speed
int start_stop=0;

//variables to read in from the bluetooth module
char recieved_data; // variable for receiving data from bluetooth serial port
int LED = 13; // On-board LED pin detail

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
  
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
  
  //initializng bluetooth controlled LED output
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

void loop() {
  //start out by turning motors off if no input is recieved
  left_motor_speed=0; // calculate speed to send to left motor
  right_motor_speed= 0;//calculate speed to send to right motor

//Gather Joystick and Button Push Values
   if( Serial.available() ) // if serial data is available to read 
   {
     recieved_data = Serial.read(); //read data & store it in ‘recd_dat’ 
   }

//Print out Joystick Data which was read
    Serial.print("Recieved from Bluetooth: ");
    Serial.print(recieved_data);
        
//CALCULATE SPEED OF MOTORS BASED ON INPUT
switch (recieved_data) {
    case 'W'://Drive Robot Forward
       left_motor_speed=254;// Turn left motor full speed
       right_motor_speed= 254;//turn right motor full speed
      break;
    case 'A'://turn robot left
      left_motor_speed=-254;// Turn left motor full speed
      right_motor_speed= 254;//turn right motor full speed
      break;
    case 'S'://drive robot backward
      left_motor_speed=-254;// Turn left motor full speed
      right_motor_speed=-254;//turn right motor full speed
      break;
    case 'D'://turn robot right
      left_motor_speed=254;// Turn left motor full speed
      right_motor_speed=-254;//turn right motor full speed
      break; 
    case 'Z'://Force Robot to Stop      left_motor_speed=254;// Turn left motor full speed
      start_stop=0;
      break; 
    case 'X'://Allow Robot to Move again      left_motor_speed=254;// Turn left motor full speed
      start_stop=1;
      break;   
    default:  // if nothing else matches, do the default
     left_motor_speed=0; // calculate speed to send to left motor
     right_motor_speed= 0;//calculate speed to send to right motor
    break;
}
//write the speeds to the motors!
  if(start_stop==1)
  {
    sendMotorSpeed(left_motor_speed, right_motor_speed);
  }
  else
  {
    sendMotorSpeed(0, 0);
  }
  
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
