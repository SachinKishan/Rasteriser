// Rasteriser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <cstdlib> 
#include <cstdio> 
#include <cstring> 
#include <algorithm> 
#include <fstream> 
#include <cassert> 
#include <exception> 
#include <vector> 
#include<conio.h>
#include "ImageHeader.h"
#include "Geometry.h"
#include "Model.h"
#include "Triangle.h"
#include "Camera.h"
#include "Plane.h"
#include "lodepng.h"

#include "ModelInstance.h"
using std::vector;


int ch = 500, cw = 500;//raster space
float depth_buffer[500][500];


Vec3f cameraRotation(0,0,0);
Vec3f cameraPosition(0,0,2);
Transform cameraTransform(cameraPosition, cameraRotation, 1);
Camera camera(cameraTransform,1,1,1);

vector<float> Interpolate(int i0,float d0,int i1,float d1)
{
    vector<float> values;
    if (i0 == i1) {
        values.push_back(d0);//or put d1, it is supposed to be the same
        return values;
    }
    float a = (d1 - d0) / (i1 - i0);
    float d = d0;
    for (int i = i0; i <= i1; i++)
    {
        values.push_back(d);
        d = d + a;
    }
    return values;

}
int colorins=0;

std::vector<unsigned char> image;

void encodeOneStep(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height) {
    //Encode the image
    unsigned error = lodepng::encode(filename, image, width, height);

    //if there's an error, display it
    if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}
void ColorIn(int x,int y,Image& img, Image::Rgb col)
{
    
    if (x < 0 || y <= 0 || x>=cw || y>ch)return;
    ///std::cout << "x: " << x << " y:" << y << std::endl;
    //col.out();
    colorins++;


    img(x, ch - y) = col;
   /*
    unsigned char red = static_cast<unsigned char>(std::min(1.f, col.r) * 255);
    unsigned char g = static_cast<unsigned char>(std::min(1.f, col.g) * 255);
    unsigned char b = static_cast<unsigned char>(std::min(1.f, col.b) * 255);
    //allot color
    int val = 4 * cw * y + 4 * x;
    image[val + 0] = red;
    image[val + 1] = g;
    image[val + 2] = b;
    image[val + 3] = 255;//alpha
    */

}
void swap(int &x0,int &y0,int &x1,int &y1)
{
    int temp = x1;
    x1 = x0;
    x0 = temp;

    temp = y1;
    y1 = y0;
    y0 = temp;
}

void PointSwap(Point &a,Point &b)
{
    Point c = a;
    a = b;
    b = c;
}




void line(int x0, int y0, int x1, int y1, Image& image, Image::Rgb color=Image::kWhite) 
{
    //Line drawing ver 1
    /*
    
    float dx = x1 - x0;
    float dy = y1 - y0;
    
    if (dx == 0 && dy == 0)
    {
        ColorIn(x1, y1, image, color);
        return;
    }
    
    if (abs(dx) > abs(dy))
    {
        
        float xmin, xmax;
        if (x0 < x1)
        {
            xmin = x0;
            xmax = x1;
        }
        else
        {
            xmin = x1;
            xmax = x0;
        }

        float a = (float)dy / (float)dx;
        float y;
        std::cout << "loop through x" << std::endl;
        for (float x = xmin; x <= xmax; x++)
        {   
            y = y0+((x-x0)*a);
           ColorIn(x, y, image, color);
        }
    }
    else
    {
        float ymin, ymax;
        if (y0 > y1)
        {
            ymin = y1;
            ymax = y0;
        }
        else
        {
            ymin = y0;
            ymax = y1;
        }
        
        float a = dx / dy;
        float x =0;
        std::cout << "loop through y" << std::endl;

        for (float y = ymin; y <= ymax; y++)
        {
            
            x = (float)x0 + ((y-y0)*a);
            ColorIn(floor(x), y, image, color);
        }
    }*/


    //Line Drawing but using interpolation

    if (abs(x1 - x0) > abs(y1 - y0))
    {
        if (x0 > x1)
        {
            swap(x0, y0, x1, y1);
        }
        vector<float> ys = Interpolate(x0, y0, x1, y1);
            for (int x = x0; x <= x1; x++)
            {
                ColorIn(x, ys[x-x0], image, color);
            }
    }
    else
    {
        if (y0 > y1)
        {
            swap(x0,y0,x1,y1);
        }
        vector<float> xs=Interpolate(y0,x0,y1,x1);
        for (int y = y0; y <= y1; y++)
        {
            ColorIn(xs[y - y0], y, image, color);
        }
    }

    
}

