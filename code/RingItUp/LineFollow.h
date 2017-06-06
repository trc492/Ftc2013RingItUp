#if 0
/// Copyright (c) Titan Robotics Club. All rights reserved.
///
/// <module name="LineFollow.h" />
///
/// <summary>
///     This module contains the implementation of the LineFollow strategy.
/// </summary>
///
/// <remarks>
///     Environment: RobotC for Lego Mindstorms NXT.
/// </remarks>
#endif

/**
 *  This function drives the robot forward and finds the line. It then follows
 *  the line and hangs the ring to the specified peg.
 *
 *  @param sm Specifies the state machine.
 *  @param delay Specifies the delay if any.
 *  @param pegHeight Specifies the peg height in inches.
 */
void DoLineFollow(SM &sm, long delay, float pegHeight)
{
    int currState = SMGetState(sm);

    switch (currState)
    {
        case SMSTATE_STARTED:
            //
            // Turn on the light sensors.
            // Turn the wrist up 45 degrees.
            // Raise elevator to specified height.
            //
#ifdef _DEBUG_LNFOLLOW
            writeDebugStreamLine("********************");
            writeDebugStreamLine("%02d: Turn wrist up.",
                                 currState - SMSTATE_STARTED);
#endif
            LSsetActive(leftLightSensor);
            LSsetActive(centerLightSensor);
            LSsetActive(rightLightSensor);
            if (pegHeight == 0.0)
            {
                //
                // Use default peg height.
                //
                pegHeight = ELEVATOR_MID_PEG;
            }
            ServoSetAngle(g_WristServo, WRIST_UP_ANGLE);
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
            // Move forward to find the line.
            //
#ifdef _DEBUG_LNFOLLOW
            writeDebugStreamLine("%02d: Drive forward.",
                                 currState - SMSTATE_STARTED);
#endif
            g_PidMode = PIDMODE_DRIVING;
            PIDDriveSetTarget(g_PidDrive,
                              45.0,     //forward in inches
                              0.0,
                              false,
                              &sm,
                              EVTTYPE_PIDDRIVE,
                              3000);    //timeout value
            SMAddWaitEvent(sm, EVTTYPE_PIDDRIVE);
            SMWaitEvents(sm, currState + 1);
            break;

        case SMSTATE_STARTED + 2:
            g_PidMode = PIDMODE_NONE;
            if (g_FindLineState != 0)
            {
                //
                // Found the line, follow it.
                //
#ifdef _DEBUG_LNFOLLOW
                writeDebugStreamLine("%02d: Found line.",
                                     currState - SMSTATE_STARTED);
#endif
                SMSetState(sm, SMSTATE_STARTED + 10);
            }
            else
            {
                //
                // We didn't find the line, try turning left.
                //
#ifdef _DEBUG_LNFOLLOW
                writeDebugStreamLine("%02d: Try left.",
                                     currState - SMSTATE_STARTED);
#endif
                g_PidMode = PIDMODE_DRIVING;
                PIDDriveSetTarget(g_PidDrive,
                                  0.0,
                                  -45.0,
                                  false,
                                  &sm,
                                  EVTTYPE_PIDDRIVE,
                                  3000);
                SMAddWaitEvent(sm, EVTTYPE_PIDDRIVE);
                SMWaitEvents(sm, currState + 1);
            }
            break;

        case SMSTATE_STARTED + 3:
            g_PidMode = PIDMODE_NONE;
            if (g_FindLineState != 0)
            {
                //
                // Found the line, follow it.
                //
#ifdef _DEBUG_LNFOLLOW
                writeDebugStreamLine("%02d: Found line.",
                                     currState - SMSTATE_STARTED);
#endif
                SMSetState(sm, SMSTATE_STARTED + 10);
            }
            else
            {
                //
                // We didn't find the line, try turning right.
                //
#ifdef _DEBUG_LNFOLLOW
                writeDebugStreamLine("%02d: Try right.",
                                     currState - SMSTATE_STARTED);
#endif
                g_PidMode = PIDMODE_DRIVING;
                PIDDriveSetTarget(g_PidDrive,
                                  0.0,
                                  90.0,
                                  false,
                                  &sm,
                                  EVTTYPE_PIDDRIVE,
                                  3000);
                SMAddWaitEvent(sm, EVTTYPE_PIDDRIVE);
                SMWaitEvents(sm, currState + 1);
            }
            break;

        case SMSTATE_STARTED + 4:
            g_PidMode = PIDMODE_NONE;
            if (g_FindLineState != 0)
            {
                //
                // Found the line, follow it.
                //
#ifdef _DEBUG_LNFOLLOW
                writeDebugStreamLine("%02d: Found line.",
                                     currState - SMSTATE_STARTED);
#endif
                SMSetState(sm, SMSTATE_STARTED + 10);
            }
            else
            {
                //
                // Still not finding the line, turn back left.
                //
#ifdef _DEBUG_LNFOLLOW
                writeDebugStreamLine("%02d: Straighten out.",
                                     currState - SMSTATE_STARTED);
#endif
                g_PidMode = PIDMODE_DRIVING;
                PIDDriveSetTarget(g_PidDrive,
                                  0.0,
                                  -45.0,
                                  false,
                                  &sm,
                                  EVTTYPE_PIDDRIVE,
                                  3000);
                SMAddWaitEvent(sm, EVTTYPE_PIDDRIVE);
                SMWaitEvents(sm, currState + 1);
            }
            break;

        case SMSTATE_STARTED + 5:
            g_PidMode = PIDMODE_NONE;
            if (g_FindLineState != 0)
            {
                //
                // Found the line, follow it.
                //
#ifdef _DEBUG_LNFOLLOW
                writeDebugStreamLine("%02d: Found line.",
                                     currState - SMSTATE_STARTED);
#endif
                SMSetState(sm, SMSTATE_STARTED + 10);
            }
            else
            {
                //
                // Just blindly go forward and hope for the best.
                //
#ifdef _DEBUG_LNFOLLOW
                writeDebugStreamLine("%02d: Go blind.",
                                     currState - SMSTATE_STARTED);
#endif
                PIDCtrlSetPowerLimits(g_EncoderPidCtrl, -30, 30);
                PIDCtrlSetPowerLimits(g_SonarPidCtrl, -30, 30);
                PIDDriveSetTarget(g_PidDrive,
                                  20.0,
                                  0.0,
                                  false,
                                  &sm,
                                  EVTTYPE_PIDDRIVE,
                                  3000);
                SMAddWaitEvent(sm, EVTTYPE_PIDDRIVE);
                SMWaitEvents(sm, SMSTATE_STARTED + 11);
            }
            break;

        case SMSTATE_STARTED + 10:
            //
            // Follow the line.
            //
#ifdef _DEBUG_LNFOLLOW
            writeDebugStreamLine("%02d: Follow line",
                                 currState - SMSTATE_STARTED);
#endif
            PIDCtrlSetPowerLimits(g_EncoderPidCtrl, -30, 30);
            PIDCtrlSetPowerLimits(g_SonarPidCtrl, -30, 30);
            PIDDriveSetTarget(g_LnFollowDrive,
                              26.0,
                              LS_CENTER,
                              false,
                              &sm,
                              EVTTYPE_PIDDRIVE,
                              3000);
            SMAddWaitEvent(sm, EVTTYPE_PIDDRIVE);
            SMWaitEvents(sm, currState + 1);
            break;

        case SMSTATE_STARTED + 11:
            //
            // Turn right just a little to align the peg.
            //
#ifdef _DEBUG_LNFOLLOW
            writeDebugStreamLine("%02d: Align right.",
                                 currState - SMSTATE_STARTED);
#endif
            PIDDriveSetTarget(g_PidDrive,
                              0.0,
                              20.0,
                              false,
                              &sm,
                              EVTTYPE_PIDDRIVE,
                              3000);
            SMAddWaitEvent(sm, EVTTYPE_PIDDRIVE);
            SMWaitEvents(sm, currState + 1);
            break;

        case SMSTATE_STARTED + 12:
            //
            // Release the ring onto the peg.
            //
#ifdef _DEBUG_LNFOLLOW
            writeDebugStreamLine("%02d: Open gripper.",
                                 currState - SMSTATE_STARTED);
#endif
            ServoSetAngle(g_GripperServo, GRIPPER_OPEN_ANGLE);
            TimerSet(g_Timer, 500, &sm, EVTTYPE_TIMER);
            SMAddWaitEvent(sm, EVTTYPE_TIMER);
            SMWaitEvents(sm, currState + 1);
            break;

        case SMSTATE_STARTED + 13:
            //
            // Swing wrist down.
            //
#ifdef _DEBUG_LNFOLLOW
            writeDebugStreamLine("%02d: Swing wrist down.",
                                 currState - SMSTATE_STARTED);
#endif
            ServoSetAngle(g_WristServo, WRIST_DOWN_ANGLE);
            TimerSet(g_Timer, 1000, &sm, EVTTYPE_TIMER);
            SMAddWaitEvent(sm, EVTTYPE_TIMER);
            SMWaitEvents(sm, currState + 1);
            break;

        case SMSTATE_STARTED + 14:
            //
            // Backup a bit.
            //
#ifdef _DEBUG_LNFOLLOW
            writeDebugStreamLine("%02d: Back up.",
                                 currState - SMSTATE_STARTED);
#endif
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
#ifdef _DEBUG_LNFOLLOW
            writeDebugStreamLine("%02d: Done!",
                                 currState - SMSTATE_STARTED);
#endif
            SMStop(sm);
            LSsetInactive(leftLightSensor);
            LSsetInactive(centerLightSensor);
            LSsetInactive(rightLightSensor);
            PlayTone(440, 50);
            break;
    }
}   //DoLineFollow
