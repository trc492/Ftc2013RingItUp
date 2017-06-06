#if 0
/// Copyright (c) Titan Robotics Club. All rights reserved.
///
/// <module name="CommonCode.h" />
///
/// <summary>
///     This module contains code common to both Autonomous and TeleOp modes.
/// </summary>
///
/// <remarks>
///     Environment: RobotC for Lego Mindstorms NXT.
/// </remarks>
#endif

//
// Program switches.
//

// Common switches.
#define _COMPETITION_ENABLED
//#define _USE_HEAVYRING_DETECTION

// Autonomous switches.
#ifndef _COMPETITION_ENABLED
  #define _ENABLE_TESTMODES
#endif
//#define _DEBUG_LNFOLLOW
//#define _CAL_IRSEEKER
//#define _DEBUG_IRSEEKING

// TeleOp switches.
//#define _ARCADE_DRIVE
//#define _DEBUG_ELEVATOR
//#define _DEBUG_RAMP

//
// Include libraries.
//

// Include RobotC drivers.
#include "..\RobotCDrivers\drivers\hitechnic-sensormux.h"
#include "..\RobotCDrivers\drivers\lego-light.h"
#include "..\RobotCDrivers\drivers\lego-ultrasound.h"
#ifdef _USE_HEAVYRING_DETECTION
#include "..\RobotCDrivers\drivers\hitechnic-superpro.h"
#endif

// Include FTC library.
#include "..\ftclib\batt.h"
#include "..\ftclib\sm.h"
#include "..\ftclib\kalman.h"
#include "..\ftclib\gyro.h"             //Drive subsystem
#include "..\ftclib\irseeker.h"         //Drive subsystem
#include "..\ftclib\touch.h"            //Elevator subsystem
#ifdef _USE_HEAVYRING_DETECTION
#include "..\ftclib\sensor.h"
#endif
#include "..\ftclib\drive.h"            //Drive subsystem
#include "..\ftclib\pidctrl.h"          //Drive and Elevator subsystems
#include "..\ftclib\piddrive.h"         //Drive subsystem
#include "..\ftclib\pidmotor.h"         //Elevator subsystem
#include "..\ftclib\servo.h"            //Wrist and Gripper subsystems

// Include local files.
#include "RobotInfo.h"

//
// Constants.
//

//
// Global data.
//

// Drive susbsystem.
DRIVE       g_Drive;

GYRO        g_Gyro;
IRSEEKER    g_LeftIR;
IRSEEKER    g_RightIR;
KALMAN      g_USFilter;

PIDCTRL     g_EncoderPidCtrl;
PIDCTRL     g_GyroPidCtrl;
PIDCTRL     g_SonarPidCtrl;
PIDCTRL     g_IrDistPidCtrl;
PIDCTRL     g_IrTurnPidCtrl;
PIDCTRL     g_LnFollowPidCtrl;

PIDDRIVE    g_PidDrive;
PIDDRIVE    g_IrDrive;
PIDDRIVE    g_LnFollowDrive;

// Elevator subsystem.
TOUCH       g_ElevatorZeroSW;
PIDCTRL     g_ElevatorPidCtrl;
PIDMOTOR    g_ElevatorMotor;

// Ramp subsystem.
PIDCTRL     g_RampPidCtrl;
PIDMOTOR    g_RampMotor;
SERVO       g_RampLatchServo;

// Wrist subsystem.
SERVO       g_WristServo;

// Gripper subsystem.
SERVO       g_GripperServo;

#ifdef _USE_HEAVYRING_DETECTION
// Weighted Ring Detection subsystem.
SENSOR      g_LightSensor1;
SENSOR      g_LightSensor2;
#endif

// Miscellaneous.
BATT        g_Batt;
int         g_LSMapping[8] =
            {
            /*000*/ 3,      //no line found, go straight
            /*001*/ 1,      //far right
            /*010*/ 3,      //right on
            /*011*/ 2,      //slight right
            /*100*/ 5,      //far left
            /*101*/ 2,      //have a 'Y', treated as slight right
            /*110*/ 4,      //slight left
            /*111*/ 1       //have a '-', treated as far right
            };

