#include "controller_publisher.h"

#include "controller.h"
// msg
#include "robot_controller/aruco_msg.h"
// cpp
#include <cstring>

using namespace std;

controller_publisher::controller_publisher(ros::NodeHandle &n, controller* Subject_):Subject(Subject_){
    if(Subject != nullptr){
        pub = n.advertise<robot_controller::aruco_msg>("/robot_controller/control", 10);
    }
}

void controller_publisher::PublishAll(){
    robot_controller::aruco_msg msg_robot1, msg_robot2, msg_robot3;
    msg_robot1.robotName = string("robot1");
    msg_robot1.linear.x = 1;
    msg_robot1.yaw = 0;

    msg_robot2.robotName = string("robot2");
    msg_robot2.linear.x = 1;
    msg_robot2.yaw = 0;

    msg_robot3.robotName = string("robot3");
    msg_robot3.linear.x = 1;
    msg_robot3.yaw = 0;

    pub.publish(msg_robot1);
    ros::Duration(0.01).sleep();
    pub.publish(msg_robot2);
    ros::Duration(0.01).sleep();
    pub.publish(msg_robot3);
    ros::Duration(0.01).sleep();
}