void line(Point p0, Point p1, Image& image, Image::Rgb color = Image::kWhite)
{
    if (abs(p0.x - p1.x) > abs(p0.y - p1.y))
    {
        if (p0.x > p1.x)PointSwap(p0, p1);
        vector<float> ys = Interpolate(p0.x, p0.y, p1.x, p1.y);
        for (int x = p0.x; x <= p1.x; x++)
        {
            ColorIn(x, ys[x - p0.x], image, color);
        }
    }
    else
    {
        if (p0.y > p1.y)
        {
            PointSwap(p0, p1);
        }
        vector<float> xs = Interpolate(p0.y, p0.x, p1.y, p1.x);
        for (int y = p0.y; y <= p1.y; y++)
        {
            ColorIn(xs[y - p0.y], y, image, color);
        }
    }
}

void DrawWireFrameTriangle(Point p0, Point p1, Point p2, Image& image, Image::Rgb color = Image::kWhite)
{
    line(p0, p1, image, color);
    line(p1, p2, image, color);
    line(p2, p0, image, color);
}

void DrawFilledTriangle(Point p0, Point p1, Point p2,Vec3f a,Vec3f b,Vec3f c, Image& image, Image::Rgb fillcolor, Image::Rgb linecolor=Image::kWhite)
{
    
	//for shading purposes
    float h0 = 1;
    float h1 = 1;
    float h2 = 1;

	//for z buffer
    float z0 = a.z;
    float z1 = b.z;
    float z2 = c.z;

    
    if (p1.y < p0.y)PointSwap(p1, p0);
    if (p2.y < p0.y)PointSwap(p2, p0);
    if (p2.y < p1.y)PointSwap(p2, p1);
   
    vector<float> x01 = Interpolate(p0.y, p0.x, p1.y, p1.x);
    vector<float> h01 = Interpolate(p0.y, h0, p1.y, h1);
    vector<float> z01 = Interpolate(p0.y, z0, p1.y, z1);

    vector<float> x12 = Interpolate(p1.y, p1.x, p2.y, p2.x);
    vector<float> h12 = Interpolate(p1.y, h1, p2.y, h2);
    vector<float> z12 = Interpolate(p1.y, z1, p2.y, z2);
    
    vector<float> x02 = Interpolate(p0.y, p0.x, p2.y, p2.x);
    vector<float> h02 = Interpolate(p0.y, h0, p2.y, h2);
    vector<float> z02 = Interpolate(p0.y, z0, p2.y, z2);
    

    
    vector<float> x012;
    vector<float> h012;
    vector<float> z012;
    
    vector<float> x_left;
    vector<float> h_left;
    vector<float> z_left;

    vector<float> x_right;
    vector<float> h_right;
    vector<float> z_right;
    
    x01.pop_back();
    h01.pop_back();
    z01.pop_back();

    x012 = x01;
    h012 = h01;
    z012 = z01;


    x012.insert(x012.end(), x12.begin(), x12.end());
    h012.insert(h012.end(), h12.begin(), h12.end());
    z012.insert(z012.end(), z12.begin(), z12.end());

    auto m = static_cast<float>(floor(x02.size()/2));
    
    if (x02[m] < x012[m])
    {
        x_left = x02;
        h_left = h02;
        z_left = z02;

        x_right = x012;
        h_right = h012;
        z_right = z012;
    }
    else
    {
        x_left = x012;
        h_left = h012;
        z_left = z012;

        x_right = x02;
        h_right = h02;
        z_right = z02;
    }

    
    
    
   
    for (int y = p0.y; y <= p2.y; y++)
    {

        float x_l = x_left[y - p0.y];
        float x_r = x_right[y - p0.y];
        float h_l = h_left[y - p0.y];
        float h_r = h_right[y - p0.y];
        float z_l = z_left[y - p0.y];
        float z_r = z_right[y - p0.y];


        vector<float> h_segment = Interpolate(x_l, h_l, x_r, h_r);
        vector<float> z_segment = Interpolate(x_l, z_l, x_r, z_r);
        
        for (float x = x_l; x <= x_r; x++)
        {
            float z = z_segment[(x - (int)x_l)];
            if(z < depth_buffer[(int)x][y])
            {
                Image::Rgb finalcol = fillcolor;
                finalcol *= h_segment[(x - (int)x_l)];
                depth_buffer[(int)x][y] = z;
                ColorIn(x, y, image, finalcol);

            }
            
        }
    }
   // DrawWireFrameTriangle(p0, p1, p2, image, Image::kWhite);
    
}
void DrawFilledTriangle(Point p0, Point p1, Point p2, Image& image, Image::Rgb fillcolor, Image::Rgb linecolor = Image::kWhite)
{

    //for shading purposes
    float h0 = 1;
    float h1 = 1;
    float h2 = 1;

    if (p1.y < p0.y)PointSwap(p1, p0);
    if (p2.y < p0.y)PointSwap(p2, p0);
    if (p2.y < p1.y)PointSwap(p2, p1);

    vector<float> x01 = Interpolate(p0.y, p0.x, p1.y, p1.x);
    vector<float> h01 = Interpolate(p0.y, h0, p1.y, h1);

    vector<float> x12 = Interpolate(p1.y, p1.x, p2.y, p2.x);
    vector<float> h12 = Interpolate(p1.y, h1, p2.y, h2);


    vector<float> x02 = Interpolate(p0.y, p0.x, p2.y, p2.x);
    vector<float> h02 = Interpolate(p0.y, h0, p2.y, h2);



    vector<float> x012;
    vector<float> h012;

    vector<float> x_left;
    vector<float> h_left;

    vector<float> x_right;
    vector<float> h_right;

    x01.pop_back();
    h01.pop_back();

    x012 = x01;
    h012 = h01;

    x012.insert(x012.end(), x12.begin(), x12.end());
    h012.insert(h012.end(), h12.begin(), h12.end());



    auto m = static_cast<float>(floor(x02.size() / 2));

    if (x02[m] < x012[m])
    {
        x_left = x02;
        h_left = h02;


        x_right = x012;
        h_right = h012;
    }
    else
    {
        x_left = x012;
        h_left = h012;

        x_right = x02;
        h_right = h02;
    }


    for (int y = p0.y; y <= p2.y; y++)
    {

        float x_l = x_left[y - p0.y];
        float x_r = x_right[y - p0.y];
        float h_l = h_left[y - p0.y];
        float h_r = h_right[y - p0.y];



        vector<float> h_segment = Interpolate(x_l, h_l, x_r, h_r);

        for (float x = x_l; x <= x_r; x++)
        {

            Image::Rgb finalcol = fillcolor;
            finalcol *= h_segment[x - x_l];



            ColorIn(x, y, image, finalcol);
        }
    }
    DrawWireFrameTriangle(p0, p1, p2, image, linecolor);

}



