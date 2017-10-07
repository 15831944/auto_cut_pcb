#ifndef GLOBAL_H
#define GLOBAL_H

/******************°å¿¨¶¨Òå************************/
#define SV_CARD 0
#define SG_CARD 1
/******************  parameters of motor ************************/

#define SERVO_MOTOR 0
#define STEP_MOTOR  1
#define PULSE_MODE  1  //+-pulse
#define DIR_MODE    2  //pulse+dir
/********************Type of homing******************************/
#define HOME_TWOLIMITS 0
#define HOME_ONELIMIT 1
#define HOME_ZERO 2
#define HOME_ZERO_INDEX 3
/********************Type of homing******************************/
#define EXC_TAB 0
#define LABAL_TAB 1
#define TECH_TAB 2
#define EDIT_TAB 3
#define SYS_TAB 4

#define RANGE_TAB 0
#define LIGHT_TAB 1
#define PARA_TAB 2
#define NEXT_TAB 3

#define MAX_POINT 128
/********************information of axis information*************/
#define  AXIS_STS_MOTION_FINISH 0x01
#define  AXIS_STS_MOTION_FINISH_X 0x01
#define  AXIS_STS_MOTION_FINISH_Y 0x1001
#define  AXIS_STS_DRIVER_ALARM 0x02
#define  AXIS_STS_BREAK_REACH 0x04
#define  AXIS_STS_INDEX_CAPTURED 0x08
#define  AXIS_STS_MOTION_WRONG 0x10
#define  AXIS_STS_POSITIVE_LIMIT 0x20
#define  AXIS_STS_POSITIVE_LIMIT_X 0x20
#define  AXIS_STS_POSITIVE_LIMIT_Y 0x1020
#define  AXIS_STS_NEGATIVE_LIMIT 0x40
#define  AXIS_STS_NEGATIVE_LIMIT_X 0x40
#define  AXIS_STS_NEGATIVE_LIMIT_Y 0x40

#define  AXIS_STS_COMMAND_WRONG  0x80

/********************define axis No.******************************/
#define AXIS_1 0
#define AXIS_2 1
#define AXIS_3 2
#define AXIS_4 3
#define AXIS_5 4
#define AXIS_6 5
#define AXIS_7 6
#define AXIS_8 7
/*********************define of data type ************************/
#define PROFILE_VALUE 0
#define ACTUAL_VALUE 1	

/********************define direction******************************/
#define POSITIVE_DIRCT 1
#define NEGTIVE_DIRCT  -1
/********************define View Plane******************************/
#define XY_PLANE 0
#define XZ_PLANE 1
#define YZ_PLANE 2

#endif
