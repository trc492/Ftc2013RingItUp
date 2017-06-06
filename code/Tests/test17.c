/**
    Correct behavior:
    Should compile without warning or error.
    LCD should display:
        Level1:Hello
*/

void PrintMsg(int level, char *msg)
{
    char *prefix;

    switch (level)
    {
        case 0:
            prefix = "Level0";
            break;

        case 1:
            prefix = "Level1";
            break;

        default:
            prefix = "Unknown";
            break;
    }

    nxtDisplayTextLine(0, "%s:%s", prefix, msg);
}

task main()
{
    PrintMsg(1, "Hello");
}
