#pragma once

#include "Geometry.h"
#include "ImageHeader.h"

class Triangle
{
private:
	
public:

	Triangle(Vec3f _a, Vec3f _b, Vec3f _c, Image::Rgb _color=Image::kWhite):a(_a),b(_b),c(_c),color(_color){}
	Vec3f a;
	Vec3f b;
	Vec3f c;
	Image::Rgb color;
	

};