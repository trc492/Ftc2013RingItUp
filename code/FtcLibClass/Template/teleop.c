#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     compassSensor,  sensorVirtualCompass)
#pragma config(Sensor, S3,     lightSensor,    sensorLightActive)
#pragma config(Sensor, S4,     sonarSensor,    sensorSONAR)
#pragma config(Motor,  motorA,          gripperMotor,  tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          wristMotor,    tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     rightMotor,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     leftMotor,     tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     armMotor,      tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     motorG,        tmotorTetrix, openLoop, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#if 0
/// Copyright (c) Titan Robotics Club. All rights reserved.
///
/// <module name="TeleOp.c" />
///
/// <summary>
///     This module contains the entry point for teleoperator mode.
///     This is template file and should not be modified. The main competition
///     code should live in TeleOpMain.h.
/// </summary>
///
/// <remarks>
///     Environment: RobotC for Lego Mindstorms NXT.
/// </remarks>
#endif

//#define _COMPETITION_ENABLED
#define _ARCADE_DRIVE

#include "JoystickDriver.c"
#include "..\ftclib\trcdefs.h"
#include "..\ftclib\dbgtrace.h"
#include "TeleOpMain.h"

/**
 *  This task is the program entry point.
 */
task main()
{
    long nextPeriod;

    TraceInit(TRACE_MODULES, TRACE_LEVEL, MSG_LEVEL);

    //
    // The RobotInit function is provided by student code in TeleOpMain.h.
    //
    RobotInit();

    //
    // Wait for the beginning of teleop mode.
    //
#ifdef _COMPETITION_ENABLED
    waitForStart();
#endif

    nextPeriod = nPgmTime;
    while (true)
    {
        HiFreqTasks();
        if (nPgmTime >= nextPeriod)
        {
            nextPeriod += LOOP_PERIOD;
            getJoystickSettings(joystick);
            //
            // The following functions are provided by student code in
            // TeleMain.h.
            //
            InputTasks();
            MainTasks();
            OutputTasks();
        }
        EndTimeSlice();
    }
}   //main
