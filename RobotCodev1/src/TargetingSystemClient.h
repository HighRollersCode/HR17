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


#define JETSON_IP "10.20.17.11" //"10.9.87.11"
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

	enum CameraMode
	{
		CAM_FRONT = 0,
		CAM_BACK
	};

	void Set_Camera_Mode(CameraMode mode);


	void Set_Turret_Angle(float a) { m_TurretAngle = a; }
	float Get_XOffset() { return m_XOffset; }
	float Get_YOffset() { return m_YOffset; }
	float Get_Cal_X() { return xCal; }
	float Get_Cal_Y() { return yCal; }
	float Get_Connected() { return m_Connected; }
	float Get_TargetArea() {return m_TargetArea; }
	void Set_Moving_Target_Offset(float x,float y);
	void DoCalibrate();
	void SmartDashboardUpdate();
	void Disconnect();

protected:
	void Handle_Incoming_Data(char * data, int size);
	void Handle_Command(char *data);
	void Handle_Target(char *data);
	void Handle_Calibration(char *data);
	void Handle_CalibrationRefresh(char *data);
	void Send_Camera_Mode_Msg(CameraMode mode);

	int m_UpdateCounter = 0;
	float m_TurretAngle = 0;
	float m_XOffset = 0;
	float m_YOffset = 0;
	float m_TargetArea = 0;
	bool m_Connected = false;
	int m_SocketHandle = -1;
	bool gotdata = false;
	float xCal = 0;
	float yCal = 0;
	CameraMode m_CamMode = CAM_FRONT;
	Timer *m_CommTimer;

	float m_MovingTargetX = 0;
	float m_MovingTargetY = 0;
	int m_LastMovingTargetTimestamp = 0;
};

#endif /* SRC_TARGETINGSYSTEMCLIENT_H_ */
