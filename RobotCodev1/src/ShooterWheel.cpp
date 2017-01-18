/*
 * ShooterWheel.cpp
 *
 *  Created on: Jan 8, 2017
 *      Author: 987
 */

#include "ShooterWheel.h"

#define Shooter_Preset_Low 5200.0f

static float rpEr;
static float rpDes;
static float cmdCorrection;

static float RPMTable[] = { 0, 160.0f, 720.0f, 1300.0f, 1900.0f, 2500.0f, 3200.0f, 3750.0f, 4500.0f, 5100.0f, 5600.0f};
static float MotorCmd[] = { 0, 0.109f, .207f , .304f  , .394f  , .492f  , .614f  , .688f  , .799f  , .905f  , 1.00f};

static int RPM_TABLE_COUNT = sizeof(RPMTable) / sizeof(RPMTable[0]);
static double currenttime = 0.0;
static double previoustime = 0.0;
double ShooterWheelClass::Shooter_WheelK = .0015f;

ShooterWheelClass::ShooterWheelClass()
{
	Shooter = new Victor(Tal_Shooter_Wheel);

	GearSensor = new GearTooth(4);
	GearSensor->Reset();

	ShooterToggle = 1;
	State = ShooterState_off;

	currentPresetSpeed = 0.0f;
	OverrideCommand = 0.0f;
	prevoverride = 0;
	prevlow = 0;

	RPMCOUNT = 0.0;
	PREVRPMCOUNT = 0.0;
	RPM = 0.0;
	ERROR = 0.0;

	DTIME = 0.0;
	DCOUNT = 0.0;
	INDICATOR = 0;
}
ShooterWheelClass::~ShooterWheelClass() {
	// TODO Auto-generated destructor stub
}
void ShooterWheelClass::SetSpeed(float command)
{
	Shooter->Set(command);
}
void ShooterWheelClass::Send_Data()
{
	SmartDashboard::PutNumber("ShooterRPM",RPM);
	SmartDashboard::PutNumber("ShooterSpeed", Shooter->Get());
	SmartDashboard::PutNumber("ShooterError", ERROR);
}
float ShooterWheelClass::EstimatePower(float desiredRPM)
{
	if(desiredRPM < RPMTable[0]){return MotorCmd[0];}
	if(desiredRPM > RPMTable[RPM_TABLE_COUNT-1]){return MotorCmd[RPM_TABLE_COUNT-1];}

	for(int i = 0; i < RPM_TABLE_COUNT; i++)
	{
		if(desiredRPM < RPMTable[i])
		{
			float norm = (desiredRPM - RPMTable[i-1])/(RPMTable[i] - RPMTable[i-1]);
			float command = (MotorCmd[i-1] + norm*(MotorCmd[i]-MotorCmd[i-1]));
			return command;
		}
	}
	return MotorCmd[RPM_TABLE_COUNT-1];
}
float ShooterWheelClass::PUpdate(float desrpm)
{
	double basecom = EstimatePower(desrpm);
	double error = desrpm - RPM;
	ERROR = error;
	double correction = (error * Shooter_WheelK);

	double command = correction + basecom;
	if(command < -.5f)
	{
		command = -.5f;
	}
	cmdCorrection = correction;
	rpDes = desrpm;
	rpEr= error;
	return command;
}
void ShooterWheelClass::ShooterOverride(float input)
{

	float normval = (input+1.0f)/2.0f;

	float MIN_COM = 0.0f;
	float MAX_COM = 1.0f;

	OverrideCommand = -(MIN_COM+normval*(MAX_COM-MIN_COM));
}
void ShooterWheelClass::ShooterOverrideRPM(float rpm)
{
	OverrideCommand = rpm;
}
void ShooterWheelClass::WheelOff()
{
	Shooter->Set(0.0f);
	ShooterToggle = 1;
}
void ShooterWheelClass::UpdateShooter(
		int EnableLow,
		int EnableOverride,
		float OverrideRPM,
		double RobotTime)
{
	PREVRPMCOUNT = RPMCOUNT;
	RPMCOUNT = GearSensor->Get();

	previoustime = currenttime;
	currenttime = RobotTime;
	double dt = currenttime-previoustime;

	RPM = ((1.0/(GearSensor->GetPeriod()))*60.0f)/6.f;

	if((dt > 0.01f) || (dt < 0.0f))
	{
		DTIME = dt;
		DCOUNT = (RPMCOUNT-PREVRPMCOUNT);
	}
	if((EnableLow == 1)&&(prevlow == 0))
	{
		SetState(ShooterState_Low);
		currentPresetSpeed = Shooter_Preset_Low;
	}
	if((EnableOverride == 1)&&(prevoverride == 0))
	{
		SetState(ShooterState_overrideRPM);
	}
	if(ShooterToggle == 1)
	{
		SetSpeed(0.0f);
		INDICATOR = 0;
	}
	else if(ShooterToggle == -1)
	{
		INDICATOR = 1;

		if(State == ShooterState_overrideRPM)
		{
			SetSpeed(PUpdate(OverrideRPM));
		}
		else if(State == ShooterState_override)
		{
			SetSpeed(OverrideCommand);
		}
		else
		{
			SetSpeed(PUpdate(currentPresetSpeed));
		}
	}

	prevlow = EnableLow;
	prevoverride = EnableOverride;
}
void ShooterWheelClass::HandleTarget(float tx, float crossX,float target_area)
{

}
void ShooterWheelClass::SetState(int newstate)
{
	ShooterToggle = -ShooterToggle;
	State = newstate;
}
