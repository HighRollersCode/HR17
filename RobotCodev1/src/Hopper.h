/*
 * Uptake.h
 *
 *  Created on: Jan 17, 2017
 *      Author: 987
 */

#include "WPILib.h"
#include "Defines.h"
#include "TalonSRX.h"
#include "CANTalon.h"

#ifndef SRC_HOPPER_H_
#define SRC_HOPPER_H_

class HopperClass {
public:

	CANTalon *Hopper;
	CANTalon *HopperIntake;
	CANTalon *Conveyor;


	HopperClass();
	virtual ~HopperClass();

	void UpdateHopper(float uptake,float downtake);
	void HopperUp();
	void HopperDown();
	void HopperOff();
};

#endif /* SRC_HOPPER_H_ */
