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

#include "Position_01.h"

Position::Position()
{
	x = y = z = 0.0;
}

Position::Position(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}
