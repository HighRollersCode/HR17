/*
 * MyRobot.h
 *
 *  Created on: Jan 7, 2017
 *      Author: 987
 */

#ifndef SRC_MYROBOT_H_
#define SRC_MYROBOT_H_

#include "WPILib.h"
//#include <unistd.h>
//#include <stdio.h>
#include <Uptake.h>
#include "Spark.h"
#include "CANTalon.h"
#include "PWM.h"

#include "Drivetrain.h"
#include "Intake.h"
#include "Turret.h"
#include "TargetingSystemClient.h"
#include "ShooterWheel.h"
#include "ShotManager.h"
#include "GearManipulator.h"
#include "Climber.h"
#include "BallManager.h"
#include "HRscript.h"
#include "Auton.h"
#include "XboxController.h"

class MyRobotClass : public SampleRobot
{
public:

	float commandLeft = 0;
	float commandRight = 0;
	float commandIntake = 0;
	float intele = 0;

	Joystick *leftStick;
	Joystick *rightStick;
	Joystick *turretStick;

	Drivetrainclass *Drivetrain;
	IntakeClass *Intake;
	TurretClass *Turret;
	ShooterWheelClass *ShooterWheel;
	TargetingSystemClient *TargClient;
	ShotManager *ShotMng;
	GearManipulator *GearMpltr;
	ClimberClass *Climber;
	UptakeClass *Uptake;
	ConveyorClass *Conveyor;
	BallManager *BallMng;
	Auton *AutonomousControl;

	Compressor *Comp;

	Spark *LightRelay;

	Timer *ReconnectTimer;
	Timer *GameTimer;
	Timer *SendTimer;
	Timer *SmartDashTimer;
	int connectionattempts = 0;

	bool flipauto = false;

	bool JetsonConnected = false;

	bool ConnectionPrevTog = false;
	bool ConnectionCurTog = false;
	bool DisConnectionPrevTog = false;
	bool DisConnectionCurTog = false;

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

	int Auto_Index = 0;
protected:

	HrScriptSystemClass * m_ScriptSystem;
	static MyRobotClass * TheRobot;
};

#endif /* SRC_MYROBOT_H_ */
