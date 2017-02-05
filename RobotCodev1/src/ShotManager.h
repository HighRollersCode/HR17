/*
 * ShotManager.h
 *
 *  Created on: Jan 18, 2017
 *      Author: 987
 */

#include <Uptake.h>
#include "WPILib.h"
#include "Turret.h"
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
	ShooterWheelClass *ShooterWheel;

	bool ShooterState_Cur = false;
	bool ShooterState_Prev = false;
	bool ShouldTrack = false;
	bool isReady = false;

	Timer *PresetTimer;

	int state = 0;
	int counter = 0;
	bool transitioning = false;

	RobotMode currentMode = RobotMode::Free;
	ShotManager(TurretClass *RobotTurret,ShooterWheelClass *RobotShooterWheel);
	virtual ~ShotManager();

	void EnterState(RobotMode mode);
	void StartTracking(float enable);
	void Update(float turret,bool ShootingState,bool EnableLow,bool EnableOverride,float OverrideRPM,float tx,float calx,float ty);//,Vector2 RobotVelocity);
	void Send_Data();

};

#endif /* SRC_SHOTMANAGER_H_ */
