/*
 * BallManager.h
 *
 *  Created on: Feb 5, 2017
 *      Author: 987
 */

#include "Uptake.h"
#include "WPILib.h"
#include "Intake.h"
#include "Conveyor.h"

#ifndef SRC_BALLMANAGER_H_
#define SRC_BALLMANAGER_H_

class BallManager {
public:

	IntakeClass *Intake;
	UptakeClass *Uptake;
	ConveyorClass *Conveyor;

	BallManager(IntakeClass *RobotIntake,UptakeClass *RobotUptake,ConveyorClass *RobotConveyor);
	virtual ~BallManager();

	void Update(bool intake, bool outake,bool uptake,bool downtake,float shaker);
	void SendData();

};

#endif /* SRC_BALLMANAGER_H_ */
