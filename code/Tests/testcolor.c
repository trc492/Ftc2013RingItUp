#pragma config(Sensor, S1,     lightSensor,    sensorLightInactive)
#pragma config(Sensor, S2,     colorSensor,    sensorCOLORFULL)
#pragma config(Sensor, S3,     hiColorSensor,  sensorI2CCustom)
#pragma config(Sensor, S4,     eopdSensor,     sensorAnalogActive)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "..\RobotCDrivers\drivers\hitechnic-colour-v2.h"
#include "..\RobotCDrivers\drivers\hitechnic-eopd.h"

void LSsetActive(tSensors link)
{
  SensorType[link] = sensorLightActive;
  SensorMode[link] = modeRaw;
  wait1Msec(5);
}

void LSsetInactive(tSensors link)
{
  SensorType[link] = sensorLightInactive;
  SensorMode[link] = modeRaw;
  wait1Msec(5);
}

task main()
{
    SensorType[colorSensor] = sensorCOLORBLUE;
    LSsetInactive(lightSensor);
    HTEOPDsetLongRange(eopdSensor);
    while (true)
    {
        int color = 0;
        int R = 0;
        int G = 0;
        int B = 0;

        color = HTCS2readColor(hiColorSensor);
        HTCS2readRGB(hiColorSensor, R, G, B);

        nxtDisplayTextLine(0, "light=%d", SensorValue[lightSensor]);
        nxtDisplayTextLine(1, "color=%d", SensorValue[colorSensor]);
        nxtDisplayTextLine(2, "R=%d,G=%d", R, G);
        nxtDisplayTextLine(3, "B=%d,C=%d", B, color);
        nxtDisplayTextLine(4, "r=%d,p=%d", HTEOPDreadRaw(eopdSensor), HTEOPDreadProcessed(eopdSensor));
        wait1Msec(50);
    }
}