/**
    Correct behavior:
    Should compile without error or warning.
*/

void Func1(char *buffer)
{
    buffer[0] = '\0';
}

task main()
{
    char buffer[10];

    Func1(buffer);
}
