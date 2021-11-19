#include <Servo.h>
#include <SoftwareSerial.h> 

#define rxPin 10 //PINO DIGITAL 10 (RX)
#define txPin 11 //PINO DIGITAL 11 (TX)

SoftwareSerial bluetooth(rxPin, txPin); //PINOS QUE EMULAM A SERIAL
Servo steering;

void setup()
{
  Serial.begin(9600);    //INICIALIZA A SERIAL
  bluetooth.begin(9600); //INICIALIZA A SERIAL DO BLUETOOTH

  delay(100); 
  
  steering.attach(6);  
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
}

void loop()
{
  
  char command;
  int value = 0;
  
  value = map(value, 0, 1023, 0, 200);
  
  
  if(bluetooth.available()>0)
  {
    command = bluetooth.read();
    Serial.println(command);
    
    if(command == 'R')
    {
      value = 200;
    }
    
    else if(command == 'L')
    {
      value = 0;
    }
    
    else 
    {
      value = 100;
    }
    
    steering.write(value);
    delay(15);

  }
}
