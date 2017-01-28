/*
 * Uptake.cpp
 *
 *  Created on: Jan 17, 2017
 *      Author: 987
 */

#include <Hopper.h>

HopperClass::HopperClass()
{
	Hopper = new CANTalon(Tal_Hopper);
	HopperIntake = new CANTalon(Tal_Hopper_Intake);
	Conveyor = new CANTalon(Tal_Conveyor_Belt);
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
	Hopper->Set(1);
	HopperIntake->Set(-1);
	Conveyor->Set(1);
}
void HopperClass::HopperDown()
{
	Hopper->Set(-1);
	HopperIntake->Set(1);
	Conveyor->Set(-1);
}
void HopperClass::HopperOff()
{
	Hopper->Set(0);
	HopperIntake->Set(0);
	Conveyor->Set(0);
}
