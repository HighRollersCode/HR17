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

	bool MovingShotEnabled = false;

	Timer *PresetTimer;

	int counter = 0;

	float AdjustRPM = 0;
	float AdjustAngle = 0;
	float AdjustForward  = 0;
	float AdjustSide = 0;
	Vector2 RobotVel = Vector2();


	RobotMode currentMode = RobotMode::Free;
	ShotManager(TurretClass *RobotTurret,ShooterWheelClass *RobotShooterWheel);
	virtual ~ShotManager();

	void EnterState(RobotMode mode);
	void StartTracking(float enable);
	void Update(float turret,bool ShootingState,bool EnableOverrideMtr,bool EnableOverride,float OverrideMtr,float OverrideRPM,float tx,
			float calx,float ty, Vector2 RobotVelocity,float ManualBoost);
	void Send_Data();

};

#endif /* SRC_SHOTMANAGER_H_ */
