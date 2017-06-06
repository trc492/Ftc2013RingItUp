#if 0
/// Copyright (c) Titan Robotics Club. All rights reserved.
///
/// <module name="TeleOpCode.h" />
///
/// <summary>
///     This module contains the TeleOp-only functions.
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
#include "..\ftclib\joybtn.h"

#define CLOSEST_TO_ZERO(x,y)    ((abs(x) < abs(y))? (x): (y))

//
// Global data.
//

// Input subsystem.
JOYBTN      g_JoyBtn1;
JOYBTN      g_JoyBtn2;

// Drive subsystem.
bool        g_DriveSlow = false;

// Elevator subsystem.
float       g_PrevElevatorTarget = 0.0;
bool        g_ElevatorOverride = false;

// Ramp subsystem.
bool        g_RampSafety1 = false;
bool        g_RampSafety2 = false;

// Miscellaneous.

//
// Callback functions.
//

/**
 *  This function is called to deploy the ramp.
 */
void DeployRamp(bool bUnlatch)
{
    if (bUnlatch)
    {
        ServoSetAngle(g_RampLatchServo, RAMP_UNLATCH_ANGLE);
        PIDMotorSetTarget(g_RampMotor, RAMP_UPPER_LIMIT);
    }
    else
    {
        ServoSetAngle(g_RampLatchServo, RAMP_LATCH_ANGLE);
    }
}   //DeployRamp

/**
 *  This function handles the joystick button notification events.
 *
 *  @param joystickID Specifies the joystick the event was from.
 *  @param eventType Specifies the event type.
 *  @param eventID Specifies the event ID.
 *  @param fPressed Specifies the event is a press or a release.
 */
void
JoyBtnEvent(
    int  joystickID,
    int  eventType,
    int  eventID,
    bool fPressed
    )
{
    if (joystickID == 1)
    {
        if (eventType == JOYEVENT_BUTTON)
        {
            switch (eventID)
            {
                case Logitech_RB8:
                    g_DriveSlow = fPressed;
                    break;

                case Logitech_Btn9:
                    g_RampSafety1 = fPressed;
                    if (g_RampSafety1 && g_RampSafety2)
                    {
                        DeployRamp(true);
                    }
                    else if (!g_RampSafety1 && !g_RampSafety2)
                    {
                        DeployRamp(false);
                    }
                    break;

                case Logitech_Btn10:
                    g_RampSafety2 = fPressed;
                    if (g_RampSafety1 && g_RampSafety2)
                    {
                        DeployRamp(true);
                    }
                    else if (!g_RampSafety1 && !g_RampSafety2)
                    {
                        DeployRamp(false);
                    }
                    break;

                default:
                    break;
            }
        }
        else if (eventType == JOYEVENT_TOPHAT)
        {
            switch (eventID)
            {
                case TopHat_Up:
                    break;

                case TopHat_Down:
                    break;

                default:
                    break;
            }
        }
    }
    else if (joystickID == 2)
    {
        if (eventType == JOYEVENT_BUTTON)
        {
            switch (eventID)
            {
                case Logitech_Btn1:
                    g_ElevatorOverride = fPressed;
                    break;

                case Logitech_Btn2:
                    // Move elevator to low peg.
                    if (fPressed)
                    {
                        PIDMotorSetTarget(g_ElevatorMotor,
                                          ELEVATOR_LOW_PEG,
                                          false);
                    }
                    break;

                case Logitech_Btn3:
                    // Move elevator to mid peg.
                    if (fPressed)
                    {
                        PIDMotorSetTarget(g_ElevatorMotor,
                                          ELEVATOR_MID_PEG,
                                          false);
                    }
                    break;

                case Logitech_Btn4:
                    // Move elevator to top peg.
                    if (fPressed)
                    {
                        PIDMotorSetTarget(g_ElevatorMotor,
                                          ELEVATOR_TOP_PEG,
                                          false);
                    }
                    break;

                case Logitech_LB5:
                    // Close gripper
                    if (fPressed)
                    {
                        ServoSetAngle(g_GripperServo, GRIPPER_CLOSE_ANGLE);
                    }
                    break;

                case Logitech_RB6:
                    // Open gripper
                    if (fPressed)
                    {
                        ServoSetAngle(g_GripperServo, GRIPPER_OPEN_ANGLE);
                    }
                    break;

                case Logitech_Btn10:
                    // Set Elevator ground zero
                    if (fPressed)
                    {
                        nMotorEncoder[elevatorMotor] = 0;
                    }
                    break;

                default:
                    break;
            }
        }
        else if (eventType == JOYEVENT_TOPHAT)
        {
            switch (eventID)
            {
                case TopHat_Up:
                    ServoSetAngle(g_WristServo,
                                  WRIST_UPPER_LIMIT,
                                  WRIST_UP_STEP_RATE);
                    break;

                case TopHat_Down:
                    ServoSetAngle(g_WristServo,
                                  WRIST_LOWER_LIMIT,
                                  WRIST_DOWN_STEP_RATE);
                    break;

                default:
                    ServoStop(g_WristServo);
                    break;
            }
        }
    }
}   //JoyBtnEvent

//
// Main functions.
//