void DrawFilledModel(Image& image)//this is constant for now
{
    Model* model = new Model("african_head.obj");
    Vec3f light_dir(0, 0, -1); // define light_dir

    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        Vec2i screen_coords[3];
        Vec3f world_coords[3];
        for (int j = 0; j < 3; j++) {
            Vec3f v = model->vert(face[j]);
            screen_coords[j] = Vec2i((v.x + 1.) * cw / 2., (v.y + 1.) * ch / 2.);
            world_coords[j] = v;
        }
        Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
        n.normalize();
        float intensity = n * light_dir;
        if (intensity > 0) {
            Image::Rgb color(intensity, intensity, intensity);
            DrawFilledTriangle(screen_coords[0], screen_coords[1], screen_coords[2], image, Image::kWhite, color);
        }
    }


    /*
    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        for (int j = 0; j < 3; j++) {
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j + 1) % 3]);
            int x0 = (v0.x + 1.) * w / 2.;
            int y0 = (v0.y + 1.) * h / 2.;
            int x1 = (v1.x + 1.) * w / 2.;
            int y1 = (v1.y + 1.) * h / 2.;
            line(x0, y0, x1, y1, K, Image::kWhite);
        }
    }*/
}

Point ProjectVertex(Vec3f vertex)
{
    //world to camera
    vertex=camera.InvertPointToCamera(vertex);

	//camera to screen/image space

    const Vec2f pScreen(vertex.x * camera.d / -vertex.z, vertex.y * camera.d / -vertex.z);
    
    //screen to NDC space
    
    const Vec2f pNDC((pScreen.x + camera.cameraViewWidth * 0.5) / camera.cameraViewWidth, 
        (pScreen.y + camera.cameraViewHeight * 0.5) / camera.cameraViewHeight);
    
    //NDC to Raster

    const Point p(pNDC.x * cw, (pNDC.y) * ch);

    //To do: fit this entire process into one constant matrix
    /*
    //3D to canvas matrix
    Matrix44<float> mat(d * cw / vw, 0, 0, 0,
        0, d * ch / vh, 0, 0,
        0, 0, 1, 0,
        0,0,0,1);

    mat.multiplyVectorMatrix(vertex);
    */

    return (p);
}


