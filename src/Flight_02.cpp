/*
 * BASED ON:
 *
 * Flight.h & Flight.cpp
 *
 *  Created on: 15/07/2014
 *      Author: paco
 *
 */
#include "ros/ros.h"
 #include "atc_sim_ros/Flight_Info.h"
 #include "atc_sim_ros/Tower_Info.h"

#include <string>
#include <iostream>
#include <math.h>

#include "Position_02.cpp"

#include <cstdlib>
#include <stdio.h>
#include <list>


typedef struct {
	Position pos;
	float speed;
} Route;

class Flight {
public:
	Flight(): n_()
	{
			sub_ = n_.subscribe("tower_info", 1000, &Flight::flightCallback,this);
			id = 0;
			bearing = 0;
			init_bearing = 0;
			inclination = 0;
			speed = 0;
	}
	Flight(int _id, Position _pos, float _bearing, float _inclination, float _speed);
	virtual ~Flight();

	void update(float delta_t);

	std::list<Route> *getRoute() { return &route;};
	bool routed() { return !route.empty();};
	Position getPosition() { return pos;};
	void setPosition(Position _position){ pos = _position;};
	float getInclination() { return inclination;};
	void setInclination(float _inclination){inclination = _inclination;};
	float getBearing() { return bearing;};
	void setBearing(float _bearing){bearing = _bearing;};
	float getInitBearing() { return init_bearing;};
	float getSpeed() { return speed;};
	void setSpeed(float tgt_speed) {speed = tgt_speed;}

	int getId(){return id;};
	void setId(int _id){id = _id;};
  bool getLanded(){return landed;};
  void setLanded(int _landed){landed = _landed;};


	void flightCallback(const atc_sim_ros::Tower_Info& tower_info_);

private:
	int id;
	Position pos, last_pos;
	float init_bearing, bearing, inclination;
	float speed, w_speed;
	std::list<Route> route;
  bool landed;


	ros::NodeHandle n_;
  ros::Subscriber sub_;

};


Flight::~Flight() {
	// TODO Auto-generated destructor stub
}

Flight::Flight(int _id, Position _pos, float _bearing, float _inclination, float _speed)
{
	id = _id;
	pos = _pos;
	bearing = _bearing;
	init_bearing = _bearing;
	inclination = _inclination;
	speed = _speed;

	route.clear();

	w_speed = 0.0;
}

void
Flight::flightCallback(const atc_sim_ros::Tower_Info& tower_info)
{

  if (tower_info.id == id){
    if (tower_info.cod == 1){
      Position pos0(tower_info.route.pos.x, tower_info.route.pos.y, tower_info.route.pos.z);
      Route r0;
      r0.pos = pos0;
      r0.speed = tower_info.route.speed;
      route.push_front(r0);
      ROS_INFO("FLIGHT: Nueva ruta: (%f, %f, %f), vel: %f",tower_info.route.pos.x,
      tower_info.route.pos.y, tower_info.route.pos.z, tower_info.route.speed);
    }else if (tower_info.cod == 4){
      ROS_INFO("FLIGHT: OK, we have landed");
      landed = true;
    }
  }

}

double normalizePi(double data) {
  if (data < M_PI && data >= -M_PI) return data;
  double ndata = data - ((int )(data / M_PI * 2))*M_PI * 2;
  while (ndata >= M_PI) {
    ndata -= M_PI * 2;
  }

  while (ndata < -M_PI) {
    ndata += M_PI * 2;
  }

  return ndata;
}

void
Flight::update(float delta_t)
{
	float trans;
	Position CPpos;
	float distXY_CP = .1f;

  if(!(landed)){

  	if(routed())
  	{
  		float goal_bearing, goal_inclination, diff_bearing, diff_inclination, new_wy, new_wz;

  		CPpos = route.front().pos;

  		if(CPpos.get_z() < 0){ // Maintain altitude
  			float current_alt = (this->getPosition()).get_z();
  			CPpos.set_z(current_alt);
  			route.front().pos.set_z(current_alt);
  			ROS_INFO("z: %f",(CPpos.get_z()));
  		}

  		pos.angles(CPpos, goal_bearing, goal_inclination);

  		goal_bearing = normalizePi(goal_bearing + M_PI);
  		diff_bearing = normalizePi(goal_bearing - bearing);
  		new_wz = diff_bearing;

  		diff_inclination = goal_inclination - inclination;
  		new_wy = diff_inclination;


  		if(fabs(new_wz)>10) new_wz = (fabs(new_wz)/new_wz) * 10;

  		bearing = bearing + new_wz*delta_t*5;
  		inclination = inclination + new_wy*delta_t*5;

  		float goal_speed, diff_speed, acc;

  		goal_speed = route.front().speed;
  		acc = (goal_speed - speed);

  		if(fabs(acc)>1) acc = (acc/fabs(acc))*1;

  		speed = speed + acc*delta_t;

      if(pos.distance(CPpos)<0 || pos.distanceXY(CPpos)<=distXY_CP){
        route.pop_front();
        ROS_INFO("POPPING ROUTE");

      }

  	}else{
  		inclination = 0.0;
  	}

  	last_pos = pos;

  	trans = speed * delta_t;


  	pos.set_x(pos.get_x() + trans * cos(bearing) * cos(inclination));
  	pos.set_y(pos.get_y() + trans * sin(bearing) * cos(inclination));
  	pos.set_z(pos.get_z() + ( trans * sin(inclination)));

  }else{
    pos.set_x(0);
    pos.set_y(0);
    pos.set_z(0);
    inclination = 0;;

  }


}
