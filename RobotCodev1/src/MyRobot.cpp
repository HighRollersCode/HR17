/*
 * MyRobot.cpp
 *
 *  Created on: Jan 7, 2017
 *      Author: 987
 */

#include <MyRobot.h>

#define IsCalibrating 1

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

	leftStick = new Joystick(0);
	rightStick = new Joystick(1);
	turretStick = new Joystick(2);
	Climber = new ClimberClass();

	Drivetrain = new Drivetrainclass(Climber->Climber_2);
	Intake = new IntakeClass();
	Turret = new TurretClass();
	ShooterWheel = new ShooterWheelClass();
	Uptake = new UptakeClass();
	Conveyor = new ConveyorClass();

	BallMng = new BallManager(Intake,Uptake,Conveyor);
	ShotMng = new ShotManager(Turret,ShooterWheel);

	GearMpltr = new GearManipulator();

	printf("Basic Initialization\r\n");
	TargClient = new TargetingSystemClient();
	TargClient->Connect(JETSON_IP,JETSON_PORT);
	printf("TargClient Initialized\r\n");
	AutonomousControl = new Auton(Drivetrain,Turret,&DriverStation::GetInstance(),TargClient,ShotMng,BallMng);//ShooterWheel,Hopper);


	Comp = new Compressor();
	Comp->SetClosedLoopControl(true);

	LightRelay = new Spark(4);

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

	m_ScriptSystem = 0;
	Init_Scripts_System();

	/*int connectionattempts = 0;
	printf("Will block to connect... \r\n");

	while ((connectionattempts < 5) && (TargClient->Get_Connected() == false))
	{
		if(ReconnectTimer->Get() > 2)
		{
			TargClient->Connect(JETSON_IP,JETSON_PORT);
			ReconnectTimer->Reset();
			connectionattempts++;
		}
	}*/
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
	LightRelay->Set(0);

	printf("Disabled\r\n");
	while(IsDisabled())
	{
		Jetson_Connection();
		Send_Data();
		Wait(0.001);
	}

}
void MyRobotClass::Reset_State()
{
	Drivetrain->StandardTank(0,0);
	Drivetrain->ResetEncoders_Timers();
	Drivetrain->Zero_Yaw();
	Turret->Tele_Start();
}
void MyRobotClass::Autonomous(void)
{
	flipauto = (DriverStation::GetInstance().GetAlliance() == DriverStation::Alliance::kBlue);

	LightRelay->Set(1);

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
		commandLeft = -LEFT_MOTOR_CMD;
		commandRight = RIGHT_MOTOR_CMD;

		if (fabs(commandLeft) < 0.2f) commandLeft = 0.0f;
		if (fabs(commandRight) < 0.2f) commandRight = 0.0f;

	#else
		commandLeft = XBoxController->GetRawAxis(1);
		commandRight = XBoxController->GetRawAxis(4);
	#endif
}
void MyRobotClass::Send_Data()
{
	if(SmartDashTimer->Get() > .02f)
	{
		SmartDashTimer->Reset();
		SmartDashboard::PutBoolean("Light", LightRelay->Get());
		SmartDashboard::PutNumber("DesiredRPM", OVERRIDE_RPM_CMD);
		SmartDashboard::PutNumber("Target Y", TargClient->Get_YOffset());
		SmartDashboard::PutNumber("Game Timer", GameTimer->Get());
		SmartDashboard::PutBoolean("DriveStation Color", flipauto);


		Drivetrain->Send_Data();
		ShotMng->Send_Data();
		TargClient->SmartDashboardUpdate();
		BallMng->SendData();
		GearMpltr->Send_Data();
		Climber->SendData();
	}
}
void MyRobotClass::OperatorControl(void)
{
	Load_Scripts();
	m_ScriptSystem->Run_Auto_Script(0);
	LightRelay->Set(1);


	Reset_State();
	Comp->SetClosedLoopControl(true);

	while (IsOperatorControl() && IsEnabled())
	{
		flipauto = (DriverStation::GetInstance().GetAlliance() == DriverStation::Alliance::kBlue);


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

		Send_Data();

		UpdateInputs();

		TargClient->Update();

		Drivetrain->StandardTank(commandLeft, commandRight);
		Drivetrain->Shifter_Update(SHIFTER_UPDATE);

		float RPM = OVERRIDE_RPM_CMD;
		if(RPM < 0)
		{
			RPM = 0;
		}
		float normz =	1.0f-((turretStick->GetZ() + 1.0f)* .5f);
		ShotMng->Update(TURRET_MOTOR_CMD,TRACKING_ENABLE,SHOOTER_ENABLE_MTR,SHOOTER_ENABLE_OVERRIDE,normz,
				RPM,TargClient->Get_XOffset(),TargClient->Get_Cal_X(),TargClient->Get_YOffset(),Compute_Robot_Velocity(),normz);

		TargClient->Set_Moving_Target_Offset(ShotMng->AdjustAngle,ShotMng->AdjustRPM);

		BallMng->Update(INTAKE_IN,INTAKE_OUT,(HOPPER_UPTAKE||ShotMng->isReady),HOPPER_OUTAKE,((rightStick->GetZ() + 1.0f) * .5f));

		if (GEAR_RESET)
		{
			GearMpltr->ResetGearEncoder();
		}

		GearMpltr->UpdateGear(GEAR_DOWN_INTAKE,GEAR_DOWN_OUTAKE,GEAR_INTAKE,GEAR_UP);

		Climber->UpdateClimber(CLIMBER_UP);

		if(SET_CAM_FRONT)
		{
			TargClient->Set_Camera_Mode(TargetingSystemClient::CAM_FRONT);
		}
		if(SET_CAM_BACK)
		{
			TargClient->Set_Camera_Mode(TargetingSystemClient::CAM_BACK);
		}

		if (SHUTDOWN_JETSON)
		{
			Shutdown_Jetson();
		}

		if(CALIBRATEJETSON)
		{
			TargClient->DoCalibrate();
		}

		Wait(0.002);
	}
}
Vector2 MyRobotClass::Compute_Robot_Velocity()
{
	Vector2 velocity;
	float speed = Drivetrain->Compute_Speed();
	float angle = Turret->Compute_Robot_Angle();
	velocity.x = -cos(angle * (3.14f/180.0f)) * speed;
	velocity.y = sin(angle * (3.14f/180.0f)) * speed;
	return velocity;
}
START_ROBOT_CLASS(MyRobotClass);
