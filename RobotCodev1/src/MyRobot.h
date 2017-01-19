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
#include "HRscript.h"
#include "Auton.h"
#include "XboxController.h"

class MyRobot : public SampleRobot
{
public:

	float commandLeft;
	float commandRight;
	float commandIntake;

	Joystick *leftStick;
	Joystick *rightStick;
	Joystick *turretStick;
	XboxController *XBoxController;

	Drivetrainclass *Drivetrain;
	IntakeClass *Intake;
	TurretClass *Turret;
	TargetingSystemClient *TargClient;
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

	MyRobot();
	virtual ~MyRobot();

	static MyRobot * Get() { return TheRobot; }

	void Disabled(void);
	void Autonomous(void);
	void UpdateInputs();
	void Init_Scripts_System();
	void Load_Scripts();
	void Send_Data();
	void OperatorControl(void);

	void Shutdown_Jetson(void);
	void Jetson_Connection();

	int Auto_Index;
protected:

	HrScriptSystemClass * m_ScriptSystem;
	static MyRobot * TheRobot;
};

#endif /* SRC_MYROBOT_H_ */
