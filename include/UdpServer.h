#ifndef __UDP__SERVER__H__
#define __UDP__SERVER__H__

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <iostream>
#include <cstring>
#include <json/json.h>
#include <vector>
#include "common.h"

class UdpServer
{
    private:
        // 套接字
        int sockfd;
        // 缓冲区
        char RecvBuff[4096];

        struct sockaddr_in client;
        // 底盘数据
        BottData bottdata;

        // 机器人位置
        AGVPose agvpose;

        // 回传的底盘信息
        AGVInfo AGV_Info;
        
    public:
        UdpServer();

        ~UdpServer();

        // 初始化套接字
        bool InitSocket();

        // 绑定端口
        bool BindAddrAndPort(int port);

        // 接受json数据
        void RecvJsonData();

        // 发送json数据
        void SendJsonData(Json::Value& val);

        void SetAGVInfo(AGVInfo& msg);

        // 16位int转二进制
        std::vector<int> DecToBit(int num);

        inline bool GetChargeFlag() const {return bottdata.ChargerSwitch;}
        
        inline float GetLinear() const {return bottdata.Linear;}

        inline float GetAngular() const {return bottdata.Angular;}

        inline float GetPosX() const {return agvpose.x;}

        inline float GetPosY() const {return agvpose.y;}

        inline float GetPosYaw() const {return agvpose.yaw;}
};

#endif