#include "ros/ros.h"
#include "geometry_msgs/Twist.h"

#include <sstream>
#include <math.h>

double fx, fy, fz, tx, ty, tz, x, y, z, roll, pitch, yaw;

double long calc_pos(double ft)
{
    int t = 8;

    double long pos = exp(-1* t) * t * ft * 30;

    return pos;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "impedance_module_node");
    ros::NodeHandle n;
    ros::Rate loop_rate(100);

    ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);

    while(ros::ok())
    {
        geometry_msgs::Twist msg;
        msg.linear.x = 0.0;
        msg.linear.y = 0.0;

        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}