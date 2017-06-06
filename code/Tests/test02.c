/**
    Correct behavior:
    LCD Screen will display:
        x0=55
        x1=36600
        x2=143
        x3=143
*/

task main()
{
    short x = 55;

    nxtDisplayTextLine(0, "x0=%d", x);
    nxtDisplayTextLine(1, "x1=%d", ((long)x + 128)*200);
    nxtDisplayTextLine(2, "x2=%d", ((long)x + 128)*200/255);
    nxtDisplayTextLine(3, "x3=%d", (((long)x + 128)*200)/255);
}
