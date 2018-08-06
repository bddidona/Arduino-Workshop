/*
* Arduino code for SN754410 H-bridge motor driver control.
*Control the speed of the motor using a bluetooth app with an on screen joystick
*Created by: Michael Bowyer 3/28/2017
*/

//H-Bridge Pin Information to control Left motor
int motorL_speedPin = 11; // Speed(PWM) control H-bridge output 
int motorL_Forward = 12; // H-bridge direction control 
int motorL_Backward = 13; // H-bridge direction control 

//H-Bridge Pin Information to control Right motor
int motorR_speedPin = 5; // Speed(PWM) control H-bridge output 
int motorR_Forward = 6; // H-bridge direction control 
int motorR_Backward = 7; // H-bridge direction control 

//HORN and Lights Declarations
int horn_pin=10;
int right_HL=9;
int left_HL=8;

//Variables needed to control motor speed
float left_motor_speed=0; // value for motor speed
float right_motor_speed=0; // value for motor speed

//variables to read in joystick or gyro data from bluetooth string
float xPosition = 0;// set initial value of x-axis to 0.
float yPosition = 0;// set initial value of y-axis to 0.

//variables to read in slider data from the bluetooth string
float left_slider = 0;// set initial value of x-axis to 0.
float right_slider = 0;// set initial value of y-axis to 0.

//Variables to Read in bluetooth communication data
char recd_dat; // variable for receiving data from bluetooth serial port
String recieved_string = "";//recieved string holds data recieved by bluetooth chip
long last_recieved_time=0;//holds the last time a message which made sense was recieved
long wait_for_BT_packet=20;//time to wait for a bluetooth packet to arrive before updating motor speeds
long max_no_com_time=500;// maximum time in which no meaningful message was recieved in order to stop the vehicle.

//LIGHT CONTROL VARIABLES
int on_brd_led = 13; // On-board LED pin detail

void setup()
{
//Initialize Serial COmmunication  
    Serial.begin(9600);
    
// Onboard LED pin as output and turn it off
    pinMode(on_brd_led, OUTPUT);    
    digitalWrite(on_brd_led, LOW);
    
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

//INitializing Head lghts and Horn
   pinMode(horn_pin, OUTPUT);// set horn pin as output
   pinMode(right_HL, OUTPUT);// set headlight pint as output
   pinMode(left_HL, OUTPUT);// set headlight pint as output
   
   digitalWrite(horn_pin, LOW);//turn off the horn to start
   digitalWrite(right_HL, LOW);//turn off the headlight to start
   digitalWrite(left_HL, LOW);//turn off the headlight to start
   
}


