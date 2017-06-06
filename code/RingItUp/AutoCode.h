#if 0
/// Copyright (c) Titan Robotics Club. All rights reserved.
///
/// <module name="AutoCode.h" />
///
/// <summary>
///     This module contains the Autonomous-only functions.
/// </summary>
///
/// <remarks>
///     Environment: RobotC for Lego Mindstorms NXT.
/// </remarks>
#endif

//
// Include libraries.
//

// Include RobotC drivers.

// Include FTC library.
#include "..\ftclib\timer.h"
#include "..\ftclib\menu.h"

//
// Constants.
//

// Event types.
#define EVTTYPE_PIDDRIVE        (EVTTYPE_NONE + 1)
#define EVTTYPE_PIDMOTOR        (EVTTYPE_NONE + 2)
#define EVTTYPE_TIMER           (EVTTYPE_NONE + 3)

// Strategies menu.
#define STRATEGY_NOAUTO         0
#define STRATEGY_SEEK_IR        1
#define STRATEGY_LINEFOLLOW     2
#define STRATEGY_STRAIGHTUP     3
#define STRATEGY_DEFENSE        4

// Start Position menu.
#define STARTPOS_CENTER         0
#define STARTPOS_RIGHT          1

// Defense distance menu.
#define DEFENSE_4FT             48.0
#define DEFENSE_6FT             72.0
#define DEFENSE_8FT             96.0

#define AUTO_INIT_DELAY         1500

#define PIDMODE_NONE            0
#define PIDMODE_DRIVING         1
#define PIDMODE_IRSEEKING       2

//
// Global data.
//

// Miscellaneous.
TIMER       g_Timer;
SM          g_AutoSM;
int         g_LightSensorStates = 0;
int         g_PidMode = PIDMODE_NONE;
int         g_FindLineState = 0;

// Menus.
MENU        g_AutoDelayMenu;
MENU        g_StrategyMenu;
MENU        g_StartPosMenu;
MENU        g_PegHeightMenu;
MENU        g_BackoutMenu;
MENU        g_DefenseDistMenu;
long        g_AutoDelay = 0;
int         g_Strategy = STRATEGY_SEEK_IR;
int         g_StartPos = STARTPOS_CENTER;
float       g_PegHeight = 0.0;
float       g_Backout = 0.0;
float       g_DefenseDist = DEFENSE_8FT;

//
// Include autonomous strategies.
//
#include "LineFollow.h"
#include "SeekIR.h"
#include "StraightUp.h"
#include "Defense.h"
#include "TestMode.h"

//
// Callback functions.
//

//
// Main functions.
//

/**
 *  This function is called once globally to do Autonomous specific
 *  initializations.
 */
void AutoInit()
{
    //
    // Perform elevator zero calibration:
    // To prevent the motor from being fried because of the touch sensor
    // failed to engage, the following code will stop the motor as soon
    // as the touch sensor is engaged or if the elevator is stuck for
    // more than the timeout period. There are a number of reasons the
    // touch sensor failed to engage. It could be a mechanical problem
    // or the touch sensor is on a MUX and we forgot to turn the MUX power
    // ON.
    //
    PIDMotorZeroCalibrate(g_ElevatorMotor,
                          g_ElevatorZeroSW,
                          ELEVATOR_CAL_POWER,
                          ELEVATOR_STALL_TIMEOUT);

    TimerInit(g_Timer);
    SMInit(g_AutoSM);

    //
    // Initialize menus.
    //

    // Autonomous delay menu.
    MenuInit(g_AutoDelayMenu, "Autonomous Delay:");
    MenuAddChoice(g_AutoDelayMenu, "None", 0.0);
    MenuAddChoice(g_AutoDelayMenu, "1 sec", 1000.0);
    MenuAddChoice(g_AutoDelayMenu, "2 sec", 2000.0);
    MenuAddChoice(g_AutoDelayMenu, "4 sec", 4000.0);
    MenuAddChoice(g_AutoDelayMenu, "6 sec", 6000.0);
    MenuAddChoice(g_AutoDelayMenu, "8 sec", 8000.0);

    // Strategy menu.
    MenuInit(g_StrategyMenu, "Strategies:");
    MenuAddChoice(g_StrategyMenu, "No Auto", (float)STRATEGY_NOAUTO);
    MenuAddChoice(g_StrategyMenu, "Seek IR", (float)STRATEGY_SEEK_IR);
    MenuAddChoice(g_StrategyMenu, "Line Follow", (float)STRATEGY_LINEFOLLOW);
    MenuAddChoice(g_StrategyMenu, "Straight Up", (float)STRATEGY_STRAIGHTUP);
    MenuAddChoice(g_StrategyMenu, "Defense", (float)STRATEGY_DEFENSE);

    // Start position menu.
    MenuInit(g_StartPosMenu, "Start Position:");
    MenuAddChoice(g_StartPosMenu, "Center", (float)STARTPOS_CENTER);
    MenuAddChoice(g_StartPosMenu, "Right", (float)STARTPOS_RIGHT);

    // Peg height menu.
    MenuInit(g_PegHeightMenu, "Peg Height:");
    MenuAddChoice(g_PegHeightMenu, "Auto", 0.0);
    MenuAddChoice(g_PegHeightMenu, "Low", ELEVATOR_LOW_PEG);
    MenuAddChoice(g_PegHeightMenu, "Mid", ELEVATOR_MID_PEG);

    // Peg height menu.
    MenuInit(g_BackoutMenu, "Backout Dist:");
    MenuAddChoice(g_BackoutMenu, "Auto", 0.0);
    MenuAddChoice(g_BackoutMenu, "24 in", -24.0);
    MenuAddChoice(g_BackoutMenu, "30 in", -30.0);
    MenuAddChoice(g_BackoutMenu, "36 in", -36.0);
    MenuAddChoice(g_BackoutMenu, "40 in", -40.0);
    MenuAddChoice(g_BackoutMenu, "46 in", -46.0);

    // Defense distance menu.
    MenuInit(g_DefenseDistMenu, "Defense Distance:");
    MenuAddChoice(g_DefenseDistMenu, "4 ft", DEFENSE_4FT);
    MenuAddChoice(g_DefenseDistMenu, "6 ft", DEFENSE_6FT);
    MenuAddChoice(g_DefenseDistMenu, "8 ft", DEFENSE_8FT);

    TestModeInit(&g_AutoSM, &g_Timer);
    if (TestModeIsCompetition())
    {
        g_AutoDelay = (long)MenuGetChoice(g_AutoDelayMenu);
        g_Strategy = (int)MenuGetChoice(g_StrategyMenu);

        if ((g_Strategy == STRATEGY_SEEK_IR) ||
            (g_Strategy == STRATEGY_LINEFOLLOW) ||
            (g_Strategy == STRATEGY_STRAIGHTUP))
        {
            g_PegHeight = MenuGetChoice(g_PegHeightMenu);
            if (g_Strategy == STRATEGY_SEEK_IR)
            {
                g_StartPos = (int)MenuGetChoice(g_StartPosMenu);
                g_Backout = MenuGetChoice(g_BackoutMenu);
            }
        }
        else if (g_Strategy == STRATEGY_DEFENSE)
        {
            g_DefenseDist = MenuGetChoice(g_DefenseDistMenu);
        }

        nxtDisplayCenteredTextLine(0, "%s",
                                   MenuGetChoiceText(g_StrategyMenu));
        nxtDisplayCenteredTextLine(1, "%s Peg [%d]",
                                   MenuGetChoiceText(g_PegHeightMenu),
                                   g_AutoDelay);
        nxtDisplayCenteredTextLine(3, "Wait to Start...");
    }
}   //AutoInit

