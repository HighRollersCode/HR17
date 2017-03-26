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
static float YDTable_Y[] = { -.50f, -.16f, .24f, .86f};//-.2667f,  0.2020f, 0.4680f, 0.9792f };
static float YDTable_Distance[] = {  12.0f, 10.0f, 8.0f, 6.25f};//11.0f, 4.0f, 2.7083f, 0.9583f };

/*Estimate RPM from Target Distance Narrow FOV
static float DRPM_DistanceTable75[] = { 8.3f, 13.1f, 15.0f};  //7.0f , 10.0f,11.15f, 12.5f};//2.0f, 6.0f, 8.0f, 11.0f};
static float DRPM_RPMTable75[] = {  3240.0f, 3660.0f, 3718.0f};//3072.0f,, 3350.0f, 3600.f, 3864.0f};//2000.0f, 2600.0f, 3000.0f, 3500.0f};*/

/*//Estimate RPM from Target Distance Wide FOV Practice Bot
static float DRPM_DistanceTable75[] = { 8.6f, 9.5f,  10.3f, 14.4f, 15.2f};
static float DRPM_RPMTable75[] = {  3132.0f, 3203.0f, 3460.0f, 3777.0f, 3800.0f};*/

//Estimate RPM from Target Distance Wide FOV Comp Bot 67 degree
static float DRPM_DistanceTable67[] = { 8.4f, /*8.9f,*/ 12.53f};//8.7f 8.9f, 9.9f, 12.4f};
static float DRPM_RPMTable67[] = { 3060.f, /*3150.f,*/ 3769.f};//3075.f 2992.f, 3106.f, 3625.f};

/*//Estimate RPM from Target Distance Wide FOV Comp Bot 69 degree
static float DRPM_DistanceTable69[] = { 8.1f, 8.55f, 9.08f, 9.3f, 10.0f, 11.2f,13.17};
static float DRPM_RPMTable69[] = { 3030.f, 3060.f, 3125.f, 3130.f, 3239.f, 3560.f, 3777.f};*/

//Estimate RPM from Target Distance Wide FOV Comp Bot 69 degrees 3 motors
static float DRPM_DistanceTable69[] = { 7.8f, 	8.0f, 	8.5f, 	9.0f, 	9.5f, 	9.7f, 	10.0f, 	10.5f, 	11.0f};
static float DRPM_RPMTable69[] = 	{ 	2987.f, 3063.f,	3085.f, 3125.f, 3175.f, 3205.f, 3351.f, 3397.f, 3461.f};

//Estimate Power from Optimum RPM
//static float RPMTable[] = { 0, 160.0f, 720.0f, 1300.0f, 1900.0f, 2500.0f, 3200.0f, 3750.0f, 4500.0f, 5100.0f, 5600.0f};
//static float MotorCmd[] = { 0, 0.109f, .207f , .304f  , .394f  , .492f  , .614f  , .688f  , .799f  , .905f  , 1.00f};

static float RPMTable_2mtr[] = { 0, 160.0f, 720.0f, 1300.0f, 1900.0f, 2500.0f, 3200.0f, 3750.0f, 4500.0f, 5100.0f, 5600.0f};
static float MotorCmd_2mtr[] = { 0, 0.109f, .207f , .304f  , .394f  , .492f  , .614f  , .688f  , .799f  , .905f  , 1.00f};

//static int RPM_TABLE_COUNT = sizeof(RPMTable) / sizeof(RPMTable[0]);
static int RPM_TABLE_2_MTR_COUNT = sizeof(RPMTable_2mtr) / sizeof(RPMTable_2mtr[0]);

static int DRPM_TABLE_COUNT67 = sizeof(DRPM_DistanceTable67) / sizeof(DRPM_DistanceTable67[0]);
static int YDTABLE_COUNT67 = sizeof(YDTable_Y) / sizeof(YDTable_Y[0]);

static int DRPM_TABLE_COUNT69 = sizeof(DRPM_DistanceTable69) / sizeof(DRPM_DistanceTable69[0]);
static int YDTABLE_COUNT69 = sizeof(YDTable_Y) / sizeof(YDTable_Y[0]);


