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
	char RecvBuff[1024];
public:

	TCPClient();

	~TCPClient();

	bool Init(std::string path, int port);

	void RecvData();

    void CloseSocket() {close(sockfd);}
};

#endif