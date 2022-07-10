#pragma once
#include "Matrix.h"

//maybe make a new header file for this
class Transform
{
public:
    Transform() = default;
    Vec3f position;
    Vec3f rotation;//in degrees 0-360?
    Vec3f scale;//how much scale the points will be

    //for now, we create 3 matrices each of which when used on a point, define it's final position

    TranslationMatrix positionMatrix;
    RotationMatrixX rotationMatrixX;
    RotationMatrixY rotationMatrixY;
    RotationMatrixZ rotationMatrixZ;
    ScaleMatrix scaleMatrix;

    Transform(Vec3f pos, Vec3f rot, Vec3f s)
        :position(pos),
        rotation(rot),
        scale(s),
        positionMatrix(pos),
        rotationMatrixX(rot.x), rotationMatrixY(rot.y), rotationMatrixZ(rot.z),
        scaleMatrix(s)
    {}

    Transform(Vec3f pos, Vec3f rot, float s)
        :position(pos),
        rotation(rot),
        scale(s, s, s),
        positionMatrix(pos),
        rotationMatrixX(rot.x), rotationMatrixY(rot.y), rotationMatrixZ(rot.z),
        scaleMatrix(s)
    {}



};

inline Vec3f ApplyTransform(Vec3f input, Transform t)
{
    const Vec3f scaled = t.scaleMatrix.multiplyVectorMatrix(input);
    const Vec3f rotated = (t.rotationMatrixX * t.rotationMatrixY * t.rotationMatrixZ).multiplyVectorMatrix(scaled);
    const Vec3f translated = t.positionMatrix.multiplyVectorMatrix(rotated);
    return translated;
}

/*
 *order of transformation
 *scale
 *rotate
 *translate
 *
 */