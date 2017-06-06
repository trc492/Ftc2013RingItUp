#if 0
/// Copyright (c) Titan Robotics Club. All rights reserved.
///
/// <module name="AutoMain.h" />
///
/// <summary>
///     This module contains the main autonomous tasks code.
/// </summary>
///
/// <remarks>
///     Environment: RobotC for Lego Mindstorms NXT.
/// </remarks>
#endif

#include "..\RobotCDrivers\drivers\lego-ultrasound.h"
#include "..\ftclib\batt.h"
#include "..\ftclib\sm.h"
#include "..\ftclib\gyro.h"
#ifdef _USE_ACCEL
    #include "..\ftclib\accel.h"
#endif
#include "..\ftclib\irseeker.h"
#include "..\ftclib\touch.h"
#include "..\ftclib\timer.h"
#include "..\ftclib\servo.h"
#include "..\ftclib\drive.h"
#include "..\ftclib\pidctrl.h"
#include "..\ftclib\piddrive.h"
#include "..\ftclib\pidmotor.h"

#pragma systemFile

#ifdef MOD_ID
    #undef MOD_ID
#endif
#define MOD_ID                          MOD_MAIN

#define TEST_COMPETITION                0
#define TEST_DRIVE_8FT                  1
#define TEST_TURN_360                   2
#define TEST_SONARIR_DRIVE              3
#define TEST_SENSOR_DIAG                4
#define TEST_0x8_RECT                   5

#define ALLIANCE_RED                    0
#define ALLIANCE_BLUE                   1

#define STRATEGY_PARK_BACK              0
#define STRATEGY_PARK_FRONT             1
#define STRATEGY_CRATE_FRONT            2
#define STRATEGY_BOTH_CRATES_FRONT1     3
#define STRATEGY_BOTH_CRATES_FRONT2     4

//
// Game info.
//

//
// State machine event types.
//
#define EVTTYPE_PIDDRIVE                (EVTTYPE_NONE + 1)
#define EVTTYPE_PIDMOTOR                (EVTTYPE_NONE + 2)
#define EVTTYPE_TIMER                   (EVTTYPE_NONE + 3)


//
// Global data.
//
BATT        g_Batt;

MENU        g_TestMenu;
MENU        g_AllianceMenu;
MENU        g_AutoDelayMenu;
MENU        g_StrategyMenu;

int         g_Test = TEST_COMPETITION;
int         g_Alliance = ALLIANCE_RED;
int         g_AutoDelay = 0;
int         g_Strategy = STRATEGY_CRATE_FRONT;

//
// Input and sensors.
//
GYRO        g_Gyro;
#ifdef _USE_ACCEL
ACCEL       g_Accel;
#endif
IRSEEKER    g_LeftIRSeeker;
IRSEEKER    g_RightIRSeeker;
TOUCH       g_BotLimitSW;
TOUCH       g_TopLimitSW;
TIMER       g_Timer;

//
// Drive subsystems.
//
DRIVE       g_Drive;
PIDCTRL     g_PidCtrlEncoderDrive;
PIDCTRL     g_PidCtrlEncoderTurn;
PIDCTRL     g_PidCtrlGyro;
PIDCTRL     g_PidCtrlSonar;
PIDCTRL     g_PidCtrlIRSeeker;
PIDCTRL     g_PidCtrlElevator;
PIDDRIVE    g_EncoderDrive;
PIDDRIVE    g_SonarIRDrive;


//
// Actuators.
//
PIDMOTOR    g_Elevator;
SERVO       g_LeftArm;
SERVO       g_RightArm;
SERVO       g_LeftWrist;
SERVO       g_RightWrist;
SERVO       g_Flap;

//
// State machines.
//
SM          g_AutoSM;

//
// Strategies
//
#include "ParkBack1.h"
#include "ParkFront1.h"
#include "CrateFront.h"

/**
 *  This function handles the touch sensor events.
 *
 *  @param touch Points to the TOUCH structure that generated the event.
 *  @param fActive If true, the touch sensor is active, false otherwise.
 */
void
TouchEvent(
    __in TOUCH &touch,
    __in bool fActive
    )
{
    TFuncName("TouchEvent");
    TLevel(EVENT);
    TEnter();

    if (fActive)
    {
        if (touch.sensorID == (int)botLimitSW)
        {
            PIDMotorReset(g_Elevator);
            nMotorEncoder[elevatorMotor] = 0;
            //
            // Play a descending set of notes.
            //
            PlayTone(800, 10);
            PlayTone(700, 10);
            PlayTone(600, 10);
        }
        else if (touch.sensorID == (int)topLimitSW)
        {
            PIDMotorReset(g_Elevator);
            //
            // Play a ascending set of notes.
            //
            PlayTone(800, 10);
            PlayTone(900, 10);
            PlayTone(1000, 10);
        }
    }
    else
    {
        PlayTone(440, 10);
    }

    TExit();
    return;
}   //TouchEvent

