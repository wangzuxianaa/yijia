#include "UdpServer.h"
#include <json/json.h>
#include "AGVPosPub.h"

AGVPosPub::AGVPosPub(ros::NodeHandle* NodeHandle):nh(*NodeHandle)
{
    InitPub();
    InitSub();
}

void AGVPosPub::InitPub()
{

}

void AGVPosPub::InitSub()
{
    
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "AGVPos");

    ros::NodeHandle nh;

    AGVPosPub AGVPos(&nh);

    UdpServer server;

    // 初始化套接字
    if(!server.InitSocket())
        return 0;

    // 绑定地址和端口
    if(!server.BindAddrAndPort(9090))
        return 0;   

    Json::Value val;
    Json::Reader reader;
    
}