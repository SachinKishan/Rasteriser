#pragma once
#include <string>
#include <vector>

#include "Geometry.h"
#include "Triangle.h"
#include "Transform.h"



class ShapeModel//this is supposed to be called model 
{
public:
	std::string name;
	std::vector<Vec3f> vertices;
	std::vector<Triangle> triangles;
	ShapeModel() = default;
	ShapeModel(const std::string& _name):name(_name){}
	ShapeModel(const std::string& _name, std::vector<Vec3f> verts, std::vector<Triangle> tris):name(_name), vertices(verts),triangles(tris)
	{
				
	}

};

class Cube:public ShapeModel
{
    public:
	Cube(const std::string& _name):ShapeModel(_name)
	{
        Vec3f p1(1, 1, 1);
        Vec3f p2(-1, 1, 1);
        Vec3f p3(-1, -1, 1);
        Vec3f p4(1, -1, 1);
        Vec3f p5(1, 1, -1);
        Vec3f p6(-1, 1, -1);
        Vec3f p7(-1, -1, -1);
        Vec3f p8(1, -1, -1);
        vertices.push_back(p1);
        vertices.push_back(p2);
        vertices.push_back(p3);
        vertices.push_back(p4);
        vertices.push_back(p5);
        vertices.push_back(p6);
        vertices.push_back(p7);
        vertices.push_back(p8);

        Triangle t1(p1, p2, p3, Image::kRed);
        Triangle t2(p1, p3, p4, Image::kRed);
        Triangle t3(p5, p1, p4, Image::kBlue);
        Triangle t4(p5, p4, p8, Image::kBlue);
        Triangle t5(p6, p5, p8, Image::kGreen);
        Triangle t6(p6, p8, p7, Image::kGreen);

        Triangle t7(p2, p6, p7, Image::kPurple);
        Triangle t8(p2, p7, p3, Image::kPurple);
        Triangle t9(p5, p6, p2, Image::kYellow);
        Triangle t10(p5, p2, p1, Image::kYellow);
        Triangle t11(p2, p6, p8, Image::kCyan);
        Triangle t12(p3, p8, p4, Image::kCyan);

        triangles.push_back(t1);
        triangles.push_back(t2);
        triangles.push_back(t3);
        triangles.push_back(t4);
        triangles.push_back(t5);
        triangles.push_back(t6);
        triangles.push_back(t7);
        triangles.push_back(t8);
        triangles.push_back(t9);
        triangles.push_back(t10);
        triangles.push_back(t11);
        triangles.push_back(t12);
	}
};

  

class Instance
{
public:

    ShapeModel model;

    //I think positioning can change and depend on model context, we'll look into this after implementing basic cubes
    Transform transform;
    

    Instance() = default;
    Instance(ShapeModel _model, Transform t):model(_model),transform(t)
    {
	    
    }


};
