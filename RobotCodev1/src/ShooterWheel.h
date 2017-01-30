/*
 * ShooterWheel.h
 *
 *  Created on: Jan 8, 2017
 *      Author: 987
 */

#include "WPILib.h"
#include "GearTooth.h"
#include "Defines.h"
#include "TalonSRX.h"
#include "CANTalon.h"
#include "Encoder.h"

#ifndef SRC_ShooterWheel_H_
#define SRC_ShooterWheel_H_




enum ShooterState
{
	ShooterState_off = 0,
	ShooterState_Low,
	ShooterState_override,
	ShooterState_overrideRPM,
	ShooterState_tracking
};
class ShooterWheelClass
{

	int prevlow;
	int prevoverride;
	int State;
	float currentPresetSpeed;
	CANTalon *Shooter;

	std::vector<float> *RPMList;

public:

	static double Shooter_WheelK;
	static double Shooter_WheelK_Down;



	int INDICATOR;
	double RPM;
	double ERROR;
	float OverrideCommand;
	float hood_angle;
	int ShooterToggle;

	GearTooth *GearSensor;
	Encoder *ShooterEnc;

	bool isDesiredRPM;
	bool isReady;
	bool isTracking;
	bool CurrentEnableTracking;
	bool PrevEnableTracking;

	ShooterWheelClass();
	virtual ~ShooterWheelClass();

	void SetSpeed(float command);
	void Send_Data();
	void WheelOff();
	void UpdateShooter(int EnableLow,int EnableOverride,float OverrideRPM,bool TrackingEnable,float ty);//,double RobotTime,float crossY);
	void ShooterOverride(float input);
	void ShooterOverrideRPM(float rpm);
	float Get_Goal_Distance(float y);
	float EstimateDistance(float ty);
	float EstimateRPM(float ty);
	float EstimatePower(float desiredRPM);

	float PUpdate(float dist);
	Solenoid *HoodUp;
	Solenoid *HoodDown;
	float ClampTarget(float tar, float lowerlim, float upperlim);

	void HandleTarget(float centerY,float calY);//,float target_area);
	void AutonomousTrackingUpdate(float ty, float crossY);//, float target_area);
	void SetState(int newstate);
};

#endif /* SRC_ShooterWheel_H_ */
