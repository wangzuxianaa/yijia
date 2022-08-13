#ifndef __AGV__POS__PUB__H__
#define __AGV__POS__PUB__H__

#include <ros/ros.h>

class AGVPosPub
{
private:
    ros::NodeHandle nh;

    ros::Publisher AGVPos_pub;
    
public:
    AGVPosPub(ros::NodeHandle* NodeHandle);

    void InitSub();

    void InitPub();
};

#endif