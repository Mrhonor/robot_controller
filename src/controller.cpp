#include "controller.h"

#include "controller_subscriber.h"
#include "controller_publisher.h"

//cpp
#include <thread>

using namespace std;

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


		Publisher->PublishAll();

		lck.unlock();
		// ros::Duration(1 / FURRENCY).sleep();
	}
}