inline float ComputeDistance(float y)
{
	float FOV = 45.f; //Narrow FOV35.f;
	float HRobot = 19.f;
	float HBoiler = 90.f;
	float CamAngle = 39.f;

	float HDelta = HBoiler-HRobot;

	float TargetAngle = (CamAngle+y*FOV/2);
	float TargetAngleRadians = TargetAngle*3.14f/180.f;

	float DistanceIn = (HDelta*(1/TargetAngleRadians));
	float DistanceFt = DistanceIn/12;

	return DistanceFt;
}


ShooterWheelClass::ShooterWheelClass()
{

	Shooter = new CANTalon(Tal_Shooter_Wheel);
	Shooter_2 = new CANTalon(Tal_Shooter_Wheel_2);

#if TALON_SPEED_CONTROL
	Shooter->SetControlMode(CANTalon::kSpeed);
	Shooter_2->SetControlMode(CANTalon::kFollower);
	Shooter_2->Set(Tal_Shooter_Wheel);
	Shooter->SetClosedLoopOutputDirection(false);
	Shooter->SetStatusFrameRateMs(CANTalon::StatusFrameRateAnalogTempVbat,10);

	Shooter->SetFeedbackDevice(CANTalon::CtreMagEncoder_Relative);
	SetShooterConstants(Shooter_WheelK,0,0,.00019);//,Shooter_WheelK_Down);
	Shooter->SetAllowableClosedLoopErr(10);
	Shooter->ConfigNominalOutputVoltage(+0.f,-0.f);
	Shooter->ConfigPeakOutputVoltage(+12.f,-12.f);
	Shooter->SelectProfileSlot(0);
	Shooter->SetSensorDirection(true);
	Shooter->SetVoltageRampRate(100);
	Shooter->SetIzone(500);
	Shooter_2->SetVoltageRampRate(100);
#else
	ShooterEnc = new Encoder(Encoder_Shooter_Wheel_1,Encoder_Shooter_Wheel_2, false, Encoder::EncodingType::k1X);
	ShooterEnc->Reset();
#endif

	RPMList =  new std::vector<float>();
	RPMList->empty();

	State = ShooterState_off;

	hood_angle = ((65.0f * 3.14f)/180.0f);

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
void ShooterWheelClass::SetShooterConstants(float p,float i,float d,float f)//,float k_down)
{
	Shooter_WheelK = p;
	Shooter_WheelK_Down = p;//k_down;
#if TALON_SPEED_CONTROL
	p *= 1024.f;
	i *= 1024.f;
	d *= 1024.f;
	f *= 1024.f;

	Shooter->SetP(p);
	Shooter->SetI(i);
	Shooter->SetD(d);
	Shooter->SetF(f);
#endif
}
float ShooterWheelClass::Get_Goal_Distance(float y)
{
	return ComputeDistance(y);
}
float ShooterWheelClass::EstimateRPM(float distance)
{
	return Interpolate(DRPM_DistanceTable69,DRPM_RPMTable69,DRPM_TABLE_COUNT69,distance);
	//return 146.486 * distance + 1860.405;
}
float ShooterWheelClass::EstimatePower(float desiredRPM)
{
	//Interpolate(RPMTable,MotorCmd,RPM_TABLE_COUNT,desiredRPM);
	return Interpolate(RPMTable_2mtr,MotorCmd_2mtr,RPM_TABLE_2_MTR_COUNT,desiredRPM);
}
float ShooterWheelClass::Interpolate(float inputs[], float outputs[],int listsize,float input)
{
	if(input < inputs[0])
	{
		return outputs[0];
		//float slope = (outputs[1]-outputs[0])/(inputs[1] - inputs[0]);
		//float b = outputs[0] - slope*inputs[0];
		//return slope * input + b;
	}
	if(input > inputs[listsize-1])
	{
		float slope = (outputs[listsize-1]-outputs[listsize-2])/(inputs[listsize - 1] - inputs[listsize-2]);
		float b = outputs[listsize-2] - slope*inputs[listsize-2];
		return slope * input + b;
	}

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
#if TALON_SPEED_CONTROL
	return PUpdate_Talon(desrpm);
#else
	return PUpdate_Roborio(desrpm);
#endif
}
float ShooterWheelClass::PUpdate_Talon(float desrpm)
{
	Shooter_2->SetControlMode(CANTalon::kFollower);
	Shooter_2->Set(Tal_Shooter_Wheel);
	Shooter->Set(desrpm);
	return 0;
}
float ShooterWheelClass::PUpdate_Roborio(float desrpm)
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
	SetSpeed(command);
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
void ShooterWheelClass::UpdateShooter(int EnableOverrideMtr,int EnableOverrideRPM,float OverrideMtr,float OverrideRPM,bool TrackingEnable,float ty,float AdjustForward)//,double RobotTime,float crossY)
{

	PrevOverridePower = CurOverridePower;
	CurOverridePower = EnableOverrideMtr;

	PrevOverrideRPM = CurOverrideRPM;
	CurOverrideRPM = EnableOverrideRPM;

	PrevEnableTracking = CurrentEnableTracking;
	CurrentEnableTracking = TrackingEnable;

	targy = ty;
	tdistance = ComputeDistance(targy);
	trpm = EstimateRPM(tdistance + AdjustForward);
	//float power = EstimatePower(trpm);

#if TALON_SPEED_CONTROL
	RPM = Shooter->GetSpeed();
	RPM_avg = .03f * RPM + (1 - .03f) * RPM_avg;
#else
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
#endif
	if((EnableOverrideMtr == 1)&&(PrevOverridePower == 0))
	{
		SetState(ShooterState_override);
	}
	if((EnableOverrideRPM == 1)&&(PrevOverrideRPM == 0))
	{
		SetState(ShooterState_overrideRPM);
	}
	if((CurrentEnableTracking == 1)&&(PrevEnableTracking == 0))
	{
		SetState(ShooterState_tracking);
	}
	if((CurrentEnableTracking == 0)&&(PrevEnableTracking == 1))
	{
		SetState(ShooterState_off);
	}
	if(ShooterToggle == 1)
	{
		Shooter->SetControlMode(CANTalon::kPercentVbus);
		Shooter_2->SetControlMode(CANTalon::kPercentVbus);
		SetSpeed(0.0f);
		INDICATOR = 0;
	}
	else if(ShooterToggle == -1)
	{
		INDICATOR = 1;

		if(State == ShooterState_overrideRPM)
		{
			Shooter->SetControlMode(CANTalon::kSpeed);
			Shooter_2->SetControlMode(CANTalon::kFollower);
			Shooter_2->Set(Tal_Shooter_Wheel);
			PUpdate(OverrideRPM);
		}
		else if(State == ShooterState_override)
		{
			Shooter->SetControlMode(CANTalon::kPercentVbus);
			Shooter_2->SetControlMode(CANTalon::kPercentVbus);
			Shooter_2->Set(-OverrideMtr);
			Shooter->Set(-OverrideMtr);
		}
		else if(State == ShooterState_tracking)
		{
			Shooter->SetControlMode(CANTalon::kSpeed);
			Shooter_2->SetControlMode(CANTalon::kFollower);
			Shooter_2->Set(Tal_Shooter_Wheel);
			PUpdate(trpm);
		}
		else
		{
			PUpdate(currentPresetSpeed);
		}
	}

}
void ShooterWheelClass::SetState(int newstate)
{
	ShooterToggle = -ShooterToggle;
	State = newstate;
}
void ShooterWheelClass::Send_Data()
{
#if TALON_SPEED_CONTROL
	SmartDashboard::PutNumber("ShooterRPM",RPM);
	SmartDashboard::PutNumber("ShooterRPM_avg",RPM_avg);
	SmartDashboard::PutNumber("ShooterSpeed", Shooter->GetOutputCurrent());
	SmartDashboard::PutNumber("ShooterError", ERROR);
	SmartDashboard::PutNumber("ShooterEnc", Shooter->GetEncPosition());
#else
	SmartDashboard::PutNumber("ShooterEnc", ShooterEnc->Get());
#endif
	SmartDashboard::PutNumber("Target RPM", trpm);
	SmartDashboard::PutNumber("Target Distance", tdistance);
	SmartDashboard::PutNumber("Target Y", targy);

	if(State == ShooterState_tracking)
	{
		printf("Shooter: dist: %f trpm: %f rpm: %f average: %f\r\n",tdistance,trpm,RPM,RPM_avg);
	}
}
