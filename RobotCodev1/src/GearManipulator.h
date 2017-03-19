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
#include "Timer.h"

#ifndef SRC_GEARMANIPULATOR_H_
#define SRC_GEARMANIPULATOR_H_

class GearManipulator {
public:

	GearManipulator();
	virtual ~GearManipulator();

	void UpdateGear(bool down, bool down_outtake,bool intake,bool up);
	void Send_Data();
	void ResetGearEncoder();

private:

	CANTalon *GearIntake;
	CANTalon *GearLift;

	Victor *LEDS;


	Timer *GearIntakeTimer;

	int PrevIntake = 0;
	int CurIntake = 0;

	bool intakedgear = false;

	void GearIn();
	void GearOut();
	void UpdateGearManual(bool down, bool down_outtake,bool intake,bool up);
	void UpdateGearTalon(bool down, bool down_outtake,bool intake,bool up);

};

#endif /* SRC_GEARMANIPULATOR_H_ */
