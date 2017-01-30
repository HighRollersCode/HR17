/*
 * Drivertrain.cpp
 *
 *  Created on: Jan 7, 2017
 *      Author: 987
 */

#include "Drivetrain.h"
#include "stdlib.h"

Drivetrainclass::Drivetrainclass()
{
	gyro = NULL;
	gyro = new AnalogGyro(0);
	gyro->SetSensitivity(0.007f);
	gyro->Calibrate();
	gyro->Reset();

	LeftDrive = new Victor (Vic_Drive_Left_1);
	LeftDrive2 = new Victor (Vic_Drive_Left_2);
	RightDrive = new Victor (Vic_Drive_Right_1);
	RightDrive2 = new Victor (Vic_Drive_Right_2);

	LeftEncoder = new Encoder(Encoder_Drive_Left_1,Encoder_Drive_Left_2,false,Encoder::EncodingType::k4X);
	RightEncoder = new Encoder(Encoder_Drive_Right_1,Encoder_Drive_Right_2,false,Encoder::EncodingType::k4X);

	Highgear = new Solenoid(Sol_Shifter_In);
	Lowgear = new Solenoid(Sol_Shifter_Out);

	mult = -.15f;

	CurrentShifterToggleTrig = false;
	PrevShifterToggleTrig = false;
	ToggleState = 1;
	ToggleStateNeutral = -1;

	LeftEncoder->Reset();
	RightEncoder->Reset();

}

Drivetrainclass::~Drivetrainclass() {
	// TODO Auto-generated destructor stub
}
void Drivetrainclass::StandardTank(float left, float right)
{
	float l = left;
	float r = right;

	LeftDrive->Set(l);
	LeftDrive2->Set(l);
	RightDrive->Set(r);
	RightDrive2->Set(r);
}
void Drivetrainclass::StandardArcade(float forward, float turn)
{
	float l = forward + turn;
	float r = -forward + turn;
	StandardTank(l,r);
}
void Drivetrainclass::ResetEncoders_Timers()
{
	LeftEncoder->Reset();
	RightEncoder->Reset();
}
int Drivetrainclass::GetLeftEncoder()
{
	return LeftEncoder->Get();
}
int Drivetrainclass::GetRightEncoder()
{
	return RightEncoder->Get();
}
void Drivetrainclass::Zero_Yaw()
{
	if(gyro != NULL)
	{
		gyro->Reset();
	}
}
float Drivetrainclass::GetHeading(void)
{
	if(gyro != NULL)
	{
		return gyro->GetAngle();
	}
	return 0.0f;
}
float Drivetrainclass::ComputeAngleDelta(float t)
{
	float cur = GetHeading();
	float err2 = t - cur;
	return err2;
}
void Drivetrainclass::Shifter_Update(bool ShifterEnable)
{
	PrevShifterToggleTrig = CurrentShifterToggleTrig;
	CurrentShifterToggleTrig = ShifterEnable;

	if(PrevShifterToggleTrig == false && CurrentShifterToggleTrig == true)
	{
		ToggleState = -ToggleState;
		ToggleStateNeutral = -1;
	}
	if(ToggleState == 1)
	{
		Lowgear->Set(false);
		Highgear->Set(true);
	}
	else if(ToggleState == -1)
	{
		Lowgear->Set(true);
		Highgear->Set(false);
	}
}
void Drivetrainclass::Drive_Auton(float Left,float Right)
{
	StandardTank(Left,Right);
}
float Drivetrainclass::Compute_Speed()
{
	float ticks_per_revolution = 1024;
	float feet_per_revolution = 1.57f;

	float left_rpm = ((1.0f/(LeftEncoder->GetPeriod()))*60.0f)/ticks_per_revolution;
	float right_rpm = ((1.0f/(RightEncoder->GetPeriod()))*60.0f)/ticks_per_revolution;
	float average_rpm = ((left_rpm + right_rpm)/2);

	float average_fpm = feet_per_revolution/average_rpm;
	float average_fps = average_fpm/60.0f;

	return average_fps;
}
void Drivetrainclass::Send_Data()
{
	SmartDashboard::PutNumber("Gyro", gyro->GetAngle());
	SmartDashboard::PutNumber("Left Encoder", LeftEncoder->Get());
	SmartDashboard::PutNumber("Right Encoder", RightEncoder->Get());
}
