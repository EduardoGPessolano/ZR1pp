#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Servo.h>

#define ENGINE_INPUT2 2
#define SPEED_PIN 3
#define ENGINE_INPUT1 4
#define MODULE_KEY 5   // the key pin of the HC05 module (AT mode)
#define RX_PIN 10
#define TX_PIN 11
#define TAILLIGHTS 12
#define LDR A0
#define TMP A5
#define AT_INIT         // AT configuration on startup

//const int TMP = A5;    //This is the Arduino Pin that will read the sensor output

SoftwareSerial btSerial(RX_PIN, TX_PIN); // connexion to HC05 

String inputString = "";        // a string to hold incoming data
boolean stringComplete = false; // whether the string is complete
char mem;
bool isAuto;
unsigned long tc, tp;    // the time, for timeout processing
const long interval = 5000;
const long interval_2 = 2000;
unsigned long previousMillis = 0;
unsigned long previousMillis_2 = 0;

// functions header
void turn(char &direction);
void engine_control(char &mode);
void onOFF(int pin1, int pin2, bool lhs, bool rhs);
void lights(char &mode);
bool waitFor(const char *rep);
double tmp();

Servo steeringWheel;
int direction;

void setup()
{
  pinMode(TAILLIGHTS, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);

  pinMode(ENGINE_INPUT1, OUTPUT);
  pinMode(ENGINE_INPUT2, OUTPUT);
  pinMode(SPEED_PIN, OUTPUT);

  steeringWheel.attach(6);

  // pinMode(ARDUINO_LED, OUTPUT);  // the on board led pin
  pinMode(MODULE_KEY, OUTPUT);   // the HC05 Key pin (switch to AT mode when HIGH)
  digitalWrite(MODULE_KEY, LOW); // communication mode

  inputString.reserve(300); // reserve 300 char max

  Serial.begin(9600);   // on board Serial
  btSerial.begin(9600); // SoftwareSerial connected to HC05 BTmodule
  tp = millis();        // get time
}

void loop() 
{
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();
  unsigned long currentMillis_2 = millis();
  analogWrite(SPEED_PIN, 255); //Aciona o motor

  // wait for a complete line from bluetooth device
  while (btSerial.available())
  {
    if (currentMillis - previousMillis >= interval)
    {
      previousMillis = currentMillis;
      btSerial.print(tmp());
    }
    char inChar = (char)btSerial.read(); // get the new byte:
    inputString += inChar;               // add it to the inputString:
    if (inChar == '\n')
    {                        // if the incoming character is a newline, set a flag
      stringComplete = true; // so the main loop can do something about it:
      break;
    }
  }
  if (stringComplete)
  { // a command is available to proceed   
      stringComplete = false;
    //Serial.println(inputString);
      Serial.print(mem);

    if(inputString[0] == 'A' || inputString[0] == 'N' || inputString[0] == 'Y')
      mem = inputString[0];

    isAuto = (mem == 'A') ? true : false;

    lights(mem);
    engine_control(inputString[0]);
    turn(inputString[0]);

    // void engine_control();
    inputString = "";     // clear the string
    //btSerial.print(?); // send a prompt char
  }
  
  if(isAuto)
  {
    if (currentMillis_2 - previousMillis_2 >= interval_2)
    {
      previousMillis_2 = currentMillis_2;
      lights(mem);
    }
  }
}

//  The waitFor a string function with time out
bool waitFor(const char *rep)
{
  unsigned long t0 = millis(), t;
  int i = 0, j = strlen(rep);
  char c;

  for (i = 0; i < j;)
  {
    if (btSerial.available())
    {
      c = (char)btSerial.read();
      Serial.print(c);
      if (c == rep[i])
        i++;
    }
    t = millis();
    if (t - t0 > 2000)
      break;
  }
  return i == j;
  delay(10);
}

//Functions definitions

//Aux function used by light and engine control functions
void onOFF(int pin1, int pin2, bool lhs, bool rhs)
{
    digitalWrite(pin1, lhs);
    digitalWrite(pin2, rhs);
    // delay(3000);
}

void engine_control(char &mode)
{ 
    Serial.println(mode);
    switch (mode)
    {
    case 'F':
      onOFF(4, 2, 0, 1); //Forward
      break;
    case 'B':
      onOFF(4, 2, 1, 0); //Backwards
      break;
    default:
      onOFF(4, 2, 0, 0); //Neutral
      break;
    }
}

void lights(char &mode)
{
    int reading;
    reading = analogRead(LDR);
    Serial.println(reading);
    switch (mode)
    {
    case 'Y':
      onOFF(7, 8, 1, 1);
      break;
    case 'N':
      onOFF(7, 8, 0, 0);
      break;
    case 'A':
      if(reading < 700){
        onOFF(7, 8, 0, 0);
      }
      else{
        onOFF(7, 8, 1, 1);
      }
      break;
    }
}

//Steering wheel
void turn(char &direction)
{
    int d;
    switch (direction)
    {
    case 'R':
      d = 130;
      break;
    case 'L':
      d = 80;
    default:
      d = 105;
      break;
    }
    steeringWheel.write(d);
}

//Engine temperature
double tmp()
{
    int sensorInput;  //The variable we will use to store the sensor input
    double temperature;     //The variable we will use to store temperature in degrees.
    sensorInput = analogRead(TMP); //read the analog sensor and store it

    temperature = (double)sensorInput / 1024; //find percentage of input reading
    temperature = temperature * 5;                   //multiply by 5V to get voltage
    temperature = temperature - 0.5;                 //Subtract the offset
    temperature = temperature * 100;                 //Convert to degrees

    return temperature;
}
