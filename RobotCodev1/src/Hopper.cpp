/*
 * Uptake.cpp
 *
 *  Created on: Jan 17, 2017
 *      Author: 987
 */

#include <Hopper.h>

HopperClass::HopperClass()
{
	Hopper = new Victor(Vic_Hopper);
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
}
void HopperClass::HopperDown()
{
	Hopper->Set(-1);
}
void HopperClass::HopperOff()
{
	Hopper->Set(0);
}
