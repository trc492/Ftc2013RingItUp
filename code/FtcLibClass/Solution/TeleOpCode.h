//
// Program switches.
//
//#define _COMPETITION_ENABLED
#define _ARCADE_DRIVE

//
// Include library modules.
//

// Include RobotC drivers.

// Include FTC library.
#include "..\..\ftclib\joybtn.h"
#include "..\..\ftclib\drive.h"

// Include local files.

//
// Constants.
//
#define ARM_POWER       30
#define WRIST_POWER     30
#define GRIPPER_POWER   30

//
// Global data.
//

// TeleOp subsystem.
JOYBTN      g_JoyBtn;

// Drive subsystem.
DRIVE       g_Drive;

// Miscellaneous.

//
// Callback functions.
//

/**
 *  This function is called when a joystick button is pressed or released.
 */
void JoyBtnEvent(int joystickID, int eventType, int eventID, bool fPressed)
{
    if (joystickID == 1)
    {
        if (eventType == JOYEVENT_BUTTON)
        {
            switch (eventID)
            {
                case Xbox_A:
                    //
                    // Lower the arm.
                    //
                    if (fPressed)
                    {
                        motor[armMotor] = -ARM_POWER;
                    }
                    else
                    {
                        motor[armMotor] = 0;
                    }
                    break;

                case Xbox_B:
                    //
                    // Raise the arm.
                    //
                    if (fPressed)
                    {
                        motor[armMotor] = ARM_POWER;
                    }
                    else
                    {
                        motor[armMotor] = 0;
                    }
                    break;

                case Xbox_X:
                    //
                    // Turn the wrist downward.
                    //
                    if (fPressed)
                    {
                        motor[wristMotor] = -WRIST_POWER;
                    }
                    else
                    {
                        motor[wristMotor] = 0;
                    }
                    break;

                case Xbox_Y:
                    //
                    // Turn the wrist upward.
                    //
                    if (fPressed)
                    {
                        motor[wristMotor] = WRIST_POWER;
                    }
                    else
                    {
                        motor[wristMotor] = 0;
                    }
                    break;

                case Xbox_LB:
                    //
                    // Open the gripper.
                    //
                    if (fPressed)
                    {
                        motor[gripperMotor] = -GRIPPER_POWER;
                    }
                    else
                    {
                        motor[gripperMotor] = 0;
                    }
                    break;

                case Xbox_RB:
                    //
                    // Close the gripper.
                    //
                    if (fPressed)
                    {
                        motor[gripperMotor] = GRIPPER_POWER;
                    }
                    else
                    {
                        motor[gripperMotor] = 0;
                    }
                    break;
            }
        }
        else if (eventType == JOYEVENT_TOPHAT)
        {
        }
    }
}

//
// Main functions.
//

void RobotInit()
{
    //
    // Init TeleOp subsystem.
    //
    JoyBtnInit(g_JoyBtn, 1);

    //
    // Init Drive subsystem.
    //
    DriveInit(g_Drive, leftMotor, rightMotor);

    //
    // Init miscellaneous.
    //
}

void StartMode(int pgmMode)
{
}

void StopMode(int pgmMode)
{
}

void MainTasks(int pgmMode, float time)
{
    nxtDisplayTextLine(0, "TeleOp [%5.1f]", time);
#ifdef _ARCADE_DRIVE
    //
    // Perform arcade drive.
    //
    int drivePower = JOYSTICK_POWER(joystick.joy1_y1);
    int turnPower = JOYSTICK_POWER(joystick.joy1_x1);
    nxtDisplayTextLine(1, "D=%4d,T=%4d", drivePower, turnPower);
    DriveArcade(g_Drive, drivePower, turnPower);
#else
    //
    // Perform tank drive.
    //
    int leftPower = JOYSTICK_POWER(joystick.joy1_y1);
    int rightPower = JOYSTICK_POWER(joystick.joy1_y2);
    nxtDisplayTextLine(1, "L=%4d,R=%4d", leftPower, rightPower);
    DriveTank(g_Drive, leftPower, rightPower);
#endif
}

//
// Other period tasks.
//
void HiFreqTasks(int pgmMode)
{
}

void InputTasks(int pgmMode)
{
    JoyBtnTask(g_JoyBtn);
}

void OutputTasks(int pgmMode)
{
    DriveTask(g_Drive);
}
