/**
    Correct behavior:
    Should give compiler error on comparing a float with a pointer.
*/

float g_LeftThreshold[2] = {11.5, 12.25};
float g_RightThreshold[2] = {8.5, 11.0};

int Func1(float value)
{
    return (value > g_LeftThreshold)? 0:
           (value < g_RightThreshold)? 2: 1;
}

task main()
{
    float value;
    int col;

    value = 10.0;
    col = Func1(value);
    nxtDisplayTextLine(0, "[%4.1f] = %d", value, col);

    value = 14.0;
    col = Func1(value);
    nxtDisplayTextLine(1, "[%4.1f] = %d", value, col);
}
