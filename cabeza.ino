#include <Servo.h>

String orden = "";
boolean stringComplete = false;
int estadoH;
int estadoV;

Servo motorH;
Servo motorV;

int delayH = 1;
int delayV = 1;

void setup() {
        /* Setup for the servo */
        motorH.attach(9);
        motorV.attach(8);
        motorH.write(100);
        motorV.write(85);
        Serial.begin(115200);
        estadoH = motorH.read();
        estadoV = motorV.read();
}

void loop()
{
        if(stringComplete){
                Serial.println("String complete");
   
                stringComplete = false;
                Serial.println("fin");
                unsigned char ord = orden.charAt(0);
                unsigned int value = orden.substring(2).toInt();
    
                switch(ord){
                case 'h':
                        Serial.println("h");
                        Serial.println(value);
                        Serial.println(estadoH);
                        if (estadoH < value){
                                Serial.println("if");
                                for (int h=estadoH; h<=value; h++){
                                        motorH.write(h);
                                        delay(delayH);
                                }
                        } else {
                                Serial.println("else");
                                for (int h=estadoH; h>=value; h--){
                                        motorH.write(h);
                                        delay(delayH);
                                }
                        }
                        break;
                case 'v':
                        Serial.println("v");
                        Serial.println(value);
                        Serial.println(estadoV);
                        if (value < 50)
                                value = 50;
                        else if (value > 100)
                                value = 100;
                        if (estadoV < value){
                                for (int v=estadoV; v<=value; v++){
                                        motorV.write(v);
                                        delay(delayV);
                                }
                        } else {
                                for (int v=estadoV; v>=value; v--){
                                        motorV.write(v);
                                        delay(delayV);
                                }
                        }
                        break;
                }
                orden="";
                ord=0;
                estadoH = motorH.read();
                estadoV = motorV.read();
        }
}

void serialEvent()
{
        while (Serial.available()) {
                // get the new byte:
                char inChar = (char)Serial.read(); 
                // if the incoming character is a newline, set a flag
                // so the main loop can do something about it:
                Serial.println("Serial event");
                if (inChar == '\n') {
                        stringComplete = true;
                        Serial.println("Serial String complete");
                }else{
                        // add it to the inputString:
                        orden += inChar;
                }
        }
}

// ------------------------------------------ 
// ---------------- NEW CODE ---------------- 
// ------------------------------------------ 

#define BYTE_SIZE 8 // Size of Coordinates
char message[BYTE_SIZE];

typedef struct
{
        int x;
        int y;
} Coordinates;

Coordinates coord;

void setup() {
        Serial.begin(9600);
}

void loop() {

        if (Serial.available() >= BYTE_SIZE) {
                Serial.write(1);

                Serial.readBytes(message , BYTE_SIZE);
                memcpy(&coord, &message, BYTE_SIZE);

                // Once finished moving the servos
                // flush the serial buffer and allow the raspberri to send more data
                Serial.flush();
                Serial.write(0);
        }
}
