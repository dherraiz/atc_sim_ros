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
	trans = speed * delta_t;

	//-----------------------

	float roll 	= att.get_angx();
	float pitch = att.get_angy();
	float yaw 	= att.get_angz();

	float xb, yb, zb, x_aux1, y_aux1, z_aux1, x_aux2, y_aux2, z_aux2, xh, yh, zh;

	xb = trans;
	yb = 0;
	zb = 0;
//----------------------------TODO: Introducir roll
	xh = xb * cos(yaw) * cos(pitch);
	yh = xb * sin(yaw) * cos(pitch);
	zh = xb * (-sin(pitch));


	pos.set_x(pos.get_x() + xh);
	pos.set_y(pos.get_y() + yh);
	pos.set_z(pos.get_z() + zh);
}
