/*
 * Defines.h
 *
 *  Created on: Jan 9, 2017
 *      Author: 987
 */

#include "WPILib.h"

#ifndef SRC_DEFINES_H_
#define SRC_DEFINES_H_


struct Vector2
{
	float x = 0;
	float y = 0;
};

#define USING_GAMEPAD 0

#define USING_MXP 0
#define USING_GYRO 1

//LeftStick controls

#define LEFT_MOTOR_CMD leftStick->GetY()
#define SHIFTER_UPDATE leftStick->GetTrigger()

#define GEAR_DOWN_OUTAKE leftStick->GetRawButton(2)
#define GEAR_INTAKE leftStick->GetRawButton(3)

#define GEAR_UP leftStick->GetRawButton(5)
#define GEAR_DOWN_INTAKE leftStick->GetRawButton(4)

#define CLIMBER_UP leftStick->GetRawButton(6)
#define CLIMBER_SLOW leftStick->GetRawButton(7)

#define GEAR_RESET leftStick->GetRawButton(10)

//RightStick controls

#define RIGHT_MOTOR_CMD rightStick->GetY()

#define INTAKE_IN (rightStick->GetTrigger() || rightStick->GetRawButton(10))
#define INTAKE_OUT rightStick->GetRawButton(2)

#define SET_CAM_BACK rightStick->GetRawButton(8)
#define SET_CAM_FRONT rightStick->GetRawButton(9)

//TurretStick controls

#define TURRET_MOTOR_CMD turretStick->GetX()

#define OVERRIDE_RPM_CMD ((((turretStick->GetZ() + 1.0f)*.5f)* 2500.0f)+2700.0f)

#define HOPPER_UPTAKE turretStick->GetTrigger()

#define TRACKING_ENABLE turretStick->GetRawButton(2)

#define SHOOTER_ENABLE_MTR turretStick->GetRawButton(11)

#define CALIBRATEJETSON turretStick->GetRawButton(7)

#define SHUTDOWN_JETSON turretStick->GetRawButton(8) && turretStick->GetRawButton(9)

#define SHOOTER_ENABLE_OVERRIDE turretStick->GetRawButton(10)

#define HOPPER_OUTAKE turretStick->GetRawButton(6)

//Ports for the Victor Drivetrain

#define Vic_Drive_Left_1 0
#define Vic_Drive_Left_2 1

#define Vic_Drive_Right_1 2
#define Vic_Drive_Right_2 3

#define Vic_Turret 4

#define Vic_Shaker 5

//Device ID for Talon Motors
#define Tal_Intake 3

#define Tal_Turret 7

#define Tal_Shooter_Wheel 21
#define Tal_Shooter_Wheel_2 2

#define Tal_Hopper 9

#define Tal_Climber 6
#define Tal_Climber_2 4

#define Tal_Gear_Intake 11
#define Tal_Gear_Lift 5

//PDP Port
#define PDP_Climber 1

#define PDP_Intake 9

//Gyro
#define CAN_PIGEON 10

//Solenoid Ports

#define Sol_Shifter_In 1
#define Sol_Shifter_Out 0

//DIO ports for Encoders
#define Encoder_Drive_Left_1 0
#define Encoder_Drive_Left_2 1
#define Encoder_Drive_Right_1 2
#define Encoder_Drive_Right_2 3

#define Encoder_Arm_Turret_1 4
#define Encoder_Arm_Turret_2 5


//
//  Encoder values to limit the arm movement to keep the arm within the 15" envelope around the robot
//

// The turret is zero'd when it is centered pointing forward.
#define TURRET_DEGREES_PER_TICK (90.f/1600.f)
#define TURRET_MIN_ENCODER	-2410  //1700 a little more than 90
#define TURRET_MAX_ENCODER  2410

#define TURRET_CORRECTION_P 0.001f   // motor power per tick to correct invalid turret configurations



#endif /* SRC_DEFINES_H_ */
