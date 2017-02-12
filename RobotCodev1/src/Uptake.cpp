/*
 * Uptake.cpp
 *
 *  Created on: Jan 17, 2017
 *      Author: 987
 */

#include <Uptake.h>

UptakeClass::UptakeClass()
{
	Uptake = new CANTalon(Tal_Hopper);

	UptakeTimer = new Timer();
	UptakeTimer->Reset();

	UptakeOffTimer = new Timer();
	UptakeOffTimer->Reset();
}

UptakeClass::~UptakeClass() {
	// TODO Auto-generated destructor stub
}
void UptakeClass::UpdateUptake(bool uptake,bool downtake)
{
	if(uptake)
	{
		UptakeUp();
	}
	else if(downtake)
	{
		UptakeDown();
	}
	else
	{
		UptakeOff();
	}
}
void UptakeClass::UptakeUp()
{
	Uptake->Set(-.8f);
}
void UptakeClass::UptakeDown()
{
	Uptake->Set(.8f);
}
void UptakeClass::UptakeOff()
{
	Uptake->Set(0);
}
void UptakeClass::Send_Data()
{
	SmartDashboard::PutNumber("Uptake Speed",Uptake->Get());
}
