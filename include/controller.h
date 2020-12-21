#ifndef __CONTROLLER__
#define __CONTROLLER__

// ros
#include "ros/ros.h"
// cpp
#include <mutex>

class controller_subscriber;
class controller_publisher;

class controller
{
public:
    std::mutex controller_process_lock;

private:
    /* data */
    controller_subscriber* Subscriber;
	controller_publisher*  Publisher;

public:
    controller(ros::NodeHandle &);
    ~controller();
    
private:
    void controller_process();
};



#endif
