/*
 *
 *
 *
 *Versión 01 de la clase Position
 *
 *25/02/19
 *
 *danielghgr
 *
 *
 *
 *
 */

class Position {
public:
	Position();
	Position(float _x, float _y, float _z);
	~Position() {;};

	float get_x() {return x;};
	float get_y() {return y;};
	float get_z() {return z;};

	void set_x(float _x) {x = _x;};
	void set_y(float _y) {y = _y;};
	void set_z(float _z) {z = _z;};

	float distance(Position pos);


private:
		float x, y, z; //meters

};
