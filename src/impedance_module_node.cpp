#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ft_sensor_msgs/ForceTorque.h"

#include <sstream>
#include <math.h>

// Scale : Newton to centimeter
// Scale : Newton to radian

double fx, fy, fz, tx, ty, tz, x, y, z, roll, pitch, yaw;

bool ft_update          = false;
bool goal_pos_update    = false;

double long calc_pos(double ft)
{
    int t = 8;

    double long pos = exp(-1* t) * t * ft * 30;

    return pos;
}

void goalposCallback(const geometry_msgs::Twist::ConstPtr& msg)
{
    if(ft_update)
    {
        x += msg->linear.x;
        y += msg->linear.y;
        z += msg->linear.z;
        roll += msg->angular.x;
        pitch += msg->angular.y;
        yaw += msg->angular.z;

        ft_update = false;
        goal_pos_update = true;
    }
}

void ftCallback(const ft_sensor_msgs::ForceTorque msg)
{
    fx = msg.force_x_raw;
    fy = msg.force_y_raw;
    fz = msg.force_z_raw;
    tx = msg.torque_x_raw;
    ty = msg.torque_y_raw;
    tz = msg.torque_z_raw;

    x = calc_pos(fx);
    y = calc_pos(fy);
    z = calc_pos(fz);
    roll = calc_pos(tx);
    pitch = calc_pos(ty);
    yaw = calc_pos(tz);

    ft_update = true;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "impedance_module_node");
    ros::NodeHandle n;
    ros::Rate loop_rate(100);

    ros::Publisher  goal_pos_pub =  n.advertise<geometry_msgs::Twist>("/stewart/platform_twist", 100);
    ros::Subscriber goal_pos_sub =  n.subscribe("/stewart/curr_pos", 100, &goalposCallback);
    ros::Subscriber ft_sub =        n.subscribe("/force_torque_data", 1, &ftCallback);

    while(ros::ok())
    {
        if(goal_pos_update)
        {
            geometry_msgs::Twist msg;
            msg.linear.x = x;
            msg.linear.y = y;
            msg.linear.z = z;
            msg.angular.x = roll;
            msg.angular.y = pitch;
            msg.angular.z = yaw;
            goal_pos_pub.publish(msg);

            goal_pos_update = false;
        }

        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}