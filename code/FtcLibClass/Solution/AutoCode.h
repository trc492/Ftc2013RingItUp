//
// Program switches.
//
//#define _COMPETITION_ENABLED
//#define _USE_COMPASS

//
// Include library modules.
//

// Include RobotC drivers.

// Include FTC library.
#include "..\..\ftclib\batt.h"
#include "..\..\ftclib\sm.h"
#ifdef _USE_COMPASS
#include "..\..\ftclib\compass.h"
#endif
#include "..\..\ftclib\drive.h"
#include "..\..\ftclib\pidctrl.h"
#include "..\..\ftclib\piddrive.h"
#include "..\..\ftclib\timer.h"

// Include local files.

//
// Constants.
//
#define DISTANCE_PER_CLICK      0.00564

#define DRIVE_KP                60.0
#define DRIVE_KI                0.0
#define DRIVE_KD                0.0
#define DRIVE_TOLERANCE         1.0     //inch
#define DRIVE_SETTLING          200     //msec

#define TURN_KP                 15.0
#define TURN_KI                 0.0
#define TURN_KD                 0.0
#define TURN_TOLERANCE          1.0     //degree
#define TURN_SETTLING           200     //msec

#define SONAR_KP                20.0
#define SONAR_KI                0.0
#define SONAR_KD                0.0
#define SONAR_TOLERANCE         1.0     //inch
#define SONAR_SETTLING          200     //msec

#define EVTTYPE_PIDDRIVE        (EVTTYPE_NONE + 1)
#define EVTTYPE_TIMER           (EVTTYPE_NONE + 2)

#define CHOICE_TIMED_DRIVE      0
#define CHOICE_DRIVE_10FT       1
#define CHOICE_SONAR_DRIVE      2
#define CHOICE_WALK_AROUND      3

//
// Global data.
//

// Drive subsystem.
#ifdef _USE_COMPASS
COMPASS     g_Compass;
#endif

DRIVE       g_Drive;

PIDCTRL     g_DrivePidCtrl;
PIDCTRL     g_TurnPidCtrl;
PIDDRIVE    g_PidDrive;

PIDCTRL     g_SonarPidCtrl;
PIDDRIVE    g_SonarDrive;

// Miscellaneous.
BATT        g_Batt;
SM          g_AutoSM;
TIMER       g_Timer;

// Menus.
MENU        g_ChoiceMenu;
int         g_Choice = 0;

//
// Callback functions.
//

/**
 *  This function is called when the PID controller needs the current input
 *  value for computing the motor output.
 */
float PIDCtrlGetInput(PIDCTRL &pidCtrl)
{
    float inputValue = 0.0;

    if (PIDCtrlCheck(pidCtrl, g_DrivePidCtrl))
    {
        //
        // DrivePID controller: current input is the encoder distance
        // (average of left and right encoders and translate to inches).
        //
        inputValue = (-nMotorEncoder[leftMotor] + nMotorEncoder[rightMotor])
                     *DISTANCE_PER_CLICK/2.0;
    }
    else if (PIDCtrlCheck(pidCtrl, g_TurnPidCtrl))
    {
#ifdef _USE_COMPASS
        //
        // TurnPID controller: current input is the compass accumulated
        // heading.
        //
        inputValue = CompassGetHeading(g_Compass);
#else
        //
        // TurnPID controller: current input is the gyro heading.
        //
        inputValue = SensorValue[gyroSensor]/10.0;
#endif
    }
    else if (PIDCtrlCheck(pidCtrl, g_SonarPidCtrl))
    {
        //
        // SonarPID controller: current input is the sonar sensor reading.
        //
        static int prevValue = 100;
        int currValue = SensorValue[sonarSensor];

        if (currValue <= 0)
        {
            currValue = prevValue;
        }
        else
        {
            prevValue = currValue;
        }

        inputValue = (float)currValue;
    }

    return inputValue;
}

//
// Main functions.
//

