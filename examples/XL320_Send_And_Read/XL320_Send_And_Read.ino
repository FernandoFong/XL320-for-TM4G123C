#include "XL320.h"
#define SEND 1
#define READ 0
#define DIR_PORT 23

XL320 robot;
char rgb[] = "rgbypcwo";

void setup() {

     Serial1.begin(115200);
     Serial.begin(9600); //To see the packet incoming.
     robot.begin(Serial1);
     pinMode(DIR_PORT, OUTPUT);

}

void loop() {

     digitalWrite(DIR_PORT, SEND);
     robot.moveJoint(0x03, random(0, 1023));
     byte buffer[256];
     XL320::Packet p = XL320::Packet(buffer, robot.readPacket(buffer, 256));
     if(p.isValid()) {
         robot.LED(0x03, &rgb[1]);
     } else {
         robot.LED(0x03, &rgb[0]);
     }
}