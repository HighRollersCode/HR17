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

void IntakeClass::UpdateIntake(float intake, float outtake,float shooting_intake)
{
	if((intake)&&(!shooting_intake))
	{
		IntakeIn();
	}
	else if(outtake)
	{
		IntakeOut();
	}
	else if (shooting_intake)
	{
		Intake->Set(.5f);
	}
	else
	{
		IntakeOff();
	}
}
void IntakeClass::IntakeIn()
{
	Intake->Set(-.8f);
}
void IntakeClass::IntakeOut()
{
	Intake->Set(.8f);
}
void IntakeClass::IntakeOff()
{
	Intake->Set(0);
}
void IntakeClass::Send_Data()
{
	SmartDashboard::PutNumber("Intake Speed", Intake->Get());
}
