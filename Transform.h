#pragma once
#include "Matrix.h"

//maybe make a new header file for this
class Transform
{
private:
    Vec3f position;
    Vec3f rotation;//in degrees 0-360?
    Vec3f scale;//how much scale the points will be

    void CalculateRotationMatrix()
    {
        rotationMatrix = rotationMatrixX * rotationMatrixY * rotationMatrixZ;
    }

public:
    Transform() = default;
    

    //for now, we create 3 matrices each of which when used on a point, define it's final position

    TranslationMatrix positionMatrix;
    RotationMatrixX rotationMatrixX;//rotation matrix on the X axis
    RotationMatrixY rotationMatrixY;//rotation matrix on the Y axis
    RotationMatrixZ rotationMatrixZ;//rotation matrix on the Z axis
    Matrix44<float> rotationMatrix;//rotation matrix for all 3 axes
    ScaleMatrix scaleMatrix;

    Transform(Vec3f pos, Vec3f rot, Vec3f s)
        :position(pos),
        rotation(rot),
        scale(s),
        positionMatrix(pos),
        rotationMatrixX(rot.x), rotationMatrixY(rot.y), rotationMatrixZ(rot.z),
        scaleMatrix(s)
    {
        CalculateRotationMatrix();
    }

    Transform(Vec3f pos, Vec3f rot, float s)
        :position(pos),
        rotation(rot),
        scale(s, s, s),
        positionMatrix(pos),
        rotationMatrixX(rot.x), rotationMatrixY(rot.y), rotationMatrixZ(rot.z),
        scaleMatrix(s)
    
    {
        CalculateRotationMatrix();
    }

    



};

inline Vec3f ApplyTransform(Vec3f input, Transform t)
{
    const Vec3f scaled = t.scaleMatrix.multiplyVectorMatrix(input);
    const Vec3f rotated = t.rotationMatrix.multiplyVectorMatrix(scaled);
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