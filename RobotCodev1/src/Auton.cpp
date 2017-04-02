/*
 * Auton.cpp
 *
 *  Created on: Jan 29, 2016
 *      Author: HighRollers
 */

#include "Auton.h"
#include "HRLogger.h"
#include "math.h"
#include "stdio.h"
#include "Defines.h"
#include "MyRobot.h"


Auton::Auton
(
Drivetrainclass *D,
TurretClass *Ts,
DriverStation *Ds,
TargetingSystemClient *T,
ShotManager *SM,
BallManager *BM
)
{
	DriveTrain = D;
	Turret = Ts;
	HRLogger::Log("autodrive created\r\n");
	ds = Ds;
	Targeting = T;
	ShotMng = SM;
	BallMng = BM;
	//turningp = -.1f;
	AutonTimer = new Timer();
	SendTimer =new Timer();
}
void Auton::Auto_Start()
{
	DriveTrain->Zero_Yaw();
	AutonTimer->Reset();
	AutonTimer->Start();

	SendTimer->Reset();
	SendTimer->Start();
	DriveTrain->ResetEncoders_Timers();
	Turret->CurrentEnableTracking = false;
	//Intake->ResetEncoderLift();

	Turret->Auto_Start();
}
void Auton::Auto_End()
{

}
bool Auton::Running()
{
	if(Abort)
	{
		printf("ABORTING: ABORT SET %f /r/n", AutonTimer->Get());
		return false;
	}
	else if (ds->IsOperatorControl() == true)
	{
		printf("ABORTING: OPERATOR CONTROL %f /r/n", AutonTimer->Get());
		return false;
	}
	else if (ds->IsEnabled() == false)
	{
		printf("ABORTING: DS DISABLED %f /r/n", AutonTimer->Get());
		return false;

	}
	return true;
}
void Auton::AutonWait(float Seconds)
{
	float targ = AutonTimer->Get() + Seconds;
	while((AutonTimer->Get() < targ)&&(Running()))
	{
		Auto_System_Update();
	}
}
void Auton::AutonWait2(float Seconds,int brake)
{
	float targ = AutonTimer->Get() + Seconds;
	//DriveTrain->ResetEncoders_Timers();
	while((AutonTimer->Get() < targ)&&(Running()))
	{
		Auto_System_Update();
		//DriveTrain->UpdateEBrake(1,brake);
	}
	DriveTrain->Drive_Auton(0.0f,0.0f);
}
void Auton::AutonWaitForTarget(float Seconds)
{
	const float MAX_X_ERROR = 1.75f; //1.8f;
	//const float MAX_Y_ERROR = 2.0f; //2.7f;
	const float MIN_LOCK_TIME = 0.30f;

	float targ = AutonTimer->Get() + Seconds;
	bool keep_waiting = true;
	float lock_time = 0.0f;
	float prevtime = AutonTimer->Get();
	float deltatime = 0.0f;

	while(keep_waiting && Running())
	{
		float curtime = AutonTimer->Get();
		deltatime = curtime - prevtime;
		prevtime = curtime;

		Auto_System_Update();
		if ((fabs(Turret->LastMoveByDegreesX) < MAX_X_ERROR))// && (fabs(Turret->LastMoveByDegreesY) < MAX_Y_ERROR))
		{
			lock_time += deltatime;
		}
		else
		{
			lock_time = 0.0f;
		}
		keep_waiting = (AutonTimer->Get() < targ) && (lock_time < MIN_LOCK_TIME);
	}
}
void Auton::AutonWaitForIntake()
{
	bool wait = true;
	while(wait)
	{
		/*bool lowenough = false;
		if(Arm->GetLifterEncoder() < -5500)
		{
			lowenough = true;
		}
		else
		{
			lowenough = false;
		}
		wait = (Collision->currentMode == CollisionManager::Intake);
		wait &= (Collision->transitioning);
		wait &= !lowenough;
		wait &= (Running());*/
		Auto_System_Update();
	}
}
void Auton::Auto_DriveTimer(float Forward, float Turn, float seconds)
{
	DriveTrain->Drive_Auton(Forward, Turn);
	AutonWait(seconds);
	DriveTrain->Drive_Auton(0.0f, 0.0f);
}
void Auton::Auto_GYROTURN(float heading)
{
	DriveTrain->ResetEncoders_Timers();

	float angle_error = DriveTrain->ComputeAngleDelta(heading);
	float turn = DriveTrain->mult * angle_error;
	printf("initial error: %f", angle_error);
	while((fabs(angle_error) > 0.0f)&&(Running()))
	{
		angle_error = DriveTrain->ComputeAngleDelta(heading);
		turn = DriveTrain->mult * angle_error;
		printf(" error: %f", angle_error);
		Auto_System_Update();
		DriveTrain->StandardArcade(0,turn);
		SmartDashboard::PutNumber("Turn", turn);
		SmartDashboard::PutNumber("gyroerror",angle_error);
	}
}
void Auton::Auto_GYROTURN_TIMED(float heading, float seconds)
{
	printf("Timing");
	DriveTrain->ResetEncoders_Timers();

	float timtarg = AutonTimer->Get()+seconds;

	while(AutonTimer->Get() < timtarg && Running())
	{
		float angle_error = DriveTrain->ComputeAngleDelta(heading);
		float turn = DriveTrain->mult* angle_error;

		DriveTrain->StandardArcade(0,turn);
	}
	DriveTrain->Drive_Auton(0,0);
}
void Auton::Auto_GYROSTRAIGHT(float forward, float ticks, float desheading)
{
	DriveTrain->ResetEncoders_Timers();
	float MAINTAIN = desheading;
	float GYRO_P = DriveTrain->mult;

	float angle_error = MAINTAIN - DriveTrain->GetHeading();
	float turn = GYRO_P * angle_error;
	if(ticks > 0)
	{
		while((DriveTrain->GetLeftEncoder() < ticks)&&(DriveTrain->GetRightEncoder() < ticks)&&(Running()))
		{
			float err = DriveTrain->ComputeAngleDelta(MAINTAIN);
			turn = err * GYRO_P;
			SmartDashboard::PutNumber("Gyro Error", err);

			DriveTrain->StandardArcade(forward,turn);
			Auto_System_Update();
			//DriveTrain->FailSafe_Update;
		}
	}
	else if (ticks < 0)
	{
		while((DriveTrain->GetLeftEncoder() > ticks)&&(DriveTrain->GetRightEncoder() > ticks)&&(Running()))
		{
			float err = DriveTrain->ComputeAngleDelta(MAINTAIN);
			turn = err * GYRO_P;

			DriveTrain->StandardArcade(-forward, turn);
			Auto_System_Update();
			//DriveTrain->Failsafe_Update();
		}
	}
	DriveTrain->Drive_Auton(0,0);
}
void Auton::Auto_DriveGyro_Encoder(float Forward, float Angle, float Ticks)
{
	DriveTrain->ResetEncoders_Timers();
	Auto_GYROTURN(Angle);
	AutonWait(.7f);
	Auto_GYROSTRAIGHT(Forward, Ticks, Angle);
}