/**
 *  This function provides the input value for various sensors.
 *
 *  @param pidCtrl Specifies the PID controller to get its input.
 *
 *  @return Returns the input value for the PID controller.
 */
float
PIDCtrlGetInput(
    __in PIDCTRL &pidCtrl
    )
{
    float inputValue = 0.0;

    TFuncName("PIDCtrlGetInput");
    TLevel(CALLBK);
    TEnterMsg(("ID=%d", inputID));

    if (PIDCtrlCheck(pidCtrl, g_PidCtrlEncoderDrive))
    {
        //
        // Encoder drive.
        //
//        inputValue = (float)(nMotorEncoder[leftMotor] +
//                             nMotorEncoder[rightMotor])*
//                     DISTANCE_PER_CLICK/2.0;
        inputValue = (float)nMotorEncoder[leftMotor] * DISTANCE_PER_CLICK;
    }
    else if (PIDCtrlCheck(pidCtrl, g_PidCtrlEncoderTurn))
    {
        //
        // Encoder turn.
        //
        inputValue = (float)(nMotorEncoder[leftMotor] -
                             nMotorEncoder[rightMotor])*
                     DEGREES_PER_CLICK;
    }
    else if (PIDCtrlCheck(pidCtrl, g_PidCtrlGyro))
    {
        //
        // Gyro turn.
        //
        inputValue = GyroGetHeading(g_Gyro);
    }
    else if (PIDCtrlCheck(pidCtrl, g_PidCtrlSonar))
    {
        //
        // Sonar drive.
        //
        inputValue = (float)USreadDist(sonarSensor)*INCHES_PER_CM;
    }
    else if (PIDCtrlCheck(pidCtrl, g_PidCtrlIRSeeker))
    {
        inputValue = IRSeekerGetACDir(g_LeftIRSeeker) +
                     IRSeekerGetACDir(g_RightIRSeeker) - 10.0;
    }
    else if (PIDCtrlCheck(pidCtrl, g_PidCtrlElevator))
    {
        inputValue = (float)nMotorEncoder[elevatorMotor]*
                     ELEVATOR_HEIGHT_PER_CLICK;
    }

    TExitMsg(("=%5.1f", inputValue));
    return inputValue;
}   //PIDCtrlGetInput

/**
 *  This function initializes the robot and its subsystems.
 */
