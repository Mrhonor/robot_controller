#ifndef __SUBSCRIBER__
#define __SUBSCRIBER__
//ros
#include "ros/ros.h"
//msg
#include "robot_controller/aruco_msg.h"
//cpp
#include "mutex"


class controller;

class controller_subscriber
{
private:
    controller* Subject;

    ros::Subscriber sub;

    std::unique_lock<std::mutex> lck;

public:
    controller_subscriber(){}
    controller_subscriber(ros::NodeHandle &n, controller* Subject_);

private:
    void controller_robotInfoCallback(const robot_controller::aruco_msg::ConstPtr& msg);

};

#endif