//
// Callback functions.
//

/**
 *  This function reads the light sensors and returns the combined state.
 *
 *  @return Returns the combined sensor states.
 */
int
GetLightSensorStates()
{
    int state = ((LSvalRaw(leftLightSensor) > LS_LEFT_THRESHOLD)? 4: 0) |
                ((LSvalRaw(centerLightSensor) > LS_CENTER_THRESHOLD)? 2: 0) |
                ((LSvalRaw(rightLightSensor) > LS_RIGHT_THRESHOLD)? 1: 0);
    return state;
}   //GetLightSensorStates

/**
 *  This function provides the input values for various PID controllers.
 *
 *  @param pidCtrl Specifies the PID controller to get its input.
 *
 *  @return Returns the input value for the PID controller.
 */
float
PIDCtrlGetInput(
    PIDCTRL &pidCtrl
    )
{
    float inputValue = 0.0;

    if (PIDCtrlCheck(pidCtrl, g_EncoderPidCtrl))
    {
        // Return the average of the left and right wheel encoders in inches.
        inputValue = (nMotorEncoder[leftRear] + nMotorEncoder[rightRear])*
                     DISTANCE_PER_CLICK/2.0;
    }
    else if (PIDCtrlCheck(pidCtrl, g_GyroPidCtrl))
    {
        // Return the gyro reading in degrees.
        inputValue = GyroGetHeading(g_Gyro);
    }
    else if (PIDCtrlCheck(pidCtrl, g_SonarPidCtrl))
    {
#ifdef _CAL_IRSEEKER
        inputValue = PIDCtrlGetTarget(g_SonarPidCtrl);
#else
//        inputValue = (float)KalmanFilter(g_USFilter, USreadDist(sonarSensor));
        inputValue = (float)USreadDist(sonarSensor);
#endif
    }
    else if (PIDCtrlCheck(pidCtrl, g_IrDistPidCtrl))
    {
        // Return the average signal strength of the left and right IR.
        inputValue = (IRSeekerGetACStrength(g_LeftIR) +
                      IRSeekerGetACStrength(g_RightIR))/2;
    }
    else if (PIDCtrlCheck(pidCtrl, g_IrTurnPidCtrl))
    {
        // Return the sum of the left and right IR.
        inputValue = IRSeekerGetACDir(g_LeftIR) + IRSeekerGetACDir(g_RightIR);
    }
    else if (PIDCtrlCheck(pidCtrl, g_LnFollowPidCtrl))
    {
        // Return the position of the line relative to the light sensors.
        inputValue = (float)g_LSMapping[GetLightSensorStates()];
    }
    else if (PIDCtrlCheck(pidCtrl, g_ElevatorPidCtrl))
    {
        // Return the height of the elevator in inches.
        inputValue = nMotorEncoder[elevatorMotor]*HEIGHT_PER_CLICK +
                     ELEVATOR_LOWER_LIMIT;
    }
    else if (PIDCtrlCheck(pidCtrl, g_RampPidCtrl))
    {
        // Return encoder value.
        inputValue = nMotorEncoder[rampMotor];
    }

    return inputValue;
}   //PIDCtrlGetInput

/**
 *  This callback function is called when the touch sensor has changed state.
 *
 *  @param touch Points to the TOUCH structure.
 *  @param fActive Specifies the touch sensor is triggered.
 */
void
TouchEvent(
    TOUCH &touch,
    bool fActive
    )
{
    if (fActive)
    {
        //
        // We reached the bottom, stop the motor.
        //
        PIDMotorReset(g_ElevatorMotor);
        PlayImmediateTone(440, 15);
    }
    else
    {
        PlayImmediateTone(880, 15);
    }
    //
    // Reset the encoder when engaging or disengaging the touch sensor.
    // This will compensate the small amount of free play on the chain
    // where the motor is winding or unwinding the chain while the elevator
    // is not yet moving.
    //
    nMotorEncoder[elevatorMotor] = 0;
}   //TouchEvent

#ifdef _USE_HEAVYRING_DETECTION
/**
 *  This function updates the weighted ring LED state.
 *
 *  @param sensor1 Specifying the first light sensor.
 *  @param sensor2 Specifying the first light sensor.
 */
