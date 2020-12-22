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
    robot_controller::aruco_msg msg;
    for(auto i : Subject->Robots){
        msg.robotName = i.Name;
        msg.linear.x = i.ControlV;
        msg.yaw = i.ControlYaw;
        pub.publish(msg);
        // ros::Duration(0.001).sleep();
    }
}