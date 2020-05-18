/*
 * BASED ON:
 *
 * Position.h & Position.cpp
 *
 *  Created on: 15/07/2014
 *      Author: paco
 *
 */

#include <string>
#include <math.h>

class Position {
public:
	Position();
	Position(float _x, float _y);
	Position(float _x, float _y, float _z);
	Position(std::string _name, float _x, float _y);
	Position(std::string _name, float _x, float _y, float _z);
	virtual ~Position();

	std::string get_name() {return name;};
	float get_x() {return x;};
	float get_y() {return y;};
	float get_z() {return z;};

	void set_name(std::string _name) {name = check_name(_name);};
	void set_x(float _x) {x = _x;};
	void set_y(float _y) {y = _y;};
	void set_z(float _z) {z = _z;};

	float distance(Position pos);
	float distanceXY(Position pos);
	float distX_EjeBody(Position pos1, Position pos2);
	float get_angle(Position pos1, Position pos2);
	void angles(Position pos, float &bearing, float &inclination);

private:
		std::string name;
		float x, y, z;

		std::string check_name(std::string name);

};

Position::Position() {

	name = "";
	x = y = z = 0.0;
}

Position::Position(float _x, float _y)
{
	name = "";
	x = _x;
	y = _y;
	z = 0;
}

Position::Position(float _x, float _y, float _z)
{
	name = "";
	x = _x;
	y = _y;
	z = _z;
}

Position::Position(std::string _name, float _x, float _y)
{
	name = check_name(_name);
	x = _x;
	y = _y;
	z = 0;
}

Position::Position(std::string _name, float _x, float _y, float _z)
{
	name = check_name(_name);
	x = _x;
	y = _y;
	z = _z;
}

Position::~Position() {

}

std::string Position::check_name(std::string name)
{
	ushort max_length = 5;
	std::string aux;
	// Limit name to 5 characters
	if(name.length() > max_length)
	{
		aux = name.substr(0, max_length);
	}
	// Fill name to 5 characters
	else if(name.length() < max_length)
	{
		aux = name.substr(0, name.length());
		for(int i=name.length(); i<max_length; i++)
		aux += "0";
	}
	else
	{
		aux = name;
	}

	return aux;
}

float
Position::distance(Position pos)
{
	return sqrtf( ( (x - pos.get_x()) * (x - pos.get_x()) ) +
			      ( (y - pos.get_y()) * (y - pos.get_y()) ) +
		       	  ( (z - pos.get_z()) * (z - pos.get_z()) ) );
}

float
Position::distanceXY(Position pos)
{
	return sqrtf( ((x-pos.get_x())*(x-pos.get_x())) + ((y-pos.get_y())*(y-pos.get_y())));
}

float
Position::distX_EjeBody(Position pos1, Position pos2)
{
	float betta = atan2f(pos1.get_y()-y, pos1.get_x()-x);

	return cos(betta)*(pos2.get_x()-pos1.get_x()) + sin(betta)*(pos2.get_y()-pos1.get_y());
}

float
Position::get_angle(Position pos1, Position pos2)
{
	float u1, u2, v1, v2;

	u1 = pos1.get_x()-x;
	u2 = pos1.get_y()-y;
	v1 = pos2.get_x()-pos1.get_x();
	v2 = pos2.get_y()-pos1.get_y();

	return acos((fabs(u1*v1+u2*v2)) / (sqrt(u1*u1+u2*u2)*sqrt(v1*v1+v2*v2)));
}

void
Position::angles(Position pos, float &bearing, float &inclination)
{
	float distxy;

	distxy = sqrtf( ((x-pos.get_x())*(x-pos.get_x())) + ((y-pos.get_y())*(y-pos.get_y())));

	bearing = atan2f(y-pos.get_y(), x-pos.get_x());
	inclination = atan2f(pos.get_z()-z, distxy);
}