int isBackFacing(Triangle t)
{
    Vec3f a = t.b - t.a;
    const Vec3f b = t.c - t.a;
    const Vec3f normal = a.CrossProduct(b);
    const Vec3f v = cameraPosition - t.a;
	return  (normal * v) <= 0;
}

void BackFaceCulling(ShapeModel object, Camera cam = camera)
{
    vector<Triangle> culledTriangles;
    for (Triangle t : object.triangles)
    {
        if (!isBackFacing(t))
        {
            culledTriangles.push_back(t);
        }
        //else std::cout << "back facing triangle found";
    }
}

void RenderObject(const Instance& instance, Image& image)
{
    
    BackFaceCulling(instance.model, camera);
   
    for (Triangle t : instance.model.triangles)
    {
        //this can optimised
        Vec3f a = ApplyTransform(t.a, instance.transform);
        Vec3f b = ApplyTransform(t.b, instance.transform);
        Vec3f c = ApplyTransform(t.c, instance.transform);
        Point q=ProjectVertex(a);
        Point r=ProjectVertex(b);
        Point s=ProjectVertex(c);
        DrawWireFrameTriangle(q, r, s, image,t.color);
        //DrawFilledTriangle(q, r, s, a,b,c ,image, t.color, t.color);
    }
}

void RenderScene(const std::vector<Instance>& instances,Image& image)
{
	for(auto& i:instances)
	{
        
        RenderObject(i, image);
	}
}

template<class T>
bool OnlyOneIsPositive(T a,T b,T c, int& whichOne)
{

    if (a > 0)whichOne = 0;
    else if (b > 0)whichOne = 1;
    else whichOne = 2;
    return a>0 && b < 0 && c < 0
        || a < 0 && b>0 && c < 0
        || a < 0 && b < 0 && c>0;
}
template<class T>
bool OnlyOneIsNegative(T a, T b, T c, int& whichOne)
{
    if (a < 0)whichOne = 0;
    else if (b < 0)whichOne = 1;
    else whichOne = 2;
	return a > 0 && b > 0 && c < 0
        || a > 0 && b<0 && c > 0
        || a < 0 && b > 0 && c>0;
}



std::vector<Triangle> ClipTriangle(Triangle t,Plane p)
{
    std::vector<Triangle> finalTriangles;
    float d0 = SignedDistance(p, t.a);
    float d1 = SignedDistance(p, t.b);
    float d2 = SignedDistance(p, t.c);
    int vnumber;

    if (d0 > 0 && d1 > 0 && d2 > 0)finalTriangles.push_back(t);
    else if (d0 < 0 && d1 < 0 && d2 < 0)return finalTriangles;
    else if (OnlyOneIsPositive(d0, d1, d2, vnumber))
    {
        Vec3f a, b, c;
        if (vnumber == 0)
        {
            a = t.a;
            b = LinePlaneIntersection(p, Line(a, t.b));
            c = LinePlaneIntersection(p, Line(a, t.c));
        }//keep a
        else if (vnumber == 1)
        {
            b = t.b;
            a = LinePlaneIntersection(p, Line(b, t.a));
            c = LinePlaneIntersection(p, Line(b, t.c));

        }//keep b
        else if (vnumber == 2)
        {
            c = t.c;
            a = LinePlaneIntersection(p, Line(c, t.a));
            b = LinePlaneIntersection(p, Line(c, t.b));

        }//keep c
        finalTriangles.emplace_back(Triangle(a, b, c));
    }
    else if (OnlyOneIsNegative(d0, d1, d2, vnumber))
    {
        Vec3f a, b, c;
        if (vnumber == 0)
        {
            a = t.a;
            b = LinePlaneIntersection(p, Line(a, t.b));
            c = LinePlaneIntersection(p, Line(a, t.c));
        }//keep a
        else if (vnumber == 1)
        {
            b = t.b;
            a = LinePlaneIntersection(p, Line(b, t.a));
            c = LinePlaneIntersection(p, Line(b, t.c));

        }//keep b
        else if (vnumber == 2)
        {
            c = t.c;
            a = LinePlaneIntersection(p, Line(c, t.a));
            b = LinePlaneIntersection(p, Line(c, t.b));
        }//keep c
        finalTriangles.emplace_back(Triangle(t.a, b, a,t.color));
        finalTriangles.emplace_back(Triangle(a, t.b, b,t.color));
    }return finalTriangles;
}

