#include "controller_subscriber.h"

#include "controller.h"
// cpp
#include <mutex>
#include <cstring>

using namespace std;

controller_subscriber::controller_subscriber(ros::NodeHandle &n, controller* Subject_){
    Subject = Subject_;
    if(Subject != nullptr){
		
        sub = n.subscribe("/aruco_listener/robot/info", 10, &controller_subscriber::controller_robotInfoCallback, this);
    }
}

void controller_subscriber::controller_robotInfoCallback(const robot_controller::aruco_msg::ConstPtr& msg){
    string name = msg->robotName;

    unique_lock<mutex> lck = unique_lock<mutex>(Subject->controller_process_lock, defer_lock);
    lck.lock();
    for(auto &i : Subject->Robots){
        if(i.Name == name){
            i.v = msg->linear.x;
            i.yaw = msg->yaw;
            i.x = msg->x;
            i.y = msg->y;
            return;
        }
    }
    RobotInfo NewInfo;
    NewInfo.Name = name;
    NewInfo.v = msg->linear.x;
    NewInfo.yaw = msg->yaw;
    NewInfo.x = msg->x;
    NewInfo.y = msg->y;
    Subject->Robots.push_back(NewInfo);

}

