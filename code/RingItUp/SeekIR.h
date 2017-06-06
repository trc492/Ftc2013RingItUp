#if 0
/// Copyright (c) Titan Robotics Club. All rights reserved.
///
/// <module name="SeekIR.h" />
///
/// <summary>
///     This module contains the implementation of the SeekIR strategy.
/// </summary>
///
/// <remarks>
///     Environment: RobotC for Lego Mindstorms NXT.
/// </remarks>
#endif

#define PEGCOL_LEFT     0
#define PEGCOL_CENTER   1
#define PEGCOL_RIGHT    2

#define PEGROW_LOW      0
#define PEGROW_MID      1

float m_PegHeight[3] = {ELEVATOR_MID_PEG, ELEVATOR_MID_PEG, ELEVATOR_MID_PEG};

// LeftThreshold = {11.5, 11.5}
// RightThreshold = {8.5, 8.5};
float m_IrLeftThreshold[2] = {11.5,             //StartPos_Center
                              11.5};            //StartPos_Right
float m_IrRightThreshold[2] = {8.5,             //StartPos_Center
                               8.5};            //StartPos_Right

// PegDist1 = {{110.0, 85.0, 100.0}, {110.0.100.0, 85.0}}
float m_PegDist1[2][3] = {{110.0, 85.0, 100.0}, //StartPos_Center
                          {110.0, 100.0, 85.0}};//StartPos_Right

// IrDir1 = {{9.8, 10.2, 10.3}, {10.0, 10.2 10.5},
//           {9.5, 9.5, 10.5}, {10.5, 9.5, 9.5}}
float m_IrDir1[4][3] = {{9.8, 10.2, 10.3},      //StartPos_Center, low row
                        {10.0, 10.2, 10.5},     //StartPos_Center, mid row
                        {9.5, 9.5, 10.5},       //StartPos_Right, low row
                        {10.5, 9.5, 9.5}};      //StartPos_Right, mid row

// PegDist2 = {{26.0, 26.0, 25.0}, {19.0, 25.5, 19.0},
//             {27.0, 29.0, 26.2}, {25.0, 22.0, 19.0}}
float m_PegDist2[4][3] = {{26.0, 26.0, 25.0},   //StartPos_Center, low row
                          {19.0, 25.5, 19.0},   //StartPos_Center, mid row
                          {27.0, 29.0, 26.2},   //StartPos_Right, low row
                          {25.0, 22.0, 19.0}};  //StartPos_Right, mid row

// IrDir2 = {{9.5, 10.2, 10.5}, {10.0, 10.2, 10.2},
//           {9.5, 9.4, 9.8}, {10.2, 9.5, 10.0}}
float m_IrDir2[4][3] = {{9.5, 10.2, 10.5},      //StartPos_Center, low row
                        {10.0, 10.2, 10.2},     //StartPos_Center, mid row
                        {9.5, 9.4, 9.8},        //StartPos_Right, low row
                        {10.2, 9.5, 10.0}};     //StartPos_Right, mid row

float m_BackupDist[2][3] = {{-36.0, -36.0, -24.0},//StartPos_Center
                            {-36.0, -36.0, -12.0}};//StartPos_Right

int   m_PegRow = PEGROW_MID;
int   m_IrCol = PEGCOL_CENTER;

/**
 *  This function drives the robot to seek the IR beacon and hangs the ring
 *  to the specified peg.
 *
 *  @param sm Specifies the state machine.
 *  @param delay Specifies the delay if any.
 *  @param pegHeight Specifies the peg height in inches.
 *  @param startPos Specifies the starting position of the robot.
 *  @param backoutDist Specifies the backout distance.
 */
