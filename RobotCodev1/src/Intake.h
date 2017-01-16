/*
 * Intake.h
 *
 *  Created on: Jan 7, 2017
 *      Author: 987
 */

#include "WPILib.h"

#ifndef SRC_INTAKE_H_
#define SRC_INTAKE_H_


#include "Defines.h"
#include "CANSpeedController.h"


class IntakeClass {


public:

	Victor *Intake;
	TalonSRX *Uptake;

	//int IsIntakeOn;
	//int IsOuttakeOn;

	IntakeClass();
	virtual ~IntakeClass();

	void UpdateIntake(float intake, float outtake);
	void IntakeIn();
	void IntakeOut();
	void IntakeOff();
};

#endif /* SRC_INTAKE_H_ */
