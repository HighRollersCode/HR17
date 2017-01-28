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

class WaitForIntakeModeCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "WaitForIntakeMode"; }
	virtual int Get_Parameter_Count() { return 0; }
	virtual HrScriptCommandClass * Create_Command() { return new WaitForIntakeModeCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->AutonomousControl->AutonWaitForIntake();
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

/*class ArmEnablePIDCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "ArmEnablePID"; }
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new ArmEnablePIDCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->Arm->ArmPIDController->Reset();
		if(m_Parameters[0] == 1)
		{
			MyRobotClass::Get()->Arm->ArmPIDController->Enable();
		}
		else if (m_Parameters[0] == 0)
		{
			MyRobotClass::Get()->Arm->ArmPIDController->Disable();
		}
	}
};

class SetArmStartPositionCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "ArmStartPosition"; }
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new SetArmStartPositionCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->Arm->SetArmStartPosition((int)m_Parameters[0]);
	}
};*/

/*class TurretEnablePIDCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "TurretEnablePID"; }
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new TurretEnablePIDCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->Turret->TurretPIDController->Reset();
		if(m_Parameters[0] == 1)
		{
			MyRobotClass::Get()->Turret->TurretPIDController->Enable();
		}
		else if (m_Parameters[0] == 0)
		{
			MyRobotClass::Get()->Turret->TurretPIDController->Disable();
		}
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
		//enable tracking if parameter 0 is true
		MyRobotClass::Get()->Arm->StartTracking((int)m_Parameters[0]);
	}
};
class SetArmCommand : public HrScriptCommandClass
{

public:
	virtual const char * Get_Command_Name() { return "Arm"; }
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new SetArmCommand(); }
	virtual void Execute()
	{
		if((int)m_Parameters[0] != -1)
		{
			MyRobotClass::Get()->Arm->SetTurret((int)m_Parameters[0]);
		}
		if((int)m_Parameters[1] != -1)
		{
		MyRobotClass::Get()->Arm->SetArm((int)m_Parameters[1]);
		}
	}
};
class FullShotCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "FullShot"; }
	virtual int Get_Parameter_Count() { return 0; }
	virtual HrScriptCommandClass * Create_Command() { return new FullShotCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->Arm->FullShot();
	}
};

class FullShotQuickCommand : public HrScriptCommandClass
public:
	virtual const char * Get_Command_Name() { return "FullShotQuick"; }
	virtual int Get_Parameter_Count() { return 0; }
	virtual HrScriptCommandClass * Create_Command() { return new FullShotQuickCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->Arm->FullShotQuick();
	}
};*/
class ShooterWheelsCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "Shooter"; }
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new ShooterWheelsCommand(); }
	virtual void Execute()
	{
		if((int)m_Parameters[0] == 1)
		{
			//MyRobotClass::Get()->ShooterWheel->ShooterOutake();
		}
		else
		{
			//MyRobotClass::Get()->ShooterWheel->WheelOff();
		}
	}
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
										//Intake Code//
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

