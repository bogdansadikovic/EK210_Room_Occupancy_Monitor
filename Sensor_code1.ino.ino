#include <LiquidCrystal_I2C.h>
#include <Wire.h>


// Try to use the sensors at a 15 degree angle, apparealntly it improves their accuracy. 
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define Motor_pin 7 //Motor Pin at Digital pin 7
const int sensorPin = A0; // Analog input pin connected to the increment sensor
const int sensorPin2 = A1; // Analog input pin connected to the decrement sensor
int counter = 0; // Initialize the counter
int maxCap = 0; // Current max capacity
float voltage;
float voltage2;
unsigned long lastTriggerTime1 = 0; // Time of the last trigger for sensor 1
unsigned long lastTriggerTime2 = 0; // Time of the last trigger for sensor 2


int similardist = 0; //Value that is used for checking the distance and to check if someone is performing foul play or not. 
float lastvolt = 0; //Last voltage to compare the distances to.

int countertime = 0;
int spincounter = 0; //Counter for how many times the motor spins. 

//Digital pin connections for the Buttons:
const int but1 = 4;
const int but2 = 2;
const int but3 = 3;
const int but4 = 5;
const int but5 = 6;


const int buzz = 8; //Digital pin for buzzer



void capcreator() {

  //Function whose purpose is to allow user to input how much they want the maximum capacity to be. They can increment/decrement by 1 or 10. Once they are done, they press the middle button to start. 
  bool isconfirmed = false;


  while (isconfirmed == false) {
    int copynum = maxCap;
    lcd.clear();

    int counternum = 0;
    int state1 = digitalRead(but1);
    int state2 = digitalRead(but2);
    int state3 = digitalRead(but3);
    int state4 = digitalRead(but4);
    int state5 = digitalRead(but5);

// Static variables retain their values between function calls:
    static int prev1 = LOW; 
    static int prev2 = LOW;
    static int prev4 = LOW;
    static int prev5 = LOW;

    // Check for button press events and update maxCap accordingly
    if (state1 != prev1) {
      if (state1 == HIGH && maxCap >= 10) {
        maxCap -= 10;
      }
      prev1 = state1;
    }

    if (state2 != prev2) {
      if (state2 == HIGH && maxCap >= 0) {
        maxCap--;
      }
      prev2 = state2;
    }

    if (state4 != prev4) {
      if (state4 == HIGH) {
        maxCap++;
      }
      prev4 = state4;
    }

    if (state5 != prev5) {
      if (state5 == HIGH) {
        maxCap += 10;
      }
      prev5 = state5;
    }

    if (state3 == HIGH) {
      isconfirmed = true;
    }

  //Just in case, so that the program doesn't mess up accidentally, one final check for the values to be between 0 and 100:
    if(maxCap > 100){
      maxCap = 100;
    }
    else if(maxCap < 0){
      maxCap = 0;
    }

    lcd.print(maxCap);
    delay(100);
  }

  return maxCap;
}

void setup() {

  //Turn off the motor upon turning on the program:
  pinMode(Motor_pin, OUTPUT);
  digitalWrite(Motor_pin, LOW);
  

  //Initialize the LCD:
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);

  


     capcreator(); //Initialize Maximum Capacity.



//Initiakuze the Sensors: 
    pinMode(sensorPin, INPUT); // Set sensor pin as input
    pinMode(sensorPin2, INPUT); // Set sensor pin 2 as input


    Serial.begin(9600); // Initialize serial communication
   
    
}


void loop() {

  if(countertime == 0){
    lcd.clear();
    lcd.print("0");
    countertime++;
    
  }
  
    unsigned long currentTime = millis(); // Get the current time
    int sensorValue1 = analogRead(sensorPin); // Read sensor value 1
    int sensorValue2 = analogRead(sensorPin2); // Read sensor value 2
   

    // Check if sensor 1 is triggered and enough time has passed since the last trigger
    if (sensorValue1 > 100 && currentTime - lastTriggerTime1 >= 3000) {
        lastTriggerTime1 = currentTime; // Update last trigger time for sensor 1
        lastTriggerTime2 = currentTime; // Reset last trigger time for sensor 2

        if (counter < maxCap) { // Check if counter has reached maximum capacity
            counter++; // Increment the counter
            voltage = sensorValue1 * (5.0 / 1023.0);
            voltage2 = sensorValue2 * (5.0 / 1023.0);
        }

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Counter: ");
        lcd.print(counter);
        Serial.print("Sensor 1 triggered! Counter: ");
        Serial.println(counter);
        Serial.print("Sensor Value is: ");
        Serial.println(sensorValue1);
        Serial.println(voltage);

    //   if((((lastvolt + 20) >= voltage) || (lastvolt - 20 <= voltage))){
    //     similardist++;
    //     if(similardist >= 4){
    //       //Beep the buzzer for a few seconds. 
    //       //Reset the coutner back to the original value (before the foul play) --> This goes off of the assumption that it is statistically incredibly low odds that 4 people will enter a room in the exact same way.:
    //         counter = counter - similardist; 
    //     }
    //   }
        
    //    lastvolt = voltage;
    }

    // Check if sensor 2 is triggered and enough time has passed since the last trigger
    if (sensorValue2 > 100 && currentTime - lastTriggerTime2 >= 3000) {
        lastTriggerTime2 = currentTime; // Update last trigger time for sensor 2
        lastTriggerTime1 = currentTime; // Reset last trigger time for sensor 1

        if (counter > 0) { // Check if counter is greater than 0
            counter--; // Decrement the counter
            spincounter = 0;
        }

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Counter: ");
        lcd.print(counter);
        Serial.print("Sensor 2 triggered! Counter: ");
        Serial.println(counter);
        Serial.print("Sensor Value is: ");
        Serial.println(sensorValue2);
        Serial.println(voltage2);
       

    //  if((((lastvolt + 20) >= voltage) || (lastvolt - 20 <= voltage))){
    //     similardist++;
    //     if(similardist >= 4){
    //       //Beep the buzzer for a few seconds. 
    //       //Reset the coutner back to the original value (before the foul play) --> This goes off of the assumption that it is statistically incredibly low odds that 4 people will enter a room in the exact same way.:
    //         counter = counter - similardist; 
    //     }
    //   }
        
    //    lastvolt = voltage;
   }


    if(counter >= maxCap){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Max Cap!");
      lcd.setCursor(0,1);
      lcd.print("No more Entry");

    
      if(spincounter == 0){
      digitalWrite(Motor_pin, HIGH);
      digitalWrite(buzz, HIGH);
      delay(5000);
      digitalWrite(Motor_pin, LOW);
      digitalWrite(buzz, LOW);
      spincounter++;//So that it doesn't redo multiple times.
      }


      // int reset = digitalRead(but3);
      // if(reset == HIGH){
      //   capcreator(); //Reset the code if button pressed after max capacity reached. 
      //   spincounter = 0;
      //   return; //Exit current loop.
      // }
      Serial.print("Max Capacity Reached, flag spinning!");
      
     // exit(0); //Ends program runtime. --> make it reactivate with button press?
      

    }


    delay(800); //Delay so the program doesn't run too fast. 
    
}
