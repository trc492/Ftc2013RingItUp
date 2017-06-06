#pragma config(Sensor, S2,     lightSensor,    sensorLightInactive)
#pragma config(Sensor, S3,     SuperPro,       sensorI2CCustom9V)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "..\RobotCDrivers\drivers\hitechnic-superpro.h"

#define LS_LOW_THRESHOLD        150
#define LS_HIGH_THRESHOLD       250
#define SPB_HEAVY_RING_LED      0x01

task main()
{
    SensorMode[lightSensor] = modeRaw;
    HTSPBsetupIO(SuperPro, SPB_HEAVY_RING_LED);
    while(true)
    {
        int lightValue = SensorValue[lightSensor];

        nxtDisplayTextLine(0, "LS=%d", lightValue);
        if (lightValue <= LS_LOW_THRESHOLD)
        {
            // Heavy ring.
            nxtDisplayCenteredBigTextLine(3, "HEAVY");
            HTSPBwriteIO(SuperPro, SPB_HEAVY_RING_LED);
        }
        else if (lightValue <= LS_HIGH_THRESHOLD)
        {
            // Light ring.
            nxtDisplayCenteredBigTextLine(3, "LIGHT");
            HTSPBwriteIO(SuperPro, 0);
        }
        else
        {
            // No ring.
            nxtDisplayCenteredBigTextLine(3, "NONE");
            HTSPBwriteIO(SuperPro, 0);
        }
        wait1Msec(50);
    }
}
