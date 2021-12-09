#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Servo.h>

#define ENGINE_INPUT2 2
#define SPEED_PIN 3
#define ENGINE_INPUT1 4
#define MODULE_KEY 5 // the key pin of the HC05 module (AT mode)
#define RX_PIN 10
#define TX_PIN 11
#define POWER 12
#define HEADLIGHTS 7
#define TAILLIGHTS 8
#define LDR A0
#define TMP A5
#define AT_INIT // AT configuration on startup

SoftwareSerial btSerial(RX_PIN, TX_PIN); // connexion to HC05

String inputString = "";        // a string to hold incoming data
boolean stringComplete = false; // whether the string is complete
char mem; // char to hold selected modes
bool LF; // controls car's movement
bool RF; //
bool LB; //
bool RB; //
bool isAuto; //
int power_state = LOW;
unsigned long tc, tp; // the time, for timeout processing
const long interval = 1000;
const long interval_2 = 1000;
const long interval_3 = 500;
unsigned long previousMillis = 0;
unsigned long previousMillis_2 = 0;
unsigned long previousMillis_3 = 0;

// functions header
void engine_control(const char *d);
void onOFF(int pin1, int pin2, bool lhs, bool rhs);
void lights(char &mode);
bool waitFor(const char *rep);
double tmp();

Servo steeringWheel;
int direction;

void setup()
{
  pinMode(HEADLIGHTS, OUTPUT);
  pinMode(TAILLIGHTS, OUTPUT);

  pinMode(ENGINE_INPUT1, OUTPUT);
  pinMode(ENGINE_INPUT2, OUTPUT);
  pinMode(SPEED_PIN, OUTPUT);
  pinMode(POWER, OUTPUT);

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
  unsigned long currentMillis_3 = millis();
  analogWrite(SPEED_PIN, 255); // Aciona o motor

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
  { 
    // a command is available to proceed
    stringComplete = false;
    
    mem = inputString[0];

    isAuto = (mem == 'A') ? true : false;

    LF = (mem == 'Q') ? true : false;
    RF = (mem == 'E') ? true : false;
    LB = (mem == 'Z') ? true : false;
    RB = (mem == 'C') ? true : false;
    
    lights(mem);

    if (!(LF || RF || LB || RB))
    {
      engine_control(&mem);
    }
    // void engine_control();
    inputString = ""; // clear the string
    // btSerial.print(?); // send a prompt char
  }

  if (LF || RF || LB || RB)
  {
    engine_control(&mem);
  }

  if (isAuto)
  {
    if (currentMillis_2 - previousMillis_2 >= interval_2)
    {
      previousMillis_2 = currentMillis_2;
      lights(mem);
    }
  }

//Allows power bank to provide energy enough to nano
  if (currentMillis_3 - previousMillis_3 >= interval_3)
    {
      previousMillis_3 = currentMillis_3;

      if(power_state == LOW)
        power_state = HIGH;
      else
        power_state = LOW;
      
      digitalWrite(POWER, power_state);
    }
}

//  The waitFor a string function with time out
// bool waitFor(const char *rep)
// {
//   unsigned long t0 = millis(), t;
//   int i = 0, j = strlen(rep);
//   char c;

//   for (i = 0; i < j;)
//   {
//     if (btSerial.available())
//     {
//       c = (char)btSerial.read();
//       Serial.print(c);
//       if (c == rep[i])
//         i++;
//     }
//     t = millis();
//     if (t - t0 > 2000)
//       break;
//   }
//   return i == j;
//   delay(10);
// }

// Functions definitions

// Aux function used by light and engine control functions
void onOFF(int pin1, int pin2, bool lhs, bool rhs)
{
  digitalWrite(pin1, lhs);
  digitalWrite(pin2, rhs);
  // delay(3000);
}

void engine_control(const char *d)
{
  int guidance = 104;
  // Serial.println(mode);
  switch (*d)
  {
  case 'F':// Forward
    onOFF(4, 2, 0, 1); 
    break;
  case 'B':// Backwards
    onOFF(4, 2, 1, 0); 
    break;
  case 'E':// Right Forward
    onOFF(4, 2, 0, 1);
    guidance = 90;
    break;
  case 'C':// Right Backwards
    onOFF(4, 2, 1, 0);
    guidance = 90;
    break;
  case 'Q':// Left Forward
    onOFF(4, 2, 0, 1);
    guidance = 120;
    break;
  case 'Z':// Left Backwards
    onOFF(4, 2, 1, 0);
    guidance = 120;
    break;
  
  default:// Neutral
    onOFF(4, 2, 0, 0); 
    break;
  }
  steeringWheel.write(guidance);
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
    if (reading < 700)
    {
      onOFF(7, 8, 0, 0);
    }
    else
    {
      onOFF(7, 8, 1, 1);
    }
    break;
  }
}
// Engine temperature
double tmp()
{
  int sensorInput;               // The variable we will use to store the sensor input
  double temperature;            // The variable we will use to store temperature in degrees.
  sensorInput = analogRead(TMP); // read the analog sensor and store it

  temperature = (double)sensorInput / 1024; // find percentage of input reading
  temperature = temperature * 5;            // multiply by 5V to get voltage
  temperature = temperature - 0.5;          // Subtract the offset
  temperature = temperature * 100;          // Convert to degrees

  return temperature;
}
