/*
 * ShotManager.cpp
 *
 *  Created on: Jan 18, 2017
 *      Author: 987
 */

#include <ShotManager.h>

ShotManager::ShotManager(TurretClass *RobotTurret,HopperClass *RobotHopper,ShooterWheelClass *RobotShooterWheel)
{

	ShooterState_Cur = false;
	ShooterState_Prev = false;
	ShouldTrack = false;
	isReady = false;

	state = 0;
	counter = 0;
	transitioning = false;

	PresetTimer = new Timer();
	PresetTimer->Reset();
	PresetTimer->Start();

	Turret = RobotTurret;
	Hopper = RobotHopper;
	ShooterWheel = RobotShooterWheel;

	currentMode = RobotMode::Free;
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
void ShotManager::Update(float turret,bool ShootingState,float tx,float ty)
{
	ShooterState_Prev = ShooterState_Cur;
	ShooterState_Cur = ShootingState;

	if(!ShooterState_Cur && ShooterState_Prev)
	{
		EnterState(RobotMode::Shooting);
		ShouldTrack = true;
	}
	if(currentMode == RobotMode::Shooting)
	{
		ShouldTrack = true;
	}
	else
	{
		ShouldTrack = false;
	}

	Turret->Update(turret,ShouldTrack,tx,0,ty);
	ShooterWheel->UpdateShooter(0,0,0,ShouldTrack,ty);

	if(currentMode == RobotMode::Shooting)
	{
		isReady = (Turret->isReady && ShooterWheel->isReady);
	}
	else
	{
		isReady = false;
	}
	if(isReady)
	{
		Hopper->HopperUp();
	}
	else
	{
		Hopper->HopperOff();
	}
}
