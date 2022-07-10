#pragma once

#include "Geometry.h"

template<typename T>
class Matrix44 
{
public:

    T x[4][4] = { {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1} };

    Matrix44() {}

    Matrix44
       (T a, T b, T c, T d, 
        T e, T f, T g, T h,
        T i, T j, T k, T l, 
        T m, T n, T o, T p=1)
    {
        x[0][0] = a;
        x[0][1] = b;
        x[0][2] = c;
        x[0][3] = d;
        x[1][0] = e;
        x[1][1] = f;
        x[1][2] = g;
        x[1][3] = h;
        x[2][0] = i;
        x[2][1] = j;
        x[2][2] = k;
        x[2][3] = l;
        x[3][0] = m;
        x[3][1] = n;
        x[3][2] = o;
        x[3][3] = p;
    }

    const T* operator [] (uint8_t i) const { return x[i]; }
    T* operator [] (uint8_t i) { return x[i]; }

    static void multiply(const Matrix44<T>& a, const Matrix44& b, Matrix44& c)
    {
        for (uint8_t i = 0; i < 4; ++i) {
            for (uint8_t j = 0; j < 4; ++j) {
                c[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] +
                    a[i][2] * b[2][j] + a[i][3] * b[3][j];
            }
        }
    }

    Matrix44 transposed() const
    {
        return Matrix44(x[0][0],
            x[1][0],
            x[2][0],
            x[3][0],
            x[0][1],
            x[1][1],
            x[2][1],
            x[3][1],
            x[0][2],
            x[1][2],
            x[2][2],
            x[3][2],
            x[0][3],
            x[1][3],
            x[2][3],
            x[3][3]);
    }

    Matrix44& transpose()//operation
    {
        Matrix44 tmp(x[0][0],
            x[1][0],
            x[2][0],
            x[3][0],
            x[0][1],
            x[1][1],
            x[2][1],
            x[3][1],
            x[0][2],
            x[1][2],
            x[2][2],
            x[3][2],
            x[0][3],
            x[1][3],
            x[2][3],
            x[3][3]);
        *this = tmp;

        return *this;
    }
    template<typename S>
    void multDirMatrix(const Vec3<S>& src, Vec3<S>& dst) const
    {
        S a, b, c;

        a = src[0] * x[0][0] + src[1] * x[1][0] + src[2] * x[2][0];
        b = src[0] * x[0][1] + src[1] * x[1][1] + src[2] * x[2][1];
        c = src[0] * x[0][2] + src[1] * x[1][2] + src[2] * x[2][2];

        dst.x = a;
        dst.y = b;
        dst.z = c;
    }
    template<typename S>
    void multVecMatrix(const Vec3<S>& src, Vec3<S>& dst) const
    {
        S a, b, c, w;

        a = src[0] * x[0][0] + src[1] * x[1][0] + src[2] * x[2][0] + x[3][0];
        b = src[0] * x[0][1] + src[1] * x[1][1] + src[2] * x[2][1] + x[3][1];
        c = src[0] * x[0][2] + src[1] * x[1][2] + src[2] * x[2][2] + x[3][2];
        w = src[0] * x[0][3] + src[1] * x[1][3] + src[2] * x[2][3] + x[3][3];

        dst.x = a / w;
        dst.y = b / w;
        dst.z = c / w;
    }

    Vec3f multiplyVectorMatrix(Vec3f src)//src=source vector
    {
        Vec3f destination;
        float a, b, c, w;

        a = src.x * x[0][0] + src.y * x[1][0] + src.z * x[2][0] + x[3][0];
        b = src.x * x[0][1] + src.y * x[1][1] + src.z * x[2][1] + x[3][1];
        c = src.x * x[0][2] + src.y * x[1][2] + src.z * x[2][2] + x[3][2];
        w = src.x * x[0][3] + src.y * x[1][3] + src.z * x[2][3] + x[3][3];

        destination.x = a / w;
        destination.y = b / w;
        destination.z = c / w;
        
        
        return destination;

    }
};


class TranslationMatrix :public Matrix44<float>
{
public:

    
    TranslationMatrix(float _x, float _y,float _z)
		:Matrix44<float>(1, 0, 0, 0,
						 0, 1, 0, 0,
						 0, 0, 1, 0,
						 _x, _y, _z)
    {
        
    }


};
/*
class RotationMatrix :public Matrix44<float>
{
public:


    TranslationMatrix(float _x, float _y, float _z)
        :Matrix44<float>(1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            _x, _y, _z)
    {

    }


};
*/
