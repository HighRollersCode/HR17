/*
 * Climber.h
 *
 *  Created on: Jan 28, 2017
 *      Author: 987
 */

#include "WPILib.h"
#include "TalonSRX.h"
#include "CanTalonSRX.h"
#include "Defines.h"
#include "PowerDistributionPanel.h"

#ifndef SRC_CLIMBER_H_
#define SRC_CLIMBER_H_

class ClimberClass {
public:
	TalonSRX *Climber;
	PowerDistributionPanel *PDP;

	bool UseFullPower;

	ClimberClass();
	virtual ~ClimberClass();

	void UpdateClimber(bool ClimbUp,bool ClimbDown);

};

#endif /* SRC_CLIMBER_H_ */