void DoSeekIR(SM &sm, long delay, float pegHeight, int startPos, float backoutDist)
{
    int currState = SMGetState(sm);

    switch (currState)
    {
        case SMSTATE_STARTED:
            //
            // Turn the wrist up 45 degrees.
            // Raise elevator to mid height.
            // Delay at least one second to allow the IR seekers to get in
            // position.
            //
#ifdef _DEBUG_IRSEEKING
            writeDebugStreamLine("********************");
            writeDebugStreamLine("%02d: Turn wrist up.",
                                 currState - SMSTATE_STARTED);
#endif
            ServoSetAngle(g_WristServo, WRIST_IR_ANGLE);
            PIDMotorSetTarget(g_ElevatorMotor, ELEVATOR_MID_PEG, false);
            if (delay == 0)
            {
                delay = AUTO_INIT_DELAY;
            }
            TimerSet(g_Timer, delay, &sm, EVTTYPE_TIMER);
            SMAddWaitEvent(sm, EVTTYPE_TIMER);
            SMWaitEvents(sm, currState + 1);
            break;

        case SMSTATE_STARTED + 1:
            //
            // Drive the robot to seek the IR beacon stage 1.
            //
            float irDir = IRSeekerGetACDir(g_LeftIR) +
                          IRSeekerGetACDir(g_RightIR);
            m_IrCol = (irDir > m_IrLeftThreshold[startPos])? PEGCOL_LEFT:
                      (irDir < m_IrRightThreshold[startPos])? PEGCOL_RIGHT:
                                                              PEGCOL_CENTER;
//#if 0
            if (pegHeight == 0.0)
            {
                //
                // Use default peg height table.
                //
                pegHeight = m_PegHeight[m_IrCol];
            }
            m_PegRow = (pegHeight == ELEVATOR_LOW_PEG)? PEGROW_LOW:
                                                        PEGROW_MID;
//#endif
//            m_PegRow = PEGROW_MID;
#ifdef _DEBUG_IRSEEKING
            writeDebugStreamLine("%02d: Seek IR 1.",
                                 currState - SMSTATE_STARTED);
            writeDebugStreamLine("    startPos=%d,pegHeight=%4.1f,irDir=%4.1f",
                                 startPos, pegHeight, irDir);
            writeDebugStreamLine("    irCol=%d,pegRow=%d",
                                 m_IrCol, m_PegRow);
#endif
            PIDMotorSetTarget(g_ElevatorMotor, pegHeight, false);
            PIDCtrlSetPowerLimits(g_SonarPidCtrl, -50, 50);
            PIDDriveSetTarget(g_IrDrive,
                              m_PegDist1[startPos][m_IrCol],
                              m_IrDir1[startPos*2 + m_PegRow][m_IrCol],
                              false,
                              &sm,
                              EVTTYPE_PIDDRIVE,
                              1500);
            SMAddWaitEvent(sm, EVTTYPE_PIDDRIVE);
            SMWaitEvents(sm, currState + 1);
            break;

        case SMSTATE_STARTED + 2:
            //
            // Drive the robot to seek the IR beacon stage 2.
            //
#ifdef _DEBUG_IRSEEKING
            writeDebugStreamLine("%02d: Seek IR 2.",
                                 currState - SMSTATE_STARTED);
#endif
            PIDCtrlSetPowerLimits(g_SonarPidCtrl, -30, 30);
            PIDDriveSetTarget(g_IrDrive,
                              m_PegDist2[startPos*2 + m_PegRow][m_IrCol],
                              m_IrDir2[startPos*2 + m_PegRow][m_IrCol],
                              false,
                              &sm,
                              EVTTYPE_PIDDRIVE,
                              5000);
            SMAddWaitEvent(sm, EVTTYPE_PIDDRIVE);
            SMWaitEvents(sm, currState + 2);
            break;

#if 0
        case SMSTATE_STARTED + 3:
            //
            // Move the gripper to the correct height.
            //
#ifdef _DEBUG_IRSEEKING
            writeDebugStreamLine("%02d: Set correct height.",
                                 currState - SMSTATE_STARTED);
#endif
            if (pegHeight == 0.0)
            {
                //
                // Use default peg height table.
                //
                pegHeight = m_PegHeight[m_IrCol];
            }

            if (pegHeight != ELEVATOR_MID_PEG)
            {
                PIDMotorSetTarget(g_ElevatorMotor, pegHeight, false);
                SMAddWaitEvent(sm, EVTTYPE_PIDMOTOR);
                SMWaitEvents(sm, currState + 1);
            }
            else
            {
                SMSetState(sm, currState + 1);
            }
            break;
#endif

        case SMSTATE_STARTED + 4:
            //
            // Release the ring onto the peg.
            //
#ifdef _DEBUG_IRSEEKING
            writeDebugStreamLine("%02d: Open gripper.",
                                 currState - SMSTATE_STARTED);
#endif
            ServoSetAngle(g_GripperServo, GRIPPER_OPEN_ANGLE);
#ifdef _DEBUG_IRSEEKING
            TimerSet(g_Timer, 5000, &sm, EVTTYPE_TIMER);
#else
            TimerSet(g_Timer, 200, &sm, EVTTYPE_TIMER);
#endif
            SMAddWaitEvent(sm, EVTTYPE_TIMER);
            SMWaitEvents(sm, currState + 1);
            break;

        case SMSTATE_STARTED + 5:
            //
            // Swing wrist down.
            //
#ifdef _DEBUG_IRSEEKING
            writeDebugStreamLine("%02d: Swing wrist down.",
                                 currState - SMSTATE_STARTED);
#endif
            ServoSetAngle(g_WristServo, WRIST_DOWN_ANGLE);
            TimerSet(g_Timer, 500, &sm, EVTTYPE_TIMER);
            SMAddWaitEvent(sm, EVTTYPE_TIMER);
            SMWaitEvents(sm, currState + 1);
            break;

        case SMSTATE_STARTED + 6:
            //
            // Backup a bit.
            //
#ifdef _DEBUG_IRSEEKING
            writeDebugStreamLine("%02d: Back up.",
                                 currState - SMSTATE_STARTED);
#endif
            if (backoutDist == 0.0)
            {
                //
                // Use default backup distance table.
                //
                backoutDist = m_BackupDist[startPos][m_IrCol];
            }
            PIDMotorSetTarget(g_ElevatorMotor, ELEVATOR_LOW_PEG, false);
            PIDDriveSetTarget(g_PidDrive,
                              backoutDist,
                              0.0,
                              false,
                              &sm,
                              EVTTYPE_PIDDRIVE,
                              3000);
            SMAddWaitEvent(sm, EVTTYPE_PIDDRIVE);
            SMWaitEvents(sm, currState + 1);
            break;

        default:
#ifdef _DEBUG_IRSEEKING
            writeDebugStreamLine("%02d: Done!",
                                 currState - SMSTATE_STARTED);
#endif
            SMStop(sm);
            PlayTone(440, 50);
            break;
    }
}   //DoSeekIR
