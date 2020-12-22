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
    p.y = 0;
    p.boundaryLength = 1.5;
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
    if(fabs(selfInfo.x - selfInfo.TargetX) <= 0.1 && fabs(selfInfo.y - selfInfo.TargetY) <= 0.1){
        selfInfo.TargetX = 100;
    }

    bool CanPassTheCrossRoad = true;
    for(auto &p : CrossRoad){
        if(p.IsWithingTuringPoints(&selfInfo)){
            if (p.IsWithingWaitQuene(&selfInfo))
            {
                // if the robot isn't the first one in the vector, it can't pass
                if(p.WaitQuene[0] != &selfInfo){
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
                if(p.WaitQuene[0] != &selfInfo){
                    CanPassTheCrossRoad = false;
                }
            }   
        }
    }

    selfInfo.ControlYaw = atan2(selfInfo.TargetY - selfInfo.y, selfInfo.TargetX - selfInfo.x) / PI * 180.0;

    if(fabs(selfInfo.ControlYaw - selfInfo.yaw) < 20 && CanPassTheCrossRoad){
        //search others to follow
        double MinDistance = -1;
        for(auto &i : Robots){
            if(&i != &selfInfo && fabs(i.yaw - selfInfo.yaw) < 30){
                double Distance = (i.x - selfInfo.x) * cos(selfInfo.yaw / 180.0 * PI) + (i.y - selfInfo.y) * sin(selfInfo.yaw / 180.0 * PI);  
                if(Distance > 0 && (MinDistance == -1 || Distance < MinDistance)){
                    MinDistance = Distance;
                    selfInfo.Following = &i;

                }
            }
        }

        if (MinDistance > 1.2) //catch up with the leader
        {
            selfInfo.ControlV = 0.7;
        }
        else if(MinDistance == -1) // nobody to follow
        {
            selfInfo.ControlV = 0.4;
        }
        else
        {
            // following
            selfInfo.ControlV = selfInfo.Following->ControlV;
        }
        
    }
    else
    {
        selfInfo.ControlV = 0;
    }
    
    
}

