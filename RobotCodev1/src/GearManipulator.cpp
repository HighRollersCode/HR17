/*
 * GearManipulator.cpp
 *
 *  Created on: Jan 28, 2017
 *      Author: 987
 */

#include <GearManipulator.h>

#define GEAR_TALON_CONTROL 0

#define Gear_Intake_Position 2000


inline float ticks_to_revs(float ticks)
{
	return (1.0f/4096.0f) * ticks;
}


GearManipulator::GearManipulator()
{
	GearIntake = new CANTalon(Tal_Gear_Intake);
	GearIntake->SetInverted(true);
	GearLift = new CANTalon(Tal_Gear_Lift);
	GearLift->SetInverted(true);

#if GEAR_TALON_CONTROL
	GearLift->SetControlMode(CANTalon::kPosition);
	GearLift->SetAllowableClosedLoopErr(1);
	GearLift->SetFeedbackDevice(CANTalon::CtreMagEncoder_Relative);

	GearLift->SelectProfileSlot(0);
	GearLift->SetPID(.7,0.001,0);
	GearLift->SetIzone(0.2);

	GearLift->SelectProfileSlot(1);
	GearLift->SetPID(.6,0,0);

	GearLift->ConfigLimitMode(CANTalon::kLimitMode_SrxDisableSwitchInputs);//  CANTalon::kLimitMode_SoftPositionLimits);
	GearLift->DisableSoftPositionLimits();
	//GearLift->ConfigForwardLimit(ticks_to_revs(Gear_Intake_Position + 10));
	//GearLift->ConfigReverseLimit(ticks_to_revs(-5));
	GearLift->SetClosedLoopOutputDirection(false);
	GearLift->SetCurrentLimit(1.0f);
#endif
	ResetGearEncoder();
}

GearManipulator::~GearManipulator()
{
	// TODO Auto-generated destructor stub
}

void GearManipulator::UpdateGear(bool down, bool down_outtake,bool intake,bool up)
{
#if GEAR_TALON_CONTROL
	UpdateGearTalon(down,down_outtake,intake,up);
#else
	UpdateGearManual(down,down_outtake,intake,up);
#endif
}

void GearManipulator::UpdateGearTalon(bool down, bool down_outtake,bool intake,bool up)
{
	//GearLift->SetControlMode(CANTalon::kPosition);
	if (down)
	{
		GearLift->SelectProfileSlot(0);
		GearLift->SetSetpoint(ticks_to_revs(Gear_Intake_Position));
	}
	else if (down_outtake)
	{
		GearLift->SelectProfileSlot(0);
		GearLift->SetSetpoint(ticks_to_revs(Gear_Intake_Position));
		GearOut();
	}
	else if(up)
	{
		GearLift->SelectProfileSlot(1);
		GearLift->SetSetpoint(ticks_to_revs(0));
	}
	else if (intake)
	{
		GearIn();
	}
	else
	{
		GearIntake->Set(0);
	}
}

void GearManipulator::UpdateGearManual(bool down, bool down_outtake,bool intake,bool up)
{
	if (down)
	{
		GearLift->Set(.25f);
	}
	else if (down_outtake)
	{
		GearLift->Set(.25f);
		GearOut();
	}
	else if(up)
	{
		GearLift->Set(-.25f);
	}
	else if (intake)
	{
		GearIn();
	}
	else
	{
		GearIntake->Set(0);
		GearLift->Set(0);
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
void GearManipulator::Send_Data()
{
	SmartDashboard::PutNumber("Gear Lift Encoder",GearLift->GetEncPosition());
	SmartDashboard::PutNumber("Gear Lift Power",GearLift->Get());
	SmartDashboard::PutNumber("Gear Lift Current",GearLift->GetOutputCurrent());
}
void GearManipulator::ResetGearEncoder()
{
	GearLift->SetEncPosition(0);
	GearLift->SelectProfileSlot(1);
	GearLift->SetSetpoint(ticks_to_revs(0));
}
