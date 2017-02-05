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
		Intake->Set(-1);
		//Uptake->Set(1);
	}
	else if(outtake)
	{
		Intake->Set(1);
		//Uptake->Set(-1);
	}
	else
	{
		Intake->Set(0);
		//Uptake->Set(0);
	}
}
void IntakeClass::IntakeIn()
{
	Intake->Set(1);
	//Uptake->Set(1);
}
void IntakeClass::IntakeOut()
{
	Intake->Set(-1);
	//Uptake->Set(-1);
}
void IntakeClass::IntakeOff()
{
	Intake->Set(0);
	//Uptake->Set(0);
}
