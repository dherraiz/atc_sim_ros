#include "ros/ros.h"
#include "atc_sim_ros/GenerateFlight.h"
#include "atc_sim_ros/Flight_Info.h"
#include <cstdlib>
#include <list>

#include "tf2/transform_datatypes.h"
#include "tf2/LinearMath/Transform.h"
#include "geometry_msgs/TransformStamped.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"
#include "tf2/convert.h"
#include "tf2_ros/transform_broadcaster.h"
#include "tf2/LinearMath/Quaternion.h"

#include "Tower.cpp"


#define OBJ_PUB_RATE  0.05
#define FLIGHT_GENERATE_RATE 300


geometry_msgs::TransformStamped generate_flight_obj(Position pos, float bearing, float inclination, int num)
{
  tf2::Stamped<tf2::Transform> object;
  object.frame_id_ = "world";
  object.stamp_ = ros::Time::now();

  object.setOrigin(tf2::Vector3(pos.get_x(), pos.get_y(), pos.get_z()));

  tf2::Quaternion q;
  q.setRPY(0, -inclination, bearing);
  object.setRotation(q);

  geometry_msgs::TransformStamped object_msg = tf2::toMsg(object);
  char name_flight[8];
  sprintf(name_flight, "flight_%d", num);
  object_msg.child_frame_id = name_flight;

  return object_msg;
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "main");  //Iniciar módulo
  ros::NodeHandle n;

  ros::ServiceClient client = n.serviceClient<atc_sim_ros::GenerateFlight>("generate_flights"); //Declarar servicio
  atc_sim_ros::GenerateFlight srv;

  ros::Publisher flight_info_pub = n.advertise<atc_sim_ros::Flight_Info>("flight_info", 1000); //Declarar publicación de info
  atc_sim_ros::Flight_Info flight_pub;

  Flight flight_created;  // Nuevo avión
  int id_count = 1;

  Tower tower; //Nueva torre de control "Tower"

  tf2_ros::TransformBroadcaster br; // Declarar tf
  geometry_msgs::TransformStamped flight_obj_msg;

  ros::Time obj_ts = ros::Time::now(); //Iniciar tiempo
  ros::Time gen_ts = ros::Time::now();

  float time_gen = 0.01;  //Declarar tiempo de actualización
  ros::Rate loop_rate(1/time_gen);

  //std::list<Flight*> flights;//---------------------------- Iciciar lista (?)
  //std::list<Flight*>::iterator it;


  //---------------------------------------------------------- SERVICIO PARA SOLICITAR UNA GENERACIÓN DE UN NUEVO AVIÓN
  srv.request.id_req = id_count++;
  if (client.call(srv)){

    float pos_x =srv.response.x;
    float pos_y =srv.response.y;
    float pos_z =srv.response.z;
    Position pos(pos_x, pos_y,  pos_z);

    flight_created.setId(srv.response.id);
    flight_created.setPosition(pos);
    flight_created.setBearing(srv.response.bearing);
    flight_created.setInclination(srv.response.inclination);
    flight_created.setSpeed(srv.response.speed);
    flight_created.setLanded(false);


  }else{
    ROS_ERROR("Failed to call service GenerateFlight");
    return 1;
  }

  while (ros::ok()) //BUCLE PRINCIPAL
  {

    if ((ros::Time::now() - obj_ts).toSec() > OBJ_PUB_RATE) // Sólo entra cada OBJ_PUB_RATE (0.05) segundos
    {
      obj_ts = ros::Time::now();
      id_count = 1;

      flight_created.update(OBJ_PUB_RATE);
      flight_obj_msg = generate_flight_obj(flight_created.getPosition(), flight_created.getBearing(), flight_created.getInclination(), id_count ++);

      //----------publicar info
      flight_pub.id = flight_created.getId();
      flight_pub.pos.x= (flight_created.getPosition()).get_x();
      flight_pub.pos.y= (flight_created.getPosition()).get_y();
      flight_pub.pos.z= (flight_created.getPosition()).get_z();
      flight_pub.route.pos.x= (flight_created.getRoute()->front().pos.get_x());
      flight_pub.route.pos.y= (flight_created.getRoute()->front().pos.get_y());
      flight_pub.route.pos.z= (flight_created.getRoute()->front().pos.get_z());
      flight_pub.speed = flight_created.getSpeed();
      flight_pub.bearing = flight_created.getBearing();
      flight_pub.inclination = flight_created.getInclination();
      flight_pub.landed = flight_created.getLanded();


      flight_info_pub.publish(flight_pub);
      tower.dowork();
        //----------publicar info

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
