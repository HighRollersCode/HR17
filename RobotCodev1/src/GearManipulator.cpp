/*
 * GearManipulator.cpp
 *
 *  Created on: Jan 28, 2017
 *      Author: 987
 */

#include <GearManipulator.h>

#define GEAR_TALON_CONTROL 1

#define Gear_Intake_Position -987


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
	GearLift->SetPID(1.2,0.001,.1);
	GearLift->SetIzone(0.2);

	GearLift->SelectProfileSlot(1);
	GearLift->SetPID(.9,0.0015,0);

	GearLift->ConfigLimitMode(CANTalon::kLimitMode_SrxDisableSwitchInputs);//  CANTalon::kLimitMode_SoftPositionLimits);
	GearLift->DisableSoftPositionLimits();
	//GearLift->ConfigForwardLimit(ticks_to_revs(Gear_Intake_Position + 10));
	//GearLift->ConfigReverseLimit(ticks_to_revs(-5));
	GearLift->SetClosedLoopOutputDirection(false);
	GearLift->SetCurrentLimit(1.0f);

	GearIntakeTimer = new Timer();
	GearIntakeTimer->Reset();
#endif
	LEDS = new Victor(19);
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
	PrevIntake = CurIntake;
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
		GearIn();
	}
	else if (intake)
	{
		GearIn();
		if(GearIntake->GetOutputCurrent() > 25.0f)
		{
			CurIntake = 1;

			//if(GearIntakeTimer->Get() > .1)
			//{
			//	MyRobotClass::TheRobot->LEDS->Set(-0.5f);
				GearLift->SelectProfileSlot(1);
				//GearLift->SetSetpoint(ticks_to_revs(0));

			//}
		}
		else
		{
			CurIntake =0;
		//	GearIntakeTimer->Reset();
		}
	}
	else
	{
		GearIntake->Set(0);
	}
	if(GearIntake->GetOutputCurrent() > 25.0f)
	{
		intakedgear = true;
		LEDS->Set(.7f);
	}
	else if(GearIntake->GetOutputCurrent() < 25.0f)
	{
		intakedgear = false;
		LEDS->Set(-.7f);
	}
	if(!PrevIntake && CurIntake)
	{
		GearIntakeTimer->Reset();
		GearIntakeTimer->Start();
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
	float speedgain = -.1;
	for(int i = 0; i < 10; i++)
	{
		speedgain = speedgain - .075f;
		GearIntake->Set(speedgain);
	}
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
	SmartDashboard::PutBoolean("Intaked Gear?",intakedgear);
	SmartDashboard::PutNumber("Gear Intake Current",GearIntake->GetOutputCurrent());
}
void GearManipulator::ResetGearEncoder()
{
	GearLift->SetEncPosition(0);
	GearLift->SelectProfileSlot(1);
	GearLift->SetSetpoint(ticks_to_revs(0));
}
