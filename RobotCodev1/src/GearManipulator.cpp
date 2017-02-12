/*
 * GearManipulator.cpp
 *
 *  Created on: Jan 28, 2017
 *      Author: 987
 */

#include <GearManipulator.h>

GearManipulator::GearManipulator()
{
	GearIntake = new CANTalon(Tal_Gear_Intake);

	IntakeUp = new Solenoid(Sol_Gear_Up);
	IntakeDown = new Solenoid(Sol_Gear_Down);

}
GearManipulator::~GearManipulator() {
	// TODO Auto-generated destructor stub
}
void GearManipulator::UpdateGear(bool down_intake, bool down_outtake,bool intake,bool up,TurretClass *Turret)
{
	if (down_intake)
	{
		Turret->TurretPIDController->Enable();
		Turret->SetTurret(1);
		IntakeUp->Set(false);
		IntakeDown->Set(true);
		GearIn();
	}
	else if (down_outtake)
	{
		GearOut();
		IntakeUp->Set(false);
		IntakeDown->Set(true);
	}
	else if (intake)
	{
		GearIn();
	}
	else if(up)
	{
		IntakeUp->Set(true);
		IntakeDown->Set(false);
	}
	else
	{
		IntakeUp->Set(true);
		IntakeDown->Set(false);
		GearIntake->Set(0);
	}
}
void GearManipulator::GearIn()
{
	GearIntake->Set(-1);
}
void GearManipulator::GearOut()
{
	GearIntake->Set(1);
}
