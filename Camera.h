#pragma once
#include "Transform.h"


class Camera
{
public:
	Transform transform;
	int cameraViewHeight = 1;
	int cameraViewWidth = 1;//raster space
	float d = 1;//distance between camera and canvas
	Camera() = default;
	Camera(Transform t, int camHeight, int camWidth,float _d):transform(t),cameraViewHeight(camHeight),cameraViewWidth(camWidth),d(_d)
	{
		
	}
	Vec3f InvertPointToCamera(Vec3f vertex)
	{
		vertex = (transform.positionMatrix * -1).multiplyVectorMatrix(vertex);
		vertex=transform.rotationMatrix.invert().multiplyVectorMatrix(vertex);
		return vertex;
	}

};
