#if 0
/// Copyright (c) Titan Robotics Club. All rights reserved.
///
/// <module name="Defense.h" />
///
/// <summary>
///     This module contains the implementation of the Defense strategy.
/// </summary>
///
/// <remarks>
///     Environment: RobotC for Lego Mindstorms NXT.
/// </remarks>
#endif

/**
 *  This function drives the robot to block the opponent from doing their
 *  autonomous.
 *
 *  @param sm Specifies the state machine.
 *  @param delay Specifies the delay if any.
 *  @param distance Specifies the distance to travel.
 */
void DoDefense(SM &sm, long delay, float distance)
{
    int currState = SMGetState(sm);

    switch (currState)
    {
        case SMSTATE_STARTED:
            if (delay != 0)
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
            // Drive forward to block the opponent.
            //
            PIDDriveSetTarget(g_PidDrive,
                              distance,
                              0.0,
                              false,
                              &sm,
                              EVTTYPE_PIDDRIVE,
                              8000);
            SMAddWaitEvent(sm, EVTTYPE_PIDDRIVE);
            SMWaitEvents(sm, currState + 1);
            break;

#if 0
        case SMSTATE_STARTED + 2:
            //
            // Turn left 45 degrees to protect our gripper from impact.
            //
            PIDDriveSetTarget(g_PidDrive,
                              0.0,
                              -45.0,
                              false,
                              &sm,
                              EVTTYPE_PIDDRIVE);
            SMAddWaitEvent(sm, EVTTYPE_PIDDRIVE);
            SMWaitEvents(sm, currState + 1);
            break;
#endif

        default:
            SMStop(sm);
            PlayTone(440, 50);
            break;
    }
}   //DoDefense
