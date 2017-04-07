    /* JY-MCU Bluetooth Wireless Serial Port module for Arduino®
    
    ON and OFF Control of a LED thru Bluetooth .
    The code example will switch ON or OFF on board LED by sending command ‘B’
    or any other character thru the Bluetooth paired PC or Laptop.
    
    The pins used are designed are for JY-MCU Bluetooth Wireless Serial Port
    module with Arduino® Uno Board available from:
    
    https://core-electronics.com.au/store/index.php/wireless/bluetooth/jy-mcu-arduino-bluetooth-wireless-serial-port-module.html
    
    This example code is in the public domain.
    
    */ 
    // Declaration of constants and variable to used by program
    
    
    char recd_dat; // variable for receiving data from bluetooth serial port
    int on_brd_led = 13; // On-board LED pin detail
    
    void setup()
    {
        // initialize the serial communications:
        // serial communication is used to receive the data from
        // Bluetooth module
        
        Serial.begin(9600);
        
        // Onboard LED pin as output
        pinMode(on_brd_led, OUTPUT);
        
        // The initial state of led is defined here.
        // HIGH on PIN will switch on the LED
        // LOW on PIN will switch off the LED
        digitalWrite(on_brd_led, LOW);
    }
    
    void loop() 
    {
       if( Serial.available() ) // if serial data is available to read 
       {
           recd_dat = Serial.read(); //read data & store it in ‘recd_dat’ 
       }
       if(recd_dat == 'B' ) // if 'B' was received
       {
           digitalWrite(on_brd_led, HIGH); // turn ON LED
        }
        else
        {
           digitalWrite(on_brd_led, LOW); // otherwise switch OFF 
        } 
        delay(10); 
        // Just wait 150ms for next reading 
    }
      
