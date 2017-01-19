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

#ifndef SRC_SHOTMANAGER_H_
#define SRC_SHOTMANAGER_H_

class ShotManager {
public:

	enum RobotMode
	{
		Free = 0,
		Shooting
	};
	/*TurretClass *Turret;
	HopperClass *Hopper;
	ShooterWheelClass *ShooterWheel;

	bool ShooterState_Cur;
	bool ShooterState_Prev;

	Timer *PresetTimer;

	int state;
	int counter;
	bool transitioning;

	RobotMode currentMode;
	ShotManager(TurretClass *Turret,HopperClass *Hopper,ShooterWheelClass *ShooterWheel);
	virtual ~ShotManager();

	void EnterState(RobotMode mode);
	void Update(bool ShootingState);
*/
};

#endif /* SRC_SHOTMANAGER_H_ */