void RobotInit()
{
    //
    // Initialize drive subsystem.
    //
#ifdef _USE_COMPASS
    CompassInit(g_Compass, compassSensor);
#endif

    DriveInit(g_Drive, leftMotor, rightMotor);

    PIDCtrlInit(g_DrivePidCtrl,
                DRIVE_KP, DRIVE_KI, DRIVE_KD,
                DRIVE_TOLERANCE, DRIVE_SETTLING);
    PIDCtrlInit(g_TurnPidCtrl,
                TURN_KP, TURN_KI, TURN_KD,
                TURN_TOLERANCE, TURN_SETTLING);
    PIDDriveInit(g_PidDrive,
                 g_Drive, g_DrivePidCtrl, g_TurnPidCtrl);

    PIDCtrlInit(g_SonarPidCtrl,
                SONAR_KP, SONAR_KI, SONAR_KD,
                SONAR_TOLERANCE, SONAR_SETTLING,
                PIDCTRLO_ABS_SETPT | PIDCTRLO_INVERSE);
    PIDDriveInit(g_SonarDrive,
                 g_Drive, g_SonarPidCtrl, g_TurnPidCtrl);

    //
    // Initialize miscellaneous.
    //
    BattInit(g_Batt, 5, true);
    TimerInit(g_Timer);

    nMotorEncoder[leftMotor] = 0;
    nMotorEncoder[rightMotor] = 0;

    //
    // Initialize autonomous.
    //
    SMInit(g_AutoSM);
    SMStart(g_AutoSM);

    //
    // Initialize menu.
    //
    MenuInit(g_ChoiceMenu, "Choice Menu:");
    MenuAddChoice(g_ChoiceMenu, "Timed Drive", (float)CHOICE_TIMED_DRIVE);
    MenuAddChoice(g_ChoiceMenu, "Drive 10 ft", (float)CHOICE_DRIVE_10FT);
    MenuAddChoice(g_ChoiceMenu, "Sonar Drive", (float)CHOICE_SONAR_DRIVE);
    MenuAddChoice(g_ChoiceMenu, "Walk Around", (float)CHOICE_WALK_AROUND);

    //
    // Get user choice.
    //
    g_Choice = (int)MenuGetChoice(g_ChoiceMenu);
    wait1Msec(3000);
}

void StartMode(int pgmMode)
{
}

void StopMode(int pgmMode)
{
}

void DoTimedDrive()
{
    if (SMIsReady(g_AutoSM))
    {
        switch (g_AutoSM.currState)
        {
            case SMSTATE_STARTED:
                //
                // Drive forward at 100% power for 7 seconds.
                //
                DriveArcade(g_Drive, 100.0, 0.0);
                TimerSet(g_Timer, 7000, &g_AutoSM, EVTTYPE_TIMER);
                SMAddWaitEvent(g_AutoSM, EVTTYPE_TIMER);
                SMWaitEvents(g_AutoSM, g_AutoSM.currState + 1);
                break;

            case SMSTATE_STARTED + 1:
                //
                // Turn at 50% power for 1.4 seconds.
                //
                DriveArcade(g_Drive, 0.0, -50.0);
                TimerSet(g_Timer, 1400, &g_AutoSM, EVTTYPE_TIMER);
                SMAddWaitEvent(g_AutoSM, EVTTYPE_TIMER);
                SMWaitEvents(g_AutoSM, g_AutoSM.currState + 1);
                break;

            case SMSTATE_STARTED + 2:
                //
                // Drive backward at 50% power for 2.5 seconds.
                //
                DriveArcade(g_Drive, -50.0, 0.0);
                TimerSet(g_Timer, 2500, &g_AutoSM, EVTTYPE_TIMER);
                SMAddWaitEvent(g_AutoSM, EVTTYPE_TIMER);
                SMWaitEvents(g_AutoSM, g_AutoSM.currState + 1);
                break;

            default:
                //
                // Stop the robot.
                //
                DriveArcade(g_Drive, 0.0, 0.0);
                SMStop(g_AutoSM);
                break;
       }
    }
}

