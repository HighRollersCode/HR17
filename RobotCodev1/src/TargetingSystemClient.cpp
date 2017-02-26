/*
 * TargetingSystemClient.cpp
 *
 *  Created on: Jan 8, 2017
 *      Author: 987
 */

#include <TargetingSystemClient.h>
#include "stdio.h"
#include "stdlib.h"
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "string.h"
#include "errno.h"

TargetingSystemClient::TargetingSystemClient()
{
	m_CommTimer = new Timer();
}

TargetingSystemClient::~TargetingSystemClient() {
	// TODO Auto-generated destructor stub
}
bool TargetingSystemClient::Connect(const char * server, unsigned short port)
{
	int status;
	sockaddr_in server_addr;

	m_SocketHandle = socket(AF_INET,SOCK_STREAM,0);
	if(m_SocketHandle == -1)
	{
		printf("ERROR: Could not create socket!\n");
		return false;
	}

	memset(&server_addr,0,sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);

	server_addr.sin_addr.s_addr = inet_addr(server);

	printf("Trying to connect to Jetson. server: %s port: %d\n",server,port);

	status = connect(m_SocketHandle,(struct sockaddr*)&server_addr,sizeof(server_addr));
	if (status == -1)
	{
		perror("Error: connect failed:\n");
		close(m_SocketHandle);
		return false;
	}

	m_Connected = true;

	printf("Jetson Connected! server: %s port: %d\n",server,port);

	Send("0\r\n",4);
	Send("5\r\n",3);

	m_CommTimer->Reset();
	m_CommTimer->Start();

	return true;
}
void TargetingSystemClient::Shutdown_Jetson()
{
	Send("q\r\n", 3);
}
void TargetingSystemClient::DoCalibrate()
{
	Send("4\r\n",3);
}

bool TargetingSystemClient::Update()
{
	if (m_Connected == false)
	{
		return false;
	}
	m_UpdateCounter++;

	gotdata = false;
	fd_set read_set;
	FD_ZERO(&read_set);
	FD_SET(m_SocketHandle,&read_set);

	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	int status = select(m_SocketHandle+1,&read_set,NULL,NULL,&timeout);
	if(status == -1)
	{
		printf("ERROR: select failed. socked: %d errno: %d",m_SocketHandle,errno);
		return false;
	}

	if(FD_ISSET(m_SocketHandle,&read_set))
	{
		char msg_buffer[4096];

		int bytes_in = recv(m_SocketHandle,msg_buffer,sizeof(msg_buffer),0);
		if (bytes_in == 0)
		{
			m_Connected = false;
			printf("Targeting Client Disconnected!");
		}
		else
		{
			msg_buffer[4095] = 0;
			Handle_Incoming_Data(msg_buffer,sizeof(msg_buffer));
		}
	}

	if (m_CommTimer->Get() > 0.0125f)
	{
		char buff[256];
		sprintf(buff,"2 %f\n",m_TurretAngle);
		Send("0\r\n",3);
		Send("5\r\n",3);
		m_CommTimer->Reset();
	}
	return gotdata;
}
void TargetingSystemClient::Handle_Incoming_Data(char * data, int size)
{
	char * endln = strchr(data, '\n');
	if (endln == NULL)
	{
		printf("Error: Targeting System got partial command! %s \r\n",data);
		return;
	}

	char *strtok_tmp;
	char *cmd = strtok_r(data,"\r",&strtok_tmp);
	while(cmd !=NULL)
	{
		Handle_Command(cmd);
		cmd = strtok_r(NULL,"\r",&strtok_tmp);
	}
}
void TargetingSystemClient::Handle_Command(char * data)
{
	if (data[1] != ' ')
	{
		return;
	}

	switch(data[0])
	{
		case '0':
			Handle_Target(data);
			break;
		case '4':
			break;
		case '5':
			Handle_CalibrationRefresh(data);
			break;
	}
}
void TargetingSystemClient::Handle_Target(char*data)
{
	float tmpx,tmpy,tmparea;
	float initial = 0;
	sscanf(data, "%f %f %f %f",&initial,&tmpx,&tmpy,&tmparea);

	m_XOffset = tmpx;
	m_YOffset = tmpy;
	m_TargetArea = tmparea;
	gotdata = true;

	SmartDashboard::PutString("Track Data", data);

	if ((fabs(m_XOffset) > 2.0f) || (fabs(m_YOffset) > 2.0f))
	{
		printf("bad target.");
	}
}
void TargetingSystemClient::Handle_Calibration(char * data)
{
	sscanf(data,"4 %f %f",&xCal,&yCal);
	gotdata = true;
}
void TargetingSystemClient::Handle_CalibrationRefresh(char * data)
{
	sscanf(data,"5 %f %f", &xCal,&yCal);
	gotdata = true;
	SmartDashboard::PutString("Calibration", data);

	if ((fabs(xCal) > 1.0f) || (fabs(yCal) > 1.0f))
	{
		printf("bad target.");
	}
}
void TargetingSystemClient::SmartDashboardUpdate()
{
	SmartDashboard::PutBoolean("Connected",m_Connected);
	SmartDashboard::PutNumber("X Offset", m_XOffset);
}
void TargetingSystemClient::Disconnect()
{
	printf("Disconnect \n");
	close(m_SocketHandle);
	m_Connected = false;
	m_SocketHandle = -1;
}
bool TargetingSystemClient::Send(const char * data_out, int size)
{
	try
	{
		int status;
		if ((m_Connected == true) && (size > 0))
		{
			status = send(m_SocketHandle,data_out,size,MSG_NOSIGNAL);
			if (status == -1)
			{
				return false;
			}
			return true;
		}
		return false;
	}
	catch (...)
	{
		printf("Crash at send, Check Jetson \n");
		return false;
	}
}
void TargetingSystemClient::Send_Camera_Mode_Msg(CameraMode mode)
{
	char buffer [256];
	sprintf(buffer,"7 %d\r\n",mode);
	Send(buffer,strlen(buffer));
}
void TargetingSystemClient::Set_Camera_Mode(CameraMode mode)
{
	if(m_CamMode != mode)
	{
		Send_Camera_Mode_Msg(mode);
		m_CamMode = mode;
	}
}

void TargetingSystemClient::Set_Moving_Target_Offset(float x,float y)
{
	float dx = x - m_MovingTargetX;
	float dy = y - m_MovingTargetY;
	int dt = m_UpdateCounter - m_LastMovingTargetTimestamp;

	if ((fabs(dx) > 0.01f) || (fabs(dy) > 0.01f))
	{
		// don't spam too much, this is just for visual feedback to the drivers
		if (dt > 10)
		{
			m_LastMovingTargetTimestamp = m_UpdateCounter;
			m_MovingTargetX = x;
			m_MovingTargetY = y;

			char buffer[256];
			sprintf(buffer,"8 %f %f\r\n",m_MovingTargetX,m_MovingTargetY);
			Send(buffer,strlen(buffer));
		}
	}
}
