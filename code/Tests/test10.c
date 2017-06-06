/**
    Correct behavior:
    LCD Screen will display:
        n1=121
        n1=122
        n1=123
*/

#define Macro1(p)   Func1((p).t1, p.n1)
#define Macro2(p)   Func1((p).t1, (p).n1)

typedef struct
{
    int t1;
    int n1;
} OBJ;

OBJ g_obj;

void Func1(int t1, int &n)
{
    n = t1 + 120;
}

task main()
{
    Func1(1, g_obj.n1);
    nxtDisplayTextLine(0, "n1=%d", g_obj.n1);

    g_obj.n1 = 0;
    g_obj.t1 = 2;
    Macro1(g_obj);
    nxtDisplayTextLine(1, "n1=%d", g_obj.n1);

    g_obj.n1 = 0;
    g_obj.t1 = 3;
    Macro2(g_obj);
    nxtDisplayTextLine(2, "n1=%d", g_obj.n1);
}
