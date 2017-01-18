/*
 * MyRobot.h
 *
 *  Created on: Jan 17, 2017
 *      Author: 987
 */

#include "WPILib.h"
#include "GamepadBase.h"
#include "Drivetrain.h"

#ifndef SRC_MYROBOT_H_
#define SRC_MYROBOT_H_

class MyRobot {
public:

	Drivetrain *Drivetrain;

	GamepadBase *XBoxController;


	MyRobot();
	virtual ~MyRobot();
};

#endif /* SRC_MYROBOT_H_ */
