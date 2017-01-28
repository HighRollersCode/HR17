/*
 * MyRobot.cpp
 *
 *  Created on: Jan 7, 2017
 *      Author: 987
 */

#include <MyRobot.h>

class ShutdownJetsonCommand : public Command
{
public:
	ShutdownJetsonCommand() {}
	virtual ~ShutdownJetsonCommand() {}

protected:
	virtual void Initialize()
	{
		MyRobotClass::Get()->Shutdown_Jetson();
	}
	virtual bool IsFinished() { return false; }
	virtual void End() {}
	virtual void Interrupted() {}
};

MyRobotClass * MyRobotClass::TheRobot = NULL;

MyRobotClass::MyRobotClass()
{
	TheRobot = this;

	SmartDashboard::init();

	commandLeft = 0;
	commandRight = 0;
	commandIntake = 0;
	connectionattempts = 0;
	intele = 0;


	leftStick = new Joystick(0);
	rightStick = new Joystick(1);
	turretStick = new Joystick(2);
	XBoxController = new XboxController(3);

	Drivetrain = new Drivetrainclass();
	Intake = new IntakeClass();
	Turret = new TurretClass();
	ShooterWheel = new ShooterWheelClass();
	Hopper = new HopperClass();
	//ShotMng = new ShotManager(Turret,Hopper,ShooterWheel);
	printf("Basic Initialization\r\n");
	TargClient = new TargetingSystemClient();
	TargClient->Connect(JETSON_IP,JETSON_PORT);
	printf("TargClient Initialized\r\n");
	//AutonomousControl = new Auton(Drivetrain,Turret,&DriverStation::GetInstance(),Intake,ShooterWheelClass,TargClient);

	LightRelay = new Relay(0,Relay::kBothDirections);

	ReconnectTimer = new Timer();
	ReconnectTimer->Reset();
	ReconnectTimer->Start();

	GameTimer = new Timer();

	SendTimer = new Timer();
	SendTimer->Reset();
	SendTimer->Start();

	SmartDashTimer = new Timer();
	SmartDashTimer->Reset();
	SmartDashTimer->Start();

	JetsonConnected = false;

	DisConnectionPrevTog = false;
	DisConnectionCurTog = false;

	Auto_Index = 0;
	m_ScriptSystem = 0;
	Init_Scripts_System();

	int connectionattempts = 0;
	printf("Will block to connect... \r\n");
	while ((connectionattempts < 5) && (TargClient->Get_Connected() == false))
	{
		if(ReconnectTimer->Get() > 2)
		{
			TargClient->Connect(JETSON_IP,JETSON_PORT);
			ReconnectTimer->Reset();
			connectionattempts++;
		}
	}
}

MyRobotClass::~MyRobotClass()
{
	TheRobot = NULL;
}
void MyRobotClass::Jetson_Connection()
{
	if(TargClient->Get_Connected() == false && ReconnectTimer->Get() > 5)
	{
		TargClient->Connect(JETSON_IP,JETSON_PORT);
		ReconnectTimer->Reset();
	}
}
void MyRobotClass::Shutdown_Jetson(void)
{
	TargClient->Shutdown_Jetson();
}

void MyRobotClass::Disabled(void)
{
	printf("Disabled\r\n");
	while(IsDisabled())
	{
		Jetson_Connection();
		Send_Data();
	}
	Wait(0.001);
}
void MyRobotClass::Reset_State()
{
	Drivetrain->StandardTank(0,0);
	Turret->Tele_Start();
}
void MyRobotClass::Autonomous(void)
{
	Load_Scripts();
		printf("loaded\n");
		m_ScriptSystem->Run_Auto_Script(0);
		printf("ransettings\n");
		AutonomousControl->Auto_Start();
		printf("startfunction\n");
		m_ScriptSystem->Run_Auto_Script(Auto_Index);
		AutonomousControl->Auto_End();
}
void MyRobotClass::UpdateInputs()
{
	#if !USING_GAMEPAD
		commandLeft = -leftStick->GetY();
		commandRight = rightStick->GetY();
	#else
		commandLeft = XBoxController->GetRawAxis(1);
		commandRight = XBoxController->GetRawAxis(4);
	#endif
}
void MyRobotClass::Send_Data()
{
	if(SmartDashTimer->Get() > .1f)
	{
		SmartDashTimer->Reset();
		SmartDashboard::PutBoolean("Light", LightRelay->Get());
		SmartDashboard::PutNumber("DesiredRPM", (((turretStick->GetZ() + 1)*.5f)* 5000.0f));

		Drivetrain->Send_Data();
		ShooterWheel->Send_Data();
		Turret->Send_Data();
		TargClient->SmartDashboardUpdate();
	}
}
void MyRobotClass::OperatorControl(void)
{
	LightRelay->Set(Relay::kForward);
	Reset_State();
	while (IsOperatorControl())
	{
		int prevtele = intele;
		intele  = 1;
		if(prevtele == 0 && intele == 1)
		{
			GameTimer->Reset();
			GameTimer->Start();
		}

		if(GameTimer->Get() > 134.9)
		{
			if(DriverStation::GetInstance().IsFMSAttached())
			{
				Shutdown_Jetson();
			}
		}
		if (IsDisabled())
		{
			Jetson_Connection();
			Reset_State();
		}

		Send_Data();
		UpdateInputs();
		TargClient->Update();
		Drivetrain->StandardTank(commandLeft, commandRight);
		Intake->UpdateIntake(rightStick->GetTrigger(), rightStick->GetRawButton(2));
		Hopper->UpdateHopper(turretStick->GetTrigger(), turretStick->GetRawButton(11));
		float RPM = (((turretStick->GetZ() + 1.0f)*.5f)* 5000.0f);
		if(RPM < 0)
		{
			RPM = 0;
		}
		ShooterWheel->UpdateShooter(turretStick->GetRawButton(4),turretStick->GetRawButton(10),RPM,turretStick->GetRawButton(2),
			TargClient->Get_YOffset());
		Drivetrain->Shifter_Update(leftStick->GetTrigger());
		Turret->Update(turretStick->GetX(),turretStick->GetRawButton(2),TargClient->Get_XOffset(),TargClient->Get_Cal_X(),
			TargClient->Get_TargetArea());
		//ShotMng->Update(turretStick->GetX(),turretStick->GetRawButton(2),TargClient->Get_XOffset(),TargClient->Get_YOffset());
		if(rightStick->GetRawButton(10))
		{
			TargClient->Set_Camera_Mode(TargetingSystemClient::CAM_FRONT);
		}
		if(rightStick->GetRawButton(11))
		{
			TargClient->Set_Camera_Mode(TargetingSystemClient::CAM_BACK);
		}
		if (turretStick->GetRawButton(8) && turretStick->GetRawButton(9))
		{
			Shutdown_Jetson();
		}

//		#else
//			float RPM = ((XBoxController->GetRawAxis(3)())* 5000.0f);
//			if(RPM < 0)
//			{
//				RPM = 0;
//			}
//			ShooterWheel->UpdateShooter(XBoxController->GetRawButton(1),XBoxController->GetRawButton(5),RPM, GameTimer->Get());
//			Drivetrain->Shifter_Update(XBoxController->GetRawButton(6));
//		#endif

		Wait(0.002);
	}
}

START_ROBOT_CLASS(MyRobotClass);
