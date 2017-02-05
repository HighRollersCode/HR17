/*
 * Intake.h
 *
 *  Created on: Jan 7, 2017
 *      Author: 987
 */

#include "WPILib.h"
#include "Defines.h"
#include "CANSpeedController.h"
#include "TalonSRX.h"
#include "CANTalon.h"

#ifndef SRC_INTAKE_H_
#define SRC_INTAKE_H_


class IntakeClass {


public:

	CANTalon *Intake;
	CANTalon *Conveyor;

	IntakeClass();
	virtual ~IntakeClass();

	void UpdateIntake(float intake, float outtake);
	void IntakeIn();
	void IntakeOut();
	void IntakeOff();
};

#endif /* SRC_INTAKE_H_ */
