#include "controller.h"

#include "controller_subscriber.h"
#include "controller_publisher.h"

//cpp
#include <thread>
#include <cmath>

using namespace std;

#define FURRENCY 100.f
#define PI       3.1415926535

controller::controller(ros::NodeHandle &n){
    ROS_INFO("controller init");

    TurningPoint p;
    p.x = 5;
    p.y = 5;
    p.boundaryLength = 0.5;
    CrossRoad.push_back(p);

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

        // control the robots
        for(auto &i : Robots){
            OnControl(i);
        }

		Publisher->PublishAll();

		lck.unlock();
		ros::Duration(1 / FURRENCY).sleep();
	}
}

void controller::OnControl(RobotInfo& selfInfo){
    if(fabs(selfInfo.x - selfInfo.TargetX) <= 0.2 && fabs(selfInfo.y - selfInfo.TargetY) <= 0.2){
        selfInfo.TargetX = 100;
    }

    bool CanPassTheCrossRoad = true;
    for(auto &p : CrossRoad){
        if(p.IsWithingTuringPoints(&selfInfo)){
            ROS_INFO("1");
            if (p.IsWithingWaitQuene(&selfInfo))
            {
                ROS_INFO("2");
                // if the robot isn't the first one in the vector, it can't pass
                if(p.WaitQuene[0] != &selfInfo){
                    ROS_INFO("3");
                    if (!p.IsWithingTuringPoints(p.WaitQuene[0]))
                    {
                        // if the first one is get out of cross road, it should be clear
                        p.WaitQuene.erase(p.WaitQuene.begin());
                    }

                    
                    CanPassTheCrossRoad = false;
                }
            }
            else
            {
                p.WaitQuene.push_back(&selfInfo);
                CanPassTheCrossRoad = false;
            }
            
        }
    }

    selfInfo.ControlYaw = atan2(selfInfo.TargetY - selfInfo.y, selfInfo.TargetX - selfInfo.x) / PI * 180.0;


    if(fabs(selfInfo.ControlYaw - selfInfo.yaw) < 15 && CanPassTheCrossRoad){
        //search others to follow
        double MinDistance = -1;
        for(auto i : Robots){
            if(i.Name != selfInfo.Name && fabs(i.yaw - selfInfo.yaw) < 10){
                double Distance = sqrt(pow((i.x -selfInfo.x), 2) + pow(i.y - selfInfo.y, 2));
                if(MinDistance == -1){
                    MinDistance = Distance;
                }
                else
                {
                    MinDistance = (MinDistance < Distance) ? MinDistance : Distance;
                }
                
            }
        }
        if (MinDistance > 1.2)
        {
            selfInfo.ControlV = 0.7;
        }
        else
        {
            selfInfo.ControlV = 0.5;
        }
        
        
    }
    else
    {
        selfInfo.ControlV = 0;
    }
    
    
}

