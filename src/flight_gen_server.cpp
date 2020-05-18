#include "ros/ros.h"
#include "atc_sim_ros/GenerateFlight.h"

#include <sstream>

#include <string>
#include <stdio.h>
#include <iostream>

#include <math.h>
#include <cmath>
#include <ctime>
#include <cstdlib>

#include "Flight_02.cpp"

Flight generate(int id){
  double PI = acos(-1.0);

  float distance = 2; //PARÁMETROS DEL AEROPUERTO
  int h_min = 0.2; //PARÁMETROS DEL AEROPUERTO
  int h_max = 0.4; //PARÁMETROS DEL AEROPUERTO
  float speed = 1; //PARÁMETROS DEL AEROPUERTO


  ROS_INFO("Generate new flight (ID = %d)",id);
	float angle, x, y, z, inclination, bearing;
  srand(time(NULL));

//	angle = toRadians((float)(rand() % 360)); FIX!!
  angle = ((float)(rand() % 360))*PI/180;

	x = distance * cos(angle);
	y = distance * sin(angle);
	//z = (h_min + (float)(rand() % (h_max - h_min)));//TODO


	Position pos(x, y, 0.2); //Forced for testing


  bearing = - PI + angle;
  inclination = 0;

  return Flight(id, pos, bearing, inclination, speed);
}


bool add(atc_sim_ros::GenerateFlight::Request  &req,
         atc_sim_ros::GenerateFlight::Response &res)
{


  Flight f1 = generate(req.id_req);

//-----------------------------flight = f1
  res.id = f1.getId();
  res.x= f1.getPosition().get_x();
  res.y= f1.getPosition().get_y();
  res.z= f1.getPosition().get_z();
  res.bearing= f1.getBearing();
  res.inclination= f1.getInclination();
  res.speed = f1.getSpeed();

//----------------------------
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "flight_gen_server");
  ros::NodeHandle n;

  ros::ServiceServer service = n.advertiseService("generate_flights", add);
  ROS_INFO("Ready to generate flights.");
  ros::spin();

  return 0;
}
