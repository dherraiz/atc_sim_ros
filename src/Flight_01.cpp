/*
 *
 *
 *
 *Versión 01 de la clase Flight
 *
 *25/02/19
 *
 *danielghgr
 *
 *
 *
 *
 */
#include <string>
#include <stdio.h>
#include <iostream>

#include <string>
#include <math.h>

#include "Position_01.cpp"
#include "Attitude_01.cpp"


typedef struct {
	Position pos;
	float speed;
} Route;

class Flight {
public:
	Flight();
	Flight(int _id, Position _pos, Attitude _att, float _speed);
	~Flight(){;};

	void update(float _delta_t);
	bool routed() { return !route.empty();};

  int getId(){return id;};
	std::list<Route> *getRoute() { return &route;};
	Position getPosition() { return pos;};
	Attitude getAttitude() { return att;};
	float getSpeed() { return speed;};
	void setId(int _id){ id = _id;};
	void setPosition(Position _pos){ pos = _pos;};
	void setAttitude(Attitude _att){ att = _att;};
	void setSpeed(float _speed){ speed = _speed;};

private:
	int id;
	Position pos;
  Attitude att;
	float speed;
	std::list<Route> route;
};


Flight::Flight()
{
	Position _pos;
	Attitude _att;
	id = 0;
	pos = _pos;
	att = _att;
	speed = 0;

}

Flight::Flight(int _id, Position _pos, Attitude _att, float _speed)
{
	id = _id;
	pos = _pos;
	att = _att;
  speed = _speed;
}




void
Flight::update(float delta_t)
{
	float trans;
	Position CPpos;

	if(routed())
	{
		ROS_INFO("ROUTED");
		Position pos_wp;
		float speed_wp;

		pos_wp = route.front().pos;
		speed_wp = route.front().speed;

		//---------Ajustar velocidad:
		float speed_dif;

		speed_dif = speed_wp - speed;
		if( abs(speed_dif) > 0.3){
			speed_dif = (speed_dif/abs(speed_dif))*0.3;
		}
		setSpeed(speed + speed_dif*delta_t);

		//---------Ajustar ángulos:
		float ang_z_goal, ang_y_goal, ang_z_dif, ang_y_dif, dist_xy;

		dist_xy = sqrtf( ((pos.get_x()-pos_wp.get_x())*(pos.get_x()-pos_wp.get_x())) + ((pos.get_y()-pos_wp.get_y())*(pos.get_y()-pos_wp.get_y())) );

		ang_z_goal = atan2f( pos.get_x()-pos_wp.get_x(), pos.get_y()-pos_wp.get_y() );
		ang_y_goal = atan2f( pos.get_z()-pos_wp.get_z(),dist_xy );

		ang_z_dif = ang_z_goal - att.get_angz();
		if( abs(ang_z_dif) > 0.2){
			ang_z_dif = (ang_z_dif/abs(ang_z_dif))*0.2;
		}
		att.set_angz(att.get_angz() + ang_z_dif*delta_t);

		ang_y_dif = ang_y_goal - att.get_angy();
		if( abs(ang_y_dif) > 0.2){
			ang_y_dif = (ang_y_dif/abs(ang_y_dif))*0.2;
		}
		att.set_angy(att.get_angy() + ang_y_dif*delta_t);

	}

	float xb, yb, zb, xh, yh, zh, roll, pitch, yaw;

	trans = speed * delta_t;

	roll 	= att.get_angx();
	pitch = att.get_angy();
	yaw 	= att.get_angz();

	xb = trans;
	yb = 0;
	zb = 0;
//----------------------------TODO: Introducir roll
	xh = xb * cos(yaw) * cos(pitch);
	yh = xb * sin(yaw) * cos(pitch);
	zh = xb * (-sin(pitch));

	pos.set_x(pos.get_x() + xh);
//	ROS_INFO("POS X: %f",pos.get_x());
	pos.set_y(pos.get_y() + yh);
	pos.set_z(pos.get_z() + zh);

	//---------Comprobar si ha llegado al wp
	if(pos.distance(CPpos)<0.1){
		route.pop_front();
	}

}
