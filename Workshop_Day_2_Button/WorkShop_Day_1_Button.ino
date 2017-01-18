/***************************************************************************************************************************************************************************************************************************************************************************
Name: Arduion Trainig Workshop
Purpose: INtroduce students to the concept of Blink Led and push button
***************************************************************************************************************************************************************************************************************************************************************************/


int ledPin = 13;                // LED connected to digital pin 13
int ledPin2 =7;
int ledValue = LOW;             // previous value of the LED
long ledStartTime = 0;          // will store last time LED was updated
long ledBlinkInterval = 1000;   // interval at which to blink (milliseconds)

int buttonPin = 2;              // an active-high momentary pushbutton on pin 2
int buttonValue = HIGH;         // the current state of the button
long buttonPressTime = 0;       // will store the time that the button was pressed


void setup()
{
 pinMode(ledPin, OUTPUT);      // sets the digital pin as output
 pinMode(buttonPin, INPUT);      // sets the digital pin as output
}

void loop()
{
       // Check to see if the button is pressed.  If so, mark the time.  When the
       // button is released, calculate how long it was pressed, and make the led
       // blink at the same rate.
       buttonValue = digitalRead(buttonPin);

       // button press
       if (buttonValue==LOW && buttonPressTime==0) {
           buttonPressTime = millis();  // capture the time of the press
       }

       // button release
       if (buttonValue==HIGH && buttonPressTime!=0) {
         ledBlinkInterval = millis() - buttonPressTime;  // set the new flash interval
         buttonPressTime = 0;  // clear the button press time
         ledStartTime = millis(); 
       }

       // check to see if it's time to blink the LED; that is, is the difference
       // between the current time and last time we blinked the LED bigger than
       // the interval at which we want to blink the LED.
      if(millis() - ledStartTime > ledBlinkInterval)// check to see if LED interval time limit has passed
      {
              ledStartTime=millis(); //resetting LED state start time
              if (ledValue == LOW)//if LED is OFF
              {  ledValue = HIGH;
                digitalWrite(ledPin, ledValue);// turn it ON.
                digitalWrite(ledPin2, ledValue);
              }
              else// if LED is ON.
              {
                ledValue = LOW;
                digitalWrite(ledPin, ledValue); //Turn it OFF. 
                digitalWrite(ledPin2, ledValue);
              }   
       }
}