/*
 *
 *
 *
 *Versión 01 de la clase Attitude
 *
 *25/02/19
 *
 *danielghgr
 *
 *
 *
 *
 */


class Attitude {
public:
	Attitude();
	Attitude(float _angx, float _angy, float _angz);
	virtual ~Attitude(){;};

	float get_angx() {return angx;};
	float get_angy() {return angy;};
	float get_angz() {return angz;};

	void set_angx(float _angx) {angx = _angx;};
	void set_angy(float _angy) {angy = _angy;};
	void set_angz(float _angz) {angz = _angz;};


private:
		float angx, angy, angz; //Radians; Roll, Pitch and Yaw (Horizonte local)

};
