#include "XL320.h"

XL320 robot;

char rgb[] = "rgbypcwo";
int servoPos = 0;
int ledCol = 0;

int servoID = 0x03;

void setup() {

     Serial1.begin(115200);
     robot.begin(Serial1);
     robot.setJointSpeed(servoID, 1023);

}

void loop() {

     robot.LED(servoID, &rgb[random(0,7)]);
     delay(500);
     robot.moveJoint(servoID, random(0, 1023));
     delay(500);
     
}
