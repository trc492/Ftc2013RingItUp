/**
    Correct behavior:
    Should compile without warning or error.
    LCD should display:
        Choice=Choice1
*/

typedef struct
{
    char choiceText[2][16];
    int choice;
} MENU;

#define GetChoiceText(m)    (&(m).choiceText[(m).choice][0])

MENU g_Menu;

task main()
{
    strcpy(&g_Menu.choiceText[0][0], "Choice0");
    strcpy(&g_Menu.choiceText[1][0], "Choice1");
    g_Menu.choice = 1;
    nxtDisplayTextLine(0, "Choice=%s",
                       (g_Menu.choice != -1)?
                            &g_Menu.choiceText[g_Menu.choice][0]:
                            "Invalid");
    while (true) {}
}
