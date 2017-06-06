#if 0
/// Copyright (c) Titan Robotics Club. All rights reserved.
///
/// <module name="StraightUp.h" />
///
/// <summary>
///     This module contains the implementation of the StraightUp strategy.
/// </summary>
///
/// <remarks>
///     Environment: RobotC for Lego Mindstorms NXT.
/// </remarks>
#endif

/**
 *  This function drives the robot forward and hangs the ring to the specified
 *  peg.
 *
 *  @param sm Specifies the state machine.
 *  @param delay Specifies the delay if any.
 *  @param pegHeight Specifies the peg height in inches.
 */
void DoStraightUp(SM &sm, long delay, float pegHeight)
{
    int currState = SMGetState(sm);

    switch (currState)
    {
        case SMSTATE_STARTED:
            //
            // Turn the wrist up 45 degrees.
            // Raise elevator to specified height.
            //
            ServoSetAngle(g_WristServo, WRIST_UP_ANGLE);
            if (pegHeight == 0.0)
            {
                //
                // Use default peg height.
                //
                pegHeight = ELEVATOR_MID_PEG;
            }
            PIDMotorSetTarget(g_ElevatorMotor, pegHeight, false);
            if (delay > 0)
            {
                TimerSet(g_Timer, delay, &sm, EVTTYPE_TIMER);
                SMAddWaitEvent(sm, EVTTYPE_TIMER);
                SMWaitEvents(sm, currState + 1);
                break;
            }
            else
            {
                currState++;
            }

        case SMSTATE_STARTED + 1:
            //
            // Move straight forward.
            //
            PIDDriveSetTarget(g_PidDrive,
                              65.0, 0.0,
                              false,
                              &sm,
                              EVTTYPE_PIDDRIVE,
                              5000);
            SMAddWaitEvent(sm, EVTTYPE_PIDDRIVE);
            SMWaitEvents(sm, currState + 1);
            break;

        case SMSTATE_STARTED + 2:
            //
            // Release the ring onto the peg.
            //
            ServoSetAngle(g_GripperServo, GRIPPER_OPEN_ANGLE);
            TimerSet(g_Timer, 500, &sm, EVTTYPE_TIMER);
            SMAddWaitEvent(sm, EVTTYPE_TIMER);
            SMWaitEvents(sm, currState + 1);
            break;

        case SMSTATE_STARTED + 3:
            //
            // Swing wrist down.
            //
            ServoSetAngle(g_WristServo, WRIST_DOWN_ANGLE);
            TimerSet(g_Timer, 1000, &sm, EVTTYPE_TIMER);
            SMAddWaitEvent(sm, EVTTYPE_TIMER);
            SMWaitEvents(sm, currState + 1);
            break;

        case SMSTATE_STARTED + 4:
            //
            // Backup a bit.
            //
            PIDMotorSetTarget(g_ElevatorMotor, ELEVATOR_LOW_PEG, false);
            PIDDriveSetTarget(g_PidDrive,
                              -24.0,
                              0.0,
                              false,
                              &sm,
                              EVTTYPE_PIDDRIVE,
                              3000);
            SMAddWaitEvent(sm, EVTTYPE_PIDDRIVE);
            SMWaitEvents(sm, currState + 1);
            break;

        default:
            SMStop(sm);
            PlayTone(440, 50);
            break;
    }
}   //DoStraightUp