void
UpdateLED(
    SENSOR &sensor1,
    SENSOR &sensor2
    )
{
    int zone1 = SensorGetZone(sensor1);
    int zone2 = SensorGetZone(sensor2);

    HTSPBwriteIO(superPro,
                 ((zone1 == SENSORZONE_LOW) ||
                  (zone2 == SENSORZONE_LOW))?
                    SPB_HEAVY_RING_LED: 0);
}   //UpdateLED

/**
 *  This function is called when the sensor value crosses to a
 *  different zone.
 *
 *  @param sensor Points to the SENSOR structure that generated the event.
 *  @param zone Specifying the current light sensor zone.
 */
void
SensorEvent(
    SENSOR &sensor,
    int zone
    )
{
    if ((&sensor == &g_LightSensor1) || (&sensor == &g_LightSensor2))
    {
        UpdateLED(g_LightSensor1, g_LightSensor2);
    }
}   //SensorEvent
#endif

//
// Main functions.
//

/**
 *  This function is called once globally to initialize the robot. Typically,
 *  this function initializes all the sensors and robot subsystems as well as
 *  global data that are common to both Autonomous and TeleOp modes.
 */
void
CommonInit()
{
    //
    // Initialize Drive subsystem.
    //
    DriveInit(g_Drive, leftFront, rightFront, leftRear, rightRear);

    KalmanInit(g_USFilter);
    GyroInit(g_Gyro, gyroSensor, GYROO_INVERSE | GYROO_FILTER);
    IRSeekerInit(g_LeftIR, leftIR, IRSEEKERO_FILTER);
    IRSeekerInit(g_RightIR, rightIR, IRSEEKERO_FILTER);
    nMotorEncoder[leftRear] = 0;
    nMotorEncoder[rightRear] = 0;

    PIDCtrlInit(g_EncoderPidCtrl,
                ENCODER_KP, ENCODER_KI, ENCODER_KD,
                ENCODER_TOLERANCE, ENCODER_SETTLING);
    PIDCtrlInit(g_GyroPidCtrl,
                GYRO_KP, GYRO_KI, GYRO_KD,
                GYRO_TOLERANCE, GYRO_SETTLING,
                PIDCTRLO_NO_OSCILLATE);
    PIDCtrlInit(g_SonarPidCtrl,
                SONAR_KP, SONAR_KI, SONAR_KD,
                SONAR_TOLERANCE, SONAR_SETTLING,
                (PIDCTRLO_ABS_SETPT |
                 PIDCTRLO_INVERSE |
                 PIDCTRLO_NO_OSCILLATE));
    PIDCtrlInit(g_IrDistPidCtrl,
                IRDIST_KP, IRDIST_KI, IRDIST_KD,
                IRDIST_TOLERANCE, IRDIST_SETTLING,
                (PIDCTRLO_ABS_SETPT |
                 PIDCTRLO_NO_OSCILLATE));
    PIDCtrlInit(g_IrTurnPidCtrl,
                IRTURN_KP, IRTURN_KI, IRTURN_KD,
                IRTURN_TOLERANCE, IRTURN_SETTLING,
                PIDCTRLO_ABS_SETPT);
    PIDCtrlInit(g_LnFollowPidCtrl,
                LNFOLLOW_KP, LNFOLLOW_KI, LNFOLLOW_KD,
                LNFOLLOW_TOLERANCE, LNFOLLOW_SETTLING,
                PIDCTRLO_ABS_SETPT);

    PIDDriveInit(g_PidDrive,
                 g_Drive,
                 g_EncoderPidCtrl,
                 g_GyroPidCtrl);
    PIDDriveInit(g_IrDrive,
                 g_Drive,
                 g_SonarPidCtrl,
                 g_IrTurnPidCtrl);
    PIDDriveInit(g_LnFollowDrive,
                 g_Drive,
                 g_SonarPidCtrl,
                 g_LnFollowPidCtrl);

    //
    // Initialize Elevator subsystem.
    //
    TouchInit(g_ElevatorZeroSW, touchSensor);
    PIDCtrlInit(g_ElevatorPidCtrl,
                ELEVATOR_KP, ELEVATOR_KI, ELEVATOR_KD,
                ELEVATOR_TOLERANCE, ELEVATOR_SETTLING,
                PIDCTRLO_ABS_SETPT);
    PIDCtrlSetSetpointLimits(g_ElevatorPidCtrl,
                             ELEVATOR_LOWER_LIMIT,
                             ELEVATOR_UPPER_LIMIT);
    PIDMotorInit(g_ElevatorMotor, elevatorMotor, g_ElevatorPidCtrl);

    //
    // Initialize Ramp subsystem.
    //
    PIDCtrlInit(g_RampPidCtrl,
                RAMP_KP, RAMP_KI, RAMP_KD,
                RAMP_TOLERANCE, RAMP_SETTLING,
                PIDCTRLO_ABS_SETPT);
    PIDCtrlSetSetpointLimits(g_RampPidCtrl,
                             RAMP_LOWER_LIMIT,
                             RAMP_UPPER_LIMIT);
    PIDCtrlSetPowerLimits(g_RampPidCtrl, -RAMP_POWER_LIMIT, RAMP_POWER_LIMIT);
    PIDMotorInit(g_RampMotor, rampMotor, g_RampPidCtrl);
    ServoInit(g_RampLatchServo, rampLatch, RAMP_LATCH_ANGLE);
    nMotorEncoder[rampMotor] = 0;

    //
    // Initialize Wrist subsystem.
    //
    ServoInit(g_WristServo,
              wrist1, wrist2,
              WRIST_INIT_ANGLE,
              SERVOO_INVERSE_SERVO2);

    //
    // Initialize Gripper subsystem.
    //
    ServoInit(g_GripperServo,
              gripper1, gripper2,
              GRIPPER_INIT_ANGLE,
              SERVOO_INVERSE_SERVO2);

#ifdef _USE_HEAVYRING_DETECTION
    //
    // Initialize weighted ring detection subsystem.
    //
    LSsetInactive(lightSensor1);
    LSsetInactive(lightSensor2);
    SensorInit(g_LightSensor1,
               lightSensor1,
               LS1_LOW_THRESHOLD,
               LS1_HIGH_THRESHOLD);
    SensorInit(g_LightSensor2,
               lightSensor2,
               LS2_LOW_THRESHOLD,
               LS2_HIGH_THRESHOLD);
    HTSPBsetupIO(superPro, SPB_HEAVY_RING_LED);
    UpdateLED(g_LightSensor1, g_LightSensor2);
#endif

    //
    // Initialize miscellaneous.
    //
    BattInit(g_Batt, 5, true);

}   //CommonInit

