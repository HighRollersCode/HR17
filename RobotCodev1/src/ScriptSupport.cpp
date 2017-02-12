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
		MyRobotClass::Get()->Turret->SetTurretConstants(m_Parameters[0],m_Parameters[1], m_Parameters[2]);
		MyRobotClass::Get()->Turret->MIN_TURRET_CMD = m_Parameters[3];
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
		MyRobotClass::Get()->AutonomousControl->Auto_GYROSTRAIGHT(m_Parameters[0], m_Parameters[1],m_Parameters[2]);
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
		MyRobotClass::Get()->AutonomousControl->Auto_DriveEncoder(m_Parameters[0], m_Parameters[1],m_Parameters[2]);
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
		MyRobotClass::Get()->AutonomousControl->Auto_GYROTURN(m_Parameters[0]);
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
		MyRobotClass::Get()->AutonomousControl->Auto_GYROTURN_TIMED(m_Parameters[0],m_Parameters[1]);
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
		MyRobotClass::Get()->ShotMng->StartTracking((int)m_Parameters[0]);
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
		MyRobotClass::Get()->Turret->SetTurret((int)m_Parameters[0]);
	}
};
/*class FullShotCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "FullShot"; }
	virtual int Get_Parameter_Count() { return 0; }
	virtual HrScriptCommandClass * Create_Command() { return new FullShotCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->Arm->FullShot();
	}
};*/

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
		MyRobotClass::Get()->GearMpltr->UpdateGear(0,m_Parameters[0],0,m_Parameters[1],0);
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
	//m_ScriptSystem->Add_Command(new FullShotCommand());
	m_ScriptSystem->Add_Command(new TrackingCommand());
	m_ScriptSystem->Add_Command(new TurretEnablePIDCommand());

	///////////////////////////////////////////////////////////////////////////////////////////////
	//m_ScriptSystem->Add_Command(new SetIntakeLiftCommand());*/
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
	m_ScriptSystem->Set_Auto_Script(1,"GEAR_MIDDLE_RED.hrs");
	m_ScriptSystem->Set_Auto_Script(11,"GEAR_RIGHT_RED.hrs");
	m_ScriptSystem->Set_Auto_Script(12,"GEAR_LEFT_RED.hrs");
	m_ScriptSystem->Set_Auto_Script(2,"GEAR_MIDDLE_BLUE.hrs");
	m_ScriptSystem->Set_Auto_Script(21,"GEAR_RIGHT_BLUE.hrs");
	m_ScriptSystem->Set_Auto_Script(22,"GEAR_LEFT_BLUE.hrs");
	m_ScriptSystem->Set_Auto_Script(3,"HOPPER_CLOSE_RED.hrs");
	m_ScriptSystem->Set_Auto_Script(31,"HOPPER_MIDDLE_RED.hrs");
	m_ScriptSystem->Set_Auto_Script(4,"HOPPER_CLOSE_BLUE.hrs");
	m_ScriptSystem->Set_Auto_Script(41,"HOPPER_MIDDLE_BLUE.hrs");
	m_ScriptSystem->Set_Auto_Script(999,"EMPTY.hrs");

	//Settings
	m_ScriptSystem->Set_Auto_Script(0,"RobotSettings.hrs");
}
