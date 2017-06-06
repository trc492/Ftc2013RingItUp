/**
    Correct behavior:
    LCD Screen will display:
        n=1
*/

typedef struct
{
    char     titleText[18];
    int      flags;
    int      numChoices;
    char     choiceTexts[16][18];
    int      choiceValues[16];
    int      currChoice;
    int      firstChoice;
    int      userChoice;
} MENU;

MENU g_menu;

void Func2(int n)
{
    nxtDisplayTextLine(0, "n=%d", n);
}

void Func1(MENU &menu)
{
    Func2(menu.currChoice - menu.firstChoice + 1);
}

task main()
{
    g_menu.currChoice = 0;
    g_menu.firstChoice = 0;
    Func1(g_menu);
}
