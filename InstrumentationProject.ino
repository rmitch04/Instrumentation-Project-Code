#include "Wire.h"
#include "MPU6050.h"

MPU6050 mpu;

const int buttonPin = 2;
bool measuring = false;
bool lastButtonState = HIGH;
char armSelected;

//Arrays to store the Range of motion (ROM) results and what the normal ranges the ROM should be
float testResults [10] = {0.0};
int normalRanges[10] = {170, 150, 150, 80, 80, 80, 80, 80, 70, 170};


void setup() 
{
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();

  pinMode(buttonPin, INPUT_PULLUP);

  //display error message if one cant connect to the sensor
  if (!mpu.testConnection()) 
  {
    Serial.println("Sensor not found");
    while (1);
  }

  printMenu();  //prints the test selection menu
}

void loop() 
{
  if (Serial.available()) 
  {
    //read the test that the user has selected
    int testSelection = Serial.parseInt();
    while (Serial.available()) Serial.read(); 

    // The follwing if, else if statements are used to show the user which test they selected and then performs the test.
    // When they select a test, it performs the ROM calculation.
    // After this the selection menu is display again
    if(testSelection == 1)
    {
      Serial.println("Shoulder Flexion test selected");
      runFlexExtendAbductTests(testSelection);
      printMenu();
    }

    else if(testSelection == 2)
    {
      Serial.println("Elbow Flexion test selected");
      runFlexExtendAbductTests(testSelection);
      printMenu();
    }

    else if(testSelection == 3)
    {
      Serial.println("Elbow Extension test selected");
      runFlexExtendAbductTests(testSelection);
      printMenu();
    }

    else if (testSelection == 4) 
    {
      armSelected = 'R';
      Serial.println("Right Forearm Supination Test Selected");
      runSupinationPronationTest(testSelection, armSelected);
      printMenu(); 
    } 

    else if(testSelection == 5)
    {
      armSelected = 'R';
      Serial.println("Right Forearm Pronation test Selected");
      runSupinationPronationTest(testSelection, armSelected);
      printMenu();
    }

    else if(testSelection == 6)
    {
      armSelected = 'L';
      Serial.println("Left Forearm Supination test Selected");
      runSupinationPronationTest(testSelection, armSelected);
      printMenu();
    }

    else if (testSelection == 7)
    {
      armSelected = 'L';
      Serial.println("Left Forearm Pronation test Selected");
      runSupinationPronationTest(testSelection, armSelected);
      printMenu();
    }

    else if(testSelection == 8)
    {
      Serial.println("Wrist Flexion test selected");
      runFlexExtendAbductTests(testSelection);
      printMenu();
    }

    else if(testSelection == 9)
    {
      Serial.println("Wrist Extension test selected");
      runFlexExtendAbductTests(testSelection);
      printMenu();
    }

    else if(testSelection == 10)
    {
      Serial.println("Shoulder Abduction test selected");
      runFlexExtendAbductTests(testSelection);
      printMenu();
    }

    else if(testSelection == 0)
    {
      printResults();
    }

    else // displays error message if user does not input a valid option
    {
      Serial.println("Invalid input. Please select a valid input");
      printMenu();
    }
  }
}


//Function that performs the supination and pronation test
void runSupinationPronationTest(int testSelection, char armSelection) 
{
  Serial.println("Press the button to start taking measurements"); // button used to start recording measurements

  float ROM;
  float max = 0;
  float min = 360;


  while (true) //keeps looping until stop button is pressed
  {
    bool currentButtonState = digitalRead(buttonPin);

    if (lastButtonState == HIGH && currentButtonState == LOW) 
    {
      delay(300);  // Button debouncing
      measuring = !measuring;

      if (measuring) {
        Serial.println("\nMeasuring has begun. Press button to stop recording"); // when button pressed gain, stop recording measurements
        Serial.println("Raw acceleration data:");
      } else {
        Serial.println("\nMeasurement Stopped.");
        break;
      }
    }

    lastButtonState = currentButtonState;

    if (measuring) //record data from the sesnor
    {
      int16_t ax, ay, az;
      mpu.getAcceleration(&ax, &ay, &az);

      //convert raw acceleration data to gravity units
      float axf = ax / 16384.0;
      float ayf = ay / 16384.0;
      float azf = az / 16384.0;
      float measuredAngle = 0.0;

      if(armSelection == 'L')
      {
         measuredAngle = atan2(axf, sqrt(ayf * ayf + azf * azf)) * 180.0 / PI; // converts to degrees
      }
      else
      {
         measuredAngle = atan2(-axf, sqrt(ayf * ayf + azf * azf)) * 180.0 / PI; // converts to degrees
      }

      if(testSelection == 4 || testSelection == 6) // when supination is selected
      {
        if (azf > 0)
        {
         measuredAngle = 180 - measuredAngle;
        }
      }

      else
      {
        if (azf < 0) 
        {
         measuredAngle = 180 - measuredAngle;
        }
      }

      if (measuredAngle < 0) measuredAngle = 0;

      if(measuredAngle > max) // records the maximum angle measured
      {
        max = measuredAngle;
      }

      if(measuredAngle < min) // records the minimum angle measured
      {
        min = measuredAngle;
      }

     ROM = max - min; // calculates ROM
     Serial.print("x: ");
     Serial.print(ax);
     Serial.print(" y: ");
     Serial.print(ay);
     Serial.print(" z: ");
     Serial.println(az);
     delay(200);
    }
  }

  Serial.print("ROM Result: ");
  Serial.println(ROM);
  testResults[testSelection-1] = ROM; // store ROM result in appropriate place in the array
  delay(2000); // delay so user can see the ROM result before selecting the next test
}

