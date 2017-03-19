/*
 * ScriptSupport.cpp
 *
 *  Created on: Jan 8, 2017
 *      Author: 987
 */
#include "MyRobot.h"
#include "HRscript.h"
#include "HRLogger.h"


// Create commands for the robot scripts

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
										//Settings Code//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

class SetDriveTrainConstantsCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "DriveTrainSettings"; }
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new SetDriveTrainConstantsCommand(); }
	virtual void Execute()
	{
	//	MyRobotClass::Get()->AutonomousControl->turningp = (float)m_Parameters[0];
		MyRobotClass::Get()->Drivetrain->mult = (float)m_Parameters[0];

		//TiltMin = m_Parameters[1];
	}
};
class SetShooterWheelConstantsCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "ShooterWheelSettings"; }
	virtual int Get_Parameter_Count() { return 4; }
	virtual HrScriptCommandClass * Create_Command() { return new SetShooterWheelConstantsCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->ShooterWheel->SetShooterConstants(m_Parameters[0],m_Parameters[1],m_Parameters[2],
				m_Parameters[3]);//,m_Parameters[4]);
	}
};
class SetTurretConstantsCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "TurretSettings"; }
	virtual int Get_Parameter_Count() { return 4; }
	virtual HrScriptCommandClass * Create_Command() { return new SetTurretConstantsCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->Turret->SetTurretConstants(m_Parameters[0],m_Parameters[1], m_Parameters[2], m_Parameters[3]);
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
										//Wait Code//
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class WaitCommand1 : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "Wait"; }
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new WaitCommand1(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->AutonWait(m_Parameters[0]);
	}
};
class WaitForBrakeCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "BrakeWait"; }
	virtual int Get_Parameter_Count() { return 2; }
	virtual HrScriptCommandClass * Create_Command() { return new WaitForBrakeCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->AutonWait2(m_Parameters[0],(int)m_Parameters[1]);
	}
};
class WaitForTargetCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "WaitForTarget"; }
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new WaitForTargetCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->AutonWaitForTarget(m_Parameters[0]);
	}
};

