/**
    Correct behavior:
    LCD Screen will display:
        n0=1
        n1=1
        n2=4
        n3=17
        n4=2
*/

task main()
{
    nxtDisplayTextLine(0, "n0=%d", 0 - 0 + 1);
    nxtDisplayTextLine(1, "n1=%d", 0 - 3 + 4);
    nxtDisplayTextLine(2, "n2=%d", 3 - 4 + 5);
    nxtDisplayTextLine(3, "n3=%d", 3 * 4 + 5);
    nxtDisplayTextLine(4, "n4=%d", 3 * 4 / 5);
}