void loop() 
{
/*READ IN BLUE TOOTH STRING*/
    recieved_string="";//reset the recieved message to nothing
    long start_time=millis();// save the start time which we begin waiting on a packet
    while(millis()-start_time < wait_for_BT_packet)//loop for wait_for_BT_Packet milliseconds
    {
      while (Serial.available() > 0) {//combine any messages sent in this time frame.
        char inChar = Serial.read();//save the character read in
        recieved_string += inChar;//add the character to a string
      }
    }
/*PARSE BLUETOOTH STRING*/ 

    if(recieved_string != "")//if string is empty
    {
       Serial.println("Message Length in Bytes: " + recieved_string.length());
       Serial.println("Recieved Message is: "+ recieved_string);
       Serial.println("Message Elements 0&1 Are: "+ recieved_string.substring(0,2));
       Serial.println("Message Elements 6&7 Are: "+ recieved_string.substring(7,9));
       
       //Gyroscope or Joystick Data in format X:+0.00Y:-1.00
       if( (recieved_string.substring(0,2)== "X:") && (recieved_string.substring(7,9)== "Y:") )// make sure the X and Y are in the right position
       {
         String turn_string=recieved_string.substring(3,7);//take a substring of the read in message
         xPosition = turn_string.toFloat();//convert it to a number
         if(recieved_string.substring(2,3)=="-"){xPosition =-xPosition;}//check the sign of the number, if it is negative then flip it
         
         String speedr_string=recieved_string.substring(10,14);//take a substring of the read in message
         yPosition = speedr_string.toFloat();//convert it to a number
         if(recieved_string.substring(9,10)=="-"){yPosition =-yPosition;}//check the sign of the number, if it is negative then flip it
         Serial.println(xPosition);
         Serial.println(yPosition);
         
         left_motor_speed=254*(yPosition+xPosition); // calculate speed to send to left motor
         right_motor_speed= 254*(yPosition-xPosition);//calculate speed to send to right motor
         
         last_recieved_time=millis();//save the last time that a message was properly recieved
       }       
       
       //Slider Data for Tank drive control
       if( (recieved_string.substring(0,2)== "L:") && (recieved_string.substring(7,9)== "R:") )
       {
         String left_string=recieved_string.substring(3,7);//take a substring of the read in message
         left_slider = left_string.toFloat();//convert it to a number
         if(recieved_string.substring(2,3)=="-"){left_slider =-left_slider;}//check the sign of the number, if it is negative then flip it
         
         String right_string=recieved_string.substring(10,14);//take a substring of the read in message
         right_slider = right_string.toFloat();//convert it to a number
         if(recieved_string.substring(9,10)=="-"){right_slider =-right_slider;}//check the sign of the number, if it is negative then flip it
         Serial.println(left_slider);
         Serial.println(right_slider);
         
         left_motor_speed=254*(left_slider); // calculate speed to send to left motor
         right_motor_speed= 254*(right_slider);//calculate speed to send to right motor
         
         last_recieved_time=millis();//save the last time that a message was properly recieved
       }
       
       /*HORN AND LIGHTS CONTROL*/
       if(recieved_string == "HL_ON")//if headlights should be turned on.
       {
           digitalWrite(right_HL, HIGH);//turn off the headlights
           digitalWrite(left_HL, HIGH);//turn off the headlights
       }
       if(recieved_string == "HL_OFF")//if headlights should be turned off.
       {
           digitalWrite(right_HL, LOW);//turn off the headlights
           digitalWrite(left_HL, LOW);//turn off the headlights
       }
       if(recieved_string == "HORN_ON")//if horn should be turned on.
       {
           digitalWrite(horn_pin, HIGH);//turn on the horn
       }
       if(recieved_string == "HORN_OFF")//if horn should be turned off.
       {
           digitalWrite(horn_pin, LOW);//turn off the headlight
       }
    }
    
/*SEND MOTOR SPEEDS*/    
    if(millis() - last_recieved_time < max_no_com_time)// if it hasn't been to long since a message has been recieved
    {
      
    }else{//it has been too long since a message was recieved, so stop the vehicle.
      left_motor_speed=0; // calculate speed to send to left motor
      right_motor_speed=0;//calculate speed to send to right moto
    }
    sendMotorSpeed(left_motor_speed, right_motor_speed);//change motor speed
}



void sendMotorSpeed(float lSpeed, float rSpeed)//change direction of motors depending on input value
{
  //saving values to send to speed as integers, instead of floats
  int leftSpeed= (int)lSpeed;
  int rightSpeed= (int)rSpeed;
  
  //limit the speeds to -254 to 255
  if(leftSpeed > 255){leftSpeed=255;}
  if(leftSpeed < -254){leftSpeed=-254;}
  if(rightSpeed > 255){rightSpeed=255;}
  if(rightSpeed < -254){rightSpeed=-254;}
  
  
  if( leftSpeed < 0 )//if left motor needs to go backward
  {
       digitalWrite(motorL_Forward, LOW);//have left motor drive backward.
       digitalWrite(motorL_Backward, HIGH);//have left motor drive backward. 
       analogWrite(motorL_speedPin, abs(leftSpeed));//change speed of left motor
       Serial.print("   LMS BACKWARD: ");//print left motor speed
       Serial.print(leftSpeed);
  }
  else //if left motor needs to go forward
  {
       digitalWrite(motorL_Forward, HIGH);//have left motor drive forward.
       digitalWrite(motorL_Backward, LOW);//have left motor drive forward. 
       analogWrite(motorL_speedPin, abs(leftSpeed));//change speed of left motor
       Serial.print("   LMS FORWARD: ");//print left motor speed
       Serial.print(leftSpeed);
  }
  if( rightSpeed < 0 )//if right motor needs to go backward
  {
       digitalWrite(motorR_Forward, LOW);//have right motor drive backward.
       digitalWrite(motorR_Backward, HIGH);//have right motor drive backward. 
       analogWrite(motorR_speedPin, abs(rightSpeed));//change speed of right motor
       Serial.print("   RMS BACKWARD: ");//print left motor speed
       Serial.println(rightSpeed);
  }
  else//if left motor needs to go forward 
  {
        digitalWrite(motorR_Forward, HIGH);//have right motor drive forward.
       digitalWrite(motorR_Backward, LOW);//have right motor drive forward. 
       analogWrite(motorR_speedPin, abs(rightSpeed));//change speed of right motor
       Serial.print("   RMS FORWARD: ");//print left motor speed
       Serial.println(rightSpeed);
  }
  return;
}
