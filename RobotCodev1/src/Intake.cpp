/*
 * Intake.cpp
 *
 *  Created on: Jan 7, 2017
 *      Author: 987
 */

#include "Intake.h"

IntakeClass::IntakeClass()
{
	Intake = new CANTalon(Tal_Intake);
}

IntakeClass::~IntakeClass()
{
	// TODO Auto-generated destructor stub
}

void IntakeClass::UpdateIntake(float intake, float outtake)
{
	if(intake)
	{
		IntakeIn();
	}
	else if(outtake)
	{
		IntakeOut();
	}
	else
	{
		IntakeOff();
	}
}
void IntakeClass::IntakeIn()
{
	Intake->Set(-1);
}
void IntakeClass::IntakeOut()
{
	Intake->Set(1);
}
void IntakeClass::IntakeOff()
{
	Intake->Set(0);
}
