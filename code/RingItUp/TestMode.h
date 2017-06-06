#if 0
/// Copyright (c) Titan Robotics Club. All rights reserved.
///
/// <module name="TestMode.h" />
///
/// <summary>
///     This module contains the test mode code.
/// </summary>
///
/// <remarks>
///     Environment: RobotC for Lego Mindstorms NXT.
/// </remarks>
#endif

#ifdef _ENABLE_TESTMODES

//
// Constants.
//
#define TESTMODE_COMPETITION    0
#define TESTMODE_TIMED_DRIVE    1
#define TESTMODE_DRIVE_8FT      2
#define TESTMODE_TURN_90        3
#define TESTMODE_TEST_IR        4
#define TESTMODE_SONAR_DRIVE    5
#define TESTMODE_DETECT_RING    6

//
// Local data.
//
PIDDRIVE m_SonarDrive;

TIMER  *m_TestModeTimer;
SM     *m_TestModeSM;

MENU    m_TestModeMenu;
int     m_TestMode = TESTMODE_COMPETITION;

//
// Macros.
//
#define TestModeIsCompetition() (m_TestMode == TESTMODE_COMPETITION)

/**
 *  This function initializes the Test Mode menu.
 *
 *  @param sm Specifies the state machine object to be used in test mode.
 *  @param timer Specifies the timer object to be used in test mode.
 */
void
TestModeInit(SM *sm, TIMER *timer)
{
    PIDDriveInit(m_SonarDrive,
                 g_Drive,
                 g_SonarPidCtrl,
                 g_GyroPidCtrl);

    m_TestModeTimer = timer;
    m_TestModeSM = sm;

    //
    // Initialize TestMode menu.
    //
    MenuInit(m_TestModeMenu, "Test modes:");
    MenuAddChoice(m_TestModeMenu, "Competition", (float)TESTMODE_COMPETITION);
    MenuAddChoice(m_TestModeMenu, "Timed Drive", (float)TESTMODE_TIMED_DRIVE);
    MenuAddChoice(m_TestModeMenu, "Drive 8ft", (float)TESTMODE_DRIVE_8FT);
    MenuAddChoice(m_TestModeMenu, "Turn 90", (float)TESTMODE_TURN_90);
    MenuAddChoice(m_TestModeMenu, "Test IR", (float)TESTMODE_TEST_IR);
    MenuAddChoice(m_TestModeMenu, "Sonar Drive", (float)TESTMODE_SONAR_DRIVE);
#ifdef _USE_HEAVYRING_DETECTION
    MenuAddChoice(m_TestModeMenu, "Detect Ring", (float)TESTMODE_DETECT_RING);
#endif

    m_TestMode = (int)MenuGetChoice(m_TestModeMenu);
    SMStart(*m_TestModeSM);
}   //TestModeInit

/**
 *  This function drives the robot forward for a fixed period of time.
 */
void DoTimedDrive()
{
    nxtDisplayTextLine(1, "L=%d,R=%d",
                       nMotorEncoder[leftRear], nMotorEncoder[rightRear]);
    if (SMIsReady(*m_TestModeSM))
    {
        int currState = SMGetState(*m_TestModeSM);

        switch (currState)
        {
            case SMSTATE_STARTED:
                //
                // Drive forward at 50% power for 3.5 seconds.
                //
                DriveArcade(g_Drive, 50.0, 0.0);
                TimerSet(*m_TestModeTimer, 3500, m_TestModeSM, EVTTYPE_TIMER);
                SMAddWaitEvent(*m_TestModeSM, EVTTYPE_TIMER);
                SMWaitEvents(*m_TestModeSM, currState + 1);
                break;

            default:
                DriveArcade(g_Drive, 0.0, 0.0);
                SMStop(*m_TestModeSM);
                PlayTone(440, 50);
                break;
        }
    }
}   //DoTimedDrive

/**
 *  This function drives the robot forward for 8 feet.
 */
void DoDrive8ft()
{
    PIDCtrlDisplayInfo(1, g_EncoderPidCtrl);
    PIDCtrlDisplayInfo(3, g_GyroPidCtrl);
    if (SMIsReady(*m_TestModeSM))
    {
        int currState = SMGetState(*m_TestModeSM);

    	switch (currState)
    	{
    	    case SMSTATE_STARTED:
    	        //
    	        // Drive forward for 8 ft.
    	        //
    	        PIDDriveSetTarget(g_PidDrive,
    	                          96.0, 0.0,
    	                          false,
    	                          m_TestModeSM,
    	                          EVTTYPE_PIDDRIVE);
    	        SMAddWaitEvent(*m_TestModeSM, EVTTYPE_PIDDRIVE);
    	        SMWaitEvents(*m_TestModeSM, currState + 1);
    	        break;

    	    default:
    	        SMStop(*m_TestModeSM);
    	        PlayTone(440, 50);
    	        break;
    	}
    }
}   //DoDrive8ft

/**
 *  This function turn the robot clockwise for 90 degrees.
 */
void DoTurn90()
{
    PIDCtrlDisplayInfo(1, g_EncoderPidCtrl);
    PIDCtrlDisplayInfo(3, g_GyroPidCtrl);
    if (SMIsReady(*m_TestModeSM))
    {
        int currState = SMGetState(*m_TestModeSM);

        switch (currState)
        {
            case SMSTATE_STARTED:
                //
                // Turn clockwise 90 degrees.
                //
                PIDDriveSetTarget(g_PidDrive,
                                  0.0, 90.0,
                                  false,
                                  m_TestModeSM,
                                  EVTTYPE_PIDDRIVE);
                SMAddWaitEvent(*m_TestModeSM, EVTTYPE_PIDDRIVE);
                SMWaitEvents(*m_TestModeSM, currState + 1);
                break;

            default:
                SMStop(*m_TestModeSM);
                PlayTone(440, 50);
                break;
        }
    }
}   //DoTurn90