/*
class DropperCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "Dropper"; }
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new DropperCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->DriveTrain->SetDropper(m_Parameters[0]);
	}
};
*/
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
										//Driving Code//
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class DisableBrakeCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "DisableBrake"; }
	virtual int Get_Parameter_Count() { return 0; }
	virtual HrScriptCommandClass * Create_Command() { return new DisableBrakeCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->AutonWait2(m_Parameters[0],(int)m_Parameters[1]);
	}
};
class DriveTimeCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "DriveTimed"; }
	virtual int Get_Parameter_Count() { return 3; }
	virtual HrScriptCommandClass * Create_Command() { return new DriveTimeCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->Auto_DriveTimer(m_Parameters[0], m_Parameters[1],m_Parameters[2]);
	}
};
class DriveCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "Drive"; }
	virtual int Get_Parameter_Count() { return 2; }
	virtual HrScriptCommandClass * Create_Command() { return new DriveCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->Drivetrain->Drive_Auton(m_Parameters[0], m_Parameters[1]);
	}
};
class DriveHeadingTicksCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "DriveTicksHeading"; }
	virtual int Get_Parameter_Count() { return 3; }
	virtual HrScriptCommandClass * Create_Command() { return new DriveHeadingTicksCommand(); }
	virtual void Execute()
	{
		if(MyRobotClass::Get()->flipauto)
		{
			MyRobotClass::Get()->AutonomousControl->Auto_GYROSTRAIGHT(m_Parameters[0], m_Parameters[1],-m_Parameters[2]);
		}
		else
		{
			MyRobotClass::Get()->AutonomousControl->Auto_GYROSTRAIGHT(m_Parameters[0], m_Parameters[1],m_Parameters[2]);
		}
	}
};
class DriveTicksCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "DriveTicks"; }
	virtual int Get_Parameter_Count() { return 3; }
	virtual HrScriptCommandClass * Create_Command() { return new DriveTicksCommand(); }
	virtual void Execute()
	{
		if(MyRobotClass::Get()->flipauto)
		{
			MyRobotClass::Get()->AutonomousControl->Auto_DriveEncoder(m_Parameters[0], -m_Parameters[1],m_Parameters[2]);
		}
		else
		{
			MyRobotClass::Get()->AutonomousControl->Auto_DriveEncoder(m_Parameters[0], m_Parameters[1],m_Parameters[2]);
		}
	}
};
class GyroTurnCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "GyroTurn"; }
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new GyroTurnCommand(); }
	virtual void Execute()
	{
		if(MyRobotClass::Get()->flipauto)
		{
			MyRobotClass::Get()->AutonomousControl->Auto_GYROTURN(-m_Parameters[0]);
		}
		else
		{
			MyRobotClass::Get()->AutonomousControl->Auto_GYROTURN(m_Parameters[0]);
		}

	}
};
class GyroTurnTimeCommand : public HrScriptCommandClass
{

public:
	virtual const char * Get_Command_Name() { return "GyroTurnTimed"; }
	virtual int Get_Parameter_Count() { return 2; }
	virtual HrScriptCommandClass * Create_Command() { return new GyroTurnTimeCommand(); }
	virtual void Execute()
	{
		if(MyRobotClass::Get()->flipauto)
		{
			MyRobotClass::Get()->AutonomousControl->Auto_GYROTURN_TIMED(-m_Parameters[0],m_Parameters[1]);
		}
		else
		{
			MyRobotClass::Get()->AutonomousControl->Auto_GYROTURN_TIMED(m_Parameters[0],m_Parameters[1]);
		}

	}
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
										//Arm Code//
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class TurretEnablePIDCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "TurretEnablePID"; }
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new TurretEnablePIDCommand(); }
	virtual void Execute()
	{
#if TURRET_TALON_CONTROL
		if(m_Parameters[0] == 1)
		{
			MyRobotClass::Get()->Turret->Turret->SetControlMode(CANTalon::kPosition);
		}
		else if(m_Parameters[1] == 0)
		{
			MyRobotClass::Get()->Turret->Turret->SetControlMode(CANTalon::kPercentVbus);
		}
#else
		MyRobotClass::Get()->Turret->TurretPIDController->Reset();
		if(m_Parameters[0] == 1)
		{
			MyRobotClass::Get()->Turret->TurretPIDController->Enable();
		}
		else if (m_Parameters[0] == 0)
		{
			MyRobotClass::Get()->Turret->TurretPIDController->Disable();
		}
#endif
	}
};
class TrackingCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() {return "Tracking"; }
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new TrackingCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->dotrack = m_Parameters[0];
	}
};
class SetTurretCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "Turret"; }
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new SetTurretCommand(); }
	virtual void Execute()
	{
		if(MyRobotClass::Get()->flipauto)
		{
			MyRobotClass::Get()->Turret->SetTurret(-(int)m_Parameters[0]);
		}
		else
		{
			MyRobotClass::Get()->Turret->SetTurret((int)m_Parameters[0]);
		}
	}
};
class ShootingCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "Shooting"; }
	virtual int Get_Parameter_Count() { return 2; }
	virtual HrScriptCommandClass * Create_Command() { return new ShootingCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->uptake = m_Parameters[0];
		MyRobotClass::Get()->AutonomousControl->intake = m_Parameters[0];
		MyRobotClass::Get()->AutonomousControl->downtake = m_Parameters[1];
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
										//Intake Code//
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

/*class SetIntakeLiftCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "IntakeLift"; }
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new SetIntakeLiftCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->Intake->SetLift((int)m_Parameters[0]);
	}
};*/
class GearManipulatorCommand : public HrScriptCommandClass
{

public:
	virtual const char * Get_Command_Name() { return "GearManipulator"; }
	virtual int Get_Parameter_Count() { return 2; }
	virtual HrScriptCommandClass * Create_Command() { return new GearManipulatorCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->GearMpltr->UpdateGear(0,m_Parameters[0],0,m_Parameters[1]);
	}
};
class SetIntakeCommand : public HrScriptCommandClass
{

public:
	virtual const char * Get_Command_Name() { return "Intake"; }
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new SetIntakeCommand(); }
	virtual void Execute()
	{
		if(m_Parameters[0] == 1)
		{
			MyRobotClass::Get()->AutonomousControl->Auto_Intake_On();
		}
		else if (m_Parameters[0] == 0)
		{
			MyRobotClass::Get()->AutonomousControl->Auto_Intake_Off();
		}
		else if (m_Parameters[0] == -1)
		{
			MyRobotClass::Get()->Intake->IntakeOut();
		}
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
										//Turret, Tilt Code//
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
										//Support Code//
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class SetAutoCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "SetAuto"; }
	virtual int Get_Parameter_Count() { return 1; }

	virtual HrScriptCommandClass * Create_Command() { return new SetAutoCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->Auto_Index = (int)m_Parameters[0];
		//MyRobotClass::Get()->Right_Auto_Index = (int)m_Parameters[1];
		//MyRobotClass::Get()->Enable_Tracking = (int)m_Parameters[2];
		printf("setupautos");
	}
};

