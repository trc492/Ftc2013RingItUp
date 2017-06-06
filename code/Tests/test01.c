/**
    Correct behavior:
    LCD Screen will display:
        n1=0
        n2=1
*/

task main()
{
    int n1, n2;

    n1 = 1;
    n2 = 1;
    n1 &= ~n2;
    nxtDisplayTextLine(0, "n1=%x", n1);
    nxtDisplayTextLine(1, "n2=%x", n2);
}
