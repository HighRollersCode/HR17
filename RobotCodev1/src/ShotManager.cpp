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
	state = 0;
	transitioning = true;
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
void ShotManager::Update(float turret,bool ShootingState,bool EnableLow,bool EnableOverride,float OverrideMtr,float OverideRPM,
		float tx,float calx,float ty,Vector2 RobotVelocity)
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

	Turret->Update(turret,ShouldTrack,tx,calx,ty);
	ShooterWheel->UpdateShooter(EnableLow,EnableOverride,OverrideMtr,OverideRPM,ShouldTrack,ty);
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

	float g = 32.174;//ft/s2

	float goal_height = 6.0f;
	float shooter_height = 2.0f;
	float YDelta = goal_height - shooter_height;

	float distance_to_goal = ShooterWheel->Get_Goal_Distance(ty);

	//float desstartspeed=sqrt(((-.5*g*(distance_to_goal/cos(ShooterWheel->hood_angle))^2)/(YDelta - tan(ShooterWheel->hood_angle)*distance_to_goal)));

	float RobotVel_Forward = RobotVel.x;
	float RobotVel_Side = RobotVel.y;

//	float Ballx = desstartspeed*cos(ShooterWheel->hood_angle));
//	float Bally = 0;
//	float Ballz = -0.5*(g)*t^2+desstartspeed*sin(ShooterWheel->hood_angle)*t+shooter_height);

	const float RPMtoFPS = .01f;

	float t = distance_to_goal/(ShooterWheel->RPM * RPMtoFPS);
	AdjustForward = RobotVel_Forward * t;
	AdjustSide = RobotVel_Side * t;

	AdjustAngle = atan2(AdjustSide,AdjustForward);
	float NewRPM = ShooterWheel->EstimateRPM(distance_to_goal + AdjustForward);
	AdjustRPM = NewRPM - ShooterWheel->EstimateRPM(distance_to_goal);


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
