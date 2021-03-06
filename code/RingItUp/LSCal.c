#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S2,     gyroSensor,     sensorI2CCustom)
#pragma config(Sensor, S3,     loHTSMUX,       sensorI2CCustom)
#pragma config(Sensor, S4,     hiHTSMUX,       sensorI2CCustom)
#pragma config(Motor,  mtr_S1_C1_1,     rightRear,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     rightFront,    tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     leftRear,      tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     leftFront,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     elevatorMotor, tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     liftMotor,     tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S1_C4_1,    wrist1,               tServoStandard)
#pragma config(Servo,  srvo_S1_C4_2,    wrist2,               tServoStandard)
#pragma config(Servo,  srvo_S1_C4_3,    gripper1,             tServoStandard)
#pragma config(Servo,  srvo_S1_C4_4,    gripper2,             tServoStandard)
#pragma config(Servo,  srvo_S1_C4_5,    rampLatch,            tServoStandard)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define rightLightSensor        msensor_S3_1
#define centerLightSensor       msensor_S3_2
#define leftLightSensor         msensor_S3_3
#define sonarSensor             msensor_S3_4

#define touchSensor             msensor_S4_1
#define leftIR                  msensor_S4_3
#define rightIR                 msensor_S4_4

#include "JoystickDriver.c"
#include "..\RobotCDrivers\drivers\hitechnic-sensormux.h"
#include "..\RobotCDrivers\drivers\lego-light.h"
#include "..\ftclib\trcdefs.h"

task main()
{
    int leftMin = 1024;
    int leftMax = 0;
    int centerMin = 1024;
    int centerMax = 0;
    int rightMin = 1024;
    int rightMax = 0;

    StopTask(displayDiagnostics);
    eraseDisplay();
    LSsetActive(leftLightSensor);
    LSsetActive(centerLightSensor);
    LSsetActive(rightLightSensor);
    while (true)
    {
        getJoystickSettings(joystick);

        int leftValue = LSvalRaw(leftLightSensor);
        int centerValue = LSvalRaw(centerLightSensor);
        int rightValue = LSvalRaw(rightLightSensor);

        if (leftValue < leftMin)
        {
            leftMin = leftValue;
        }
        if (leftValue > leftMax)
        {
            leftMax = leftValue;
        }

        if (centerValue < centerMin)
        {
            centerMin = centerValue;
        }
        if (centerValue > centerMax)
        {
            centerMax = centerValue;
        }

        if (rightValue < rightMin)
        {
            rightMin = rightValue;
        }
        if (rightValue > rightMax)
        {
            rightMax = rightValue;
        }

        nxtDisplayTextLine(0, "L=%d,C=%d", leftValue, centerValue);
        nxtDisplayTextLine(1, "R=%d", rightValue);

        nxtDisplayTextLine(2, "%3d<=L<=%3d", leftMin, leftMax);
        nxtDisplayTextLine(3, "%3d<=C<=%3d", centerMin, centerMax);
        nxtDisplayTextLine(4, "%3d<=R<=%3d", rightMin, rightMax);

        nxtDisplayTextLine(5, "LTh=%3d", (leftMin + leftMax)/2);
        nxtDisplayTextLine(6, "CTh=%3d", (centerMin + centerMax)/2);
        nxtDisplayTextLine(7, "RTh=%3d", (rightMin + rightMax)/2);

        int leftPower = JOYSTICK_POWER(joystick.joy1_y1);
        int rightPower = JOYSTICK_POWER(joystick.joy1_y2);
        motor[leftFront] = leftPower;
        motor[leftRear] = leftPower;
        motor[rightFront] = rightPower;
        motor[rightRear] = rightPower;

        wait1Msec(50);
    }
}
