/*
 * Climber.cpp
 *
 *  Created on: Jan 28, 2017
 *      Author: 987
 */

#include <Climber.h>

ClimberClass::ClimberClass() {

	Climber = new TalonSRX(Tal_Climber);

	PDP = new PowerDistributionPanel(0);

	UseFullPower = true;
}

ClimberClass::~ClimberClass() {
	// TODO Auto-generated destructor stub
}
void ClimberClass::UpdateClimber(bool ClimbUp,bool ClimbDown)
{
//	if(PDP->GetCurrent(PDP_Climber) > 1.0f)
//	{
//		UseFullPower = true;
//	}
//	else
//	{
//		UseFullPower = false;
//	}

	if(ClimbUp)
	{
		if(UseFullPower)
		{
			Climber->Set(1);
		}
		else
		{
			Climber->Set(0.25f);
		}
	}
	else if(ClimbDown)
	{
		Climber->Set(-1);
	}
	else
	{
		Climber->Set(0);
	}
}
