#include "controller_subscriber.h"

#include "controller.h"

using namespace std;

controller_subscriber::controller_subscriber(ros::NodeHandle &n, controller* Subject_){
    Subject = Subject_;
    if(Subject != nullptr){
		lck = unique_lock<mutex>(Subject->controller_process_lock, defer_lock);
        sub = n.subscribe("/aruco_listener/robot/info", 10, &controller_subscriber::controller_robotInfoCallback, this);
    }
}

void controller_subscriber::controller_robotInfoCallback(const robot_controller::aruco_msg::ConstPtr& msg){

}

