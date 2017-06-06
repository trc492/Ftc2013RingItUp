#pragma config(Sensor, S1,     HTCS2,               sensorI2CCustom)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
 * $Id: hitechnic-colour-v2-conf-sa.c 127 2012-12-05 19:32:39Z xander $
 */

/**
 * This program allows you to configure the operating frequency of the sensor to either
 * 50 or 60Hz. This reduces the amount of interference from the mains frequency
 * affecting the lights in the room.
 *
 * Changelog:
 * - 0.1: Initial release
 * - 0.2: More comments
 *
 * Credits:
 * - Big thanks to HiTechnic for providing me with the hardware necessary to write and test this.
 *
 * License: You may use this code as you wish, provided you give credit where it's due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 3.55 beta 2 AND HIGHER.
 * Xander Soldaat (xander_at_botbench.com)
 * 25 November 2009
 * version 0.2
 */


// Set the sensor to the selected frequency
void setHz(tSensors link, byte hertz) {
  byte msg[4] = { 3, 0x02, 0x41, 0x00 };

  if (hertz == 50) {
    msg[3] = 0x35;
  } else if (hertz == 60) {
    msg[3] = 0x36;
  } else {
    PlaySound(soundException);
    eraseDisplay();
    nxtDisplayCenteredTextLine(3, "Wrong freq.");
    nxtDisplayCenteredTextLine(3, "specified");
    wait1Msec(5000);
    StopAllTasks();
  }

  while (nI2CStatus[link] == STAT_COMM_PENDING){
    wait1Msec(5);
  }

  sendI2CMsg(link, &msg[0], 0);

  if (nI2CStatus[link] == ERR_COMM_BUS_ERR) {
    PlaySound(soundException);
    eraseDisplay();
    nxtDisplayCenteredTextLine(3, "Error setting");
    nxtDisplayCenteredTextLine(3, "sensor to 50Hz");
    wait1Msec(5000);
    StopAllTasks();
  }
}


task main () {
  bool selected_50hz = true;

  nxtDisplayCenteredTextLine(0, "HiTechnic");
  nxtDisplayCenteredBigTextLine(1, "Color V2");
  nxtDisplayCenteredTextLine(4, "Config operating");
  nxtDisplayCenteredTextLine(5, "frequency to");
  nxtDisplayCenteredTextLine(6, "50 or 60 Hz");
  wait1Msec(2000);

  eraseDisplay();
  nxtDisplayCenteredTextLine(0, "Use arrow keys");
  nxtDisplayCenteredTextLine(1, "to select a");
  nxtDisplayCenteredTextLine(2, "frequency");
  nxtDisplayCenteredBigTextLine(4, "50 60");
  nxtDisplayCenteredTextLine(6, "[enter] to set");
  nxtDisplayCenteredTextLine(7, "[exit] to cancel");

  nxtDrawRect(19, 34, 44, 16);

  while (true) {
    while (nNxtButtonPressed == kNoButton) {
      wait1Msec(1);
    }

    switch (nNxtButtonPressed) {
      // if the left button is pressed, set the sensor for 50Hz
      case kLeftButton:
            if (selected_50hz) {
              PlaySound(soundBlip);
              while(bSoundActive) {wait1Msec(1);}
            } else {
              selected_50hz = true;
              nxtEraseRect(55, 34, 80, 16);
              nxtDisplayCenteredBigTextLine(4, "50 60");
              nxtDrawRect(19, 34, 44, 16);
            }
            break;
       // if the right button is pressed, set the sensor for 60Hz
       case kRightButton:
						if (!selected_50hz) {
						  PlaySound(soundBlip);
						  while(bSoundActive) {wait1Msec(1);}
						} else {
						  selected_50hz = false;
						  nxtEraseRect(19, 34, 44, 16);
						  nxtDisplayCenteredBigTextLine(4, "50 60");
						  nxtDrawRect(55, 34, 80, 16);
						}
            break;
        // Make the setting permanent by saving it to the sensor
        case kEnterButton:
            eraseDisplay();

            if (selected_50hz)
              setHz(HTCS2, 50);
            else
              setHz(HTCS2, 60);

            nxtDisplayCenteredTextLine(2, "The Sensor is");
            nxtDisplayCenteredTextLine(3, "configured for");
            if (selected_50hz)
              nxtDisplayCenteredTextLine(4, "50 Hz operating");
            else
              nxtDisplayCenteredTextLine(4, "60 Hz operating");
            nxtDisplayCenteredTextLine(5, "frequency");
            for (int i = 5; i > 0; i--) {
              nxtDisplayCenteredTextLine(7, "Exiting in %d sec", i);
              wait1Msec(1000);
            }
            StopAllTasks();
            break;
    }

    // Debounce the button
    while (nNxtButtonPressed != kNoButton) {
      wait1Msec(1);
    }

  }
}

/*
 * $Id: hitechnic-colour-v2-conf-sa.c 127 2012-12-05 19:32:39Z xander $
 */
