/*
 * ShotManager.h
 *
 *  Created on: Jan 18, 2017
 *      Author: 987
 */

#include "WPILib.h"
#include "Turret.h"
#include "Hopper.h"
#include "ShooterWheel.h"
#include "Defines.h"
#include "math.h"
#include "vector"

#ifndef SRC_SHOTMANAGER_H_
#define SRC_SHOTMANAGER_H_

/*struct Vector2
{
	float x;
	float y;
};*/

class ShotManager {
public:

	enum RobotMode
	{
		Free = 0,
		Shooting
	};
	TurretClass *Turret;
	HopperClass *Hopper;
	ShooterWheelClass *ShooterWheel;

	bool ShooterState_Cur;
	bool ShooterState_Prev;
	bool ShouldTrack;
	bool isReady;

	Timer *PresetTimer;

	int state;
	int counter;
	bool transitioning;

	RobotMode currentMode;
	ShotManager(TurretClass *RobotTurret,HopperClass *RobotHopper,ShooterWheelClass *RobotShooterWheel);
	virtual ~ShotManager();

	void EnterState(RobotMode mode);
	void StartTracking(float enable);
	void Update(float turret,bool ShootingState,bool EnableLow,bool EnableOverride,float OverrideRPM,float tx,float calx,float ty,
			float outtake,float uptake);//,Vector2 RobotVelocity);
	void Send_Data();

};

#endif /* SRC_SHOTMANAGER_H_ */
