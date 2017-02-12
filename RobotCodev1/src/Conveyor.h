/*
 * Conveyor.h
 *
 *  Created on: Feb 5, 2017
 *      Author: 987
 */

#include "WPILib.h"
#include "Defines.h"
#include "CANTalon.h"

#ifndef SRC_CONVEYOR_H_
#define SRC_CONVEYOR_H_

class ConveyorClass {
public:
	CANTalon *Conveyor;

	ConveyorClass();
	virtual ~ConveyorClass();

	void Update(bool intake, bool outake);
	void Send_Data();
};

#endif /* SRC_CONVEYOR_H_ */
