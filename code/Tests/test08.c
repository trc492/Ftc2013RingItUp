/**
    Correct behavior:
    LCD Screen will display:
        n1=1
        n2=2
*/

#define GetData(p)  (GetRawData( \
                     &((p)->n1), \
                     &((p)->n2)))

typedef struct
{
    int n1;
    int n2;
} OBJ;

OBJ g_obj;

void GetRawData(int *p1, int *p2)
{
    *p1 = 1;
    *p2 = 2;
}

task main()
{
    GetRawData(&g_obj.n1, &g_obj.n2);
    GetData(&g_obj);
    nxtDisplayTextLine(0, "n1=%d", g_obj.n1);
    nxtDisplayTextLine(1, "n2=%d", g_obj.n2);
}