void
RobotInit()
{
    TFuncName("RobotInit");
    TLevel(INIT);
    TEnter();

    BattInit(g_Batt, 5, true);
    //
    // Init Test choice menu.
    //
    MenuInit(g_TestMenu, "Tests:");
    MenuAddChoice(g_TestMenu, "Competition", (float)TEST_COMPETITION);
    MenuAddChoice(g_TestMenu, "Drive 8ft", (float)TEST_DRIVE_8FT);
    MenuAddChoice(g_TestMenu, "Turn 360", (float)TEST_TURN_360);
    MenuAddChoice(g_TestMenu, "SonarIR Drive", (float)TEST_SONARIR_DRIVE);
    MenuAddChoice(g_TestMenu, "Sensor Diag", (float)TEST_SENSOR_DIAG);
    MenuAddChoice(g_TestMenu, "0x8 Rectangle", (float)TEST_0x8_RECT);
    //
    // Init Alliance choice menu.
    //
    MenuInit(g_AllianceMenu, "Alliances:");
    MenuAddChoice(g_AllianceMenu, "Red", (float)ALLIANCE_RED);
    MenuAddChoice(g_AllianceMenu, "Blue", (float)ALLIANCE_BLUE);
    //
    // Init the AutoDelay menu: the time the robot waits before starting
    // the autonomous routine.
    //
    MenuInit(g_AutoDelayMenu, "Autonomous Delay:");
    MenuAddChoice(g_AutoDelayMenu, "None", 0.0);
    MenuAddChoice(g_AutoDelayMenu, "1 sec", 1.0);
    MenuAddChoice(g_AutoDelayMenu, "2 sec", 2.0);
    MenuAddChoice(g_AutoDelayMenu, "4 sec", 4.0);
    MenuAddChoice(g_AutoDelayMenu, "6 sec", 6.0);
    MenuAddChoice(g_AutoDelayMenu, "8 sec", 8.0);
    MenuAddChoice(g_AutoDelayMenu, "10 sec", 10.0);
    //
    // Init Strategy choice menu.
    // (decision collected at end of function)
    //
    MenuInit(g_StrategyMenu, "Strategies:");
    MenuAddChoice(g_StrategyMenu, "ParkBack", (float)STRATEGY_PARK_BACK);
    MenuAddChoice(g_StrategyMenu, "ParkFront", (float)STRATEGY_PARK_FRONT);
    MenuAddChoice(g_StrategyMenu, "CrateFront", (float)STRATEGY_CRATE_FRONT);
    MenuAddChoice(g_StrategyMenu, "BothCratesFront1",
                  (float)STRATEGY_BOTH_CRATES_FRONT1);
    MenuAddChoice(g_StrategyMenu, "BothCratesFront2",
                  (float)STRATEGY_BOTH_CRATES_FRONT2);
    //
    // Initialize the input subsystems.
    //
#ifdef _USE_MUX
    GyroInit(g_Gyro, gyroSensor);
#else
    GyroInit(g_Gyro, gyroSensor);
#endif
#ifdef _USE_ACCEL
    AccelInit(g_Accel, accelSensor);
#endif
    IRSeekerInit(g_LeftIRSeeker, leftIRSensor);
    IRSeekerInit(g_RightIRSeeker, rightIRSensor);
    TouchInit(g_BotLimitSW, botLimitSW);
    TouchInit(g_TopLimitSW, topLimitSW);
    TimerInit(g_Timer);

    //
    // Intialize the Drive subsystem of the robot running base.
    //
    DriveInit(g_Drive, leftMotor, rightMotor);

    //
    // Encoder Drive.
    //
    PIDCtrlInit(g_PidCtrlEncoderDrive,
                ENCODER_DRIVE_KP, ENCODER_DRIVE_KI, ENCODER_DRIVE_KD,
                ENCODER_DRIVE_TOLERANCE, ENCODER_DRIVE_SETTLING,
                PIDCTRLO_NO_OSCILLATE);
    PIDCtrlInit(g_PidCtrlEncoderTurn,
                ENCODER_TURN_KP, ENCODER_TURN_KI, ENCODER_TURN_KD,
                ENCODER_TURN_TOLERANCE, ENCODER_TURN_SETTLING);
    PIDCtrlInit(g_PidCtrlGyro,
                GYRO_KP, GYRO_KI, GYRO_KD,
                GYRO_TOLERANCE, GYRO_SETTLING,
                PIDCTRLO_NO_OSCILLATE);
    PIDDriveInit(g_EncoderDrive,
                 g_Drive,
                 g_PidCtrlEncoderDrive,
                 g_PidCtrlGyro);
    //
    // SonarIR Drive.
    //
    PIDCtrlInit(g_PidCtrlSonar,
                SONAR_KP, SONAR_KI, SONAR_KD,
                SONAR_TOLERANCE, SONAR_SETTLING,
                PIDCTRLO_ABS_SETPT | PIDCTRLO_INVERSE);
    PIDCtrlInit(g_PidCtrlIRSeeker,
                IRSEEK_KP, IRSEEK_KI, IRSEEK_KD,
                IRSEEK_TOLERANCE, IRSEEK_SETTLING,
                PIDCTRLO_ABS_SETPT | PIDCTRLO_INVERSE);
    PIDDriveInit(g_SonarIRDrive,
                 g_Drive,
                 g_PidCtrlEncoderDrive,
                 g_PidCtrlIRSeeker);
    //
    // Initialize subsystems.
    //
    PIDCtrlInit(g_PidCtrlElevator,
                ELEVATOR_KP, ELEVATOR_KI, ELEVATOR_KD,
                ELEVATOR_TOLERANCE, ELEVATOR_SETTLING,
                PIDCTRLO_ABS_SETPT);
    PIDMotorInit(g_Elevator,
                 elevatorMotor,
                 g_PidCtrlElevator);

    ServoInit(g_LeftArm, leftArm, ARM_LEFT_GRAB_BALL);
    ServoInit(g_RightArm, rightArm, ARM_RIGHT_GRAB_BALL);
    ServoInit(g_LeftWrist, leftWrist, LEFT_WRIST_DOWN);
    ServoInit(g_RightWrist, rightWrist, RIGHT_WRIST_DOWN);

    //
    // Calibrate elevator.
    //
    if (!TouchGetState(g_TopLimitSW))
    {
        PIDMotorSetPower(g_Elevator, ELEVATOR_CAL_POWER);
        while(TouchGetState(g_TopLimitSW) == false) {}
        PIDMotorReset(g_Elevator);
        PlayTone(440, 15);
    }

    ServoInit(g_Flap, flap, FLAP_CLOSED);
    ServoSetAngle(g_Flap, FLAP_CLOSED);

    //
    // Make the Elevator to go down.
    //
    PIDMotorSetPower(g_Elevator, -ELEVATOR_CAL_POWER);
    while(TouchGetState(g_BotLimitSW) == false) {}
    PIDMotorReset(g_Elevator);
    nMotorEncoder[elevatorMotor] = 0;
    PlayTone(440, 15);

    //
    // Show the autonomous choices and collect user input
    // (choices are defined near top of function)
    //
#ifdef _TESTMENU_ENABLED
    g_Test = (int)MenuGetChoice(g_TestMenu);
#endif
    if (g_Test == TEST_COMPETITION)
    {
        g_Alliance = (int)MenuGetChoice(g_AllianceMenu);
        g_AutoDelay = (int)MenuGetChoice(g_AutoDelayMenu);
        g_Strategy = (int)MenuGetChoice(g_StrategyMenu);
        nxtDisplayCenteredTextLine(3, "%s [%d]",
                                   g_Alliance == ALLIANCE_RED? "Red": "Blue",
                                   (int)MenuGetChoiceValue(g_AutoDelayMenu));
        nxtDisplayCenteredTextLine(4, MenuGetChoiceText(g_StrategyMenu));
    }
    else
    {
        nxtDisplayCenteredTextLine(4, MenuGetChoiceText(g_TestMenu));
    }

    //
    // Initialize the Autonomous state machine.
    //
    SMInit(g_AutoSM);
    SMStart(g_AutoSM);

    TExit();
    return;
}   //RobotInit