//
// Other periodic tasks.
//

/**
 *  This function is called periodically to perform tasks common to both
 *  Autonomous and TeleOp modes that require high resolution (e.g. gyro
 *  integration).
 */
void CommonHiFreqTasks()
{
    GyroTask(g_Gyro);
}   //CommonHiFreqTasks

/**
 *  This function is called periodically to perform tasks related to sensors
 *  and inputs that are common to both Autonomous and TeleOp modes.
 */
void CommonInputTasks()
{
    TouchTask(g_ElevatorZeroSW);
    IRSeekerTask(g_LeftIR);
    IRSeekerTask(g_RightIR);
#ifdef _USE_HEAVYRING_DETECTION
    SensorTask(g_LightSensor1);
    SensorTask(g_LightSensor2);
#endif
    BattTask(g_Batt);
}   //CommonInputTasks

/**
 *  This function is called periodically to perform tasks related to motors
 *  and other outputs that are common to both Autonomous and TeleOp modes.
 */
void CommonOutputTasks()
{
    ServoTask(g_GripperServo);
    ServoTask(g_WristServo);
    ServoTask(g_RampLatchServo);
    PIDMotorTask(g_RampMotor);
    PIDMotorTask(g_ElevatorMotor);
    PIDDriveTask(g_LnFollowDrive);
    PIDDriveTask(g_IrDrive);
    PIDDriveTask(g_PidDrive);
    DriveTask(g_Drive);
}   //CommonOutputTasks
