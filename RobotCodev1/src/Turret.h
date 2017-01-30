/*
 * Turret.h
 *
 *  Created on: Jan 9, 2017
 *      Author: 987
 */

#ifndef SRC_TURRET_H_
#define SRC_TURRET_H_

#include "WPILib.h"
#include "Defines.h"
#include "CANSpeedController.h"
#include "TalonSRX.h"
#include "CANTalon.h"

#include "PIDController.h"
#include "Encoder.h"

class TurretClass {
public:

	static double MIN_TURRET_CMD;
	static double TURRET_P;
	static double TURRET_I;
	static double TURRET_D;

	CANTalon *Turret;

	Encoder *TurretEncoder;

	bool Resetting;
	bool CurrentEnableTracking;
	bool PrevEnableTracking;

	bool isTracking;
	bool isLockedOn;
	bool isReady;

	float LastMoveByDegreesX;
	float LockonDegreesX;

	int TurretEncoder_Cur;
	int TurretEncoder_Targ;

	float TurretCommand_Prev;
	float TurretCommand_Cur;

	Timer *ArmTimer;

	Timer *ArmLockonTimer;
	Timer *LastShotTimer;
	PIDController *TurretPIDController;

	void Auto_Start();
	void Tele_Start();

	void Update(float Turret, bool TrackingEnable, float cx, float calx, float target_area);
	void UpdateTurret(float turret);
	int GetTurretEncoder();
	void SetTurret(int targ);
	void StartTracking(int enable);
	void AutonomousTrackingUpdate(float tx, float crossX, float target_area);

	void HandleTarget(float centerX, float calX,float target_a);

	void Send_Data();

	void ResetEncoderTurret();
	float Clamp_Target(float tar, float lowerlim, float upperlim);
	TurretClass();
	virtual ~TurretClass();
	bool isauto;

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
