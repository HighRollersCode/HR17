/*
 * Uptake.cpp
 *
 *  Created on: Jan 17, 2017
 *      Author: 987
 */

#include <Hopper.h>

Hopper::Hopper()
{

	Hopper = new Victor();
}

Hopper::~Hopper() {
	// TODO Auto-generated destructor stub
}
void Hopper::UpdateIntake(float uptake,float downtake)
{
	if(uptake)
	{
		Hopper->Set(1);
	}
	else if(downtake)
	{
		Hopper->Set(-1);
	}
	else
	{
		Hopper->Set(0);
	}
}
void Hopper::HopperUp()
{
	Hopper->Set(1);
}
void Hopper::HopperDown()
{
	Hopper->Set(-1);
}
void Hopper::HopperOff()
{
	Hopper->Set(0);
}
