/*
 * GearManipulator.cpp
 *
 *  Created on: Jan 28, 2017
 *      Author: 987
 */

#include <GearManipulator.h>

GearManipulator::GearManipulator() {
	GearIntake = new CANTalon(Tal_Gear_Intake);

	IntakeUp = new Solenoid(Sol_Gear_Up);
	IntakeDown = new Solenoid(Sol_Gear_Down);

}
GearManipulator::~GearManipulator() {
	// TODO Auto-generated destructor stub
}
void GearManipulator::UpdateGear(bool intake, bool outtake) {
	if (intake) {
		IntakeUp->Set(false);
		IntakeDown->Set(true);
		GearIn();
	} else if (outtake) {
		GearOut();
	} else {
		IntakeUp->Set(true);
		IntakeDown->Set(false);
		GearIntake->Set(0);
	}
}
void GearManipulator::GearIn() {
	GearIntake->Set(1);
}
void GearManipulator::GearOut() {
	GearIntake->Set(-1);
}
