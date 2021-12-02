#include <SoftwareSerial.h>
#include <Servo.h>

#define ENGINE_INPUT2 2
#define SPEED_PIN 3
#define ENGINE_INPUT1 4
#define MODULE_KEY 5   // the key pin of the HC05 module (AT mode)
#define RX_PIN 10
#define TX_PIN 11
#define TAILLIGHTS 12
#define HEADLIGHTS 13
#define AT_INIT         // AT configuration on startup

const int LDR = A4;
const int TMP = A5;    //This is the Arduino Pin that will read the sensor output

SoftwareSerial btSerial(RX_PIN, TX_PIN); // connexion to HC05 

String inputString = "";        // a string to hold incoming data
boolean stringComplete = false; // whether the string is complete

unsigned long tc, tp;    // the time, for timeout processing

// function waiting for a specific string received
bool waitFor(const char *rep);
void onOFF(int pin1, int pin2, bool lhs, bool rhs);
double tmp();

Servo steeringWheel;

int direction;

// SETUP  FUNCTION
void setup()
{
  pinMode(TAILLIGHTS, OUTPUT);
  pinMode(HEADLIGHTS, OUTPUT);

  pinMode(ENGINE_INPUT1, OUTPUT);
  pinMode(ENGINE_INPUT2, OUTPUT);
  pinMode(SPEED_PIN, OUTPUT);

  steeringWheel.attach(6);

  // pinMode(ARDUINO_LED, OUTPUT);  // the on board led pin
  pinMode(MODULE_KEY, OUTPUT);   // the HC05 Key pin (switch to AT mode when HIGH)
  digitalWrite(MODULE_KEY, LOW); // communication mode

  inputString.reserve(200); // reserve 200 char max

  Serial.begin(9600);   // on board Serial
  btSerial.begin(9600); // SoftwareSerial connected to HC05 BTmodule
  tp = millis();        // get time

#ifdef AT_INIT // AT configuration of the HC05, to make once time

  // programmation du module
  digitalWrite(MODULE_KEY, HIGH); // switch to AT mode
  Serial.println("AT mode configuration:\r\n");

  Serial.print("AT: "); // verify we are in AT mode
  btSerial.print("AT\r\n");
  if (!waitFor("OK\r\n"))
    Serial.println("time out error AT");

  Serial.print("UART: "); // serial communication parameters
  btSerial.print("AT+UART=9600,0,0\r\n");
  if (!waitFor("OK\r\n"))
    Serial.println("time out error AT+UART");

  Serial.print("NAME: "); // bluetooth device name
  btSerial.print("AT+NAME=hc05-arduino-nano\r\n");
  if (!waitFor("OK\r\n"))
    Serial.println("time out error AT+NAME");

  Serial.print("PSWD: "); // password for pairing
  btSerial.print("AT+PSWD=1234\r\n");
  if (!waitFor("OK\r\n"))
    Serial.println("time out error AT+PSWD");

  Serial.print("ROLE: "); // device in slave mode
  btSerial.print("AT+ROLE=0\r\n");
  if (!waitFor("OK\r\n"))
    Serial.println("time out error AT+ROLE");

  Serial.print("RMAAD: "); // Delete all authenticated devices in the pair list
  btSerial.print("AT+RMAAD\r\n");
  if (!waitFor("OK\r\n"))
    Serial.println("time out error AT+RMAAD");

  Serial.print("CMODE: "); // connection mode
  btSerial.print("AT+CMODE=0\r\n");
  if (!waitFor("OK\r\n"))
    Serial.println("time out error AT+CMODE");

  digitalWrite(MODULE_KEY, LOW); // leave AT mode & switch back to communication mode
#endif

  Serial.println("Ready to communicate in RFCOMM mode\r\n");
}

//  THE LOOP FUNCTION
void loop()
{ 

  
  analogWrite(SPEED_PIN, 255); //Aciona o motor 

  // wait for a complete line from bluetooth device
  while (btSerial.available())
  {
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
    if (inputString == "F\n")
      digitalWrite(13, HIGH);
    else if (inputString == "S\n")
      digitalWrite(13, LOW);
    // and more commands you want...
    inputString = "";     // clear the string
    btSerial.print(tmp()); // send a prompt char
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

//light and engine control
  void onOFF(int pin1, int pin2, bool lhs, bool rhs){
    digitalWrite(pin1, lhs);
    digitalWrite(pin2, rhs);
    // delay(3000);
  }

  void engine_control(char &mode){ 
    switch (mode)
    {
    case 'F':
      onOFF(4, 2, 1, 0); //Forward
      break;
    case 'B':
      onOFF(4, 2, 0, 1); //Backwards
      break;
    default:
      onOFF(4, 2, 0, 0); //Neutral
      break;
    }
  }

  void lights(char &mode){
    int reading;
    reading = analogRead(LDR);
    switch (mode)
    {
    case 'Y':
      onOFF(12, 13, 0, 0);
      break;
    case 'N':
      onOFF(12, 13, 1, 1);
      break;
    case 'A':
      if(reading < 700){
        onOFF(12, 13, 0, 0);
      }
      else{
        onOFF(12, 13, 1, 1);
      }
      break;
    }    
  }

//Steering wheel
  void turn(char &direction){
    int d;
    direction = map(direction, 0, 1023, 0, 200);
    switch (direction)
    {
    case 'R':
      d = 200;
      break;
    case 'L':
      d = 0;
    default:
      d = 100;
      break;
    }
    steeringWheel.write(d);
  }

//Engine temperature
  double tmp(){
    int sensorInput;  //The variable we will use to store the sensor input
    double temperature;     //The variable we will use to store temperature in degrees.
    sensorInput = analogRead(TMP); //read the analog sensor and store it

    temperature = (double)sensorInput / 1024; //find percentage of input reading
    temperature = temperature * 5;                   //multiply by 5V to get voltage
    temperature = temperature - 0.5;                 //Subtract the offset
    temperature = temperature * 100;                 //Convert to degrees

    return temperature;
  }
