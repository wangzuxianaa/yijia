#ifndef __TCPCLIENT__H__
#define __TCPCLIENT__H__

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ros/ros.h>


class TCPClient
{
private:
	int sockfd;
	int flag; 
	int RelativePosX;
	int RelativePosY;
	int RelativePosZ;
	char RecvBuff[1024];
public:

	TCPClient();

	~TCPClient();

	bool Init(const char* path, int port);

	void RecvData();

    inline void CloseSocket() const {close(sockfd);}

	inline int GetFlag() const {return flag;}

	inline int GetRelaPosX() const {return RelativePosX;}

	inline int GetRelaPosY() const {return RelativePosY;}

	inline int GetRelaPosZ() const {return RelativePosZ;}
};

#endif