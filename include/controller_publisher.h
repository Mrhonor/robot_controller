#ifndef __PUBLISHER__
#define __PUBLISHER__

#include "ros/ros.h"

class controller;

class controller_publisher
{
private:
    controller* Subject;
    ros::Publisher pub;

public:
    controller_publisher(ros::NodeHandle &n_, controller* Subject_);
    
    void PublishAll();

    
};




#endif