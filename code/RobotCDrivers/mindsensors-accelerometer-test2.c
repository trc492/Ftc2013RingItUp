#pragma config(Sensor, S1,     MSAC,                sensorI2CCustom)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
 * $Id: mindsensors-accelerometer-test2.c 127 2012-12-05 19:32:39Z xander $
 */

/**
 * mindsensors-accelerometer.h provides an API for the Mindsensors Acceleration Sensor.  This program
 * demonstrates how to use that API.
 *
 * Changelog:
 * - 0.1: Initial release
 * - 0.2: More comments<br>
 *        Make use of the new range defines
 *
 * Credits:
 * - Big thanks to Mindsensors for providing me with the hardware necessary to write and test this.
 *
 * License: You may use this code as you wish, provided you give credit where it's due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 3.55 beta 2 AND HIGHER.
 * Xander Soldaat (xander_at_botbench.com)
 * 28 November 2009
 * version 0.2
 */

#include "drivers/mindsensors-accelerometer.h"

task main () {
  int _x_tilt = 0;
  int _y_tilt = 0;
  int _z_tilt = 0;

  int tone1;
  int tone2;
  int waitTime;

  nxtDisplayCenteredTextLine(0, "Mindsensors");
  nxtDisplayCenteredBigTextLine(1, "ACCEL-Nx");
  nxtDisplayCenteredTextLine(3, "Test 2");
  wait1Msec(2000);

  // There are four ranges the ACCL-Nx can measure in
  // up to 2.5G - MSAC_RANGE_2_5
  // up to 3.3G - MSAC_RANGE_3_3
  // up to 6.7G - MSAC_RANGE_6_7
  // up to 10G  - MSAC_RANGE_10
  MSACsetRange(MSAC, MSAC_RANGE_10);

  PlaySound(soundBeepBeep);
  while(bSoundActive) EndTimeSlice();

  while (true) {
    // Read the tilt data from the sensor
    if (!MSACreadTilt(MSAC, _x_tilt, _y_tilt, _z_tilt)) {
      nxtDisplayTextLine(4, "ERROR!!");
      wait1Msec(2000);
      StopAllTasks();
    }

    // Tilt values seem to go from about -20 to +20.
    // Adding 20 to them makes them go from 0 to 40.

    // Make sure the tones are at least 0Hz
    tone1 = max2(0, (_x_tilt + 20) * 20);
    tone2 = max2(0, (_z_tilt + 20) * 25);

    // Make sure the wait time is at least 10ms
    waitTime = max2(10, (_y_tilt + 20));

    PlayImmediateTone(tone1, 5);
    wait1Msec(waitTime);
    PlayImmediateTone(tone2, 1);
  }
}

/*
 * $Id: mindsensors-accelerometer-test2.c 127 2012-12-05 19:32:39Z xander $
 */
