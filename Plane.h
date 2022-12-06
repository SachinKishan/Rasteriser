#pragma once
#include "Geometry.h"


class Plane
{
public:


	float A, B, C, D;//Ax+By+Cz+D=0
	//where D is distance from origin along plane normal

	Vec3f point;
	


	Vec3f normal;//normal to the plane

	Plane() = default;
	Plane(float a,float b,float c,float d):A(a),B(b),C(c),D(d)
	{
		normal = Vec3f(a,b,c).normalize();
		point = Vec3f(0,0,D);
	}


};

inline float SignedDistance(const Plane& plane, const Vec3f& vertex)
{
	const Vec3f normal = plane.normal;
	return normal * vertex + plane.D;
}

class Line
{
public:
	Vec3f start;
	Vec3f end;
	Vec3f direction;

	Line(Vec3f p1, Vec3f p2):start(p1),end(p2),direction(p2-p1)
	{
	}

};

inline Vec3f LinePlaneIntersection(Plane p, Line l)
{
	
	const float t = ((p.normal * p.point)-(p.normal*l.start))/(p.normal*l.direction);
	return l.start + (l.direction * t);

}

