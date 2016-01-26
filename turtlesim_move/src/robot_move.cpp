/*
The MIT License (MIT)

Copyright (c) 2016 TatsuyaAmano

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/



#include <stdio.h>
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"
#include <sstream>

using namespace std;

ros::Publisher velocity_publisher;
ros::Subscriber pose_subscriber;
turtlesim::Pose turtlesim_pose;

const double x_min = 0.0;
const double y_min = 0.0;
const double x_max = 11.0;
const double y_max = 11.0;
const double PI = 3.14159265359;

void move(double speed, double distance, bool isForward);
void rotate (double angular_speed, double angle, bool clockwise);
double degrees2radians(double angle_in_degrees);
void poseCallback(const turtlesim::Pose::ConstPtr & pose_message);
void writeCircle(double radius, double turn_angle, bool clockwise);

int main(int argc, char **argv)
{
	ros::init(argc, argv, "turtlesim_move");
	ros::NodeHandle n;
	double speed, angular_speed;
	double distance, angle;
	bool isForward, clockwise;
        int i;
	turtlesim::Pose goal_pose;

	velocity_publisher = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1000);
	pose_subscriber = n.subscribe("/turtle1/pose", 10, poseCallback);
	ros::Rate loop_rate(10);

	while(ros::ok()){
                 rotate(degrees2radians(50), degrees2radians(120), 0);
                 move(7 ,2 ,1);
                 for(i=1;i<=1;i++){
                 }
                 writeCircle(i, 2*PI, 0);
                 for(i=1; i<=3; i++){ 
                 writeCircle(i, 2*PI, 1);
                 }
                 move(7, 4, 0);
                 for(i=1; i<=4; i++){ 
                 writeCircle(i, 2*PI, 0);
                 }
                 for(i=1; i<=2; i++){ 
                 writeCircle(i, 2*PI, 1);
                 }
		double t0 = ros::Time::now().toSec(); 
		double t1 = ros::Time::now().toSec(); 
		ROS_INFO ("Cleaning execution time: %.2f", (t1-t0));
		return 0;
	}

	ros::spin();

   return 0;
}
void poseCallback(const turtlesim::Pose::ConstPtr & pose_message){
     turtlesim_pose.x = pose_message -> x;
     turtlesim_pose.y = pose_message -> y;
     turtlesim_pose.theta = pose_message -> theta;
}

void move(double speed, double distance, bool isForward){
	geometry_msgs::Twist vel_msg;

   if (isForward)
	   vel_msg.linear.x =abs(speed);
   else
	   vel_msg.linear.x =-abs(speed);
   vel_msg.linear.y =0;
   vel_msg.linear.z =0;
   vel_msg.angular.x = 0;
   vel_msg.angular.y = 0;
   vel_msg.angular.z =0;

   double t0 = ros::Time::now().toSec();
   double current_distance = 0.0;
   ros::Rate loop_rate(100);
   do{
	   velocity_publisher.publish(vel_msg);
	   double t1 = ros::Time::now().toSec();
	   current_distance = speed * (t1-t0);
	   ros::spinOnce();
	   loop_rate.sleep();
   }while(current_distance<distance);
   vel_msg.linear.x =0;
   velocity_publisher.publish(vel_msg);

}

void rotate (double angular_speed, double relative_angle, bool clockwise){

	geometry_msgs::Twist vel_msg;
	   vel_msg.linear.x =0;
	   vel_msg.linear.y =0;
	   vel_msg.linear.z =0;
	   vel_msg.angular.x = 0;
	   vel_msg.angular.y = 0;
	   if (clockwise)
	   		   vel_msg.angular.z =-abs(angular_speed);
	   	   else
	   		   vel_msg.angular.z =abs(angular_speed);

	   double t0 = ros::Time::now().toSec();
	   double current_angle = 0.0;
	   ros::Rate loop_rate(1000);
	   do{
		   velocity_publisher.publish(vel_msg);
		   double t1 = ros::Time::now().toSec();
		   current_angle = angular_speed * (t1-t0);
		   ros::spinOnce();
		   loop_rate.sleep();
	   }while(current_angle<relative_angle);
	   vel_msg.angular.z =0;
	   velocity_publisher.publish(vel_msg);
}

void writeCircle(double radius, double turn_angle, bool clockwise)
{
     double t0;
     double t1;
     double current_angle = 0.0;

     geometry_msgs::Twist vel_msg;

     vel_msg.linear.x = 0;
     vel_msg.linear.y = 0;
     vel_msg.linear.z = 0;
     vel_msg.angular.x = 0;
     vel_msg.angular.y = 0;
     vel_msg.angular.z = 0;
     t0 = ros::Time::now().toSec();
     ros::Rate loop_rate(1000);

     if(clockwise){
          vel_msg.angular.z = abs(turn_angle);
          vel_msg.linear.x = abs(radius * PI);
     }
     else {
          vel_msg.angular.z = -abs(turn_angle);
          vel_msg.linear.x = abs(radius * PI);
     }

     do{
          velocity_publisher.publish(vel_msg);
          t1 = ros::Time::now().toSec();
          current_angle = turn_angle * (t1 - t0);
          loop_rate.sleep();
     }while(current_angle < turn_angle);

     vel_msg.angular.z = 0;
     vel_msg.linear.x = 0;
     velocity_publisher.publish(vel_msg);
}

double degrees2radians(double angle_in_degrees){
	return angle_in_degrees *PI /180.0;
}


