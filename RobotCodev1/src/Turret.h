/*
 * Turret.h
 *
 *  Created on: Jan 9, 2017
 *      Author: 987
 */

#include "WPILib.h"
#include "Defines.h"
#include "CANSpeedController.h"
#include "TalonSRX.h"
#include "CANTalon.h"

#include "PIDController.h"
#include "Encoder.h"

#ifndef SRC_TURRET_H_
#define SRC_TURRET_H_
#define TURRET_TALON_CONTROL 0

class TurretClass {
public:

	double MIN_TURRET_CMD = 0.20f;
	double TURRET_P = .005f;
	double TURRET_I = 0.00001f;
	double TURRET_D = 0.01;

#if TURRET_TALON_CONTROL
	CANTalon *Turret;
#else
	CANTalon *Turret;
	Encoder *TurretEncoder;
#endif
	bool Resetting = false;
	bool CurrentEnableTracking = false;
	bool PrevEnableTracking = false;

	bool isTracking = false;
	bool isLockedOn = false;
	bool isReady = false;

	float LastMoveByDegreesX = 360.0f;
	float LockonDegreesX;

	int TurretEncoder_Cur = 0;
	int TurretEncoder_Targ = 0;

	float TurretCommand_Prev = 0;
	float TurretCommand_Cur = 0;

	Timer *ArmTimer;

	Timer *ArmLockonTimer;
	Timer *LastShotTimer;
#if !TURRET_TALON_CONTROL
	PIDController *TurretPIDController;
#endif
	void Auto_Start();
	void Tele_Start();

	void Update(float Turret, bool TrackingEnable, float cx, float calx, float target_area);
	void HandleUser(float turret);
	int GetTurretEncoder();
	void SetTurret(int targ);
	void StartTracking(int enable);
	void AutonomousTrackingUpdate(float tx, float crossX, float target_area);

	void HandleTarget(float centerX, float calX,float target_a);

	void Send_Data();

	void ResetEncoderTurret();
	void SetTurretConstants(float p,float i,float d);
	float Clamp_Target(float tar, float lowerlim, float upperlim);
	TurretClass();
	virtual ~TurretClass();
	bool isauto = false;

	// These two functions will return a modified command if the
	// given command would push the arm into an illegal configuration.
	// If the command would help move the arm out of the invalid
	// configuration it should return it unmodified.  If the arm is already in an invalid configuration
	// then these commands could return a command that moves it toward a valid configuration.
	// The 'ArmTurretVictorClass' and 'ArmLiftVictorClass' call these functions.

	float Turret_Encoder_To_Degrees(int enc);
	float Compute_Robot_Angle();
	float Validate_Turret_Command(float cmd, bool ispidcmd = false);

};

#endif /* SRC_TURRET_H_ */