/*class SetIntakeStartPositionCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "IntakeStartPosition"; }
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new SetIntakeStartPositionCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->Intake->SetIntakeStartPosition((int)m_Parameters[0]);
	}
};

class SetIntakeLiftCommand : public HrScriptCommandClass
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
/*class EnterIntakeModeCommand : public HrScriptCommandClass
{

public:
	virtual const char * Get_Command_Name() { return "EnterIntakeMode"; }
	virtual int Get_Parameter_Count() { return 0; }
	virtual HrScriptCommandClass * Create_Command() { return new EnterIntakeModeCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->CollManager->EnterState(CollisionManager::Intake);
	}
};
class EnterDefensiveModeCommand : public HrScriptCommandClass
{

public:
	virtual const char * Get_Command_Name() { return "EnterDefensiveMode"; }
	virtual int Get_Parameter_Count() { return 0; }
	virtual HrScriptCommandClass * Create_Command() { return new EnterDefensiveModeCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->CollManager->EnterState(CollisionManager::Defensive);
	}
};
class IntakeEnablePIDCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "IntakeEnablePID"; }
	virtual int Get_Parameter_Count() { return 1; }
	virtual HrScriptCommandClass * Create_Command() { return new IntakeEnablePIDCommand(); }
	virtual void Execute()
	{
		MyRobotClass::Get()->Intake->LiftPIDController->Reset();
		if(m_Parameters[0] == 1)
		{
			MyRobotClass::Get()->Intake->LiftPIDController->Enable();
		}
		else if (m_Parameters[0] == 0)
		{
			MyRobotClass::Get()->Intake->LiftPIDController->Disable();
		}
	}
};*/

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

	///////////////////////////////////////////////////////////////////////////////////////////////

	m_ScriptSystem->Add_Command(new WaitCommand1());
	m_ScriptSystem->Add_Command(new WaitForBrakeCommand());
	m_ScriptSystem->Add_Command(new WaitForTargetCommand());
	m_ScriptSystem->Add_Command(new WaitForIntakeModeCommand());

	///////////////////////////////////////////////////////////////////////////////////////////////

	m_ScriptSystem->Add_Command(new DisableBrakeCommand());
	m_ScriptSystem->Add_Command(new DriveTimeCommand());
	m_ScriptSystem->Add_Command(new DriveHeadingTicksCommand());
	m_ScriptSystem->Add_Command(new DriveTicksCommand());
	m_ScriptSystem->Add_Command(new DriveCommand());
	m_ScriptSystem->Add_Command(new GyroTurnCommand());
	m_ScriptSystem->Add_Command(new GyroTurnTimeCommand());

	///////////////////////////////////////////////////////////////////////////////////////////////

	/*m_ScriptSystem->Add_Command(new SetArmCommand());
	m_ScriptSystem->Add_Command(new SetArmStartPositionCommand());
	m_ScriptSystem->Add_Command(new FullShotCommand());
	m_ScriptSystem->Add_Command(new FullShotQuickCommand());*/
	m_ScriptSystem->Add_Command(new ShooterWheelsCommand());
	/*m_ScriptSystem->Add_Command(new TrackingCommand());
	m_ScriptSystem->Add_Command(new ArmEnablePIDCommand());
	m_ScriptSystem->Add_Command(new TurretEnablePIDCommand());*/

	///////////////////////////////////////////////////////////////////////////////////////////////
	/*m_ScriptSystem->Add_Command(new SetIntakeStartPositionCommand());
	m_ScriptSystem->Add_Command(new SetIntakeLiftCommand());*/
	m_ScriptSystem->Add_Command(new SetIntakeCommand());
	//m_ScriptSystem->Add_Command(new EnterIntakeModeCommand());
	//m_ScriptSystem->Add_Command(new EnterDefensiveModeCommand());
	/*m_ScriptSystem->Add_Command(new IntakeEnablePIDCommand());

	m_ScriptSystem->Add_Command(new DropperCommand());*/


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
	m_ScriptSystem->Set_Auto_Script(1,"GEAR_SHOOTER_RED.hrs");
	m_ScriptSystem->Set_Auto_Script(2,"GEAR_SHOOTER_BLUE.hrs");
	m_ScriptSystem->Set_Auto_Script(999,"EMPTY.hrs");

	/*m_ScriptSystem->Set_Auto_Script(2,"SerpentineStrafe.hrs");
	m_ScriptSystem->Set_Auto_Script(3,"Autoconhook.hrs");
	m_ScriptSystem->Set_Auto_Script(4,"StealCans.hrs");
	m_ScriptSystem->Set_Auto_Script(2,"AUTO_TWOBALL.hrs");
	m_ScriptSystem->Set_Auto_Script(3,"AUTO_TWOBALLPUSH.hrs");
	m_ScriptSystem->Set_Auto_Script(4,"AUTO_THREEBALL.hrs");
	m_ScriptSystem->Set_Auto_Script(12,"AUTO_THREE_RIGHT.hrs");
	m_ScriptSystem->Set_Auto_Script(13,"AUTO_THREE_LEFT.hrs");

	m_ScriptSystem->Set_Auto_Script(14,"AUTO_TWO_RIGHT.hrs");
	m_ScriptSystem->Set_Auto_Script(15,"AUTO_TWO_LEFT.hrs");
	m_ScriptSystem->Set_Auto_Script(17,"AUTO_TWO_LEFT_FLOPPY.hrs");

	m_ScriptSystem->Set_Auto_Script(21,"AUTO_GOALIE_LEFT.hrs");
	m_ScriptSystem->Set_Auto_Script(22,"AUTO_GOALIE_RIGHT.hrs");
	m_ScriptSystem->Set_Auto_Script(31,"AUTO_ONEBALL_NOWAIT.hrs");
	m_ScriptSystem->Set_Auto_Script(32,"AUTO_ONEBALL_WAIT.hrs");

	m_ScriptSystem->Set_Auto_Script(44,"AUTO_ONE_RIGHT.hrs");
	m_ScriptSystem->Set_Auto_Script(45,"AUTO_ONE_LEFT.hrs");
	m_ScriptSystem->Set_Auto_Script(49,"AUTO_TWOBALL.hrs");

	m_ScriptSystem->Set_Auto_Script(50,"AUTO_GOALIE_RIGHT_ONE_WAIT.hrs");
	m_ScriptSystem->Set_Auto_Script(51,"AUTO_GOALIE_RIGHT_ONE_NOWAIT.hrs");

	m_ScriptSystem->Set_Auto_Script(52,"AUTO_GOALIE_LEFT_ONE_WAIT.hrs");
	m_ScriptSystem->Set_Auto_Script(53,"AUTO_GOALIE_LEFT_ONE_NOWAIT.hrs");
	m_ScriptSystem->Set_Auto_Script(59,"AUTO_FORWARD.hrs");
	*/

	//Settings
	m_ScriptSystem->Set_Auto_Script(0,"RobotSettings.hrs");

}




