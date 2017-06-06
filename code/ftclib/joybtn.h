#if 0
/// Copyright (c) Titan Robotics Club. All rights reserved.
///
/// <module name="joybtn.h" />
///
/// <summary>
///     This module contains the library functions for the joystick buttons.
/// </summary>
///
/// <remarks>
///     Environment: RobotC for Lego Mindstorms NXT.
/// </remarks>
#endif

#ifndef _JOYBTN_H
#define _JOYBTN_H

#pragma systemFile

#ifdef MOD_ID
    #undef MOD_ID
#endif
#define MOD_ID                  MOD_JOYBTN

//
// Constants.
//
#ifndef NUM_BTNS
  #define NUM_BTNS              12
#endif

#define JOYEVENT_BUTTON         0
#define JOYEVENT_TOPHAT         1

#define Logitech_Btn1           Bit(0)
#define Logitech_Btn2           Bit(1)
#define Logitech_Btn3           Bit(2)
#define Logitech_Btn4           Bit(3)
#define Logitech_LB5            Bit(4)
#define Logitech_RB6            Bit(5)
#define Logitech_LB7            Bit(6)
#define Logitech_RB8            Bit(7)
#define Logitech_Btn9           Bit(8)
#define Logitech_Btn10          Bit(9)
#define Logitech_LStick         Bit(10)
#define Logitech_RStick         Bit(11)

#define Xbox_A                  Bit(0)
#define Xbox_B                  Bit(1)
#define Xbox_X                  Bit(2)
#define Xbox_Y                  Bit(3)
#define Xbox_LB                 Bit(4)
#define Xbox_RB                 Bit(5)
#define Xbox_Back               Bit(6)
#define Xbox_Start              Bit(7)
#define Xbox_LStick             Bit(8)
#define Xbox_RStick             Bit(9)

#define TopHat_Released         -1
#define TopHat_Up               0
#define TopHat_UpRight          1
#define TopHat_Right            2
#define TopHat_DownRight        3
#define TopHat_Down             4
#define TopHat_DownLeft         5
#define TopHat_Left             6
#define TopHat_UpLeft           7

//
// Type definitions.
//
typedef struct
{
    int  joystickID;
    int  prevButtons;
    int  prevTopHat;
} JOYBTN;

//
// Callback function prototypes.
//

/**
 *  This callback function is called when a joystick button is pressed or
 *  released.
 *
 *  @param joystickID Specifies the joystick ID.
 *  @param eventType Specifies the joystick event type:
 *         JOYEVENT_BUTTON - Specifies a button event
 *         JOYEVENT_TOPHAT - Specifies a tophat event
 *  @param eventID Specifies the button or tophat event ID.
 *  @param fPressed TRUE if a pressed event, otherwise it is a released event.
 */
void
JoyBtnEvent(
    int  joystickID,
    int  eventType,
    int  eventID,
    bool fPressed
    );

/**
 *  This function initializes the joystick button system.
 *
 *  @param joybtn Points to the JOYBTN structure.
 *  @param joystickID Specifies the joystick ID.
 */
void
JoyBtnInit(
    JOYBTN &joybtn,
    int joystickID
    )
{
    TFuncName("JoyBtnInit");
    TLevel(INIT);
    TEnter();

    joybtn.joystickID = joystickID;
#if defined(_Target_Robot_)
    joybtn.prevButtons = (joystickID == 1)? joystick.joy1_Buttons:
                                            joystick.joy2_Buttons;
    joybtn.prevTopHat = (joystickID == 1)? joystick.joy1_TopHat:
                                           joystick.joy2_TopHat;
#else
    joybtn.prevButtons = joystick.joy1_Buttons;
    joybtn.prevTopHat = joystick.joy1_TopHat;
#endif

    TExit();
    return;
}   //JoyBtnInit

/**
 *  This function processes the changed buttons and sends button event
 *  notifications.
 *
 *  @param joybtn Points to the JOYBTN structure.
 */
void
JoyBtnTask(
    JOYBTN &joybtn
    )
{
#if defined(_Target_Robot_)
    int currButtons = (joybtn.joystickID == 1)? joystick.joy1_Buttons:
                                                joystick.joy2_Buttons;
    int currTopHat = (joybtn.joystickID == 1)? joystick.joy1_TopHat:
                                               joystick.joy2_TopHat;
#else
    int currButtons = joystick.joy1_Buttons;
    int currTopHat = joystick.joy1_TopHat;
#endif

    TFuncName("JoyBtnTask");
    TLevel(TASK);
    TEnterMsg(("Prev=%x,Curr=%x", joybtn.prevButtons, currButtons));

    //
    // If callback is not enabled, there is nothing to do except for
    // updating the button values.
    //
#if defined(_Target_Robot_)
    if (!bDisconnected)
#endif
    {
        int changedButtons = currButtons^joybtn.prevButtons;
        int buttonMask;

        while (changedButtons != 0)
        {
            //
            // maskButton contains the least significant set bit.
            //
            buttonMask = changedButtons & ~(changedButtons^-changedButtons);
            if ((currButtons & buttonMask) != 0)
            {
                //
                // Button is pressed.
                //
                JoyBtnEvent(joybtn.joystickID,
                            JOYEVENT_BUTTON,
                            buttonMask,
                            true);
            }
            else
            {
                //
                // Button is released.
                //
                JoyBtnEvent(joybtn.joystickID,
                            JOYEVENT_BUTTON,
                            buttonMask,
                            false);
            }
            changedButtons &= ~buttonMask;
        }

        if (currTopHat != joybtn.prevTopHat)
        {
            JoyBtnEvent(joybtn.joystickID,
                        JOYEVENT_TOPHAT,
                        currTopHat,
                        currTopHat != -1);
        }
    }

    joybtn.prevButtons = currButtons;
    joybtn.prevTopHat = currTopHat;

    TExit();
    return;
}   //JoyBtnTask

#endif  //ifndef _JOYBTN_H
