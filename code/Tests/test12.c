/**
    Correct behavior:
    LCD Screen will display:
        obj1[1, 2]
        obj2[3, 4]
*/

typedef struct
{
    int n1;
    int n2;
} OBJ;

OBJ g_obj1;
OBJ g_obj2;

void Func1(OBJ &obj)
{
    if (&obj == &g_obj1)
    {
        nxtDisplayTextLine(0, "obj1[%d, %d]", obj.n1, obj.n2);
    }
    else if (&obj == &g_obj2)
    {
        nxtDisplayTextLine(1, "obj2[%d, %d]", obj.n1, obj.n2);
    }
    else
    {
        nxtDisplayTextLine(2, "Pointer mismatch!");
    }
}

task main()
{
    g_obj1.n1 = 1;
    g_obj1.n2 = 2;
    g_obj2.n1 = 3;
    g_obj2.n2 = 4;
    Func1(g_obj1);
    Func1(g_obj2);
}
