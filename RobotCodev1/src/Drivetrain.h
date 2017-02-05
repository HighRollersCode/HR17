/*
 * Drivertrain.h
 *
 *  Created on: Jan 7, 2017
 *      Author: 987
 */

#include "WPILib.h"
#include "Defines.h"
#include "AnalogGyro.h"

#ifndef SRC_DRIVETRAIN_H_
#define SRC_DRIVETRAIN_H_

class Drivetrainclass {

private:
	AnalogGyro *gyro;
public:
	Victor *LeftDrive;
	Victor *RightDrive;
	Victor *LeftDrive2;
	Victor *RightDrive2;

	Encoder *LeftEncoder;
	Encoder *RightEncoder;

	Solenoid * Highgear;
	Solenoid * Lowgear;

	bool CurrentShifterToggleTrig = false;
	bool PrevShifterToggleTrig = false;
	bool ToggleState = 1;
	bool ToggleStateNeutral = -1;

	float mult = -.15f;

	Drivetrainclass();
	virtual ~Drivetrainclass();

	int GetLeftEncoder();
	int GetRightEncoder();
	void ResetEncoders_Timers();

	void Zero_Yaw();
	void IMUCalibration();
	float ComputeAngleDelta(float t);
	float GetHeading(void);
	float Compute_Speed();

	void StandardTank(float left, float right);
	void StandardArcade(float forward, float turn);
	void Shifter_Update(bool ShifterEnable);
	void Drive_Auton(float Left,float Right);
	void Send_Data();
};

#endif /* SRC_DRIVERTRAIN_H_ */
