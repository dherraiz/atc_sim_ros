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



class Flight {
public:
	Flight();
	Flight(int _id, Position _pos, Attitude _att, float _speed);
	~Flight(){;};

  int getId(){return id;};
	Position getPosition() { return pos;};
	Attitude getAttitude() { return att;};
	float getSpeed() { return speed;};
	void setPosition(Position _pos){ pos = _pos;};


private:
	int id;
	Position pos;
  Attitude att;
	float speed;
};
