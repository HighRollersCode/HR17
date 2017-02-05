/*
 * Uptake.cpp
 *
 *  Created on: Jan 17, 2017
 *      Author: 987
 */

#include <Hopper.h>

HopperClass::HopperClass()
{
	Uptake = new CANTalon(Tal_Hopper);

	Conveyor = new CANTalon(Tal_Conveyor_Belt);
	//Conveyor->SetControlMode(CANSpeedController::kFollower);
	//Conveyor->Set(Tal_Hopper);
	//Conveyor->SetInverted(true);

	speed = .5f;

	HopperTimer = new Timer();
	HopperTimer->Reset();

	HopperOffTimer = new Timer();
	HopperOffTimer->Reset();
}

HopperClass::~HopperClass() {
	// TODO Auto-generated destructor stub
}
void HopperClass::UpdateHopper(float uptake,float downtake)
{
	if(uptake)
	{
		HopperUp();
	}
	else if(downtake)
	{
		HopperDown();
	}
	else
	{
		HopperOff();
	}
}
void HopperClass::HopperUp()
{
	Uptake->Set(1);
	Conveyor->Set(.6f);
}
void HopperClass::HopperDown()
{
	Uptake->Set(-1);
	Conveyor->Set(-.6f);
}
void HopperClass::HopperOff()
{
	Uptake->Set(0);
	Conveyor->Set(0);
}
void HopperClass::Send_Data()
{
	SmartDashboard::PutNumber("Hopper Speed",speed);
}
