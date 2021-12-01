#include <Arduino.h>
#include <SoftwareSerial.h> 

#define rxPin 10 //PINO DIGITAL 10 (RX)
#define txPin 11 //PINO DIGITAL 11 (TX)

//PINOS PONTE H
int IN1 = 12;
int IN2 = 11;
int IN3 = 7;
int IN4 = 6;

//FUNCOES MOTOR
void Forward();
void Backwards();
void Stop();

SoftwareSerial bluetooth(rxPin, txPin); //PINOS QUE EMULAM A SERIAL

void setup()
{
    Serial.begin(9600);    //INICIALIZA A SERIAL
    bluetooth.begin(9600); //INICIALIZA A SERIAL DO BLUETOOTH

    delay(100); //INTERVALO DE 100 MILISSEGUNDOS

    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
}

void loop()
{
    char user_input;
    if (bluetooth.available())
    {
        user_input = bluetooth.read(); 

        if (user_input == 'F')
        {                              
            Serial.println("FORWARD"); 
            Forward();
        }
        if (user_input == 'B')
        {                                
            Serial.println("BACKWARDS"); 
            Backwards();
        }
        if (user_input == 'S')
        {                           
            Serial.println("STOP"); 
            Stop();
        }
    }
}

void Forward()
{
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void Backwards()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void Stop()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}
