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

//Estimate Distance from Target Pixels
static float YDTable_Y[] = { -.39f, -.16, .24};//-.2667f,  0.2020f, 0.4680f, 0.9792f };
static float YDTable_Distance[] = {  12.5f, 10.0f, 7.0f};//11.0f, 4.0f, 2.7083f, 0.9583f };

//Estimate RPM from Target Distance
static float DRPM_DistanceTable75[] = { 7.0f , 10.0f, 12.5f};//2.0f, 6.0f, 8.0f, 11.0f};
static float DRPM_RPMTable75[] = { 3072.0f, 3650.0f, 3864.0f};//2000.0f, 2600.0f, 3000.0f, 3500.0f};

//Estimate Power from Optimum RPM
//static float RPMTable[] = { 0, 160.0f, 720.0f, 1300.0f, 1900.0f, 2500.0f, 3200.0f, 3750.0f, 4500.0f, 5100.0f, 5600.0f};
//static float MotorCmd[] = { 0, 0.109f, .207f , .304f  , .394f  , .492f  , .614f  , .688f  , .799f  , .905f  , 1.00f};

static float RPMTable_2mtr[] = { 0, 160.0f, 720.0f, 1300.0f, 1900.0f, 2500.0f, 3200.0f, 3750.0f, 4500.0f, 5100.0f, 5600.0f};
static float MotorCmd_2mtr[] = { 0, 0.109f, .207f , .304f  , .394f  , .492f  , .614f  , .688f  , .799f  , .905f  , 1.00f};

//static int RPM_TABLE_COUNT = sizeof(RPMTable) / sizeof(RPMTable[0]);
static int RPM_TABLE_2_MTR_COUNT = sizeof(RPMTable_2mtr) / sizeof(RPMTable_2mtr[0]);

static int DRPM_TABLE_COUNT = sizeof(DRPM_DistanceTable75) / sizeof(DRPM_DistanceTable75[0]);
static int YDTABLE_COUNT = sizeof(YDTable_Y) / sizeof(YDTable_Y[0]);

