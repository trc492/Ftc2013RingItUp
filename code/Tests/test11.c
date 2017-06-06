/**
    Correct behavior:
    LCD Screen will display:
        x/2 <= y
*/

task main()
{
    ubyte x = 10;
    ubyte y = 20;

    if ((ubyte)(x/2) > y)
    {
        nxtDisplayTextLine(0, "x/2 > y");
    }
    else
    {
        nxtDisplayTextLine(0, "x/2 <= y");
    }
}
