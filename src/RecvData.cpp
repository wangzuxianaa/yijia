#include "UdpServer.h"
#include <ros/ros.h>
#include "common.h"
#include <json/json.h>
#include "SerialPort.h"

int main(int argc, char** argv) 
{
    ros::init(argc, argv, "RecvData");

    ros::NodeHandle nh;

    UdpServer server;
    SerialPort serialport;

    // 初始化端口并打开
    serialport.InitPort("", 115200);
    if(!serialport.OpenPort())
        return -1;

    // 初始化socket,绑定端口
    if(!server.InitSocket())
        return -1;
    if(!server.BindAddrAndPort(12322))
        return -1;

    ros::Rate loop_rate(20);
    while(ros::ok()) {
        // 接收数据
        server.RecvJsonData();

        
        loop_rate.sleep();
    }
}