void runFlexExtendAbductTests(int testSelection) 
{
  Serial.println("Press the button to start measurement"); // button to start recording measurements

  float ROM;
  float max = 0;
  float min = 360;

  while (true) //keeps looping until stop button is pressed
  {
    bool currentButtonState = digitalRead(buttonPin);

    if (lastButtonState == HIGH && currentButtonState == LOW) 
    {
      delay(300);  // Debounce button
      measuring = !measuring;

      if (measuring) {
        Serial.println("\nMeasuring has begun. Press button to stop recording"); // when button pressed gain, stop recording measurements
        Serial.println("Raw acceleration data:");
      } else {
        Serial.println("\nMeasurement Stopped.");
        break;
      }
    }

    lastButtonState = currentButtonState;

    if (measuring) 
    {
      int16_t ax, ay, az;
      mpu.getAcceleration(&ax, &ay, &az);

      //converts raw acceleration data to gravity units
      float axf = ax / 16384.0;
      float ayf = ay / 16384.0;
      float azf = az / 16384.0;
      float angle = 0.0;

      angle = atan2(ayf, sqrt(axf * axf + azf * azf)) * 180.0 / PI; //converts to degrees

    
      if (azf > 0) 
      {
        angle = 180 - angle;
      }

      
      angle = angle - 90; // shift angle so that starting point is 0 degrees

    
      if (angle < 0) // allows an angle of 0 to 360 degrees to be measured
      {
        angle += 360;
      }

      if (angle > 320) // caps measuremnt angle at 320 degrees so that if in starting position and hand moves backwards, a negative value will not be recorded
      {
        angle = 0;
      }

      if(angle > max) // record max angle
      {
        max = angle;
      }

      if(angle < min) // record min angle
      {
        min = angle;
      }

     ROM = max - min; // calculate ROM
     Serial.print("x: ");
     Serial.print(ax);
     Serial.print(" y: ");
     Serial.print(ay);
     Serial.print(" z: ");
     Serial.println(az);

      delay(200);
    }
  }

  //display ROM for user to see what themeasured value was
  Serial.print("ROM Result: ");
  Serial.print(ROM);
  Serial.println(" degrees");
  testResults[testSelection-1] = ROM;
  delay(2000);
}

//function that displays the test selection menu
void printMenu() 
{
  Serial.println("\nPlease select a measurement test:");
  Serial.println("1. Shoulder Flexion");
  Serial.println("2. Elbow Flexion");
  Serial.println("3. Elbow Extension");
  Serial.println("4. Right Forearm Supination");
  Serial.println("5. Right Forearm Pronation");
  Serial.println("6. Left Forearm Supination");
  Serial.println("7. Left Forearm Pronation");
  Serial.println("8. Wrist Flexion");
  Serial.println("9. Wrist Extension");
  Serial.println("10. Shoulder Abduction");
  Serial.println("Enter 0 when all necessary tests are done to display results\n");
}

