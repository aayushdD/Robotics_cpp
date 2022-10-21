
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/Pose2D.h"
#include "turtlesim/Pose.h"
#include <cstdlib> //for abs()
#include <iostream>

using namespace std;
 
ros::Publisher pubVelocity;
geometry_msgs::Twist cmdVel;
geometry_msgs::Pose2D current;
geometry_msgs::Pose2D desired;
const double PI = 3.141592;

const double Ka = .5;   

const double Klv = .5;

const double angularTolerance = .1;

const double distanceTolerance = .1;

bool waypointActive = false;

void misc_setup()
{
    desired.x = 5.54;  
    desired.y = 5.54;  
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

void update_goal(const geometry_msgs::Pose2D &desiredPose)
{
    desired.x = desiredPose.x;
    desired.y = desiredPose.y;
    waypointActive = true;
}


double getDistanceError()
{
    return sqrt(pow(desired.x - current.x, 2) + pow(desired.y - current.y, 2));
}

double getAngularError()
{
    double deltaX = desired.x - current.x;
    double deltaY = desired.y - current.y;
    double thetaBearing = atan2(deltaY, deltaX);
    double angularError = thetaBearing - current.theta;
    angularError = (angularError > PI) ? angularError - (2 * PI) : angularError;
    angularError = (angularError < -PI) ? angularError + (2 * PI) : angularError;
    return angularError;
}

void set_velocity()
{
    cmdVel.linear.x = 0;
    cmdVel.linear.y = 0;
    cmdVel.linear.z = 0;
    cmdVel.angular.x = 0;
    cmdVel.angular.y = 0;
    cmdVel.linear.z = 0;

    double angularError = getAngularError();
    double distanceError = getDistanceError();

    if (waypointActive == true && abs(distanceError) > .15)
    {
        if (abs(angularError) < .1)
        {
            cmdVel.linear.x = Klv * distanceError;
            cmdVel.angular.z = 0;
        }
        else
        {
            cmdVel.angular.z = Ka * angularError;
            cmdVel.linear.x = 0;
        }
    }
    else
    {
        cout << "I'm HERE!" << endl;
        cmdVel.linear.x = 0;
        cmdVel.angular.z = 0;
        waypointActive = false;
    }

    pubVelocity.publish(cmdVel);
}

int main(int argc, char **argv)
{
    misc_setup();  

    ros::init(argc, argv, "go_to_x");
    ros::NodeHandle node;

    ros::Subscriber subCurrentPose =
	    node.subscribe("turtle1/pose", 0, update_pose);

    ros::Subscriber subDesiredPose = node.subscribe("waypoint", 0, update_goal, ros::TransportHints().tcpNoDelay());
    pubVelocity = node.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 1);

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