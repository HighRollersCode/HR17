/*
 * Drivertrain.cpp
 *
 *  Created on: Jan 7, 2017
 *      Author: 987
 */

#include "Drivetrain.h"
#include "stdlib.h"

Drivetrainclass::Drivetrainclass(CANTalon *GyroTalon)
{
#if USEPIGEON
	GyroTalon->SetStatusFrameRateMs(CANTalon::StatusFrameRateAnalogTempVbat,10);
	GyroTalon->SetStatusFrameRateMs(CANTalon::StatusFrameRateFeedback,10);
	GyroTalon->SetStatusFrameRateMs(CANTalon::StatusFrameRateGeneral,10);
	pigeon = NULL;
	pigeon = new PigeonImu(GyroTalon);
	pigeon->SetFusedHeading(0.0);
#else
	gyro = NULL;
	gyro = new AnalogGyro(0);
	gyro->SetSensitivity(0.007f);
	gyro->Calibrate();
	gyro->Reset();
#endif

	LeftDrive = new Victor (Vic_Drive_Left_1);
	LeftDrive2 = new Victor (Vic_Drive_Left_2);
	RightDrive = new Victor (Vic_Drive_Right_1);
	RightDrive2 = new Victor (Vic_Drive_Right_2);

	LeftDrive->Set(0.0f);
	LeftDrive2->Set(0.0f);
	RightDrive->Set(0.0f);
	RightDrive2->Set(0.0f);

	LeftEncoder = new Encoder(Encoder_Drive_Left_1,Encoder_Drive_Left_2,false,Encoder::EncodingType::k4X);
	RightEncoder = new Encoder(Encoder_Drive_Right_1,Encoder_Drive_Right_2,false,Encoder::EncodingType::k4X);

	Highgear = new Solenoid(Sol_Shifter_In);
	Lowgear = new Solenoid(Sol_Shifter_Out);

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
#if USEPIGEON
	pigeon->SetFusedHeading(0.0f);
	pigeon->SetAccumZAngle(0.0f);
	pigeon->SetYaw(0);
#else
	if(gyro != NULL)
	{
		gyro->Reset();
	}
#endif
}
double Drivetrainclass::GetHeading()
{
	double yawpitchroll[3]{0,0,0};
#if USEPIGEON
	if(pigeon->GetState() == PigeonImu::Ready)
	{
		pigeon->GetYawPitchRoll(yawpitchroll);
	}
#else
	if(gyro != NULL)
	{
		yawpitchroll[0] = gyro->GetAngle();
	}
#endif
	return -yawpitchroll[0];
}
float Drivetrainclass::ComputeAngleDelta(float t)
{
	float cur = GetHeading();
	float err2 = t - cur;
	return err2;
}
void Drivetrainclass::Shifter_Update(bool ShifterEnable)
{
	/*
	PrevShifterToggleTrig = CurrentShifterToggleTrig;
	CurrentShifterToggleTrig = ShifterEnable;

	if((PrevShifterToggleTrig == false) && (CurrentShifterToggleTrig == true))
	{
		ToggleState = -ToggleState;
		ToggleStateNeutral = -1;
	}*/
	if(ShifterEnable)//ToggleState == 1)
	{
		Lowgear->Set(true);
		Highgear->Set(false);
	}
	else// if(ToggleState == -1)
	{
		Lowgear->Set(false);
		Highgear->Set(true);
	}
}
void Drivetrainclass::Drive_Auton(float Forward,float Turn)
{
	StandardArcade(Forward,Turn);
}
float Drivetrainclass::Compute_Speed()
{
	// high gear - 100 ticks = 1.5ft
	float ft_per_pulse = 1.5f / 100.0f;

	LeftEncoder->SetDistancePerPulse(ft_per_pulse);
	RightEncoder->SetDistancePerPulse(ft_per_pulse);

	LeftEncoder->SetSamplesToAverage(5);
	RightEncoder->SetSamplesToAverage(5);

	float left_fps = LeftEncoder->GetRate();
	float right_fps = RightEncoder->GetRate();
	float average_fps = 0.5f*(left_fps + right_fps);
	return average_fps;
#if 0
	float ticks_per_revolution = 1024;
	float feet_per_revolution = 1.57f;

	float left_rpm = 0.0f;
	float right_rpm = 0.0f;
	if (!LeftEncoder->GetStopped())
	{
		left_rpm = ((1.0f/(LeftEncoder->GetPeriod()))*60.0f)/ticks_per_revolution;
	}
	if (!RightEncoder->GetStopped())
	{
		right_rpm = ((1.0f/(RightEncoder->GetPeriod()))*60.0f)/ticks_per_revolution;
	}
	float average_rpm = ((left_rpm + right_rpm)/2);

	float average_fpm = feet_per_revolution/average_rpm;
	float average_fps = average_fpm/60.0f;

	return average_fps;
#endif
}
void Drivetrainclass::Send_Data()
{
#if USEPIGEON
	SmartDashboard::PutBoolean("Pigeon Status",(pigeon->GetState() == PigeonImu::Ready));
	SmartDashboard::PutNumber("Pigeon Yaw", GetHeading());
#else
	SmartDashboard::PutNumber("Gyro", GetHeading());
#endif
	SmartDashboard::PutNumber("Left Encoder", LeftEncoder->Get());
	SmartDashboard::PutNumber("Right Encoder", RightEncoder->Get());
}
