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

#ifndef SRC_Uptake_H_
#define SRC_Uptake_H_

class UptakeClass {
public:

	CANTalon *Uptake;

	float speed = .5f;
	bool slowdown = false;

	Timer *UptakeTimer;
	Timer *UptakeOffTimer;

	UptakeClass();
	virtual ~UptakeClass();

	void UpdateUptake(bool uptake,bool downtake,bool slow = false);
	void UptakeUp();
	void UptakeDown();
	void UptakeOff();
	void Send_Data();
};

#endif /* SRC_Uptake_H_ */
