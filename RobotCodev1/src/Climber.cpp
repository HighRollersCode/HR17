/*
 * Climber.cpp
 *
 *  Created on: Jan 28, 2017
 *      Author: 987
 */

#include <Climber.h>

ClimberClass::ClimberClass() {

	Climber = new CANTalon(Tal_Climber);
	//Climber->SetCurrentLimit(1.0f); //To make the climber stop when we get to the top of the rope

	Climber_2 = new CANTalon(Tal_Climber_2);
}

ClimberClass::~ClimberClass() {
	// TODO Auto-generated destructor stub
}
void ClimberClass::UpdateClimber(bool ClimbUp)
{
	/*if(Climber->GetOutputCurrent() > 100.0f)
	{
		UseFullPower = true;
	}
	else
	{
		UseFullPower = false;
	}*/

	if(ClimbUp)
	{
		/*if(UseFullPower)
		{*/
			Climber->Set(1);
			Climber_2->Set(1);
		/*}
		else
		{
			Climber->Set(0.25f);
			Climber_2->Set(0.25f);
		}*/
	}
	else
	{
		Climber->Set(0);
		Climber_2->Set(0);
	}
}
void ClimberClass::SendData()
{
	SmartDashboard::PutNumber("Climber Current", Climber->GetOutputCurrent());
}
