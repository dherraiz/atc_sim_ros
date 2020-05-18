#include "ros/ros.h"
#include "atc_sim_ros/Flight_Info.h"
#include "atc_sim_ros/Tower_Info.h"

#include <cstdlib>
#include <stdio.h>
#include <list>

#include "Flight_02.cpp"


typedef struct {
  int id;
	Route next_route;
	Position pos;
  float bearing;
  float inclination;
  float speed;
  bool landed;
} Flight_info;

typedef struct {
  int id;
	Route next_route;
	int cod;
} Tower_info;


class Tower
{
public:
  Tower(): n_()
  {
    sub_ = n_.subscribe("flight_info", 1000, &Tower::towerCallback,this);
    pub_ = n_.advertise<atc_sim_ros::Tower_Info>("tower_info", 1000);
    tower_info_pub.id = -1;
    times = 0;
  }

  void towerCallback(const atc_sim_ros::Flight_Info& flight);
  void dowork();

private:
  ros::NodeHandle n_;
  ros::Subscriber sub_;
  ros::Publisher pub_;
  Flight_info flight_info;
  atc_sim_ros::Tower_Info tower_info_pub;
  int times;

};


void
Tower::towerCallback(const atc_sim_ros::Flight_Info& flight)
{

  Position pos(flight.pos.x, flight.pos.y, flight.pos.z);
  Position pos_route(flight.route.pos.x, flight.route.pos.y, flight.route.pos.z);

  flight_info.id = flight.id;
  flight_info.pos = pos;
  flight_info.next_route.pos=pos_route;
  flight_info.next_route.speed=flight.route.speed;
  flight_info.bearing = flight.bearing;
  flight_info.inclination = flight.inclination;
  flight_info.landed = flight.landed;

}

void
Tower::dowork(){

  if(times>0){

    float dist_min_AP = .05f;
    float landing_speed = 0.1;
    float landing_bear_max_error = 0.5; //En radianes
    float landing_incl_max_error = 0.3;

    Position AirportPos(0,0,0);

    if(flight_info.pos.distanceXY(AirportPos) <= dist_min_AP &&
        flight_info.speed <= landing_speed &&
        ((abs(flight_info.bearing)>=M_PI-landing_incl_max_error) ||
        (abs(flight_info.bearing)<=0+landing_incl_max_error)) &&
        abs(flight_info.inclination) <= landing_incl_max_error &&
        !(flight_info.landed)){

           ROS_INFO("TOWER: Flight number %d has landed", flight_info.id);
           tower_info_pub.cod = 4;
           pub_.publish(tower_info_pub);
  	}

    if(tower_info_pub.id != flight_info.id){
      int cuadrante;
      Position new_route, airport_route;
      if((flight_info.pos).get_x() >= 0){
        new_route.set_x(1);
        new_route.set_y(0);
        new_route.set_z(0.02);
        if((flight_info.pos).get_y() >= 0){
          cuadrante = 1;
        }else{
          cuadrante = 4;
        }
      }else{
        new_route.set_x(-1);
        new_route.set_y(0);
        new_route.set_z(0.02);
        if((flight_info.pos).get_y() >= 0){
          cuadrante = 2;
        }else{
          cuadrante = 3;

        }
      }
      ROS_INFO("TOWER: NUEVO AVION DETECTADO EN EL %d CUADRANTE", cuadrante);

      airport_route.set_x(0.0);
      airport_route.set_y(0.0);
      airport_route.set_z(0.01);

      tower_info_pub.route.pos.x=airport_route.get_x();
      tower_info_pub.route.pos.y=airport_route.get_y();
      tower_info_pub.route.pos.z=airport_route.get_z();
      tower_info_pub.route.speed=0.05;
      tower_info_pub.id = flight_info.id;
      tower_info_pub.cod = 1;

      pub_.publish(tower_info_pub);


      tower_info_pub.route.pos.x=new_route.get_x();
      tower_info_pub.route.pos.y=new_route.get_y();
      tower_info_pub.route.pos.z=new_route.get_z();
      tower_info_pub.route.speed=0.1;

      pub_.publish(tower_info_pub);


    }

    tower_info_pub.id = flight_info.id;

  }

  times++;
}
