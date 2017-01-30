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
static float YDTable_Y[] = { 0.9792f, 0.4680f, 0.2020f};
static float YDTable_Distance[] = { 0.9583f, 2.7083f, 4.0f};

//Estimate RPM from Target Distance
static float DRPM_DistanceTable75[] = { 0.9583f, 2.7083f, 4.0f};
static float DRPM_RPMTable75[] = { 3050.0f, 3340.0f, 3680.0f};

//Estimate Power from Optimum RPM
static float RPMTable[] = { 0, 160.0f, 720.0f, 1300.0f, 1900.0f, 2500.0f, 3200.0f, 3750.0f, 4500.0f, 5100.0f, 5600.0f};
static float MotorCmd[] = { 0, 0.109f, .207f , .304f  , .394f  , .492f  , .614f  , .688f  , .799f  , .905f  , 1.00f};

static int RPM_TABLE_COUNT = sizeof(RPMTable) / sizeof(RPMTable[0]);
static int DRPM_TABLE_COUNT = sizeof(DRPM_DistanceTable75) / sizeof(DRPM_DistanceTable75[0]);
static int YDTABLE_COUNT = sizeof(YDTable_Y) / sizeof(YDTable_Y[0]);
//static int DISTANCE_RPM_TABLE_COUNT = sizeof(DistanceRPMTable) / sizeof(DistanceRPMTable[0]);
//static double currenttime = 0.0;
//static double previoustime = 0.0;
double ShooterWheelClass::Shooter_WheelK = .00075f; //.000375f; this number was to big of a correction
double ShooterWheelClass::Shooter_WheelK_Down = .000375f;

ShooterWheelClass::ShooterWheelClass()
{
	Shooter = new CANTalon(Tal_Shooter_Wheel);
	//Shooter->SetInverted(true);
	//Shooter->SetFeedbackDevice(CANTalon::CtreMagEncoder_Relative);

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

	INDICATOR = 0;

	isDesiredRPM = false;
	isReady = false;
	isTracking = false;
	CurrentEnableTracking = false;
	PrevEnableTracking = false;

}
ShooterWheelClass::~ShooterWheelClass() {
	// TODO Auto-generated destructor stub
}
void ShooterWheelClass::SetSpeed(float command)
{
	Shooter->Set(command);
}
float ShooterWheelClass::Get_Goal_Distance(float y)
{
	return EstimateDistance(y);
}
float ShooterWheelClass::EstimateDistance(float ty)
{
	float goal_distance;
	if(ty < YDTable_Y[0])
	{
		goal_distance = YDTable_Distance[0];
	}

	if(ty > YDTable_Y[YDTABLE_COUNT-1])
	{
		goal_distance = YDTable_Distance[YDTABLE_COUNT-1];
	}

	for(int i = 0; i < YDTABLE_COUNT; i++)
	{
		if(ty < YDTable_Y[i])
		{
			float norm = (ty - YDTable_Y[i-1])/(YDTable_Y[i] - YDTable_Y[i-1]);
			float command = (YDTable_Distance[i-1] + norm*(YDTable_Distance[i]-YDTable_Distance[i-1]));
			goal_distance = command;
		}
	}
	return goal_distance;
}
float ShooterWheelClass::EstimateRPM(float ty)
{
	float desiredRPM;
	if(ty < DRPM_DistanceTable75[0])
	{
		desiredRPM = DRPM_RPMTable75[0];
	}

	if(ty > DRPM_DistanceTable75[DRPM_TABLE_COUNT-1])
	{
		desiredRPM = DRPM_RPMTable75[DRPM_TABLE_COUNT-1];
	}

	for(int i = 0; i < DRPM_TABLE_COUNT; i++)
	{
		if(ty < DRPM_DistanceTable75[i])
		{
			float norm = (ty - DRPM_DistanceTable75[i-1])/(DRPM_DistanceTable75[i] - DRPM_DistanceTable75[i-1]);
			float command = (DRPM_RPMTable75[i-1] + norm*(DRPM_RPMTable75[i]-DRPM_RPMTable75[i-1]));
			desiredRPM = command;
		}
	}
	return desiredRPM;
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
	isReady = false;
	double basecom = EstimatePower(desrpm);
	double error = desrpm - RPM;
	ERROR = error;
	double correction;
	if(error < 0.0f)
	{
		correction = (error * Shooter_WheelK_Down);
	}
	if(error > 0.0f)
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
	ShooterToggle = 1;
}
/*void ShooterWheelClass::HandleTarget(float centerY, float calY)//,float target_area)
{
	float t = (target_a - LOCKON_FAR_AREA) / (LOCKON_CLOSE_AREA - LOCKON_FAR_AREA);
	if (t < 0.0f) t = 0.0f;
	if (t > 1.0f) t = 1.0f;
	LockonDegreesY = LOCKON_DEGREES_Y + (LOCKON_DEGREES_Y_CLOSE - LOCKON_DEGREES_Y);// * t;

	float moveByY_Degrees = 0;

	float moveByY_Ticks = 0;

	float yFOV = 61.2f;

	moveByY_Degrees = (calY - centerY) * (yFOV*.5f);

	LastMoveByDegreesY = moveByY_Degrees;

	moveByY_Ticks = moveByY_Degrees / TURRET_DEGREES_PER_TICK;
	if(CurrentEnableTracking)
	{
		int shooter_new_targ = GearSensor->Get();
		ShooterOverrideRPM(shooter_new_targ);
		SmartDashboard::PutNumber("Shooter Targ",shooter_new_targ);
	}
}
void ShooterWheelClass::AutonomousTrackingUpdate(float ty,float crossY)//,float target_area)
{

}*/
void ShooterWheelClass::UpdateShooter(int EnableLow,int EnableOverride,float OverrideRPM,bool TrackingEnable,float ty)//,double RobotTime,float crossY)
{
	isTracking = TrackingEnable;
	PrevEnableTracking = CurrentEnableTracking;
	CurrentEnableTracking = TrackingEnable;

	float y = ty;
	float distance = EstimateDistance(y);
	float rpm = EstimateRPM(distance);
	float power = EstimatePower(rpm);

	printf("Target Y",y);
	printf("Estimated Distance",distance);
	printf("Estimated RPM",rpm);
	printf("Estimated Power",power);

	RPM = ((1.0f/(ShooterEnc->GetPeriod()))*60.0f)/1024.0f;//Shooter->GetEncVel();((1.0f/(GearSensor->GetPeriod()))*60.0f)/6.0f;
	//Limit the RPM ouput and average it
	RPMList->push_back(RPM);
	if(RPMList->size() > 25)
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
			SetSpeed(PUpdate(EstimateRPM(ty)));
		}
		else
		{
			SetSpeed(PUpdate(currentPresetSpeed));
		}
	}

	prevlow = EnableLow;
	prevoverride = EnableOverride;
	PrevEnableTracking = CurrentEnableTracking;
	//HandleTarget(ty,crossY);
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
}
