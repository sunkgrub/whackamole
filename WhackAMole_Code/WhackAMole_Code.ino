// Initial Code for Whack-A-Mole
// Written for EDD 111 2024
// Written By: Katherine O'Keefe
// Edited by: Koen Gieskes
// Edited further by: Aiden Rivera

//define variables used in code
//variables associated with digital pin number
//defining LED lights

const int redLED = 2;
const int blueLED = 3;
const int greenLED = 4;
const int yellowLED = 5;

//defining the corresponding buttons
const int redButton = 9;
const int blueButton = 8;
const int greenButton = 7;
const int yellowButton = 6;

//defining the piezo buzzer
const int Piezo = 10;

//defining time variables
int starttime = 0;
int elapsed = 0;

//defining the score
int score;

//sets up the the variables as inputs or outputs

void display_lights(bool Light1, bool Light2, bool Light3, bool Light4) {
  digitalWrite(redLED, Light1);
  digitalWrite(blueLED, Light2);
  digitalWrite(yellowLED, Light3);
  digitalWrite(greenLED, Light4);

}

void setup()
{
  //initialize the LED lights as OUTPUTS
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  //initialize the corrresponding buttons as INPUTS
  //(can also initialize as INPUT_PULLUP 
    //which sets the buttons to HIGH when unpressed)
  pinMode(redButton, INPUT);
  pinMode(blueButton, INPUT);
  pinMode(yellowButton, INPUT);
  pinMode(greenButton, INPUT);
  
  //initialize the random variable pick
  randomSeed(analogRead(0));
  
  //initialize the piezo buzzer
  pinMode(Piezo, OUTPUT);
}