ShooterWheelClass::ShooterWheelClass()
{
	Shooter_WheelK = .00075f;
	Shooter_WheelK_Down = .000375f;

	Shooter = new CANTalon(Tal_Shooter_Wheel);
	Shooter_2 = new CANTalon(Tal_Shooter_Wheel_2);

	HoodUp = new Solenoid(Sol_Hood_Up);
	HoodDown = new Solenoid(Sol_Hood_Down);

	GearSensor = new GearTooth(6);
	GearSensor->Reset();

	ShooterEnc = new Encoder(Encoder_Shooter_Wheel_1,Encoder_Shooter_Wheel_2, false, Encoder::EncodingType::k1X);
	ShooterEnc->Reset();

	RPMList =  new std::vector<float>();
	RPMList->empty();

	ShooterToggle = 1;
	State = ShooterState_off;

	currentPresetSpeed = 0.0f;
	OverrideCommand = 0.0f;
	hood_angle = ((65.0f * 3.14f)/180.0f);
	prevoverride = 0;
	prevlow = 0;

	RPM = 0.0;
	ERROR = 0.0;

	trpm = 0;
	tdistance = 0;
	targy = 0;

	INDICATOR = 0;

	isDesiredRPM = false;
	isReady = false;
	isTracking = false;
	CurrentEnableTracking = false;
	PrevEnableTracking = false;

	RPMList->push_back(0.0f);

}
ShooterWheelClass::~ShooterWheelClass() {
	// TODO Auto-generated destructor stub
}
void ShooterWheelClass::SetSpeed(float command)
{
	Shooter->Set(-command);
	Shooter_2->Set(-command);
}
float ShooterWheelClass::Get_Goal_Distance(float y)
{
	return EstimateDistance(y);
}
float ShooterWheelClass::EstimateDistance(float ty)
{
	return Interpolate(YDTable_Y,YDTable_Distance,YDTABLE_COUNT,ty);
}
float ShooterWheelClass::EstimateRPM(float distance)
{
	return Interpolate(DRPM_DistanceTable75,DRPM_RPMTable75,DRPM_TABLE_COUNT,distance);
}
float ShooterWheelClass::EstimatePower(float desiredRPM)
{
	//Interpolate(RPMTable,MotorCmd,RPM_TABLE_COUNT,desiredRPM);
	return Interpolate(RPMTable_2mtr,MotorCmd_2mtr,RPM_TABLE_2_MTR_COUNT,desiredRPM);
}
float ShooterWheelClass::Interpolate(float inputs[], float outputs[],int listsize,float input)
{
	if(input < inputs[0]){return outputs[0];}
	if(input > inputs[listsize-1]){return outputs[listsize-1];}

	for(int i = 0; i < listsize; i++)
	{
		if(input < inputs[i])
		{
			float norm = (input - inputs[i-1])/(inputs[i] - inputs[i-1]);
			float output = (outputs[i-1] + norm*(outputs[i]-outputs[i-1]));
			return output;
		}
	}
	return outputs[listsize-1];
}
float ShooterWheelClass::PUpdate(float desrpm)
{
	isReady = false;
	double basecom = EstimatePower(desrpm);
	double error = desrpm - RPM;
	ERROR = error;
	double correction;
	if(error < 0.0f)
	{
		correction = (error * Shooter_WheelK_Down);
	}
	if(error >= 0.0f)
	{
		correction = (error * Shooter_WheelK);
	}

	double command = correction + basecom;
	if(command < -.5f)
	{
		command = -.5f;
	}
	cmdCorrection = correction;
	rpDes = desrpm;
	rpEr= error;
	if(fabs(RPM-desrpm) < 1000.0f)
	{
		isReady= true;
	}
	return command;
}
float ShooterWheelClass::ClampTarget(float tar, float lowerlim, float upperlim)
{
	if(tar >= upperlim)
	{
		return upperlim;
	}
	else if(tar <= lowerlim)
	{
		return lowerlim;
	}
	else
	{
		return tar;
	}
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
	Shooter_2->Set(0.0f);
	ShooterToggle = 1;
}
void ShooterWheelClass::UpdateShooter(int EnableLow,int EnableOverride,float OverrideRPM,bool TrackingEnable,float ty)//,double RobotTime,float crossY)
{
	PrevEnableTracking = CurrentEnableTracking;
	CurrentEnableTracking = TrackingEnable;

	/*float y = ty;
	float distance = EstimateDistance(y);
	float rpm = EstimateRPM(distance);
	float power = EstimatePower(rpm);*/

	//printf("Target Y",y);
	//printf("Estimated Distance",distance);
	//printf("Estimated RPM",rpm);
	//printf("Estimated Power",power);

	RPM = ((1.0f/(ShooterEnc->GetPeriod()))*60.0f)/1024.0f;//Shooter->GetEncVel();((1.0f/(GearSensor->GetPeriod()))*60.0f)/6.0f;

	//Limit the RPM ouput and average it
	RPMList->push_back(RPM);
	if(RPMList->size() > 50)
	{
		RPMList->erase(RPMList->begin());
	}
	float sum = 0;
	for(uint i = 0; i < RPMList->size(); i++)
	{
		sum += (RPMList->at(i));
	}
	RPM = sum/RPMList->size();

	if((EnableLow == 1)&&(prevlow == 0))
	{
		SetState(ShooterState_Low);
		currentPresetSpeed = Shooter_Preset_Low;
	}
	if((EnableOverride == 1)&&(prevoverride == 0))
	{
		SetState(ShooterState_overrideRPM);
	}
	if((CurrentEnableTracking == 1)&&(PrevEnableTracking == 0))
	{
		SetState(ShooterState_tracking);
	}
	else if((CurrentEnableTracking == 0)&&(PrevEnableTracking == 1))
	{
		SetState(ShooterState_off);
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
		else if(State == ShooterState_tracking)
		{
			float distance = EstimateDistance(ty);
			float rpm = EstimateRPM(distance);
			float command = PUpdate(rpm);
			SetSpeed(command);
			tdistance = distance;
			trpm = rpm;
			targy = ty;
		}
		else
		{
			SetSpeed(PUpdate(currentPresetSpeed));
		}
	}

	prevlow = EnableLow;
	prevoverride = EnableOverride;

}
void ShooterWheelClass::SetState(int newstate)
{
	ShooterToggle = -ShooterToggle;
	State = newstate;
}
void ShooterWheelClass::Send_Data()
{
	SmartDashboard::PutNumber("ShooterRPM",RPM);
	SmartDashboard::PutNumber("ShooterSpeed", Shooter->Get());
	SmartDashboard::PutNumber("ShooterError", ERROR);
	SmartDashboard::PutNumber("ShooterEnc", ShooterEnc->Get());
	SmartDashboard::PutNumber("Target RPM", trpm);
	SmartDashboard::PutNumber("Target Distance", tdistance);
	SmartDashboard::PutNumber("Target Y", targy);
	//sSmartDashboard::PutBoolean("Encoder Status", sensorpluggedin);
}
