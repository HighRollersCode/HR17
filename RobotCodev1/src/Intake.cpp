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
	PDP = new PowerDistributionPanel(1);
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
		if(PDP->GetCurrent(PDP_Intake) > 12.0f)
		{
			motor_stall = true;
		}
		else
		{
			motor_stall = false;
		}
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