/**
 *  This function is called once globally to do TeleOp specific
 *  initializations.
 */
void TeleOpInit()
{
    //
    // Initialize Input subsystem.
    //
    JoyBtnInit(g_JoyBtn1, 1);
    JoyBtnInit(g_JoyBtn2, 2);
}   //TeleOpInit

/**
 *  This function is called before TeleOp mode starts.
 */
void
TeleOpStart()
{
}   //TeleOpStart

/**
 *  This function is called after TeleOp mode ends.
 */
void
TeleOpStop()
{
}   //TeleOpStop

/**
 *  This function is called periodically to perform the TeleOp tasks.
 *
 *  @param time Specifies the running time since TeleOp started.
 */
void
TeleOpTasks(
    float time
    )
{
    nxtDisplayTextLine(0, "TeleOp [%5.1f]", time);
    //
    // Process Drive subsystem.
    //
#ifdef _ARCADE_DRIVE
    int drivePower = JOYSTICK_POWER(joystick.joy1_y1);
    int turnPower = JOYSTICK_POWER(joystick.joy1_x1);
    nxtDisplayTextLine(1, "D=%4d,T=%4d", drivePower, turnPower);
    DriveArcade(g_Drive, drivePower, turnPower);
#else
    int leftPower = JOYSTICK_POWER(joystick.joy1_y1);
    int rightPower = JOYSTICK_POWER(joystick.joy1_y2);
    if (g_DriveSlow)
    {
        leftPower /= 2;
        rightPower /= 2;
    }
    nxtDisplayTextLine(1, "L=%4d,R=%4d", leftPower, rightPower);
    DriveTank(g_Drive, leftPower, rightPower);
#endif

    //
    // Process Elevator subsystem.
    //
    if (g_ElevatorOverride)
    {
        int elevatorPower = JOYSTICK_POWER(joystick.joy2_y1);
        PIDMotorSetPower(g_ElevatorMotor,
                         elevatorPower,
                         ELEVATOR_STALL_MINPOWER,
                         ELEVATOR_STALL_TIMEOUT);
#ifdef _DEBUG_ELEVATOR
        nxtDisplayTextLine(2, "P=%4d,H=%5.1f",
                           elevatorPower,
                           PIDCtrlGetInput(g_ElevatorPidCtrl));
#endif
    }
    else
    {
        //
        // Increase deadband to make sure the elevator motor doesn't get
        // stalled when joystick is not returned to zero.
        //
        int joystickPower = NORMALIZE_DRIVE(DEADBAND(joystick.joy2_y1, 50),
                                            -128, 127);
        float currTarget = (joystickPower > 0)? ELEVATOR_UPPER_LIMIT:
                           (joystickPower < 0)? ELEVATOR_LOWER_LIMIT: 0.0;
#ifdef _DEBUG_ELEVATOR
        nxtDisplayTextLine(2, "pT=%5.1f,cT=%5.1f",
                           g_PrevElevatorTarget, currTarget);
#endif
        if (currTarget != g_PrevElevatorTarget)
        {
            if (joystickPower == 0)
            {
                PIDMotorReset(g_ElevatorMotor);
            }
            else
            {
                PIDMotorSetTarget(g_ElevatorMotor, currTarget, false);
            }
            g_PrevElevatorTarget = currTarget;
        }
#ifdef _DEBUG_ELEVATOR
        nxtDisplayTextLine(3, "H=%5.1f,E=%5.1f",
                           PIDCtrlGetInput(g_ElevatorPidCtrl),
                           PIDCtrlGetError(g_ElevatorPidCtrl));
#endif
    }

#ifdef _DEBUG_RAMP
    //
    // Process the Ramp subsystem.
    //
    static bool bRampActive = false;
    int rampPower = JOYSTICK_POWER(joystick.joy2_y2)/2;
    if (rampPower != 0)
    {
        PIDMotorSetPower(g_RampMotor,
                         rampPower,
                         RAMP_STALL_MINPOWER,
                         RAMP_STALL_TIMEOUT);
        bRampActive = true;
    }
    else if (bRampActive)
    {
        PIDMotorSetPower(g_RampMotor,
                         0,
                         RAMP_STALL_MINPOWER,
                         RAMP_STALL_TIMEOUT);
        bRampActive = false;
    }
    PIDCtrlDisplayInfo(2, g_RampPidCtrl);
#endif

}   //TeleOpTasks

//
// Other periodic tasks.
//

/**
 *  This function is called periodically to perform tasks specific to
 *  TeleOp mode that require high resolution (e.g. gyro integration).
 */
void TeleOpHiFreqTasks()
{
}   //TeleOpHiFreqTasks

/**
 *  This function is called periodically to perform tasks related to sensors
 *  and inputs that are specific to TeleOp mode.
 */
void TeleOpInputTasks()
{
    JoyBtnTask(g_JoyBtn1);
    JoyBtnTask(g_JoyBtn2);
}   //TeleOpInputTasks

/**
 *  This function is called periodically to perform tasks related to motors
 *  and other outputs that are specific to TeleOp mode.
 */
void TeleOpOutputTasks()
{
}   //TeleOpOutputTasks
