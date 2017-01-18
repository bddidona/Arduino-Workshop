int xPin = A1;//initialize  the X-axis of the joystick to an anolog pin
int yPin = A0;//initialize  the Y-axis of the joystick to an anolog pin
int buttonPin = 2;//initialize  the button of the joystick to an digitalpin

int xPosition = 0;// set initial value of x-axis to 0.
int yPosition = 0;// set initial value of y-axis to 0.
int buttonState = 0;// set initial value of button to 0.

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
  
  pinMode(xPin, INPUT);// set x axis as input. 
  pinMode(yPin, INPUT);//set y axis as input.

  //activate pull-up resistor on the push-button pin
  pinMode(buttonPin, INPUT_PULLUP); 
  
  // For versions prior to Arduino 1.0.1
  // pinMode(buttonPin, INPUT);
  // digitalWrite(buttonPin, HIGH);
}

void loop() {
  //Gather Joystick and Button Push Values
  xPosition = analogRead(xPin);//read the x axis value
  delay(10);//this is required inbetween analog reads. 
  yPosition = analogRead(yPin);//read the y axis value
  buttonState = digitalRead(buttonPin);//read the button push value
  
  Serial.print("X: ");
  Serial.print(xPosition);
  Serial.print(" | Y: ");
  Serial.print(yPosition);
  Serial.print(" | Button: ");
  Serial.println(buttonState);

  delay(100); // add some delay between reads
}