void DoDrive10ft()
{
    if (SMIsReady(g_AutoSM))
    {
        switch (g_AutoSM.currState)
        {
            case SMSTATE_STARTED:
                //
                // Move forward 120 inches.
                //
                PIDDriveSetTarget(g_PidDrive,
                                  120.0, 0.0,
                                  false,
                                  &g_AutoSM,
                                  EVTTYPE_PIDDRIVE);
                SMAddWaitEvent(g_AutoSM, EVTTYPE_PIDDRIVE);
                SMWaitEvents(g_AutoSM, g_AutoSM.currState + 1);
                break;

            default:
                SMStop(g_AutoSM);
                break;
       }
    }
}

void DoSonarDrive()
{
    if (SMIsReady(g_AutoSM))
    {
        switch (g_AutoSM.currState)
        {
            case SMSTATE_STARTED:
                //
                // Move forward until obstacle detected ahead at 50 cm.
                //
                PIDDriveSetTarget(g_SonarDrive,
                                  50.0, 0.0,
                                  false,
                                  &g_AutoSM,
                                  EVTTYPE_PIDDRIVE);
                SMAddWaitEvent(g_AutoSM, EVTTYPE_PIDDRIVE);
                SMWaitEvents(g_AutoSM, g_AutoSM.currState + 1);
                break;

            default:
                SMStop(g_AutoSM);
                break;
       }
    }
}

void DoWalkAround()
{
    if (SMIsReady(g_AutoSM))
    {
        switch (g_AutoSM.currState)
        {
            case SMSTATE_STARTED:
                //
                // Move forward 100 inches.
                //
                PIDDriveSetTarget(g_PidDrive,
                                  100.0, 0.0,
                                  false,
                                  &g_AutoSM,
                                  EVTTYPE_PIDDRIVE);
                SMAddWaitEvent(g_AutoSM, EVTTYPE_PIDDRIVE);
                SMWaitEvents(g_AutoSM, g_AutoSM.currState + 1);
                break;

            case SMSTATE_STARTED + 1:
                //
                // Turn left 90 degrees.
                //
                PIDDriveSetTarget(g_PidDrive,
                                  0.0, -90.0,
                                  false,
                                  &g_AutoSM,
                                  EVTTYPE_PIDDRIVE);
                SMAddWaitEvent(g_AutoSM, EVTTYPE_PIDDRIVE);
                SMWaitEvents(g_AutoSM, g_AutoSM.currState + 1);
                break;

            case SMSTATE_STARTED + 2:
                //
                // Move forward 65 inches.
                //
                PIDDriveSetTarget(g_PidDrive,
                                  65.0, 0.0,
                                  false,
                                  &g_AutoSM,
                                  EVTTYPE_PIDDRIVE);
                SMAddWaitEvent(g_AutoSM, EVTTYPE_PIDDRIVE);
                SMWaitEvents(g_AutoSM, g_AutoSM.currState + 1);
                break;

            case SMSTATE_STARTED + 3:
                //
                // Turn left 45 degrees.
                //
                PIDDriveSetTarget(g_PidDrive,
                                  0.0, -45.0,
                                  false,
                                  &g_AutoSM,
                                  EVTTYPE_PIDDRIVE);
                SMAddWaitEvent(g_AutoSM, EVTTYPE_PIDDRIVE);
                SMWaitEvents(g_AutoSM, g_AutoSM.currState + 1);
                break;

            case SMSTATE_STARTED + 4:
                //
                // Move forward 20 inches.
                //
                PIDDriveSetTarget(g_PidDrive,
                                  20.0, 0.0,
                                  false,
                                  &g_AutoSM,
                                  EVTTYPE_PIDDRIVE);
                SMAddWaitEvent(g_AutoSM, EVTTYPE_PIDDRIVE);
                SMWaitEvents(g_AutoSM, g_AutoSM.currState + 1);
                break;

            case SMSTATE_STARTED + 5:
                //
                // Turn left 90 degrees.
                //
                PIDDriveSetTarget(g_PidDrive,
                                  0.0, -90.0,
                                  false,
                                  &g_AutoSM,
                                  EVTTYPE_PIDDRIVE);
                SMAddWaitEvent(g_AutoSM, EVTTYPE_PIDDRIVE);
                SMWaitEvents(g_AutoSM, g_AutoSM.currState + 1);
                break;

            case SMSTATE_STARTED + 6:
                //
                // Move forward 80 inches.
                //
                PIDDriveSetTarget(g_PidDrive,
                                  80.0, 0.0,
                                  false,
                                  &g_AutoSM,
                                  EVTTYPE_PIDDRIVE);
                SMAddWaitEvent(g_AutoSM, EVTTYPE_PIDDRIVE);
                SMWaitEvents(g_AutoSM, g_AutoSM.currState + 1);
                break;

            case SMSTATE_STARTED + 7:
                //
                // Turn left 45 degrees.
                //
                PIDDriveSetTarget(g_PidDrive,
                                  0.0, -45.0,
                                  false,
                                  &g_AutoSM,
                                  EVTTYPE_PIDDRIVE);
                SMAddWaitEvent(g_AutoSM, EVTTYPE_PIDDRIVE);
                SMWaitEvents(g_AutoSM, g_AutoSM.currState + 1);
                break;

            case SMSTATE_STARTED + 8:
                //
                // Move forward 24 inches.
                //
                PIDDriveSetTarget(g_PidDrive,
                                  24.0, 0.0,
                                  false,
                                  &g_AutoSM,
                                  EVTTYPE_PIDDRIVE);
                SMAddWaitEvent(g_AutoSM, EVTTYPE_PIDDRIVE);
                SMWaitEvents(g_AutoSM, g_AutoSM.currState + 1);
                break;

            case SMSTATE_STARTED + 9:
                //
                // Turn right 270 degrees.
                //
                PIDDriveSetTarget(g_PidDrive,
                                  0.0, 270.0,
                                  false,
                                  &g_AutoSM,
                                  EVTTYPE_PIDDRIVE);
                SMAddWaitEvent(g_AutoSM, EVTTYPE_PIDDRIVE);
                SMWaitEvents(g_AutoSM, g_AutoSM.currState + 1);
                break;

            case SMSTATE_STARTED + 10:
                //
                // Move backward 30 inches.
                //
                PIDDriveSetTarget(g_PidDrive,
                                  -30.0, 0.0,
                                  false,
                                  &g_AutoSM,
                                  EVTTYPE_PIDDRIVE);
                SMAddWaitEvent(g_AutoSM, EVTTYPE_PIDDRIVE);
                SMWaitEvents(g_AutoSM, g_AutoSM.currState + 1);
                break;

            default:
                SMStop(g_AutoSM);
                break;
        }
    }
}

