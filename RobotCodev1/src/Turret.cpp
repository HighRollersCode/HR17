/*
 * Turret.cpp
 *
 *  Created on: Jan 9, 2017
 *      Author: 987
 */

#include "Turret.h"

const float TURRET_TOLERANCE = 1;

const float LOCKON_DEGREES_X = 2.5f;
const float LOCKON_SECONDS = 0.35f;

const float LOCKON_DEGREES_X_CLOSE = 2.5f;
const float LOCKON_CLOSE_AREA = 0.03f;
const float LOCKON_FAR_AREA = 0.0175f;

TurretClass::TurretClass()
{
	LockonDegreesX = LOCKON_DEGREES_X;

#if TURRET_TALON_CONTROL
	Turret = new CANTalon(Tal_Turret);
	Turret->SetClosedLoopOutputDirection(true);

	Turret->SetFeedbackDevice(CANTalon::CtreMagEncoder_Relative);
	Turret->SetEncPosition(0);
	SetTurretConstants(TURRET_P,TURRET_I,TURRET_D);
	Turret->SetAllowableClosedLoopErr(0);
	Turret->ConfigNominalOutputVoltage(+0.f,-0.f);
	Turret->ConfigPeakOutputVoltage(+12.f,-12.f);
	Turret->SelectProfileSlot(0);
	Turret->SetSensorDirection(true);
	Turret->SetVoltageRampRate(100);

#else
	Turret = new CANTalon(Tal_Turret);
	TurretEncoder = new Encoder(Encoder_Arm_Turret_1,Encoder_Arm_Turret_2);
#endif

	ArmTimer = new Timer();
	ArmTimer->Reset();
	ArmTimer->Start();

	ArmLockonTimer = new Timer();
	ArmLockonTimer->Reset();
	ArmLockonTimer->Start();

	LastShotTimer = new Timer();
	LastShotTimer->Reset();
	LastShotTimer->Start();

#if !TURRET_TALON_CONTROL

	TurretEncoder->Reset();
	TurretPIDController = new PIDController(TURRET_P,TURRET_I,TURRET_D,TurretEncoder,Turret,.01f);
	TurretPIDController->SetContinuous(false);
	TurretPIDController->Enable();
	TurretPIDController->SetOutputRange(-1.0f,1.0f);
	TurretPIDController->SetInputRange(TURRET_MIN_ENCODER,TURRET_MAX_ENCODER);
#endif
}

