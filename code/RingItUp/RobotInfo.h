#if 0
/// Copyright (c) Titan Robotics Club. All rights reserved.
///
/// <module name="RobotInfo.h" />
///
/// <summary>
///     This module contains the Robot Info constants.
/// </summary>
///
/// <remarks>
///     Environment: RobotC for Lego Mindstorms NXT.
/// </remarks>
#endif

#ifndef _ROBOTINFO_H
#define _ROBOTINFO_H

//
// Drive subsystem info.
//
#define DISTANCE_PER_CLICK      0.00864
#define ENCODER_KP              2.5     //4.0, 2.5
#define ENCODER_KI              0.001   //0.0, 0.001
#define ENCODER_KD              0.01    //0.0, 0.01
#define ENCODER_TOLERANCE       1.0
#define ENCODER_SETTLING        200

#define GYRO_KP                 0.62    //1.7, 0.62
#define GYRO_KI                 0.006   //0.001, 0.006
#define GYRO_KD                 0.015   //0.0, 0.015
#define GYRO_TOLERANCE          2.0
#define GYRO_SETTLING           200

#define SONAR_KP                1.1     //1.8, 0.68, 0.8
#define SONAR_KI                0.0     //0.0, 0.001, 0.0
#define SONAR_KD                0.0     //0.0, 0.004, 0.0
#define SONAR_TOLERANCE         1.0
#define SONAR_SETTLING          200

#define IRTURN_KP               16.0    //15.0, 16.0
#define IRTURN_KI               0.005   //0.0, 0.005
#define IRTURN_KD               0.001   //0.0, 0.001
#define IRTURN_TOLERANCE        0.1
#define IRTURN_SETTLING         200

#define IRDIST_KP               5.5     //5.5 not used
#define IRDIST_KI               0.0
#define IRDIST_KD               0.0
#define IRDIST_TOLERANCE        0.5
#define IRDIST_SETTLING         200

//
// Line follower subsystem info.
//
#define LS_LEFT_THRESHOLD       282
#define LS_CENTER_THRESHOLD     284
#define LS_RIGHT_THRESHOLD      245

#define LS_CENTER               3

#define LNFOLLOW_KP             8.0
#define LNFOLLOW_KI             0.0
#define LNFOLLOW_KD             0.0
#define LNFOLLOW_TOLERANCE      1.0
#define LNFOLLOW_SETTLING       200

//
// Elevator subsystem info.
//
#define ELEVATOR_KP             40.0
#define ELEVATOR_KI             0.0
#define ELEVATOR_KD             0.0
#define ELEVATOR_TOLERANCE      1.0
#define ELEVATOR_SETTLING       200

#define HEIGHT_PER_CLICK        0.00278 //4.0/1440 (inches/click)
#define ELEVATOR_SPEED_SCALE    0.5     //50.0/100
#define ELEVATOR_CAL_POWER      -30     //30% power going downward

#define ELEVATOR_STALL_MINPOWER 30
#define ELEVATOR_STALL_TIMEOUT  100     //in msec

#define ELEVATOR_LOWER_LIMIT    14.0    //inches
#define ELEVATOR_UPPER_LIMIT    40.0    //inches
#define ELEVATOR_LOW_PEG        14.0    //inches (23.8)
#define ELEVATOR_MID_PEG        26.0    //inches
#define ELEVATOR_TOP_PEG        40.0    //inches

//
// Ramp subsystem info.
//
#define RAMP_KP                 0.05
#define RAMP_KI                 0.0
#define RAMP_KD                 0.0
#define RAMP_TOLERANCE          1.0
#define RAMP_SETTLING           200
#define RAMP_STALL_MINPOWER     30
#define RAMP_STALL_TIMEOUT      100     //in msec

#define RAMP_POWER_LIMIT        50
#define RAMP_LOWER_LIMIT        0.0     //encoder clicks
#define RAMP_UPPER_LIMIT        1300.0  //encoder clicks
#define RAMP_LATCH_ANGLE        0.0     //degrees
#define RAMP_UNLATCH_ANGLE      115.0   //degrees

//
// Wrist subsystem info.
//
#define WRIST_LOWER_LIMIT       0.0     //degrees
#define WRIST_UPPER_LIMIT       200.0   //degrees
#define WRIST_DOWN_ANGLE        20.0    //degrees
#define WRIST_UP_ANGLE          200.0   //degrees
#define WRIST_INIT_ANGLE        WRIST_DOWN_ANGLE
#define WRIST_IR_ANGLE          180.0   //degrees
#define WRIST_DOWN_STEP_RATE    120.0   //degrees/sec
#define WRIST_UP_STEP_RATE      120.0   //degrees/sec

//
// Gripper subsystem info.
//
#define GRIPPER_CLOSE_ANGLE     100.0   //degrees
#define GRIPPER_OPEN_ANGLE      0.0     //degrees
#define GRIPPER_INIT_ANGLE      GRIPPER_OPEN_ANGLE

#ifdef _USE_HEAVYRING_DETECTION
//
// Weighted ring detection subsystem info.
//
#define LS1_LOW_THRESHOLD       120
#define LS1_HIGH_THRESHOLD      180
#define LS2_LOW_THRESHOLD       120
#define LS2_HIGH_THRESHOLD      180
#define SPB_HEAVY_RING_LED      Bit(0)
#endif

#endif  //ifndef _ROBOTINFO_H
