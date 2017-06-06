/**
    According to nxtFileIODefs.h:
    kNoButton = -1
    kExitButton = 0
    kRightButton = 1
    kLeftButton = 2
    kEnterButton = 3

    But in Virtual World:
    kNoButton = -1
    kLeftButton = 1
    kRightButton = 0
    kEnterButton and kExitButton has no effect
*/

task main()
{
    while (true)
    {
        nxtDisplayTextLine(0, "Btn=%x", nNxtButtonPressed);
        wait1Msec(100);
    }
}
