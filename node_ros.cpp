

#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/Pose2D.h"
#include "turtlesim/Pose.h"
#include <cstdlib> //for abs()
#include <iostream>

using namespace std;


geometry_msgs::Twist cmdVel;
geometry_msgs::Pose2D current;
geometry_msgs::Pose2D desired;

const double GOAL = 1.5;

const double Kl = 1;

const double distanceTolerance = .1;

void misc_setup()
{
    desired.x = GOAL;
    cmdVel.linear.x = 0;
    cmdVel.linear.y = 0;
    cmdVel.linear.z = 0;
    cmdVel.angular.x = 0;
    cmdVel.angular.y = 0;
    cmdVel.linear.z = 0;
}

void update_pose(const turtlesim::PoseConstPtr &currentPose)
{
    current.x = currentPose->x;
    current.y = currentPose->y;
    current.theta = currentPose->theta;
}

double getDistanceError()
{
    return desired.x - current.x;
}

void set_velocity()
{
    if (abs(getDistanceError()) > distanceTolerance)
    {
        cmdVel.linear.x = Kl * getDistanceError();
    }
    else
    {
        cout << "I'm HERE!" << endl;
        cmdVel.linear.x = 0;
    }
}

int main(int argc, char **argv)
{
    misc_setup();  

    ros::init(argc, argv, "go_to_x");
    ros::NodeHandle node;

    ros::Subscriber subCurrentPose =
	node.subscribe("turtle1/pose", 0, update_pose);

    ros::Publisher pubVelocity =
      node.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 0);

    ros::Rate loop_rate(10); 
 
    while (ros::ok)
    {

        ros::spinOnce();

        set_velocity();

        pubVelocity.publish(cmdVel);

        cout << "goal x = " << desired.x << endl
             << "current x = " << current.x <<  endl
             << "  disError = " << getDistanceError() << endl
             << "cmd_vel = " << cmdVel.linear.x<< endl;

        loop_rate.sleep();
    }

    return 0;
}