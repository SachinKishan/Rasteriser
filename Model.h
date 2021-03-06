#pragma once
#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include<string>
#include<sstream>
#include "geometry.h"

class Model {
private:
	std::vector<Vec3f> verts_; // array/vector of vertices for each triangle
	std::vector<std::vector<int> > faces_; //array/vector of 
public:
	Model(const char* filename);
	~Model();
	int nverts();
	int nfaces();
	Vec3f vert(int i);
	std::vector<int> face(int idx);
};

Model::Model(const char* filename) : verts_(), faces_() {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        
        char trash;
        
        if (!line.compare(0, 2,"v " )) 
        {
            //std::cout << "Counting vertices"<<std::endl;
            iss >> trash;//remove starting character v
            Vec3f v;//temp varaible to store from each line of the file
            for (int i = 0; i < 3; i++) iss >> v.raw[i];//next three strings are passed into the vec3 object 
            verts_.push_back(v);//pass the vec3 object into the vector/array of vertices
        }


        else if (!line.compare(0, 2, "f ")) //used for something called polygonal face elements, idk what that means. We'll get back to this later
        {
            std::vector<int> f;
            int itrash, idx;
            iss >> trash;
            while (iss >> idx >> trash >> itrash >> trash >> itrash) 
            {
                idx--; // in wavefront obj all indices start at 1, not zero
                f.push_back(idx);
            }
            faces_.push_back(f);
        }
    }
    //std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

std::vector<int> Model::face(int idx) {
    return faces_[idx];
}

Vec3f Model::vert(int i) {
    return verts_[i];
}

#endif //__MODEL_H__