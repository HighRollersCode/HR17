/*
 * Conveyor.cpp
 *
 *  Created on: Feb 5, 2017
 *      Author: 987
 */

#include <Conveyor.h>

ConveyorClass::ConveyorClass() {

	Conveyor = new CANTalon(Tal_Conveyor_Belt);
}

ConveyorClass::~ConveyorClass() {
	// TODO Auto-generated destructor stub
}
void ConveyorClass::Update(bool intake,bool outake)
{
	if(intake)
	{
		Conveyor->Set(1);
	}
	else if(outake)
	{
		Conveyor->Set(-1);
	}
	else
	{
		Conveyor->Set(0);
	}
}
void ConveyorClass::Send_Data()
{
	SmartDashboard::PutNumber("Conveyor Speed", Conveyor->Get());
}
