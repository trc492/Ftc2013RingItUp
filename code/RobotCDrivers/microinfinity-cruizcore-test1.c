#pragma config(Sensor, S1,     MICC,                sensorI2CCustom)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
 * $Id: microinfinity-cruizcore-test1.c 127 2012-12-05 19:32:39Z xander $
 */

/**
 * microinfinity-cruizcore.h provides an API for the MicroInfinity CruizCore XG1300L sensor.
 * This program demonstrates how to use that API.
 *
 * Changelog:
 * - 0.1: Initial release
 *
 * Credits:
 * - Big thanks to MicroInfinity for providing me with the hardware necessary to write and test this.
 *
 * License: You may use this code as you wish, provided you give credit where it's due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 3.55 beta 2 AND HIGHER.
 * Xander Soldaat (xander_at_botbench.com)
 * 29 May 2011
 * version 0.1
 */

#include "drivers/microinfinity-cruizcore.h"

task main () {
  int _x_accel = 0;
  int _y_accel = 0;
  int _z_accel = 0;

  int angleI = 0;
  float angleF = 0.0;

  int rotI = 0;
  float rotF = 0.0;

  nxtDisplayCenteredTextLine(0, "MicroInfinity");
  nxtDisplayTextLine(1, "CruizCore XG1300L");
  nxtDisplayCenteredTextLine(3, "Test 1");
  wait1Msec(2000);
  eraseDisplay();

  // There are 3 ranges the Cruizcore XG1300L can measure in
  // up to 2G
  // up to 4G
  // up to 8G
  MICCsetRange8G(MICC);

  // Make sure you always reset the sensor at the beginning of your program
  // The robot needs to be completely stationary or your heading and gyro
  // data won't be accurate.
  MICCreset(MICC);

  while(bSoundActive) EndTimeSlice();

  nxtDisplayTextLine(0, "CruizCore XG1300L");
  while (true) {
    // Read the relative heading from the sensor.
    angleI = MICCreadRelativeHeading(MICC);
    angleF = angleI / 100.0;

    // Read the rate of turn
    rotI = MICCreadTurnRate(MICC);
    rotF = rotI / 100.0;

    // Read the acceleration data from the sensor
    if (!MICCreadAccel(MICC, _x_accel, _y_accel, _z_accel)) {
      nxtDisplayTextLine(4, "ERROR!!");
      wait1Msec(2000);
      StopAllTasks();
    }

    nxtDisplayTextLine(2, "Heading: %4.2f", angleF);
    nxtDisplayTextLine(3, "RoT:     %4.2f", rotF);
    nxtDisplayTextLine(5, "X:      %5.2f", _x_accel/100.0);
    nxtDisplayTextLine(6, "Y:      %5.2f", _y_accel/100.0);
    nxtDisplayTextLine(7, "Z:      %5.2f", _z_accel/100.0);
    wait1Msec(50);
  }
}

/*
 * $Id: microinfinity-cruizcore-test1.c 127 2012-12-05 19:32:39Z xander $
 */