TurretClass::~TurretClass() {
	// TODO Auto-generated destructor stub
}
void TurretClass::Auto_Start()
{
	CurrentEnableTracking = false;
	PrevEnableTracking = false;

	LastMoveByDegreesX = 360.0f;
	LockonDegreesX = LOCKON_DEGREES_X;

	ResetEncoderTurret();
#if TURRET_TALON_CONTROL
	Turret->SetControlMode(CANTalon::kPercentVbus);
#else
	TurretPIDController->Disable();
	TurretPIDController->Reset();
#endif
	SetTurret(GetTurretEncoder());

	isTracking = false;
	isLockedOn = false;

	ArmLockonTimer->Reset();
	ArmLockonTimer->Start();

	LastShotTimer->Reset();
	LastShotTimer->Start();
}
void TurretClass::HandleUser(float turret)
{
	TurretCommand_Prev = TurretCommand_Cur;
	TurretCommand_Cur = turret;
	float turretOut = 0;

	TurretEncoder_Cur = GetTurretEncoder();

	if(fabs(TurretCommand_Cur) > .2f)
	{
#if TURRET_TALON_CONTROL
		Turret->SetControlMode(CANTalon::kPercentVbus);
#else
		if(TurretPIDController->IsEnabled())
		{
			TurretPIDController->Disable();
		}
		if(GetTurretEncoder() < TURRET_MIN_ENCODER)
		{
			if(TurretCommand_Cur < 0)
			{
				TurretCommand_Cur = 0;
			}
		}
		if(GetTurretEncoder() > TURRET_MAX_ENCODER)
		{
			if(TurretCommand_Cur > 0)
			{
				TurretCommand_Cur = 0;
			}
		}
#endif
		turretOut = TurretCommand_Cur;
		Turret->Set(turretOut);
	}
	else
	{
#if TURRET_TALON_CONTROL
		if(Turret->GetControlMode() == CANTalon::ControlMode::kPercentVbus)
		{
			Turret->Set(0);
		}
#else
		if(!TurretPIDController->IsEnabled())
		{
			Turret->Set(0);
		}
#endif
	}
}
void TurretClass::Tele_Start()
{
	CurrentEnableTracking = false;
	PrevEnableTracking = false;

	LastMoveByDegreesX = 360.0f;
#if TURRET_TALON_CONTROL
	Turret->SetControlMode(CANTalon::kPercentVbus);
#else

	TurretPIDController->Reset();
	TurretPIDController->Disable();
	Turret->Set(0);
	//	SetTurret(GetTurretEncoder());
#endif
	isTracking = false;

	ArmLockonTimer->Reset();
	ArmLockonTimer->Start();

	LastShotTimer->Reset();
	LastShotTimer->Start();

}
void TurretClass::Update(float turret,bool TrackingEnable,float cx,float calx,float target_area)
{
	isTracking = TrackingEnable;
	PrevEnableTracking = CurrentEnableTracking;
	CurrentEnableTracking = TrackingEnable;

	if(CurrentEnableTracking)
	{

#if TURRET_TALON_CONTROL
	Turret->SetControlMode(CANTalon::kVoltage);
#else
		TurretPIDController->Enable();
#endif
		isLockedOn = (fabs(LastMoveByDegreesX) < LockonDegreesX);
		if(!isLockedOn)
		{
			ArmLockonTimer->Reset();
		}
		else if(ArmLockonTimer->Get() > LOCKON_SECONDS)
		{

		}
		isReady = isLockedOn;
	}
	else
	{
		isReady = true;

		#if TURRET_TALON_CONTROL
	Turret->SetControlMode(CANTalon::kPercentVbus);
#else
		TurretPIDController->Disable();
#endif
	}
		HandleTarget(cx,calx,target_area);
		ArmTimer->Reset();
		ArmTimer->Start();

	HandleUser(turret);
}
void TurretClass::AutonomousTrackingUpdate(float tx, float crossX,float target_area)
{
	if(ArmTimer->Get() > .01f)
	{
		HandleTarget(tx,crossX,target_area);
		ArmTimer->Reset();
		ArmTimer->Start();
	}
}
float TurretClass::Clamp_Target(float tar,float lowerlim,float upperlim)
{
	if(tar >= upperlim)
	{
		return upperlim;
	}
	else if (tar <= lowerlim)
	{
		return lowerlim;
	}
	else
	{
		return tar;
	}
}
void TurretClass::SetTurret(int targ)
{
	TurretEncoder_Targ = targ;
#if TURRET_TALON_CONTROL
	Turret->SetControlMode(CANTalon::kPosition);
	Turret->Set(TurretEncoder_Targ/4096.f);
#else
	TurretPIDController->SetSetpoint(TurretEncoder_Targ);
#endif
}
int TurretClass::GetTurretEncoder()
{
#if TURRET_TALON_CONTROL
	return Turret->GetEncPosition();
#else
	return TurretEncoder->Get();
#endif
}
void TurretClass::HandleTarget(float centerX,float calX,float target_a)
{
	if(fabs(centerX) >= 1)
	{
		LastMoveByDegreesX = 360.0f;
		LockonDegreesX = LOCKON_DEGREES_X;

		if (CurrentEnableTracking)
		{
			SetTurret(GetTurretEncoder());
		}
		return;
	}
	else
	{
		float t = (target_a - LOCKON_FAR_AREA) / (LOCKON_CLOSE_AREA - LOCKON_FAR_AREA);
		if (t < 0.0f) t = 0.0f;
		if (t > 1.0f) t = 1.0f;
		LockonDegreesX = LOCKON_DEGREES_X + (LOCKON_DEGREES_X_CLOSE - LOCKON_DEGREES_X) * t;

		float moveByX_Degrees = 0;

		float moveByX_Ticks = 0;

		float xFOV = 76.00f;

		moveByX_Degrees = (calX - centerX) * (xFOV*.5f);

		LastMoveByDegreesX = moveByX_Degrees;

		moveByX_Ticks = moveByX_Degrees / TURRET_DEGREES_PER_TICK;

		if(CurrentEnableTracking)
		{
			const float FRACTION = 1.0f;
			int new_turret_target = GetTurretEncoder()-(moveByX_Ticks* FRACTION);
			SetTurret(new_turret_target);
			SmartDashboard::PutNumber("Turret Targ", new_turret_target);
		}
	}
}
void TurretClass::ResetEncoderTurret()
{
#if TURRET_TALON_CONTROL
	Turret->SetPosition(0);
#else
	TurretEncoder->Reset();
	TurretPIDController->Reset();
#endif
}
void TurretClass::SetTurretConstants(float p,float i,float d)
{
#if TURRET_TALON_CONTROL
	p *= 1024;
	i *= 1024;
	d *= 1024;

	Turret->SetPID(p,i,d);
#else
	TURRET_P = p;
	TURRET_I = i;
	TURRET_D = d;
	TurretPIDController->SetPID(TURRET_P,TURRET_I,TURRET_D);
#endif
}
float TurretClass::Turret_Encoder_To_Degrees(int enc)
{
	return enc * TURRET_DEGREES_PER_TICK;
}
float TurretClass::Compute_Robot_Angle()
{
#if TURRET_TALON_CONTROL
	float dir = -Turret_Encoder_To_Degrees(Turret->GetPosition()) - LockonDegreesX;
#else
	float dir = -Turret_Encoder_To_Degrees(TurretEncoder->Get()) - LockonDegreesX;
#endif
	return dir;
}

void TurretClass::Send_Data()
{
	SmartDashboard::PutNumber("Turret Speed", Turret->Get());
#if TURRET_TALON_CONTROL
	SmartDashboard::PutNumber("Talon Turret Encoder", Turret->GetEncPosition());
	SmartDashboard::PutNumber("Turret Error", Turret->GetClosedLoopError());
#else
	SmartDashboard::PutNumber("Turret Encoder", TurretEncoder->Get());
#endif
}
