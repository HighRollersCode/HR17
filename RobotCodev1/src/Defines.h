/*
 * Defines.h
 *
 *  Created on: Jan 9, 2017
 *      Author: 987
 */

#ifndef SRC_DEFINES_H_
#define SRC_DEFINES_H_

#define USING_GAMEPAD 0

#define USING_MXP 0
#define USING_GYRO 1


#define Vic_Drive_Right_1 0
#define Vic_Drive_Right_2 1

#define Vic_Drive_Left_1 2
#define Vic_Drive_Left_2 3

#define Tal_Intake 2

#define Tal_Turret 5

#define Tal_Shooter_Wheel 9

#define Tal_Hopper 6
#define Tal_Hopper_Intake 4
#define Tal_Conveyor_Belt 1

#define Tal_Climber 3

#define Tal_Gear_Intake 7

#define PDP_Climber 1


#define Sol_Shifter_In 0 //On Solenoid block tubes plug into port 1
#define Sol_Shifter_Out 1 //On Solenoid block tubes plug into port 6

#define Sol_Hood_Up 2 //On Solenoid block tubes plug into port 2
#define Sol_Hood_Down 3 //On Solenoid block tubes plug into port 5

#define Sol_Gear_Down 4 //On Solenoid block tubes plug into port 3
#define Sol_Gear_Up 5 //On Solenoid block tubes plug into port 4


#define Encoder_Drive_Left_1 0
#define Encoder_Drive_Left_2 1
#define Encoder_Drive_Right_1 2
#define Encoder_Drive_Right_2 3

#define Encoder_Arm_Turret_1 4
#define Encoder_Arm_Turret_2 5

#define Encoder_Shooter_Wheel_1 8
#define Encoder_Shooter_Wheel_2 9

#define Encoder_Intake_Lift_1 6
#define Encoder_Intake_Lift_2 7


#define Preset_Intake_Floor -500
#define Preset_Intake_Defense -10
#define Preset_Intake_Intake -435//-448
#define Preset_Intake_Down -500     // value to use when reseting the intake encoder in-match
#define Preset_Intake_Tower_Shot -195
#define INTAKE_LIFT_MIN -500
#define INTAKE_LIFT_MAX 10

/*#define Preset_Arm_Floor -5850
#define Preset_Arm_Defense -795
#define Preset_Arm_Intake -5760
#define Preset_Arm_Back_Shot -2826
#define Preset_Arm_Far_Shot -2826
#define Preset_Arm_Tower_Shot -1275
#define Preset_Arm_Safe_Zone -5190
#define Preset_Arm_Down -5700		// value to use when resetting the arm lift encoder in-match
*/

#define Preset_Tower_Shot_Shooter_Wheels -0.45f

#define Intake_Lift_Limit_Switch 10
#define TURRET_CENTER_LIMIT_SWITCH 11

//
//  Encoder values to limit the arm movement to keep the arm within the 15" envelope around the robot
//

// The turret is zero'd when it is centered pointing forward.
#define TURRET_DEGREES_PER_TICK .1f
#define TURRET_MIN_ENCODER	-2000  //(-1650 * (60.0f/24.0f))   // we need a shot to the left in the spybot two-ball at -1500
#define TURRET_MAX_ENCODER	2000

#define TURRET_CORRECTION_P 0.001f   // motor power per tick to correct invalid turret configurations



#endif /* SRC_DEFINES_H_ */
