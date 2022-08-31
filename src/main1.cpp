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
#include <pthread.h>


AGVInfo AGV_Info;
UdpServer Udp_Server;

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

void* UdpRecvInfo(void* UdpRecvData)
{

}

int main(int argc, char** argv)
{
    // 开启节点接收底盘数据，接收导航电脑数据并回传必要的信息
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

    // 线程所需设置
    pthread_t thread[Num_Thread]; // 线程ID
    pthread_attr_t attr; // 线程属性配置
    int rc; // 线程是否配置

    // 初始化套接字
    if(!Udp_Server.InitSocket())
        return 0;

    // 绑定地址和端口
    if(!Udp_Server.BindAddrAndPort(9090))
        return 0;  

    // 创建线程
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

    rc = pthread_create(&thread[0], &attr ,UdpRecvInfo,(void *)&thread_rec);   //收数据
    if (rc){
        printf("Error:unable to create thread");
        exit(-1);
    }

    ros::Rate LoopRate(50);

    while(ros::ok()) {
        // 将接受到的底盘信息发送，以便server端返回响应数据
        Udp_Server.SetAGVInfo(AGV_Info);
        // 接受数据
        Udp_Server.RecvJsonData();

        std::cout << Udp_Server.GetLinear() << "    " << Udp_Server.GetAngular()<< "      " << Udp_Server.GetChargeFlag() << std::endl;

        // 发布速度
        if( std::abs(Udp_Server.GetAngular()) < 0.5 && std::abs(Udp_Server.GetLinear() < 0.4)) {
            vel.angular.z = Udp_Server.GetAngular();
            vel.linear.x = Udp_Server.GetLinear();
            cmd_pub.publish(vel);
        }
    
        // 发布充电状态
        if(Udp_Server.GetChargeFlag())
            charge_flag.data = 1;
        else
            charge_flag.data = 0;
        charge_pub.publish(charge_flag);

        // 发布小车位置
        AGVPose.AGVx = Udp_Server.GetPosX();
        AGVPose.AGVy = Udp_Server.GetPosY();
        AGVPose.AGVyaw = Udp_Server.GetPosYaw();
        pos_pub.publish(AGVPose);
        
        LoopRate.sleep();
        ros::spinOnce();
    }

    Udp_Server.ClostSocket();
    return 0;
    
}