std::vector<Triangle> ClipTrianglesAgainstPlane(std::vector<Triangle> tris, Plane p)
{
    std::vector<Triangle> clipped_tris;
    for(auto& t:tris)
    {
        std::vector<Triangle> newTriangles = ClipTriangle(t, p);
        clipped_tris.insert(clipped_tris.end(),newTriangles.begin(),newTriangles.end());
    }
    return tris;
}

bool ClipInstanceAgainstPlane(Instance &i, Plane p)
{
    const float d = SignedDistance(p, i.model.center);
    if (d > i.model.r)return true;//completely in bounds
    else if (d < -i.model.r)return false;//completely not in bounds
    else//partially in bounds- make new triangles
    {
        i.model.triangles = ClipTrianglesAgainstPlane(i.model.triangles, p);
    	return true;
    }
}

bool ClipInstance(Instance& i, const std::vector<Plane>& planes)
{
    for(auto p:planes)
    {
        const bool clipped = ClipInstanceAgainstPlane(i, p);
        if (!clipped)return false;
    }
    return true;
}
//clipping
void ClipScene(std::vector<Instance>& instances, const std::vector<Plane>& planes)
{
    std::vector<Instance> clippedInstances;
    for(auto& i:instances)
    {
        const bool clipped = ClipInstance(i, planes);
        if (clipped)clippedInstances.push_back(i);
    }
    instances=clippedInstances;

}


int main()
{
   // const char* filename = "out2.png";
    image.resize(cw * ch * 4);
    Image K = Image(cw, ch);


	std::cout << "We Work!\n";
    for (int i = 0; i < ch; i++)
        for (int j = 0; j < cw; j++)depth_buffer[i][j] = 10000;//depth buffer initialized to some big value

    Plane top(0, -1, 1, 0);
    Plane bottom(0, 1, 1, 0);
    Plane left(1, 0, 1, 0);
    Plane right(-1, 0, 1, 0);
    Plane near(0, 0, 1, camera.d);
    std::vector<Plane> clippingPlanes;
    std::cout << "We Work!\n";

    float initialRot = 0;
    float finalRot = 90;
    float step_size = 1;
    /*
    Vec3f positiona(2, 0, 7);
    Vec3f rotationa(0, 90, 45);
    Vec3f scalea(2,1,1);
    Transform ta(positiona,rotationa,scalea);
    Cube cubea("A");
    Instance cubeInstancea(cubea, ta);
    std::vector<Instance> instances;
	instances.push_back(cubeInstancea);
    ClipScene(instances, clippingPlanes);
    RenderScene(instances, K);
    */
    /*
    for (int i = 0; i <= 5; i++)
    {
        const char* filename = "out" +char(i)+ "png";
        Vec3f positiona(2, 0, 7);
        Vec3f rotationa(0, 90, 45);
        Vec3f scalea(2, 1, 1);
        Transform ta(positiona, rotationa, scalea);
        Cube cubea("A");
        Instance cubeInstancea(cubea, ta);
        std::vector<Instance> instances;
        instances.push_back(cubeInstancea);
        ClipScene(instances, clippingPlanes);
        RenderScene(instances, K);


        encodeOneStep(filename, image, cw, ch);
    }*/
    DrawFilledModel(K);
	savePPM(K, "out.ppm");

	encodeOneStep("out2.png", image, cw, ch);
}