/*
 * ShotManager.cpp
 *
 *  Created on: Jan 18, 2017
 *      Author: 987
 */

#include <ShotManager.h>

ShotManager::ShotManager(TurretClass *RobotTurret,ShooterWheelClass *RobotShooterWheel)
{

	PresetTimer = new Timer();
	PresetTimer->Reset();
	PresetTimer->Start();

	Turret = RobotTurret;
	ShooterWheel = RobotShooterWheel;
}

ShotManager::~ShotManager() {
	// TODO Auto-generated destructor stub
}
void ShotManager::EnterState(RobotMode mode)
{
	currentMode = mode;
}
void ShotManager::StartTracking(float Enable)
{
	if(Enable == 1)
	{
		currentMode = RobotMode::Shooting;
	}
	else if(Enable == 0)
	{
		currentMode = RobotMode::Free;
	}
}
void ShotManager::Update(float turret,bool ShootingState,bool EnableOverrideMtr,bool EnableOverride,float OverrideMtr,float OverideRPM,
		float tx,float calx,float ty,Vector2 RobotVelocity,float ManualBoost)
{
	ShooterState_Prev = ShooterState_Cur;
	ShooterState_Cur = ShootingState;
	RobotVel = RobotVelocity;
	if(ShooterState_Cur)
	{
		EnterState(RobotMode::Shooting);
	}
	else
	{
		EnterState(RobotMode::Free);
	}
	if(currentMode == RobotMode::Shooting)
	{
		ShouldTrack = true;
	}
	else
	{
		ShouldTrack = false;
	}

	if(ShouldTrack)
	{
		Turret->HandleTarget(tx,calx,ty,true,0);

		float distance_to_goal = ShooterWheel->Get_Goal_Distance(ty);

		float RobotVel_Forward = RobotVel.x;
		float RobotVel_Side = RobotVel.y;

		const float RPMtoFPS = 8.69f/3400.0f;

		float ideal_stationary_rpm = ShooterWheel->EstimateRPM(distance_to_goal);
		ideal_stationary_rpm = fmax(ideal_stationary_rpm, 1000.f);
		float t = distance_to_goal/(ideal_stationary_rpm * RPMtoFPS);
		AdjustForward = RobotVel_Forward * t;
		AdjustSide = RobotVel_Side * t;

		if(fabs(AdjustSide) < 0.01f)
		{
			AdjustAngle = 0.0f;
		}
		else
		{

			AdjustAngle = -std::atan2(AdjustSide,AdjustForward + distance_to_goal);
			AdjustAngle *= (180.f/3.14f);
		}

		float NewRPM = ShooterWheel->EstimateRPM(distance_to_goal + AdjustForward);
		AdjustRPM = NewRPM - ideal_stationary_rpm;

		for(int i = 0; i < 100; i++)
		{
			distance_to_goal = ShooterWheel->Get_Goal_Distance(ty) + AdjustForward;
			t = distance_to_goal/(NewRPM * RPMtoFPS);

			AdjustForward = RobotVel_Forward * t;
			AdjustSide = RobotVel_Side * t;

			if(fabs(AdjustSide) < 0.01f)
			{
				AdjustAngle = 0.0f;
			}
			else
			{
				AdjustAngle = -std::atan2(AdjustSide,AdjustForward + distance_to_goal);
				AdjustAngle *= (180.f/3.14f);
				const float MAX_ADJUST = 25.0f;
				if (AdjustAngle < -MAX_ADJUST)
				{
					AdjustAngle = -MAX_ADJUST;
				}
				if (AdjustAngle > MAX_ADJUST)
				{
					AdjustAngle = MAX_ADJUST;
				}
			}

			NewRPM = ShooterWheel->EstimateRPM(distance_to_goal + AdjustForward);
			AdjustRPM = NewRPM - ideal_stationary_rpm;
		}
	}
	else
	{
		AdjustForward = 0.0f;
		AdjustSide = 0.0f;
		AdjustRPM = 0.0f;
		AdjustAngle = 0.0f;
	}

	if(MovingShotEnabled == false)
	{
		AdjustAngle = 0;
		AdjustForward = 0;
	}

	Turret->Update(turret,ShouldTrack);
	Turret->HandleTarget(tx,calx,ty,false,AdjustAngle);
	ShooterWheel->UpdateShooter(EnableOverrideMtr,EnableOverride,OverrideMtr,OverideRPM,ShouldTrack,ty,AdjustForward,ManualBoost);

	if(ShouldTrack)
	{
		if(currentMode == RobotMode::Shooting)
		{
			isReady = (Turret->isReady && ShooterWheel->isReady);
		}
		else
		{
			isReady = false;
		}
	}
}

void ShotManager::Send_Data()
{
	Turret->Send_Data();
	ShooterWheel->Send_Data();
	SmartDashboard::PutNumber("AdjRPM", AdjustRPM);
	SmartDashboard::PutNumber("AdjAngle", AdjustAngle);
	SmartDashboard::PutNumber("AdjForward", AdjustForward);
	SmartDashboard::PutNumber("AdjSide", AdjustSide);
	SmartDashboard::PutNumber("VelFor", RobotVel.x);
	SmartDashboard::PutNumber("VelSide", RobotVel.y);

}
