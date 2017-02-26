/*
 * Auton.h
 *
 *  Created on: Jan 29, 2016
 *      Author: HighRollers
 */

#ifndef AUTON_H_
#define AUTON_H_

#include <Uptake.h>
#include "WPILib.h"
#include "Drivetrain.h"
#include "Turret.h"
#include "Intake.h"
#include "ShooterWheel.h"
#include "ShotManager.h"
#include "BallManager.h"
#include "TargetingSystemClient.h"

class Auton
{
	public:
		Drivetrainclass *DriveTrain;
		TurretClass *Turret;
		DriverStation *ds;
		TargetingSystemClient *Targeting;
		ShotManager *ShotMng;
		BallManager *BallMng;
		Timer *AutonTimer;
		Timer *SendTimer;
		//float turningp;

		bool Abort = false;
		bool intake = false;
		bool outake = false;
		bool uptake = false;
		bool downtake = false;
		bool dotrack = false;

		bool Running();
		void AutonWait(float Seconds);
		void AutonWait2(float Seconds, int brake);
		void AutonWaitForTarget(float Seconds);
		void AutonWaitForIntake();
		bool Auto_System_Update();

		void Auto_DriveTimer(float Forward, float Turn, float seconds);
		void Auto_Intake_On();
		void Auto_Intake_Off();
		void Auto_GYROTURN(float heading);
		void Auto_GYROTURN_TIMED(float heading, float seconds);
		void Auto_GYROSTRAIGHT(float forward, float ticks, float desheading);
		void Auto_DriveGyro_Encoder(float Forward, float Angle, float Ticks);

		void SendData();
		//void Auto_fullShot();

		void Auto_DriveEncoder(float Forward, float Turn, float Ticks);
		//void Auto_CameraAim();

		Auton
		(
				Drivetrainclass *D,
				TurretClass *Tu,
				DriverStation *Ds,
				TargetingSystemClient *T,
				ShotManager *SM,
				BallManager *BM
		);
		~Auton();
		void Auto_Start();
		void Auto_End();
		void FireAll();
};

#endif /* AUTON_H_ */
