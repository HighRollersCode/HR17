/*
 * Drivetrain.h
 *
 *  Created on: Jan 17, 2017
 *      Author: 987
 */

#include "WPILib.h"

#ifndef SRC_DRIVETRAIN_H_
#define SRC_DRIVETRAIN_H_

class Drivetrain {
public:

	Victor *LeftDrive;
	Victor *LeftDrive2;
	Victor *RightDrive;
	Victor *RightDrive2;

	Drivetrain();
	virtual ~Drivetrain();
};

#endif /* SRC_DRIVETRAIN_H_ */
