/**
    Correct behavior:
    LCD Screen will display:
        n1=1
        n2=2
*/

typedef struct
{
    int n1;
    int n2;
} OBJ, *POBJ;

void Func1(POBJ obj)
{
    nxtDisplayTextLine(0, "n1=%d", obj->n1);
    nxtDisplayTextLine(1, "n2=%d", obj->n2);
}

task main()
{
    OBJ obj;

    obj.n1 = 1;
    obj.n2 = 2;
    Func1(&obj);
}