/**
 *  This function is called periodically to perform tasks specific to
 *  Autonomous mode that require high resolution (e.g. gyro integration).
 */
void AutoHiFreqTasks()
{
    TimerTask(g_Timer);
}   //AutoHiFreqTasks

/**
 *  This function is called periodically to perform tasks related to sensors
 *  and inputs that are specific to Autonomous mode.
 */
void AutoInputTasks()
{
    g_LightSensorStates = GetLightSensorStates();
    if (g_LightSensorStates != 0)
    {
        //
        // We found the line.
        //
        if (g_PidMode == PIDMODE_DRIVING)
        {
            PIDDriveReset(g_PidDrive);
            g_PidMode = PIDMODE_NONE;
            SMSetEvent(g_AutoSM, EVTTYPE_PIDDRIVE);
        }
        else if (g_PidMode == PIDMODE_IRSEEKING)
        {
            PIDDriveReset(g_IrDrive);
            g_PidMode = PIDMODE_NONE;
            SMSetEvent(g_AutoSM, EVTTYPE_PIDDRIVE);
        }
        g_FindLineState = g_LightSensorStates;
    }
}   //AutoInputTasks

/**
 *  This function is called periodically to perform tasks related to motors
 *  and other outputs that are specific to Autonomous mode.
 */
void AutoOutputTasks()
{
}   //AutoOutputTasks

/**
 *  This function is called before Autonomous mode starts.
 */
void
AutoStart()
{
    SMStart(g_AutoSM);
}   //AutoStart

/**
 *  This function is called after Autonomous mode ends.
 */
void
AutoStop()
{
    SMStop(g_AutoSM);
}   //AutoStop

/**
 *  This function is called periodically to perform the Autonomous tasks.
 *
 *  @param time Specifies the running time since Autonomous started.
 */
void AutoTasks(
    float time
    )
{
    nxtDisplayTextLine(0, "Auto: %d [%5.1f]",
                       SMGetState(g_AutoSM) - SMSTATE_STARTED, time);
#ifdef _DEBUG_IRSEEKING
    PIDCtrlDisplayInfo(1, g_SonarPidCtrl);
    PIDCtrlDisplayInfo(3, g_IrTurnPidCtrl);
#endif
//    PIDCtrlDisplayInfo(1, g_EncoderPidCtrl);
//    PIDCtrlDisplayInfo(3, g_LnFollowPidCtrl);
//    PIDCtrlDisplayInfo(1, g_ElevatorPidCtrl);
    if (!TestModeIsCompetition())
    {
        TestModeExecute();
    }
    else if (SMIsReady(g_AutoSM))
    {
        switch (g_Strategy)
        {
            case STRATEGY_NOAUTO:
                SMStop(g_AutoSM);
                break;

            case STRATEGY_SEEK_IR:
                DoSeekIR(g_AutoSM, g_AutoDelay, g_PegHeight, g_StartPos, g_Backout);
                break;

            case STRATEGY_LINEFOLLOW:
                DoLineFollow(g_AutoSM, g_AutoDelay, g_PegHeight);
                break;

            case STRATEGY_STRAIGHTUP:
                DoStraightUp(g_AutoSM, g_AutoDelay, g_PegHeight);
                break;

            case STRATEGY_DEFENSE:
                DoDefense(g_AutoSM, g_AutoDelay, g_DefenseDist);
                break;

            default:
                TErr(("Invalid strategy %d!", g_Strategy));
                break;
        }
    }
}   //AutoTasks