/**
 *  This function processes all the high frequency tasks that needs to run
 *  more often than other tasks such as sensor integration tasks.
 */

void
HiFreqTasks()
{
    TFuncName("HiFreqTasks");
    TLevel(TASK);
    TEnter();

    GyroTask(g_Gyro);

#ifdef _USE_ACCEL
    static float minAccelZ = 10.0;
    static float maxAccelZ = -10.0;
    float currZ;

    AccelTask(g_Accel);
    currZ = AccelGetZAccel(g_Accel);
    if (currZ > maxAccelZ)
    {
        maxAccelZ = currZ;
    }
    else if (currZ < minAccelZ)
    {
        minAccelZ = currZ;
    }
    nxtDisplayTextLine(3, "Lo=%5.2f,Hi=%5.2f", minAccelZ, maxAccelZ);
#endif


    TExit();
    return;
}   //HiFreqTasks

/**
 *  This function processes all the input tasks.
 */
void
InputTasks()
{
    TFuncName("InputTasks");
    TLevel(TASK);
    TEnter();

    TouchTask(g_BotLimitSW);
    TouchTask(g_TopLimitSW);
    TimerTask(g_Timer);

    TExit();
    return;
}   //InputTasks

/**
 *  This function processes all the main tasks.
 */
void
MainTasks()
{
    TFuncName("MainTasks");
    TLevel(TASK);
    TEnter();

    if (SMIsEnabled(g_AutoSM) && SMIsReady(g_AutoSM))
    {
        //
        // Autonomous mode.
        //
        nxtDisplayTextLine(0, "Auto=%02d [%5.1f]",
                           g_AutoSM.currState - SMSTATE_STARTED,
                           (float)(nPgmTime - g_AutoStartTime)/1000.0);

        if (g_Test == TEST_COMPETITION)
        {
            float direction = g_Alliance == ALLIANCE_RED?  -1.0: 1.0;
            long delay = (int)(g_AutoDelay*1000.0);

            switch (g_Strategy)
            {
                case STRATEGY_PARK_BACK:
                    ParkBack1(g_AutoSM, direction, delay);
                    break;

                case STRATEGY_PARK_FRONT:
                    ParkFront1(g_AutoSM, direction, delay);
                    break;

                case STRATEGY_CRATE_FRONT:
                case STRATEGY_BOTH_CRATES_FRONT1:
                case STRATEGY_BOTH_CRATES_FRONT2:
                    CrateFront(g_AutoSM, direction, delay, g_Strategy);
                    break;

                default:
                    TErr(("Invalid Strategy!"));
                    break;
            }
        }
        else
        {
            switch (g_Test)
            {
                case TEST_DRIVE_8FT:
                    switch (g_AutoSM.currState)
                    {
                        case SMSTATE_STARTED:
                            PIDDriveSetTarget(g_EncoderDrive,
                                              96.0, 0.0,
                                              false,
                                              &g_AutoSM,
                                              EVTTYPE_PIDDRIVE);
                            SMAddWaitEvent(g_AutoSM, EVTTYPE_PIDDRIVE);
                            SMWaitEvents(g_AutoSM, g_AutoSM.currState + 1);
                            break;

                        default:
                            SMStop(g_AutoSM);
                            PlayTone(440, 50);
                            break;
                    }
                    break;

                case TEST_TURN_360:
                    switch (g_AutoSM.currState)
                    {
                        case SMSTATE_STARTED:
                            PIDDriveSetTarget(g_EncoderDrive,
                                              0.0, 360.0,
                                              false,
                                              &g_AutoSM,
                                              EVTTYPE_PIDDRIVE);
                            SMAddWaitEvent(g_AutoSM, EVTTYPE_PIDDRIVE);
                            SMWaitEvents(g_AutoSM, g_AutoSM.currState + 1);
                            break;

                        default:
                            SMStop(g_AutoSM);
                            PlayTone(440, 50);
                            break;
                    }
                    break;

                case TEST_SONARIR_DRIVE:
                    switch (g_AutoSM.currState)
                    {
                        case SMSTATE_STARTED:
                            PIDDriveSetTarget(g_SonarIRDrive,
                                              12.0, 0.0,
                                              false,
                                              &g_AutoSM,
                                              EVTTYPE_PIDDRIVE);
                            SMAddWaitEvent(g_AutoSM, EVTTYPE_PIDDRIVE);
                            SMWaitEvents(g_AutoSM, g_AutoSM.currState + 1);
                            break;

                        case SMSTATE_STARTED + 1:
                            PlayTone(440, 10);
                            g_AutoSM.currState++;
                            //
                            // Let it fall through to stop.
                            //
                        default:
                            SMStop(g_AutoSM);
                            break;
                    }
                    break;

                case TEST_SENSOR_DIAG:
                    nxtDisplayTextLine(2, "LEn=%d,REn=%d",
                                       nMotorEncoder[leftMotor],
                                       nMotorEncoder[rightMotor]);
                    nxtDisplayTextLine(3, "ElEn=%d,Gyro=%5.1f",
                                       nMotorEncoder[elevatorMotor],
                                       GyroGetHeading(g_Gyro));
                    break;

                case TEST_0x8_RECT:
                    switch (g_AutoSM.currState)
                    {
                        case SMSTATE_STARTED:
                            PIDDriveSetTarget(g_EncoderDrive,
                                              8*12.0, 0.0,
                                              false,
                                              &g_AutoSM,
                                              EVTTYPE_PIDDRIVE);
                            SMAddWaitEvent(g_AutoSM, EVTTYPE_PIDDRIVE);
                            SMWaitEvents(g_AutoSM, g_AutoSM.currState + 1);
                            break;

                        case SMSTATE_STARTED + 1:
                            PIDDriveSetTarget(g_EncoderDrive,
                                              0.0, 180.0,
                                              false,
                                              &g_AutoSM,
                                              EVTTYPE_PIDDRIVE);
                            SMAddWaitEvent(g_AutoSM, EVTTYPE_PIDDRIVE);
                            SMWaitEvents(g_AutoSM, g_AutoSM.currState - 1);
                            break;
                     }
                     break;

                default:
                    TErr(("Invalid Test!"));
                    break;
            }
        }
    }


    nxtDisplayTextLine(3, "dis%f5.2", (float)(nMotorEncoder[leftMotor]) * DISTANCE_PER_CLICK);

    TExit();
    return;
}   //MainTasks

/**
 *  This function processes all the output tasks. Output tasks are where all
 *  the actions are taking place. All other tasks are just changing states of
 *  various objects. There is no action taken until the output tasks.
 */
void
OutputTasks()
{
    TFuncName("OutputTasks");
    TLevel(TASK);
    TEnter();

    //
    // The Drive task programs the drive motors and set the robot into
    // action.
    //
    PIDDriveTask(g_EncoderDrive);
    PIDDriveTask(g_SonarIRDrive);
    DriveTask(g_Drive);
    PIDMotorTask(g_Elevator);
    nxtDisplayTextLine(1, "L=%d,R=%d",
                       g_Drive.motorPowers[IDX_FRONT_LEFT],
                       g_Drive.motorPowers[IDX_FRONT_RIGHT]);
#ifdef _USE_ACCEL
    nxtDisplayTextLine(2, "Z=%5.2f", AccelGetZAccel(g_Accel));
    nxtDisplayTextLine(3, "X=%5.2f,Y=%5.2f",
                       AccelGetXAccel(g_Accel),
                       AccelGetYAccel(g_Accel));
#endif
    BattTask(g_Batt);

    TExit();
    return;
}   //OutputTasks
