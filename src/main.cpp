#include <iostream>
#include <cpr/cpr.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "UdpServer.h"
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <car_control/agv1.h>  
#include <car_control/Position.h>
#include <std_msgs/UInt8.h> 


AGVInfo AGV_Info;

void rosmsgCallback(const car_control::agv1ConstPtr& msg)
{
    AGV_Info.Vx = msg->Vx;
    AGV_Info.Vz = msg->Vz;
    AGV_Info.AccX = msg->AccX;
    AGV_Info.AccY = msg->AccY;
    AGV_Info.AccZ = msg->AccZ;
    AGV_Info.GyrX = msg->GyrX;
    AGV_Info.GyrY = msg->GyrY;
    AGV_Info.GyrZ = msg->GyrZ;
    AGV_Info.Voltage = msg->Voltage;
    AGV_Info.State = msg->State;
    AGV_Info.Light12 = msg->Light12;
    AGV_Info.Light34 = msg->Light34;
}

int main(int argc, char** argv)
{
    // 开启节点接受设置底盘数据
    ros::init(argc, argv, "RecvAndPubData");

    ros::NodeHandle nh;

    // 速度发布给底盘
    ros::Publisher cmd_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    // 发布定位数据
    ros::Publisher pos_pub = nh.advertise<car_control::Position>("/position", 10);
    // 发布充电任务
    ros::Publisher charge_pub = nh.advertise<std_msgs::UInt8>("go_charge", 10);
    // 订阅底盘数据
    ros::Subscriber rosmsg_sub = nh.subscribe("agv_info", 10, rosmsgCallback);


    geometry_msgs::Twist vel;
    car_control::Position AGVPose;
    std_msgs::UInt8 charge_flag;
    // UDP Server
    UdpServer server;

    // 初始化套接字
    if(!server.InitSocket())
        return 0;

    // 绑定地址和端口
    if(!server.BindAddrAndPort(9090))
        return 0;  

    ros::Rate LoopRate(50);

    while(ros::ok()) {
        // 将接受到的底盘信息发送，以便server端返回响应数据
        server.SetAGVInfo(AGV_Info);
        // 接受数据
        server.RecvJsonData();

        std::cout << server.GetLinear() << "    " << server.GetAngular()<< "      " << server.GetChargeFlag() << std::endl;

        // 发布速度
        if( std::abs(server.GetAngular()) < 0.5 && std::abs(server.GetLinear() < 0.4)) {
            vel.angular.z = server.GetAngular();
            vel.linear.x = server.GetLinear();
            cmd_pub.publish(vel);
        }
    
        // 发布充电状态
        if(server.GetChargeFlag())
            charge_flag.data = 1;
        else
            charge_flag.data = 0;
        charge_pub.publish(charge_flag);

        // 发布小车位置
        AGVPose.AGVx = server.GetPosX();
        AGVPose.AGVy = server.GetPosY();
        AGVPose.AGVyaw = server.GetPosYaw();
        pos_pub.publish(AGVPose);
        
        LoopRate.sleep();
        ros::spinOnce();
    }

    server.ClostSocket();
    return 0;
    
}