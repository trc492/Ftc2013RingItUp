/**
    Correct behavior:
    LCD Screen will display:
        pi=00000000
*/

typedef struct
{
    int *pi;
} OBJ;

OBJ g_obj;

void ObjInit(OBJ *obj, int *pi)
{
    obj->pi = pi;
}

task main()
{
    ObjInit(&g_obj, NULL);
    nxtDisplayTextLine(0, "pi=%p", g_obj.pi);
}
