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

typedef struct
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
}RobotInfo;


class controller
{
public:
    std::mutex controller_process_lock;

    std::vector<RobotInfo> Robots;


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
