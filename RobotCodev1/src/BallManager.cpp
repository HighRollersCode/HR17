/*
 * BallManager.cpp
 *
 *  Created on: Feb 5, 2017
 *      Author: 987
 */

#include <BallManager.h>

BallManager::BallManager(IntakeClass *RobotIntake,UptakeClass *RobotUptake,ConveyorClass *RobotConveyor) {

	Intake = RobotIntake;
	Uptake = RobotUptake;
	Conveyor = RobotConveyor;
}

BallManager::~BallManager() {
	// TODO Auto-generated destructor stub
}
void BallManager::Update(bool intake,bool outake,bool uptake,bool downtake)
{
	bool intake_mtr = ((intake)||(uptake));
	bool outake_mtr = (outake);

	bool conveyor_in_mtr = ((intake)||(uptake));
	bool conveyor_out_mtr = ((outake)||(downtake));

	bool uptake_mtr = (uptake);
	bool downtake_mtr = (downtake);

	Intake->UpdateIntake(intake_mtr,outake_mtr);
	Uptake->UpdateUptake(uptake_mtr,downtake_mtr);
	Conveyor->Update(conveyor_in_mtr,conveyor_out_mtr);
}
void BallManager::SendData()
{
	Uptake->Send_Data();
}
