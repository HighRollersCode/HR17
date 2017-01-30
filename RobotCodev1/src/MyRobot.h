/*
 * MyRobot.h
 *
 *  Created on: Jan 7, 2017
 *      Author: 987
 */

#ifndef SRC_MYROBOT_H_
#define SRC_MYROBOT_H_

#include "WPILib.h"
#include <unistd.h>
#include <stdio.h>

#include "Drivetrain.h"
#include "Intake.h"
#include "Turret.h"
#include "TargetingSystemClient.h"
#include "ShooterWheel.h"
#include "Hopper.h"
#include "ShotManager.h"
#include "GearManipulator.h"
#include "Climber.h"
#include "HRscript.h"
#include "Auton.h"
#include "XboxController.h"

struct Vector2
{
	float x;
	float y;
};
class MyRobotClass : public SampleRobot
{
public:

	float commandLeft;
	float commandRight;
	float commandIntake;
	float intele;

	Joystick *leftStick;
	Joystick *rightStick;
	Joystick *turretStick;
	XboxController *XBoxController;

	Drivetrainclass *Drivetrain;
	IntakeClass *Intake;
	TurretClass *Turret;
	ShooterWheelClass *ShooterWheel;
	TargetingSystemClient *TargClient;
	ShotManager *ShotMng;
	GearManipulator *GearMpltr;
	ClimberClass *Climber;
	HopperClass *Hopper;
	Auton *AutonomousControl;

	Relay *LightRelay;

	Timer *ReconnectTimer;
	Timer *GameTimer;
	Timer *SendTimer;
	Timer *SmartDashTimer;
	int connectionattempts;

	bool JetsonConnected;

	bool ConnectionPrevTog;
	bool ConnectionCurTog;
	bool DisConnectionPrevTog;
	bool DisConnectionCurTog;

	MyRobotClass();
	virtual ~MyRobotClass();

	static MyRobotClass * Get() { return TheRobot; }

	void Disabled(void);
	void Enabled(void);
	void Reset_State();
	void Autonomous(void);
	void UpdateInputs();
	void Init_Scripts_System();
	void Load_Scripts();
	void Send_Data();
	void OperatorControl(void);

	void Shutdown_Jetson(void);
	void Jetson_Connection();

	 Vector2 Compute_Robot_Velocity();

	int Auto_Index;
protected:

	HrScriptSystemClass * m_ScriptSystem;
	static MyRobotClass * TheRobot;
};

#endif /* SRC_MYROBOT_H_ */
