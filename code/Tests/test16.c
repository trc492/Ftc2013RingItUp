/**
    Correct behavior:
    Should compile without error or warning.
    LCD display:
    n1=1,n2=2
    Different
    Different
*/

#define IsSameObj1(obj1,obj2)   (&obj1 == &obj2)
#define IsSameObj2(obj1,obj2)   (&(obj1) == &(obj2))

void CheckSameObj(int &obj1, int &obj2)
{
    nxtDisplayTextLine(0, "n1=%d,n2=%d", obj1, obj2);

    if (IsSameObj1(obj1, obj2))
    {
        nxtDisplayTextLine(1, "Same");
    }
    else
    {
        nxtDisplayTextLine(1, "Different");
    }

    if (IsSameObj2(obj1, obj2))
    {
        nxtDisplayTextLine(2, "Same");
    }
    else
    {
        nxtDisplayTextLine(2, "Different");
    }
}

task main()
{
    int n1 = 1;
    int n2 = 2;

    CheckSameObj(n1, n2);
}
