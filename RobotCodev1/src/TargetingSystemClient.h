/*
 * TargetingSystemClient.h
 *
 *  Created on: Jan 8, 2017
 *      Author: 987
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "stdio.h"
#include "Timer.h"
#include "WPILib.h"

#ifndef SRC_TARGETINGSYSTEMCLIENT_H_
#define SRC_TARGETINGSYSTEMCLIENT_H_


#define JETSON_IP "10.20.17.11"
#define JETSON_PORT 5800

class TargetingSystemClient {
public:
	TargetingSystemClient();
	virtual ~TargetingSystemClient();

	void Shutdown_Jetson();
	bool Is_Connected(void) {return m_Connected; }
	bool Connect(const char * server,unsigned short port);
	bool Update();
	bool Send(const char * data_out, int size);

	void Set_Turret_Angle(float a) { m_TurretAngle = a; }
	float Get_XOffset() { return m_XOffset; }
	float Get_YOffset() { return m_YOffset; }
	float Get_Cal_X() { return xCal; }
	float Get_Cal_Y() { return yCal; }
	float Get_Connected() { return m_Connected; }
	float Get_TargetArea() {return m_TargetArea; }
	void StartCalibrate();
	void SmartDashboardUpdate();
	void Disconnect();

protected:
	void Handle_Incoming_Data(char * data, int size);
	void Handle_Command(char *data);
	void Handle_Target(char *data);
	void Handle_Calibration(char *data);
	void Handle_CalibrationRefresh(char *data);

	float m_TurretAngle;
	float m_XOffset;
	float m_YOffset;
	float m_TargetArea;
	bool m_Connected;
	int m_SocketHandle;
	bool gotdata;
	float xCal;
	float yCal;
	Timer *m_CommTimer;
};

#endif /* SRC_TARGETINGSYSTEMCLIENT_H_ */