void MainTasks(int pgmMode, float time)
{
    //
    // Perform the routines according to user choice.
    //
    nxtDisplayTextLine(0, "Auto: %d [%5.1f]",
                       SMGetState(g_AutoSM) - SMSTATE_STARTED, time);
    nxtDisplayCenteredTextLine(1, MenuGetChoiceText(g_ChoiceMenu));
    nxtDisplayTextLine(2, "Dist=%5.1f", PIDCtrlGetInput(g_DrivePidCtrl));
    nxtDisplayTextLine(3, "Heading=%5.1f", PIDCtrlGetInput(g_TurnPidCtrl));
    nxtDisplayTextLine(4, "Sonar=%5.1f", PIDCtrlGetInput(g_SonarPidCtrl));

    switch (g_Choice)
    {
        case CHOICE_TIMED_DRIVE:
            DoTimedDrive();
            break;

        case CHOICE_DRIVE_10FT:
            DoDrive10ft();
            break;

        case CHOICE_SONAR_DRIVE:
            DoSonarDrive();
            break;

        case CHOICE_WALK_AROUND:
            DoWalkAround();
            break;

        default:
            break;
    }
}

//
// Other periodic tasks.
//

void HiFreqTasks(int pgmMode)
{
#ifdef _USE_COMPASS
    CompassTask(g_Compass);
#endif
}

void InputTasks(int pgmMode)
{
    BattTask(g_Batt);
    TimerTask(g_Timer);
}

void OutputTasks(int pgmMode)
{
    PIDDriveTask(g_SonarDrive);
    PIDDriveTask(g_PidDrive);
    DriveTask(g_Drive);
}
