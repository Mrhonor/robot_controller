#ifndef __CONTROLLER__
#define __CONTROLLER__

// ros
#include "ros/ros.h"
// cpp
#include <mutex>
#include <vector>
#include <cstring>

class controller_subscriber;
class controller_publisher;

typedef struct RobotInfo
{
    std::string Name;
    double x = 0;
    double y = 0;
    double v = 0;
    double yaw = 0;

    double TargetX = 5;
    double TargetY = 0;
    double ControlV = 0;
    double ControlYaw = 0;

    struct RobotInfo* Following = nullptr;
}RobotInfo;

typedef struct
{
    double x;
    double y;
    double boundaryLength;
    std::vector<RobotInfo*> WaitQuene;
    bool IsWithingTuringPoints(RobotInfo* info){
        if(fabs(info->x - x) < boundaryLength && fabs(info->y - y) < boundaryLength){
            return true;
        }
        else
        {
            return false;
        }
        
    }

    bool IsWithingWaitQuene(RobotInfo* info){
        for (auto &i : WaitQuene)
        {
            if(i == info){
                return true;
            }
        }
        return false;
    }
}TurningPoint;


class controller
{
public:
    std::mutex controller_process_lock;

    std::vector<RobotInfo> Robots;
    std::vector<TurningPoint> CrossRoad;

private:
    /* data */
    controller_subscriber* Subscriber;
	controller_publisher*  Publisher;

public:
    controller(ros::NodeHandle &);
    ~controller();
    
private:
    void controller_process();
    void OnControl(RobotInfo&);
};



#endif
