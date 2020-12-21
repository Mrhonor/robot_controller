#include "controller.h"

#include "controller_subscriber.h"
#include "controller_publisher.h"

//cpp
#include <thread>

using namespace std;

#define FURRENCY 50.f
#define PI       3.1415926535

controller::controller(ros::NodeHandle &n){
    ROS_INFO("controller init");

    Subscriber = new controller_subscriber(n, this);

	Publisher = new controller_publisher(n, this);

    thread thread(&controller::controller_process, this);
	thread.detach();
}

controller::~controller(){
    delete Subscriber;
    delete Publisher;
}

void controller::controller_process(){
    unique_lock<mutex> lck(controller_process_lock, defer_lock);
    while(ros::ok()){
		lck.lock();

        for(auto &i : Robots){
            OnControl(i);
        }

		Publisher->PublishAll();

		lck.unlock();
		ros::Duration(1 / FURRENCY).sleep();
	}
}

void controller::OnControl(RobotInfo& selfInfo){
    if(fabsf(selfInfo.x - selfInfo.TargetX) <= 0.01 && fabsf(selfInfo.y - selfInfo.TargetY) <= 0.01){
        selfInfo.TargetX = 100;
    }

    selfInfo.ControlYaw = atan2(selfInfo.TargetY - selfInfo.y, selfInfo.TargetX - selfInfo.x) / PI * 180.0;
    ROS_INFO_STREAM(selfInfo.Name << " ControlYaw: " << selfInfo.ControlYaw);
    if(fabsf(selfInfo.ControlYaw - selfInfo.yaw) < 0.1){
        selfInfo.ControlV = 1;
    }
    else
    {
        selfInfo.ControlV = 0;
    }
    
    
}

