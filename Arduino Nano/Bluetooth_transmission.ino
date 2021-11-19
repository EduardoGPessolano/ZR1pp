#include <Arduino.h>
#include <SoftwareSerial.h> //INCLUSÃO DE BIBLIOTECA

const int pinoRX = 0; //PINO DIGITAL 1 (RX)
const int pinoTX = 1; //PINO DIGITAL 2 (TX)

//PINOS PONTE H
int IN1 = 12;
int IN2 = 11;
int IN3 = 7;
int IN4 = 6;

//FUNCOES MOTOR
void Forward();
void Backwards();
void Stop();

SoftwareSerial bluetooth(pinoRX, pinoTX); //PINOS QUE EMULAM A SERIAL, ONDE
//O PINO 1 É O RX E O PINO 2 É O TX

void setup()
{
    Serial.begin(9600);    //INICIALIZA A SERIAL
    bluetooth.begin(9600); //INICIALIZA A SERIAL DO BLUETOOTH

    //   bluetooth.print("$"); //IMPRIME O CARACTERE
    //   bluetooth.print("$"); //IMPRIME O CARACTERE
    //   bluetooth.print("$"); //IMPRIME O CARACTERE

    delay(100); //INTERVALO DE 100 MILISSEGUNDOS

    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    Serial.println("Enter number for control option:");
    Serial.println("1. FORWARD");
    Serial.println("2. BACKWARDS");
    Serial.println("3. STOP");
    Serial.println();
}

void loop()
{

    char user_input;
    if (bluetooth.available())
    {
    //SE O BLUETOOTH ESTIVER HABILITADO, FAZ
        user_input = bluetooth.read(); //VARIÁVEL RECEBE O VALOR ENVIADO PELO BLUETOOTH

        if (user_input == 'F')
        {                              //SE O VALOR RECEBIDO FOR IGUAL A 1, FAZ
            Serial.println("FORWARD"); //IMPRIME O TEXTO NA SERIAL
            digitalWrite(12, HIGH);
//            Forward();
        }
        if (user_input == '2')
        {                                //SE O VALOR RECEBIDO FOR IGUAL A 0, FAZ
            Serial.println("BACKWARDS"); //IMPRIME O TEXTO NA SERIAL
            Backwards();
        }
        if (user_input == '3')
        {                           //SE O VALOR RECEBIDO FOR IGUAL A b, FAZ
            Serial.println("STOP"); //IMPRIME O TEXTO NA SERIAL
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
