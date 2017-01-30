/*
 * GearManipulator.h
 *
 *  Created on: Jan 28, 2017
 *      Author: 987
 */
#include "WPILib.h"
#include "TalonSRX.h"
#include "CanTalonSRX.h"
#include "Defines.h"

#ifndef SRC_GEARMANIPULATOR_H_
#define SRC_GEARMANIPULATOR_H_

class GearManipulator {
public:
	TalonSRX *GearIntake;

	Solenoid *IntakeDown;
	Solenoid *IntakeUp;

	GearManipulator();
	virtual ~GearManipulator();

	void UpdateGear(float intake, float outtake);
	void GearIn();
	void GearOut();
};

#endif /* SRC_GEARMANIPULATOR_H_ */