bool Auton::Auto_System_Update()
{
	if(Running())
	{
		Targeting->Update();
		ShotMng->Update(0,dotrack,false,dorpmoverride,0,rpmoverride,Targeting->Get_XOffset(),Targeting->Get_Cal_X(),Targeting->Get_YOffset(), MyRobotClass::Get()->Compute_Robot_Velocity(),0);
		BallMng->Update(intake,outake,(uptake || ShotMng->isReady),downtake,0);

		SendData();

		if(AutonTimer->Get() > 14.95)
		{
		}
		Wait(.001f);
	}
	return true;
}
void Auton::SendData()
{
	if(SendTimer->Get() > .05)
	{
		SendTimer->Reset();
		SendTimer->Start();

		DriveTrain->Send_Data();
		ShotMng->Send_Data();
		BallMng->SendData();
		//Intake->SendData();
		SmartDashboard::PutNumber("AUTOTIMER",AutonTimer->Get());
	}
}
void Auton::Auto_Intake_Off()
{
	intake = false;
	outake = false;
}
void Auton::Auto_Intake_On()
{
	intake = true;
	outake = false;
}
void Auton::Auto_DriveEncoder(float Forward, float Turn, float Ticks)
{
	DriveTrain->ResetEncoders_Timers();

	if(Ticks > 0)
	{
		while((DriveTrain->GetLeftEncoder() < Ticks)&& Running())
		{
			DriveTrain->Drive_Auton(Forward ,Turn);
			Auto_System_Update();
			//DriveTrain->Failsafe_Update();
		}
	}
	else
	{
		while((DriveTrain->GetLeftEncoder() > Ticks)&& Running())
		{
			DriveTrain->Drive_Auton(Forward, Turn);
			Auto_System_Update();
			//DriveTrain->FailSafe_Update();
		}
	}
	printf("Finished Driving");
	DriveTrain->Drive_Auton(0, 0);
}
