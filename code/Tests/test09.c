/**
    Correct behavior:
    LCD Screen will display:
        obj1.n1=1
        obj1.n2=2
        obj2.n3=3
*/

typedef struct
{
    int n1;
    int n2;
} OBJ1;

typedef struct
{
    OBJ1 *obj1;
    int n3;
} OBJ2;

OBJ1 g_obj1;
OBJ2 g_obj2;

void InitObj1(OBJ1 &obj1)
{
    obj1.n1 = 1;
    obj1.n2 = 2;
}

void InitObj2(OBJ2 &obj2, OBJ1 &obj1, int n)
{
    obj2.obj1 = &obj1;
    obj2.n3 = n;
}

void Func1(OBJ1 &obj1)
{
    nxtDisplayTextLine(0, "obj1.n1=%d", obj1.n1);
    nxtDisplayTextLine(1, "obj1.n2=%d", obj1.n2);
}

void Func2(OBJ2 &obj2)
{
    Func1(*obj2.obj1);
    nxtDisplayTextLine(2, "obj2.n3=%d", obj2.n3);
}

task main()
{
    InitObj1(g_obj1);
    InitObj2(g_obj2, g_obj1, 3);

    Func2(g_obj2);
}
