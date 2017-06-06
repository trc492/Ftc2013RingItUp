#pragma config(Sensor, S1,     rightIR,         sensorI2CCustom)
#pragma config(Sensor, S2,     leftIR,        sensorI2CCustom)
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

#include "..\ftclib\trcdefs.h"
#include "..\ftclib\dbgtrace.h"
#include "..\ftclib\irseeker.h"

IRSEEKER g_LeftIR;
IRSEEKER g_RightIR;

/**
 *  This task is the program entry point.
 */
task main()
{
    IRSeekerInit(g_LeftIR, leftIR);
    IRSeekerInit(g_RightIR, rightIR);
    while (true)
    {
        float leftDir = IRSeekerGetACDir(g_LeftIR);
        float rightDir = IRSeekerGetACDir(g_RightIR);
        int leftIdx = (int)(leftDir + 1)/2 - 1;
        int rightIdx = (int)(rightDir + 1)/2 - 1;
        int leftStrength = (leftIdx < 0)? 0:
                           ((int)leftDir%2 == 1)?
                           g_LeftIR.acStrength[leftIdx]:
                           (g_LeftIR.acStrength[leftIdx] +
                            g_LeftIR.acStrength[leftIdx + 1])/2;
        int rightStrength = (rightIdx < 0)? 0:
                            ((int)rightDir%2 == 1)?
                            g_RightIR.acStrength[rightIdx]:
                            (g_RightIR.acStrength[rightIdx] +
                             g_RightIR.acStrength[rightIdx + 1])/2;
        float strength = (leftStrength == 0)?
                         (float)rightStrength:
                         (float)(leftStrength + rightStrength)/2.0;

        nxtDisplayTextLine(0, "L=%3.2f [%d]", leftDir, leftStrength);
        nxtDisplayTextLine(1, "R=%3.2f [%d]", rightDir, rightStrength);
        nxtDisplayTextLine(2, "S=%5.2f", strength);

        int enhLeftDir, enhLeftStrength;
        int enhRightDir, enhRightStrength;
        HTIRS2readEnhanced(leftIR, enhLeftDir, enhLeftStrength);
        HTIRS2readEnhanced(rightIR, enhRightDir, enhRightStrength);
        nxtDisplayTextLine(3, "leDir=%d,leStr=%d", enhLeftDir, enhLeftStrength);
        nxtDisplayTextLine(4, "reDir=%d,reStr=%d", enhRightDir, enhRightStrength);

        wait1Msec(100);
    }
}   //main
