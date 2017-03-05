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
void BallManager::Update(bool intake,bool outake,bool uptake,bool downtake,float shaker)
{
	bool intake_mtr = (intake);
	bool shooting_intake_mtr = ((intake)&&(uptake));
	bool outake_mtr = (outake);

	bool uptake_mtr = (uptake);
	bool downtake_mtr = ((downtake) || (intake));

	Intake->UpdateIntake(intake_mtr,outake_mtr,shooting_intake_mtr);
	Uptake->UpdateUptake(uptake_mtr,downtake_mtr,intake);
}
void BallManager::SendData()
{
	Uptake->Send_Data();
	Intake->Send_Data();
	Conveyor->Send_Data();
}