//function that formats the results obtained into a table and compares the measured values to the normal ranges of motion
void printResults()
{

  Serial.println("|------------------------------------------------------------------------------------|");
  Serial.println("|       Name of Test          |   Normal ROM (Degrees)   |   Measured ROM (Degrees)  |");
  Serial.println("|------------------------------------------------------------------------------------|");

  for(int i=0; i<10; i++)
  {
    //each if statemnt ensures the correct layout of each column in the table
     if(i == 0) 
    {
      Serial.print("|  ");
      Serial.print("Shoulder Flexion");
      Serial.print("           |          0-");
      Serial.print(normalRanges[i]);

      if(testResults[i] == 0.0) 
      {
        Serial.print("           |         ");
        Serial.print("Not Performed");
        Serial.println("     |");
      }
      else
      {
        Serial.print("           |         0-");
        Serial.print(testResults[i]);
        if(testResults[i] >= 100.00) Serial.println("          |");
        else if(testResults[i]<10.00) Serial.println("            |");
        else Serial.println ("           |");
      }
      
      
    }

    if(i == 1) 
    {
      Serial.print("|  ");
      Serial.print("Elbow Extension");
      Serial.print("            |          0-");
      Serial.print(normalRanges[i]);

      if(testResults[i] == 0.0) 
      {
        Serial.print("           |         ");
        Serial.print("Not Performed");
        Serial.println("     |");
      }
      else
      {
        Serial.print("           |         0-");
        Serial.print(testResults[i]);
        if(testResults[i] >= 100.00) Serial.println("          |");
        else if(testResults[i]<10.00) Serial.println("            |");
        else Serial.println ("           |");
      }

    }

    if(i == 2) 
    {
      Serial.print("|  ");
      Serial.print("Elbow Flexion");
      Serial.print("              |          0-");
      Serial.print(normalRanges[i]);

      if(testResults[i] == 0.0) 
      {
        Serial.print("           |         ");
        Serial.print("Not Performed");
        Serial.println("     |");
      }
      else
      {
        Serial.print("           |         0-");
        Serial.print(testResults[i]);
        if(testResults[i] >= 100.00) Serial.println("          |");
        else if(testResults[i]<10.00) Serial.println("            |");
        else Serial.println ("           |");
      }
      
      
    }

    if(i == 3) 
    {
      Serial.print("|  ");
      Serial.print("Right Forearm Supination");
      Serial.print("   |          0-");
      Serial.print(normalRanges[i]);

      if(testResults[i] == 0.0) 
      {
        Serial.print("            |         ");
        Serial.print("Not Performed");
        Serial.println("     |");
      }
      else
      {
        Serial.print("            |         0-");
        Serial.print(testResults[i]);
        if(testResults[i] >= 100.00) Serial.println("          |");
        else if(testResults[i]<10.00) Serial.println("            |");
        else Serial.println ("           |");
      }
      
    }

    if(i == 4) 
    {
      Serial.print("|  ");
      Serial.print("Right Forearm Pronation");
      Serial.print("    |          0-");
      Serial.print(normalRanges[i]);

      if(testResults[i] == 0.0) 
      {
        Serial.print("            |         ");
        Serial.print("Not Performed");
        Serial.println("     |");
      }
      else
      {
        Serial.print("            |         0-");
        Serial.print(testResults[i]);
        if(testResults[i] >= 100.00) Serial.println("          |");
        else if(testResults[i]<10.00) Serial.println("            |");
        else Serial.println ("           |");
      }
      
    }

    if(i == 5) 
    {
      Serial.print("|  ");
      Serial.print("Left Forearm Supination");
      Serial.print("    |          0-");
      Serial.print(normalRanges[i]);

      if(testResults[i] == 0.0) 
      {
        Serial.print("            |         ");
        Serial.print("Not Performed");
        Serial.println("     |");
      }
      else
      {
        Serial.print("            |         0-");
        Serial.print(testResults[i]);
        if(testResults[i] >= 100.00) Serial.println("          |");
        else if(testResults[i]<10.00) Serial.println("            |");
        else Serial.println ("           |");
      }
      
    }

    if(i == 6) 
    {
      Serial.print("|  ");
      Serial.print("Left Forearm Pronation");
      Serial.print("     |          0-");
      Serial.print(normalRanges[i]);

      if(testResults[i] == 0.0) 
      {
        Serial.print("            |         ");
        Serial.print("Not Performed");
        Serial.println("     |");
      }
      else
      {
        Serial.print("            |         0-");
        Serial.print(testResults[i]);
        if(testResults[i] >= 100.00) Serial.println("          |");
        else if(testResults[i]<10.00) Serial.println("            |");
        else Serial.println ("           |");
      }
      
    }

    if(i == 7) 
    {
      Serial.print("|  ");
      Serial.print("Wrist Flexion");
      Serial.print("              |          0-");
      Serial.print(normalRanges[i]);

      if(testResults[i] == 0.0) 
      {
        Serial.print("            |         ");
        Serial.print("Not Performed");
        Serial.println("     |");
      }
      else
      {
        Serial.print("            |         0-");
        Serial.print(testResults[i]);
        if(testResults[i] >= 100.00) Serial.println("          |");
        else if(testResults[i]<10.00) Serial.println("            |");
        else Serial.println ("           |");
      }
      
    }

    if(i == 8) 
    {
      Serial.print("|  ");
      Serial.print("Wrist Extension");
      Serial.print("            |          0-");
      Serial.print(normalRanges[i]);

      if(testResults[i] == 0.0) 
      {
        Serial.print("            |         ");
        Serial.print("Not Performed");
        Serial.println("     |");
      }
      else
      {
        Serial.print("            |         0-");
        Serial.print(testResults[i]);
        if(testResults[i] >= 100.00) Serial.println("          |");
        else if(testResults[i]<10.00) Serial.println("            |");
        else Serial.println ("           |");
      }
      
    }

    if(i == 9) 
    {
      Serial.print("|  ");
      Serial.print("Shoulder Abduction");
      Serial.print("         |          0-");
      Serial.print(normalRanges[i]);

      if(testResults[i] == 0.0) 
      {
        Serial.print("           |         ");
        Serial.print("Not Performed");
        Serial.println("     |");
      }
      else
      {
        Serial.print("           |         0-");
        Serial.print(testResults[i]);
        if(testResults[i] >= 100.00) Serial.println("          |");
        else if(testResults[i]<10.00) Serial.println("            |");
        else Serial.println ("           |");
      }

    }


  }

  Serial.println("|------------------------------------------------------------------------------------|");

}
