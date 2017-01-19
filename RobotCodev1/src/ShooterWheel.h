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

	GearTooth *GearSensor;
	double DTIME;
	double DCOUNT;

public:
	Victor *Shooter;

	Solenoid *HoodUp;
	Solenoid *HoodDown;

	static double Shooter_WheelK;
	int INDICATOR;
	double RPMCOUNT;
	double RPM;
	double PREVRPMCOUNT;
	double ERROR;
	float OverrideCommand;
	int ShooterToggle;

	bool isDesiredRPM;

	ShooterWheelClass();
	virtual ~ShooterWheelClass();

	void SetSpeed(float command);
	void Send_Data();
	void WheelOff();
	void UpdateShooter(int EnableLow,int EnableOverride,float OverrideRPM,double RobotTime,bool HoodEnable);
	void ShooterOverride(float input);
	void ShooterOverrideRPM(float rpm);
	float EstimatePower(float desiredRPM);

	float PUpdate(float dist);
	float ClampTarget(float tar, float lowerlim, float upperlim);

	void HandleTarget(float centerX,float calX,float target_area);
	void AutonomousTrackingUpdate(float tx, float crossX, float target_area);
	void SetState(int newstate);
	void SendData();
};

#endif /* SRC_ShooterWheel_H_ */
