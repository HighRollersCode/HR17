/*
 * Turret.cpp
 *
 *  Created on: Jan 9, 2017
 *      Author: 987
 */

#include <Turret.h>
#include "Defines.h"

const float MIN_TURRET_CMD = 0.10f;

const float TURRET_P = -.00075f;
const float TURRET_I = -.0000035f;
const float TURRET_D = 0.0f;

const float TURRET_TOLERANCE = 1;

const float LOCKON_DEGREES_X = 2.5f;
const float LOCKON_SECONDS = 0.35f;

const float LOCKON_DEGREES_X_CLOSE = 2.5f;
const float LOCKON_CLOSE_AREA = 0.03f;
const float LOCKON_FAR_AREA = 0.0175f;

TurretClass::TurretClass()
{
	Turret = new TalonSRX(Tal_Turret);

	TurretEncoder = new Encoder(Encoder_Arm_Turret_1,Encoder_Arm_Turret_2);

	ShooterWheel = new ShooterWheelClass();

	Resetting = false;

	isTracking = false;
	isLockedOn = false;

	CurrentEnableTracking = false;
	PrevEnableTracking = false;

	LastMoveByDegreesX = 360.0f;
	LockonDegreesX = LOCKON_DEGREES_X;

	TurretEncoder_Cur = 0;
	TurretEncoder_Targ = 0;

	TurretCommand_Cur = 0.0f;
	TurretCommand_Prev = 0.0f;

	ArmTimer = new Timer();
	ArmTimer->Reset();
	ArmTimer->Start();

	ArmLockonTimer = new Timer();
	ArmLockonTimer->Reset();
	ArmLockonTimer->Start();
	LastShotTimer = new Timer();
	LastShotTimer->Reset();
	LastShotTimer->Start();

	isauto = false;
	TurretEncoder->Reset();

	TurretPIDController = new PIDController(TURRET_P,TURRET_I,TURRET_D,TurretEncoder,Turret,.01f);

	TurretPIDController->SetContinuous(false);
	TurretPIDController->Enable();
	TurretPIDController->SetOutputRange(-.75f,.75f);
	TurretPIDController->SetInputRange(-100,100);
}

TurretClass::~TurretClass() {
	// TODO Auto-generated destructor stub
}
void TurretClass::Auto_Start()
{
	TurretPIDController->Disable();
	TurretPIDController->Reset();

	SetTurret(GetTurretEncoder());
}
void TurretClass::UpdateTurret(float turret)
{
	TurretCommand_Prev = TurretCommand_Cur;
	TurretCommand_Cur = turret;
	float turretOut = 0;

	TurretEncoder_Cur = GetTurretEncoder();

	if(fabs(TurretCommand_Cur) > .2f)
	{
		if(TurretPIDController->IsEnabled())
		{
			TurretPIDController->Disable();
		}
		turretOut = TurretCommand_Cur;
		Turret->Set(turretOut);
	}
	else
	{
		if(!TurretPIDController->IsEnabled())
		{
			Turret->Set(0);
		}
	}
}
void TurretClass::Tele_Start()
{
	TurretPIDController->Disable();
	TurretPIDController->Reset();
	SetTurret(GetTurretEncoder());
}
void TurretClass::Update(float turret,float cx,float calx,float target_area)
{

	if(CurrentEnableTracking)
	{
		TurretPIDController->Enable();
		isLockedOn = (fabs(LastMoveByDegreesX) < LockonDegreesX)
		if(!isLockedOn)
		{
			ArmLockonTimer->Reset();
		}
		else if(ArmLockonTimer->Get() > LOCKON_SECONDS)
		{
			if(ShooterWheel->Shooter->Get() > 0)
			{
				if(LastShotTimer->Get() > 0.0f)
				{
					//FullShotQuick();
					printf("SHOT!\r\n");
					ArmLockonTimer->Reset();
					LastShotTimer->Reset();
					LastShotTimer->Start();
				}
			}
			else
			{
				printf("SHOT!\r\n");
			}
		}
		/*else if (Ball)
		{
			//FullShotQuick();
			printf("Manual shot during tracking!\r\n");
			ArmLockonTimer->Reset();
		}*/
	}
	//FullShotUpdate();

	if(ArmTimer->Get() > .01f)
	{
		HandleTarget(cx,calx,target_area);
		ArmTimer->Reset();
		ArmTimer->Start();
	}
	UpdateTurret(turret);

}
void TurretClass::StartTracking(int enable)
{
	isTracking = enable;
	PrevEnableTracking = false;
}
void TurretClass::AutonomousTrackingUpdate(float tx, float crossX,float target_area)
{
	if(fabs(TurretPIDController->GetError()) < 200)
	{
		TurretPIDController->SetPID(TURRET_P,TURRET_I,TURRET_D);
	}
	else
	{
		TurretPIDController->SetPID(TURRET_P,0,TURRET_D);
	}
	if(ArmTimer->Get() > .01f)
	{
		HandleTarget(tx,crossX,0.001f);
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
	TurretPIDController->SetSetpoint(TurretEncoder_Targ);
}
int TurretClass::GetTurretEncoder()
{
	return TurretEncoder->Get();
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
		LockonDegreesX = LOCKON_DEGREES_X + (LOCKON_DEGREES_X_CLOSE- LOCKON_DEGREES_X) * t;

		float moveByX_Degrees = 0;

		float moveByX_Ticks = 0;

		float xFOV = 76.00f;

		moveByX_Degrees = (calX - centerX) * (xFOV*.5f);

		LastMoveByDegreesX = moveByX_Degrees;

		moveByX_Ticks = moveByX_Degrees / ARM_TURRET_DEGREES_PER_TICK;

		if(CurrentEnableTracking)
		{
			const float FRACTION = 1.0f;
			int new_turret_target = GetTurretEncoder()-(moveByX_Ticks* FRACTION);
			SetTurret(new_turret_target);
		}
	}
}
float TurretClass::Turret_Encoder_To_Degrees(int enc)
{
	return enc * ARM_TURRET_DEGREES_PER_TICK;
}
float TurretClass::Validate_Turret_Command(float cmd,bool ispidcmd)
{
	if(ispidcmd)
	{
		float min = MIN_TURRET_CMD;
		if(cmd > 0.0f)
		{
			cmd = fmaxf(cmd, min);
		}
		else if (cmd < 0.0f)
		{
			cmd = fminf(cmd, -min);
		}
	}
	return cmd;
}
