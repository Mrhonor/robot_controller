#include "controller.h"

int main(int argc, char **argv)
{
	ros::init(argc, argv, "controller");

	ros::NodeHandle n;
	ROS_INFO("START");
	controller node(n);

	
    ros::AsyncSpinner spinner(4);
    spinner.start();
    ros::waitForShutdown();

	return 0;
}
