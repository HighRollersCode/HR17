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
		MyRobot::Get()->Shutdown_Jetson();
	}
	virtual bool IsFinished() { return false; }
	virtual void End() {}
	virtual void Interrupted() {}
};

MyRobot * MyRobot::TheRobot = NULL;

MyRobot::MyRobot()
{
	TheRobot = this;

	SmartDashboard::init();

	commandLeft = 0;
	commandRight = 0;
	commandIntake = 0;
	connectionattempts = 0;

	leftStick = new Joystick(0);
	rightStick = new Joystick(1);
	turretStick = new Joystick(2);
	XBoxController = new XboxController(3);

	Drivetrain = new Drivetrainclass();
	Intake = new IntakeClass();
	Turret = new TurretClass();
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

MyRobot::~MyRobot()
{
	TheRobot = NULL;
}
void MyRobot::Jetson_Connection()
{
	if(TargClient->Get_Connected() == false && ReconnectTimer->Get())
	{
		TargClient->Connect(JETSON_IP,JETSON_PORT);
		ReconnectTimer->Reset();
	}
}
void MyRobot::Shutdown_Jetson(void)
{
	TargClient->Shutdown_Jetson();
}

void MyRobot::Disabled(void)
{
	printf("Disabled\r\n");
	Wait(0.005);
}

void MyRobot::Autonomous(void)
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
void MyRobot::UpdateInputs()
{
	#if !USING_GAMEPAD
		commandLeft = -leftStick->GetY();
		commandRight = rightStick->GetY();
	#else
		commandLeft = XBoxController->GetRawAxis(1);
		commandRight = XBoxController->GetRawAxis(4);
	#endif
}
void MyRobot::Send_Data()
{
	if(SmartDashTimer->Get() > .1f)
	{
		SmartDashTimer->Reset();

		SmartDashboard::PutNumber("Shooter Speed", turretStick->GetZ());
		SmartDashboard::PutBoolean("Light", LightRelay->Get());
		SmartDashboard::PutNumber("RPM", (((turretStick->GetZ() + 1)*.5f)* 5000.0f));
		SmartDashboard::PutNumber("Turret Speed", Turret->Turret->Get());
		SmartDashboard::PutNumber("Turret Encoder", Turret->TurretEncoder->Get());

		Drivetrain->Send_Data();
		//ShooterWheel->Send_Data();
		TargClient->SmartDashboardUpdate();
	}
}
void MyRobot::OperatorControl(void)
{
	LightRelay->Set(Relay::kForward);
	Turret->Tele_Start();
	while (IsOperatorControl() && IsEnabled())
	{

		Send_Data();

		UpdateInputs();
		TargClient->Update();
		Drivetrain->StandardTank(commandLeft, commandRight);
		Intake->UpdateIntake(leftStick->GetRawButton(3), leftStick->GetTrigger());
		#if !USING_GAMEPAD
			float RPM = (((turretStick->GetZ() + 1)*.5f)* 5000.0f);
			if(RPM < 0)
			{
				RPM = 0;
			}
			//ShooterWheel->UpdateShooter(turretStick->GetRawButton(4),turretStick->GetRawButton(10),RPM, GameTimer->Get());
			Drivetrain->Shifter_Update(rightStick->GetTrigger());
			Turret->Update(turretStick->GetX(),turretStick->GetRawButton(2),TargClient->Get_XOffset(),TargClient->Get_Cal_X(),TargClient->Get_TargetArea());

		#else
			float RPM = ((XBoxController->GetRawAxis(3)())* 5000.0f);
			if(RPM < 0)
			{
				RPM = 0;
			}
			ShooterWheel->UpdateShooter(XBoxController->GetRawButton(1),XBoxController->GetRawButton(5),RPM, GameTimer->Get());
			Drivetrain->Shifter_Update(XBoxController->GetRawButton(6));
		#endif

		Wait(0.002);
	}
}

START_ROBOT_CLASS(MyRobot);
