/*
 * GearManipulator.h
 *
 *  Created on: Jan 28, 2017
 *      Author: 987
 */
#include "WPILib.h"
#include "TalonSRX.h"
#include "CanTalon.h"
#include "Defines.h"
#include "Turret.h"

#ifndef SRC_GEARMANIPULATOR_H_
#define SRC_GEARMANIPULATOR_H_

class GearManipulator {
public:

	CANTalon *GearIntake;

	Solenoid *IntakeDown;
	Solenoid *IntakeUp;

	GearManipulator();
	virtual ~GearManipulator();

	void UpdateGear(bool down_intake, bool down_outtake,bool intake,bool up,TurretClass* Turret);
	void GearIn();
	void GearOut();
};

#endif /* SRC_GEARMANIPULATOR_H_ */
