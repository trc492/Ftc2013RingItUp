/**
    Correct behavior:
    Code should compile without errors or warnings.
*/

#include "..\RobotCDrivers\drivers\hitechnic-irseeker-v2.h"

#define IRSeekerGetRawACData(p) (HTIRS2readAllACStrength( \
                                    (p).sensorID, \
                                    (p).acStrength[0], \
                                    (p).acStrength[1], \
                                    (p).acStrength[2], \
                                    (p).acStrength[3], \
                                    (p).acStrength[4]))
typedef struct
{
    tSensors sensorID;
    int acStrength[5];
} IRSEEKER;

IRSEEKER g_irSeeker;

task main()
{
    g_irSeeker.sensorID = (tSensors)0;
    IRSeekerGetRawACData(g_irSeeker);
}
