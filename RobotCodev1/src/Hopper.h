/*
 * Uptake.h
 *
 *  Created on: Jan 17, 2017
 *      Author: 987
 */

#include "WPILib.h"

#ifndef SRC_HOPPER_H_
#define SRC_HOPPER_H_

class Hopper {
public:

	Victor *Hopper;


	Hopper();
	virtual ~Hopper();

	void UpdateHopper(float uptake,float downtake);
	void HopperUp();
	void HopperDown();
	void HopperOff();
};

#endif /* SRC_HOPPER_H_ */
