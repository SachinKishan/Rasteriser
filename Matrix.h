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

    friend Matrix44 operator *(Matrix44<float> a,Matrix44<float> b)
    {
        Matrix44 c;
        c.multiply(a, b, c);
        return c;
    }

    friend Matrix44 operator *(Matrix44<float> a,float f)
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)a[i][j]=a[i][j] * f;
        return a;
    }
    void out()
    {
	    for(int i=0;i<4;i++)
	    {
		    for(int j=0;j<4;j++)
		    {
                std::cout << x[i][j]<<" ";
		    }
            std::cout << std::endl;
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
        //vector is in row form. Vector * Matrix (V*M)
        a = src.x * x[0][0] + src.y * x[1][0] + src.z * x[2][0] + x[3][0];
        b = src.x * x[0][1] + src.y * x[1][1] + src.z * x[2][1] + x[3][1];
        c = src.x * x[0][2] + src.y * x[1][2] + src.z * x[2][2] + x[3][2];
        w = src.x * x[0][3] + src.y * x[1][3] + src.z * x[2][3] + x[3][3];

        destination.x = a / w;
        destination.y = b / w;
        destination.z = c / w;
        
        
        return destination;

    }
    /*
    Matrix44<T>& invert()
    {
        Matrix44<T> mat;
        for (unsigned column = 0; column < 4; ++column) {
            // Swap row in case our pivot point is not working
            if (x[column][column] == 0) {
                unsigned big = column;
                for (unsigned row = 0; row < 4; ++row)
                    if (fabs(x[row][column]) > fabs(x[big][column])) big = row;
                // Print this is a singular matrix, return identity ?
                if (big == column) fprintf(stderr, "Singular matrix\n");
                // Swap rows                               
                else for (unsigned j = 0; j < 4; ++j) {
                    std::swap(x[column][j], x[big][j]);
                    std::swap(mat.x[column][j], mat.x[big][j]);
                }
            }
            // Set each row in the column to 0  
            for (unsigned row = 0; row < 4; ++row) {
                if (row != column) {
                    T coeff = x[row][column] / x[column][column];
                    if (coeff != 0) {
                        for (unsigned j = 0; j < 4; ++j) {
                            x[row][j] -= coeff * x[column][j];
                            mat.x[row][j] -= coeff * mat.x[column][j];
                        }
                        // Set the element to 0 for safety
                        x[row][column] = 0;
                    }
                }
            }
        }
        // Set each element of the diagonal to 1
        for (unsigned row = 0; row < 4; ++row) {
            for (unsigned column = 0; column < 4; ++column) {
                mat.x[row][column] /= x[row][row];
            }
        }
        mat.out();
        *this = mat;
        return *this;
    }*/


    Matrix44<float> invert()
    {
        Matrix44<float> inverse;
        float determinant =
            x[0][0]
				*( AddAndMultiply(1,1,2,2,3,3,1,2,2,3,3,1,1,3,2,1,3,2,1,3,2,2,3,1,1,2,2,1,3,3,1,1,2,3,3,2));

        determinant -= x[1][0] *
				( AddAndMultiply(0,1,2,2,3,3,0,2,2,3,3,1,0,3,2,1,3,2,0,3,2,2,3,1,0,2,2,1,3,3,0,1,2,3,3,2));

        determinant+=x[2][0]*
				( AddAndMultiply(0,1,1,2,3,3,0,2,1,3,3,1,0,3,1,1,3,2,0,3,1,2,3,1,0,2,1,1,3,3,0,1,1,3,3,2));

        determinant-=x[3][0]*
				( AddAndMultiply(0,1,1,2,2,3,0,2,1,3,2,1,0,3,1,1,2,2,0,3,1,2,2,1,0,2,1,1,2,3,0,1,1,3,2,2));

    	inverse[0][0]=
				( x[1][1] * x[2][2] * x[3][3]
                + x[1][2] * x[2][3] * x[3][1]
                + x[1][3] * x[2][1] * x[3][2]
                - x[1][3] * x[2][2] * x[3][1]
                - x[1][2] * x[2][1] * x[3][3]
                - x[1][1] * x[2][3] * x[3][2]);
#pragma region
        inverse[0][0] = AddAndMultiply(1,1,2,2,3,3,1,2,2,3,3,1,1,3,2,1,3,2,1,3,2,2,3,1,1,2,2,1,3,3,1,1,2,3,3,2);
        inverse[0][1] -= AddAndMultiply(0, 1, 2, 2, 3, 3, 0, 2, 2, 3, 3, 1, 0, 3, 2, 1, 3, 2, 0, 3, 2, 2, 3, 1, 0, 2, 2, 1, 3, 3, 0, 1, 2, 3, 3, 2);
        inverse[0][2] = AddAndMultiply(0,1,1,2,3,3,0,2,1,3,3,1,0,3,1,1,3,2,0,3,1,2,3,1,0,2,1,1,3,3,0,1,1,3,3,2);
        inverse[0][3] -= AddAndMultiply(0,1,1,2,2,3,0,2,1,3,2,1,0,3,1,1,2,2,0,3,1,2,2,1,0,2,1,1,2,3,0,1,1,3,2,2);
        inverse[1][0] -= AddAndMultiply(0,1,2,2,3,3,1,2,2,3,3,0,1,3,2,0,3,2,1,3,2,2,3,0,1,2,2,0,3,3,1,0,2,3,3,2);
    	inverse[1][1] = AddAndMultiply(0, 0, 2, 2, 3, 3, 0, 2, 2, 3, 3, 0, 0, 3, 2, 0, 3, 2, 0, 3, 2, 2, 3, 0, 0, 2, 2, 0, 3, 3, 0, 0, 2, 3, 3, 2);
        inverse[1][2] -= AddAndMultiply(0,0,1,2,3,3,0,2,1,3,3,0,0,3,1,0,3,2,0,3,1,2,3,0,0,2,1,0,3,3,0,0,1,3,3,2);
        inverse[1][3] = AddAndMultiply(0,0,1,2,2,3,0,2,1,3,2,0,0,3,1,0,2,2,0,3,1,2,2,0,0,2,1,0,2,3,0,0,1,3,2,2);
        inverse[2][0] = AddAndMultiply(1,0,2,1,3,3,1,1,2,3,3,0,1,3,2,0,3,1,1,3,2,1,3,0,1,1,2,0,3,3,1,0,2,3,3,1);
        inverse[2][1] -= AddAndMultiply(0,0,2,1,3,3,0,1,2,3,3,0,0,3,2,0,3,1,0,3,2,1,3,0,0,1,2,0,3,3,0,0,2,3,3,1);
        inverse[2][2] = AddAndMultiply(0, 0, 1, 1, 3, 3, 0, 1, 1, 3, 3, 0, 0, 3, 1, 0, 3, 1, 0, 3, 1, 1, 3, 0, 0, 1, 1, 0, 3, 3, 0, 0, 1, 3, 3, 1);
    	inverse[2][3] -= AddAndMultiply(0, 0, 1, 1, 2, 3, 0, 1, 1, 3, 2, 0, 0, 3, 1, 0, 2, 1, 0, 3, 1, 1, 2, 0, 0, 1, 1, 0, 2, 3, 0, 0, 1, 3, 2, 1);
        inverse[3][0] -= AddAndMultiply(1,0,2,1,3,2,1,1,2,2,3,0,1,2,2,0,3,1,1,2,2,1,3,0,1,1,2,0,3,2,1,0,2,2,3,1);
        inverse[3][1] = AddAndMultiply(0, 0, 2, 1, 3, 2, 0, 1, 2, 2, 3, 0, 0, 2, 2, 0, 3, 1, 0, 2, 2, 1, 3, 0, 0, 1, 2, 0, 3, 2, 0, 0, 2, 2, 3, 1);
        inverse[3][2] -= AddAndMultiply(0, 0, 1, 1, 3, 2, 0, 1, 1, 2, 3, 0, 0, 2, 1, 0, 3, 1, 0, 2, 1, 1, 3, 0, 0, 1, 1, 0, 3, 2, 0, 0, 1, 2, 3, 1);
        inverse[3][3] = AddAndMultiply(0, 0, 1, 1, 2, 2, 0, 1, 1, 2, 2, 0, 0, 2, 1, 0, 2, 1, 0, 2, 1, 1, 2, 0, 0, 1, 1, 0, 2, 2, 0, 0, 1, 2, 2, 1);
        
#pragma endregion

        //std::cout << std::endl << determinant;
        inverse = inverse * determinant;
        return inverse;
    }
	float AddAndMultiply(
        int a1, int a2, int a3, int a4, int a5, int a6,
        int a7, int a8, int a9, int a10, int a11, int a12,
        int a13, int a14, int a15, int a16, int a17, int a18,
        int a19, int a20, int a21, int a22, int a23, int a24,
        int a25, int a26, int a27, int a28, int a29, int a30,
        int a31, int a32, int a33, int a34, int a35, int a36)
    {
        return x[a1][a2] * x[a3][a4] * x[a5][a6] +
            x[a7][a8] * x[a9][a10] * x[a11][a12] +
            x[a13][a14] * x[a15][a16] * x[a17][a18] -
            x[a19][a20] * x[a21][a22] * x[a23][a24] -
            x[a25][a26] * x[a27][a28] * x[a29][a30] -
            x[a31][a32] * x[a33][a34] * x[a35][a36];
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

    TranslationMatrix(Vec3f translate)
        :Matrix44<float>(1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            translate.x, translate.y, translate.z)
    {

    }

};


class RotationMatrixX :public Matrix44<float>
{
public:
    RotationMatrixX() = default;
    RotationMatrixX(float degrees)
        :Matrix44<float>
			(1, 0, 0, 0,
            0, cos(degrees), -sin(degrees), 0,
            0, sin(degrees), cos(degrees), 0,
            0, 0, 0){}
};

class RotationMatrixY :public Matrix44<float>
{
public:

    RotationMatrixY() = default;
    RotationMatrixY(float degrees)
        :Matrix44<float>
        (cos(degrees), 0, sin(degrees), 0,
            0, 1,0, 0,
            -sin(degrees), 0, cos(degrees), 0,
            0, 0, 0) {}
};

class RotationMatrixZ :public Matrix44<float>
{
public:

    RotationMatrixZ() = default;
    RotationMatrixZ(float degrees)
        :Matrix44<float>
        (cos(degrees), -sin(degrees), 0, 0,
            sin(degrees), cos(degrees), 0, 0,
            0, 0, 1, 0,
            0, 0, 0) {}
};

class ScaleMatrix:public Matrix44<float>
{
public:
    ScaleMatrix() = default;
    ScaleMatrix(Vec3f scale):
		Matrix44<float>(
        scale.x,0,0,0,
        0,scale.y,0,0,
        0,0,scale.z,0,
        0,0,0)
    {
	    
    }
    ScaleMatrix(float scale) :
        Matrix44<float>(
            scale, 0, 0, 0,
            0, scale, 0, 0,
            0, 0, scale, 0,
            0, 0, 0)
    {

    }
};

