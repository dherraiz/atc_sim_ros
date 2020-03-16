#include "ros/ros.h"
#include "atc_sim_ros/GenerateFlight.h"
#include <cstdlib>
#include <list>

#include "tf2/transform_datatypes.h"
#include "tf2/LinearMath/Transform.h"
#include "geometry_msgs/TransformStamped.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"
#include "tf2/convert.h"
#include "tf2_ros/transform_broadcaster.h"
#include "tf2/LinearMath/Quaternion.h"

#include "Flight_01.cpp"

#define OBJ_PUB_RATE  0.05
#define FLIGHT_GENERATE_RATE  5


geometry_msgs::TransformStamped generate_flight_obj(Position pos, Attitude att, int num)
{
  tf2::Stamped<tf2::Transform> object;
  object.frame_id_ = "world";
  object.stamp_ = ros::Time::now();

  object.setOrigin(tf2::Vector3(pos.get_x(), pos.get_y(), pos.get_z()));

  tf2::Quaternion q;
  q.setRPY(att.get_angx(), att.get_angy(), att.get_angz());
  object.setRotation(q);

  geometry_msgs::TransformStamped object_msg = tf2::toMsg(object);
  char name_flight[8];
  sprintf(name_flight, "flight_%d", num);
  object_msg.child_frame_id = name_flight;

  return object_msg;
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "main");
  ros::NodeHandle n;

  ros::ServiceClient client = n.serviceClient<atc_sim_ros::GenerateFlight>("generate_flights");
  atc_sim_ros::GenerateFlight srv;

  Flight flight_created_1;
  int id_count = 1;


  tf2_ros::TransformBroadcaster br;

  geometry_msgs::TransformStamped flight_obj_msg;

  ros::Time obj_ts = ros::Time::now();
  ros::Time gen_ts = ros::Time::now();

  float time_gen = 0.01;
  ros::Rate loop_rate(1/time_gen);

  std::list<Flight*> flights;
  std::list<Flight*>::iterator it;


  while (ros::ok())
  {
    if ((ros::Time::now() - gen_ts).toSec() > FLIGHT_GENERATE_RATE)
    {
      gen_ts= ros::Time::now();
      if (client.call(srv)){
        srv.request.id_req = id_count++;
        Position pos(srv.response.x,srv.response.y,srv.response.z);
        Attitude att(srv.response.angx,srv.response.angy,srv.response.angz);
        flight_created_1.setId(srv.response.id);
        flight_created_1.setPosition(pos);
        flight_created_1.setAttitude(att);
        flight_created_1.setSpeed(srv.response.speed);

        flights.push_back(&flight_created_1);
      }else{
        ROS_ERROR("Failed to call service GenerateFlight");
        return 1;
      }
    }

    if ((ros::Time::now() - obj_ts).toSec() > OBJ_PUB_RATE)
    {
      obj_ts = ros::Time::now();
      for(it = flights.begin(); it!=flights.end(); ++it)
      {
        (*it)->update(time_gen);
        flight_obj_msg = generate_flight_obj(flight_created_1.getPosition(), flight_created_1.getAttitude(), 1);
      }
    }

    try
    {
      flight_obj_msg.header.stamp = ros::Time::now();
      br.sendTransform(flight_obj_msg);
    }
    catch(tf2::TransformException &exception)
    {
      ROS_ERROR("04");
      ROS_ERROR("%s", exception.what());
    }

    ros::spinOnce();
    loop_rate.sleep();

  }

  return 0;
}
