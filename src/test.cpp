#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

int main(int argc, char** argv)
{
    // 开启节点接受设置底盘数据
    ros::init(argc, argv, "test1");

    ros::NodeHandle nh;

    // 速度发布者
    ros::Publisher cmd_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);


    geometry_msgs::Twist vel;


    ros::Rate LoopRate(20);

    while(ros::ok()) {
        vel.angular.z = 0;
        vel.linear.x = 0.2;
        cmd_pub.publish(vel);
        LoopRate.sleep();
    }

    return 0;
    
}