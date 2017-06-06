/**
    Correct behavior:
    Should end with the following LCD display:
    Value1=3
    Value2=3
*/

void Func1(bool test)
{
    static int persistValue1 = 0;

    if (test)
    {
        static int persistValue2 = 0;

        persistValue1++;
        persistValue2++;
        nxtDisplayTextLine(0, "Value1=%d", persistValue1);
        nxtDisplayTextLine(1, "Value2=%d", persistValue2);
    }
}

task main()
{
    Func1(true);
    wait1Msec(1000);
    Func1(true);
    wait1Msec(1000);
    Func1(true);
    wait1Msec(1000);
}