/**
 *  This function tests the IR sensor readings.
 */
void DoTestIR()
{
    nxtDisplayTextLine(1, "Ld=%3.1f,Rd=%3.1f",
                       IRSeekerGetACDir(g_LeftIR),
                       IRSeekerGetACDir(g_RightIR));
    nxtDisplayTextLine(2, "Dir=%5.2f",
                       IRSeekerGetACDir(g_LeftIR) +
                       IRSeekerGetACDir(g_RightIR));
    nxtDisplayTextLine(3, "Ls=%03d,Rs=%03d",
                       IRSeekerGetACStrength(g_LeftIR),
                       IRSeekerGetACStrength(g_RightIR));
    nxtDisplayTextLine(4, "Strength=%d",
                       (IRSeekerGetACStrength(g_LeftIR) +
                        IRSeekerGetACStrength(g_RightIR))/2);
    if (SMIsReady(*m_TestModeSM))
    {
        int currState = SMGetState(*m_TestModeSM);

        switch (currState)
        {
            case SMSTATE_STARTED:
                //
                // Turn the wrist up 45 degrees.
                // Raise elevator to specified height.
                // Delay at least one second to allow the IR seekers to get
                // in position.
                //
                ServoSetAngle(g_WristServo, WRIST_IR_ANGLE);
                PIDMotorSetTarget(g_ElevatorMotor, ELEVATOR_MID_PEG, false);
                TimerSet(g_Timer, 1500, m_TestModeSM, EVTTYPE_TIMER);
                SMAddWaitEvent(*m_TestModeSM, EVTTYPE_TIMER);
                SMWaitEvents(*m_TestModeSM, currState + 1);
                break;

            case SMSTATE_STARTED + 1:
                PIDDriveSetTarget(g_IrDrive, 50.0, 10.0, true);
                break;

            default:
                SMStop(*m_TestModeSM);
                PlayTone(440, 50);
                break;
        }
    }
}   //DoTestIR

/**
 *  This function drives the robot using the sonar sensor.
 */
void DoSonarDrive()
{
    PIDCtrlDisplayInfo(1, g_SonarPidCtrl);
    if (SMIsReady(*m_TestModeSM))
    {
        int currState = SMGetState(*m_TestModeSM);

        switch (currState)
        {
            case SMSTATE_STARTED:
                PIDCtrlSetPowerLimits(g_SonarPidCtrl, -30, 30);
                PIDDriveSetTarget(m_SonarDrive, 30.0, 0.0);
                SMAddWaitEvent(*m_TestModeSM, EVTTYPE_PIDDRIVE);
                SMWaitEvents(*m_TestModeSM, currState + 1);
                break;

            default:
                SMStop(*m_TestModeSM);
                PlayTone(440, 50);
                break;
        }
    }
}   //DoSonarDrive

#ifdef _USE_HEAVYRING_DETECTION
/**
 *  This function detects the different ring types.
 */
void DoDetectRing()
{
    int lightValue1 = SensorGetValue(g_LightSensor1);
    int lightValue2 = SensorGetValue(g_LightSensor2);
    bool fHasHeavyRing = false;

    nxtDisplayTextLine(1, "LS1=%d,LS2=%d",
                       lightValue1, lightValue2);
    if (lightValue1 <= LS1_LOW_THRESHOLD)
    {
        // Detected heavy ring.
        nxtDisplayTextLine(2, "Ring1 is HEAVY");
        fHasHeavyRing = true;
    }
    else if (lightValue1 <= LS1_HIGH_THRESHOLD)
    {
        nxtDisplayTextLine(2, "Ring1 is LIGHT");
    }
    else
    {
        nxtDisplayTextLine(2, "Ring1 is NONE");
    }

    if (lightValue2 <= LS2_LOW_THRESHOLD)
    {
        // Detected heavy ring.
        nxtDisplayTextLine(3, "Ring2 is HEAVY");
        fHasHeavyRing = true;
    }
    else if (lightValue2 <= LS2_HIGH_THRESHOLD)
    {
        nxtDisplayTextLine(3, "Ring2 is LIGHT");
    }
    else
    {
        nxtDisplayTextLine(3, "Ring2 is NONE");
    }

    if (fHasHeavyRing)
    {
        HTSPBwriteIO(superPro, SPB_HEAVY_RING_LED);
    }
    else
    {
        HTSPBwriteIO(superPro, 0);
    }
}   //DoDetectRing
#endif

/**
 *  This function executes the test mode routine.
 */
void
TestModeExecute()
{
    switch (m_TestMode)
    {
        case TESTMODE_TIMED_DRIVE:
            DoTimedDrive();
            break;

        case TESTMODE_DRIVE_8FT:
            DoDrive8ft();
            break;

        case TESTMODE_TURN_90:
            DoTurn90();
            break;

        case TESTMODE_TEST_IR:
            DoTestIR();
            break;

        case TESTMODE_SONAR_DRIVE:
            DoSonarDrive();
            break;

#ifdef _USE_HEAVYRING_DETECTION
        case TESTMODE_DETECT_RING:
            DoDetectRing();
            break;
#endif

        default:
            TErr(("Invalid test mode %d!", m_TestMode));
            break;
    }
    TimerTask(*m_TestModeTimer);
    PIDDriveTask(m_SonarDrive);
}   //TestModeExecute

#else

//
// Macros.
//
#define TestModeIsCompetition() (true)
#define TestModeInit(s,t)
#define TestModeExecute()

#endif
