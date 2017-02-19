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
#include "CanTalonSRX.h"

#include "Encoder.h"

#ifndef SRC_ShooterWheel_H_
#define SRC_ShooterWheel_H_
#define TALON_SPEED_CONTROL 1



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

	int State = 0;

	float currentPresetSpeed = 0;

	//bool sensorpluggedin;


	std::vector<float> *RPMList;

public:

	CANTalon *Shooter;
	CANTalon *Shooter_2;

	double Shooter_WheelK = .00075f;
	double Shooter_WheelK_Down = .000375f;

	float trpm = 0;
	float tdistance = 0;
	float targy = 0;

	int INDICATOR = 0;
	double RPM = 0;
	double ERROR = 0;
	float OverrideCommand = 0;
	float hood_angle = 0;
	int ShooterToggle = 1;

	Encoder *ShooterEnc;

	bool isDesiredRPM = false;
	bool isReady = false;
	bool isTracking = false;
	bool CurrentEnableTracking = false;
	bool PrevEnableTracking = false;
	bool CurOverridePower = false;
	bool CurOverrideRPM = false;
	bool PrevOverridePower = false;
	bool PrevOverrideRPM = false;

	ShooterWheelClass();
	virtual ~ShooterWheelClass();

	void SetSpeed(float command);
	void Send_Data();
	void WheelOff();
	void UpdateShooter(int EnableOverrideMtr,int EnableOverrideRPM,float OverrideMtr,float OverrideRPM,bool TrackingEnable,float ty);//,double RobotTime,float crossY);
	void ShooterOverride(float input);
	void ShooterOverrideRPM(float rpm);
	void SetShooterConstants(float p,float i,float d,float f);//,float k_down);
	float Get_Goal_Distance(float y);
	float EstimateDistance(float ty);
	float EstimateRPM(float distance);
	float EstimatePower(float desiredRPM);
	float Interpolate(float inputs[],float outputs[],int listsize,float input);

	float PUpdate(float desrpm);
	float PUpdate_Talon(float desrpm);
	float PUpdate_Roborio(float desrpm);
	Solenoid *HoodUp;
	Solenoid *HoodDown;
	float ClampTarget(float tar, float lowerlim, float upperlim);

	void HandleTarget(float centerY,float calY);//,float target_area);
	void AutonomousTrackingUpdate(float ty, float crossY);//, float target_area);
	void SetState(int newstate);
};

#endif /* SRC_ShooterWheel_H_ */
