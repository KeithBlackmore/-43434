/*

    joy node publish joystick meassage,than sub acall the callback function 

    to publish vel which caculate from parameters named i_velLinear,i_velAngular

    to the topic named /turtle1/cmd_vel,turtle node receieve the meassage and 

    control turtle to move

*/

 

#include <ros/ros.h>

#include <iostream>

#include <geometry_msgs/Twist.h>

#include <sensor_msgs/Joy.h>

#include <sensor_msgs/JointState.h>

 

using namespace std;

 

class TeleopJoy{

    public:

    TeleopJoy();    //define a constructor

    private:

    void callBack(const sensor_msgs::Joy::ConstPtr& joy);   //define a callback function,param is the data adress which sub recieve

    ros::NodeHandle n;

    ros::Publisher pub;

    ros::Subscriber sub;

    int i_velLinear,i_velAngular;   //device's axis input value

};

 

TeleopJoy::TeleopJoy()

{

    i_velLinear = 1;

    i_velAngular = 1;

    n.param("axis_linear",i_velLinear,i_velLinear); //set vaule from param

    n.param("axis_angular",i_velAngular,i_velAngular);

    /*

        pub advertise meassage to topic named /joint_states,maximum quene:100

        meassage type is ensor_msgs::JointState    

    */

    pub = n.advertise<sensor_msgs::JointState>("/joint_states",100); 

    /*

        sub subscribe from topic named 'joy' and after receive using the callback function

        meassage type is sensor_msgs::Joy

    */

    sub = n.subscribe<sensor_msgs::Joy>("joy",100,

    &TeleopJoy::callBack,this);

}

/*

    every time sub node receive messages will recall callback function

    it will create a new value named vel and publish it 

*/

void TeleopJoy::callBack(const sensor_msgs::Joy::ConstPtr& joy)

{

    sensor_msgs::JointState joint_state;

    joint_state.header.stamp = ros::Time::now();

 

    joint_state.name.resize(5); //指定name数组长度

    joint_state.position.resize(5);//指定position数组长度

    joint_state.velocity.resize(1);//指定velocity数组长度

    joint_state.effort.resize(1);//指定effort数组长度

    joint_state.name ={"joint1","joint2","joint3","joint4","joint5"};

    joint_state.position = {0.00001*joy->axes[0],0.00001*joy->axes[1],0.00001*(joy->axes[2]+32767),0.00001*joy->axes[3],0.00001*joy->axes[4]};//position数组的元素赋值

    joint_state.velocity[0] = 0.00001*(joy->axes[5]+32767);

    joint_state.effort[0] = 0.00001*(joy->axes[5]+32767);

 

    pub.publish(joint_state);   //publish the meassage

}

int main(int argc,char** argv)

{

    ros::init(argc,argv,"joystick");

    TeleopJoy teleop_joy;   //create an object 

    ros::spin();

}
