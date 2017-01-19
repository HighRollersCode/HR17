/*
 * ShotManager.cpp
 *
 *  Created on: Jan 18, 2017
 *      Author: 987
 */

#include <ShotManager.h>

/*ShotManager::ShotManager(TurretClass *Turret,HopperClass *Hopper,ShooterWheelClass *ShooterWheel)
{
	ShooterState_Cur = false;
	ShooterState_Prev = false;

	state = 0;
	counter = 0;
	transitioning = false;

	PresetTimer = new Timer();
	PresetTimer->Reset();
	PresetTimer->Start();
}

ShotManager::~ShotManager() {
	// TODO Auto-generated destructor stub
}
/*void ShotManager::EnterState(RobotMode mode)
{
	currentMode = mode;
	state = 0;
	transitioning = true;
}
void ShotManager::Update(bool ShootingState)
{
	ShooterState_Prev = ShooterState_Cur;
	ShooterState_Cur = ShootingState;

	if(!ShooterState_Cur && ShooterState_Prev)
	{
		EnterState(RobotMode::Shooting);
	}

	if(transitioning == true)
	{
		if(currentMode == RobotMode::Shooting)
		{
			switch(state)
			{
				case 0 :
					{
						Turret->isTracking = true;
						break;
					}
				case 1 :
					{
						if(Turret->isLockedOn)
						{
							ShooterWheel->AutonomousTrackingUpdate(float tx,float crossX,float target_area);
						}
						break;
					}
				case 2 :
					{
						if(ShooterWheel->isDesiredRPM)
						{
							Hopper->HopperUp();
						}
						break;
					}
			}
		}
	}
}*/