//main function loop
void loop()
{
  //starts the game with a score of 0
  //will go through this loop until score hits 3
  while(score < 4)
  {
    //define randomColor as integer variable 
    int randomColor;
    
    //generate random # from 2 to 5 that matches led Pins
    //2 = red, 3 = blue, 4 = green, 5 = yellow
    randomColor = random(2,6);
    
    if(randomColor==2)
    {
      //digitalWrite function turns the LED ON or OFF
      //HIGH means turning the LED ON
      digitalWrite(redLED, HIGH);
    }
    
    if(randomColor==3)
    {
      //digitalWrite function turns the LED ON or OFF
      //HIGH means turning the LED ON
      digitalWrite(blueLED, HIGH);
    }
    
    if(randomColor==4)
    {
      //digitalWrite function turns the LED ON or OFF
      //HIGH means turning the LED ON
      digitalWrite(greenLED, HIGH);
    }
      
    if(randomColor==5)
    {
      //digitalWrite function turns the LED ON or OFF
      //HIGH means turning the LED ON
      digitalWrite(yellowLED, HIGH);
    }
    
    starttime = millis();
    elapsed = 0;

    //digitalRead function reads whether I/O are HIGH or LOW
    //[||] denotes "or"
    while((digitalRead(redLED)==HIGH||digitalRead(blueLED)==HIGH||digitalRead(yellowLED)==HIGH||digitalRead(greenLED)==HIGH)&&(elapsed<=1000))
    {
      //reads if the redLED is ON (HIGH)
      //if redLED is ON continue into if statement if not move on
      if(digitalRead(redLED)==HIGH)
      {
        //reads if the button corrosponding to the redLED is ON
        //if redButton is ON proceed into if statement
        if(digitalRead(redButton)==HIGH)
        {
          //shows that a point was gotten
          display_lights(1,1,1,1);
          delay(250);//in milliseconds
          
          display_lights(0,0,0,0);
          delay(250);//in milliseconds
          
          //turns the piezo button ON
          tone(Piezo,250);
          
          delay(200);//in milliseconds
          
          //turns the piezo button OFF
          noTone(Piezo);
          
          //adds 1 point to your score
          score++;

        //turns OFF the LED light
        digitalWrite(redLED, LOW);
        }
      }
      
      //reads if the blueLED is ON (HIGH)
      //if blueLED is ON continue into if statement if not move on
      if(digitalRead(blueLED)==HIGH)
      {
        //reads if the button corrosponding to the blueLED is ON
        //if blueButton is ON proceed into if statement
        if(digitalRead(blueButton)==HIGH)
        {
          //shows that a point was gotten
          digitalWrite(redLED, HIGH);
          digitalWrite(blueLED, HIGH);
          digitalWrite(greenLED, HIGH);
          digitalWrite(yellowLED, HIGH);
          
          delay(250);//in milliseconds
          
          digitalWrite(redLED, LOW);
          digitalWrite(blueLED, LOW);
          digitalWrite(greenLED, LOW);
          digitalWrite(yellowLED, LOW);
          
          delay(250);//in milliseconds
          
          //turns the piezo button ON
         tone(10,250);
          
          delay(200);//in milliseconds
          
          //turns the piezo button OFF
          noTone(10);
          
          //adds 1 point to your score
          score++;
        
        
        //turns OFF the LED light
        digitalWrite(blueLED, LOW);
        }
      }
      
      //reads if the yellowLED is ON (HIGH)
      //if yellowLED is ON continue into if statement if not move on
      if(digitalRead(yellowLED)==HIGH)
      {
        //reads if the button corrosponding to the yellowLED is ON
        //if yellowButton is ON proceed into if statement
        if(digitalRead(yellowButton)==HIGH)
        {
          //shows that a point was gotten
          digitalWrite(redLED, HIGH);
          digitalWrite(blueLED, HIGH);
          digitalWrite(greenLED, HIGH);
          digitalWrite(yellowLED, HIGH);
          
          delay(250);//in milliseconds
          
          digitalWrite(redLED, LOW);
          digitalWrite(blueLED, LOW);
          digitalWrite(greenLED, LOW);
          digitalWrite(yellowLED, LOW);
          
          delay(250);//in milliseconds
          
          //turns the piezo button ON
          tone(10,250);
          
          delay(200);//in milliseconds
          
          //turns the piezo button OFF
          noTone(10);
          
          //adds 1 point to your score
          score++;
        
        
        //turns OFF the LED light
        digitalWrite(yellowLED, LOW);
        }
      }
      
      //reads if the greenLED is ON (HIGH)
      //if greenLED is ON continue into if statement if not move on
      if(digitalRead(greenLED)==HIGH)
      {
        //reads if the button corrosponding to the greenLED is ON
        //if greenButton is ON proceed into if statement
        if(digitalRead(greenButton)==HIGH)
        {
          //shows that a point was gotten
          digitalWrite(redLED, HIGH);
          digitalWrite(blueLED, HIGH);
          digitalWrite(greenLED, HIGH);
          digitalWrite(yellowLED, HIGH);
          
          delay(250);//in milliseconds
          
          digitalWrite(redLED, LOW);
          digitalWrite(blueLED, LOW);
          digitalWrite(greenLED, LOW);
          digitalWrite(yellowLED, LOW);
          
          delay(250);//in milliseconds
          
          //turns the piezo button ON
          tone(10,250);
          
          delay(200);//in milliseconds
          
          //turns the piezo button OFF
          noTone(10);
          
          //adds 1 point to your score
          score++;
        
        
        //turns OFF the LED light
        digitalWrite(greenLED, LOW);
        }
      }
      
      elapsed = millis()-starttime;
      delay(150);//in milliseconds
    }
    display_lights(0,0,0,0);

  }
  score = 0;
  //Shows game is over
  display_lights(1,1,1,1);
  delay(500);

  display_lights(0,0,0,0);
  delay(400);

  display_lights(1,1,1,1);
  delay(300);

  display_lights(0,0,0,0);
  delay(200);

  display_lights(1,1,1,1);
  delay(200);

  display_lights(0,0,0,0);
  delay(100);

  display_lights(1,1,1,1);
  delay(500);

  display_lights(0,0,0,0);
}