void MyRobotClass::Init_Scripts_System()
{
	m_ScriptSystem = new HrScriptSystemClass();
	printf("Script Initializing \r\n");
	// register commands with the system
	///////////////////////////////////////////////////////////////////////////////////////////////

	//m_ScriptSystem->Add_Command(new SetEBrakeConstantsCommand());
	m_ScriptSystem->Add_Command(new SetDriveTrainConstantsCommand());
	m_ScriptSystem->Add_Command(new SetShooterWheelConstantsCommand());
	m_ScriptSystem->Add_Command(new SetTurretConstantsCommand());


	///////////////////////////////////////////////////////////////////////////////////////////////

	m_ScriptSystem->Add_Command(new WaitCommand1());
	m_ScriptSystem->Add_Command(new WaitForBrakeCommand());
	m_ScriptSystem->Add_Command(new WaitForTargetCommand());

	///////////////////////////////////////////////////////////////////////////////////////////////

	m_ScriptSystem->Add_Command(new DisableBrakeCommand());
	m_ScriptSystem->Add_Command(new DriveTimeCommand());
	m_ScriptSystem->Add_Command(new DriveHeadingTicksCommand());
	m_ScriptSystem->Add_Command(new DriveTicksCommand());
	m_ScriptSystem->Add_Command(new DriveCommand());
	m_ScriptSystem->Add_Command(new GyroTurnCommand());
	m_ScriptSystem->Add_Command(new GyroTurnTimeCommand());

	///////////////////////////////////////////////////////////////////////////////////////////////

	m_ScriptSystem->Add_Command(new SetTurretCommand());
	m_ScriptSystem->Add_Command(new ShootingCommand());
	m_ScriptSystem->Add_Command(new TrackingCommand());
	m_ScriptSystem->Add_Command(new TurretEnablePIDCommand());

	///////////////////////////////////////////////////////////////////////////////////////////////
	m_ScriptSystem->Add_Command(new GearManipulatorCommand());
	m_ScriptSystem->Add_Command(new SetIntakeCommand());
	//m_ScriptSystem->Add_Command(new DropperCommand());


	///////////////////////////////////////////////////////////////////////////////////////////////

	m_ScriptSystem->Add_Command(new SetAutoCommand());

	///////////////////////////////////////////////////////////////////////////////////////////////

	Load_Scripts();

}
void MyRobotClass::Load_Scripts()
{
	// Load and run the robot settings script:
	m_ScriptSystem->Load_And_Run_Script("RobotSettings.hrs");

	// Load all of the auto-mode scripts

	//Red Side Autos
	m_ScriptSystem->Set_Auto_Script(111,"GEAR_RIGHT.hrs");
	m_ScriptSystem->Set_Auto_Script(112,"GEAR_MIDDLE.hrs");
	m_ScriptSystem->Set_Auto_Script(113,"GEAR_LEFT.hrs");
	m_ScriptSystem->Set_Auto_Script(121,"FARHOPPER_RIGHT.hrs");
	m_ScriptSystem->Set_Auto_Script(122,"FARHOPPER_MIDDLE.hrs");
	m_ScriptSystem->Set_Auto_Script(131,"CLOSEHOPPER_RIGHT.hrs");
	m_ScriptSystem->Set_Auto_Script(132,"CLOSEHOPPER_MIDDLE.hrs");
	m_ScriptSystem->Set_Auto_Script(141,"RIGHTGEAR_FARHOPPER.hrs");
	m_ScriptSystem->Set_Auto_Script(142,"RIGHTGEAR_CLOSEHOPPER.hrs");
	m_ScriptSystem->Set_Auto_Script(151,"MIDDLEGEAR_CLOSEHOPPER.hrs");

	//Blue Side Auto
	m_ScriptSystem->Set_Auto_Script(241,"RIGHTGEAR_FARHOPPER_BLUETWEAK.hrs");
	m_ScriptSystem->Set_Auto_Script(242,"RIGHTGEAR_CLOSEHOPPER_BLUETWEAK.hrs");
	m_ScriptSystem->Set_Auto_Script(251,"MIDDLEGEAR_CLOSEHOPPER_BLUETWEAK.hrs");
	//EmptyAuto
	m_ScriptSystem->Set_Auto_Script(999,"EMPTY.hrs");

	//Settings
	m_ScriptSystem->Set_Auto_Script(0,"RobotSettings.hrs");
}
