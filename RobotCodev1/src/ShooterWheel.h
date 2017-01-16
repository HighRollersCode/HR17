/*
 * ShooterWheel.h
 *
 *  Created on: Jan 8, 2017
 *      Author: 987
 */

#include "WPILib.h"
#include "GearTooth.h"
#include "Defines.h"

#ifndef SRC_ShooterWheel_H_
#define SRC_ShooterWheel_H_




enum ShooterState
{
	ShooterState_off = 0,
	ShooterState_Low,
	ShooterState_override,
	ShooterState_overrideRPM
};
class ShooterWheelClass
{

	int prevlow;
	int prevoverride;
	int State;
	float currentPresetSpeed;

	Victor *Shooter;
	GearTooth *GearSensor;
	double DTIME;
	double DCOUNT;

public:
	static double Shooter_WheelK;
	int INDICATOR;
	double RPMCOUNT;
	double RPM;
	double PREVRPMCOUNT;
	double ERROR;
	float OverrideCommand;
	int ShooterToggle;

	ShooterWheelClass();
	virtual ~ShooterWheelClass();

	void SetSpeed(float command);
	void Send_Data();
	void WheelOff();
	void UpdateShooter(int EnableLow,int EnableOverride,float OverrideRPM,double RobotTime);
	void ShooterOverride(float input);
	void ShooterOverrideRPM(float rpm);
	float EstimatePower(float desiredRPM);

	float PUpdate(float dist);
	void SetState(int newstate);
	void SendData();
};

#endif /* SRC_ShooterWheel_H_ */
