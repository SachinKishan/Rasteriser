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
#include "Matrix.h"

using std::vector;


int ch = 1000, cw = 1000;//raster space
int vh = 5, vw = 5;//aint //viewport space height, width, known as canvas in some places
float d = - 1;//distance between camera and canvas

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

void ColorIn(int x,int y,Image& image, Image::Rgb col)
{
    
    if (x < 0 || y <= 0 || x>=cw || y>ch)return;
    std::cout << "x: " << x << " y:" << y << std::endl;
    //col.out();
    image(x, ch - y) = col;
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

void DrawFilledTriangle(Point p0, Point p1, Point p2, Image& image, Image::Rgb fillcolor, Image::Rgb linecolor=Image::kWhite)
{
    
  
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
    
   

    float m = floor(x02.size()/2);
    
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
            
            Image::Rgb finalcol=fillcolor;
            finalcol*= h_segment[x - x_l];
            
           
            ColorIn(x, y,image, finalcol);
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


Vec2f ViewportToCanvas(Vec2f p)
{
    Vec2f coord(p.x*cw/vw,p.y*cw/vw);
    //vec coordpixel(coord.x, coord.y);
    return coord;
}

Point ProjectVertex(Vec3f vertex)
{
    //world to camera

    //camera to screen/image space

    Vec2f pScreen(vertex.x * d / -vertex.z, vertex.y * d / -vertex.z);
    
    //screen to NDC space
    
    Vec2f pNDC((pScreen.x + vw * 0.5) / vw, (pScreen.y + vh * 0.5) / vh);
    
    //NDC to Raster

    Point p(pNDC.x * cw, (1- pNDC.y) * ch);

    return (p);
}


void RenderObject(vector<Triangle> triangles, Image& image)
{

    Matrix44<float> translation(1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        -1.5, 0, 7);
    for (Triangle t : triangles)
    {

        Vec3f a=translation.multiplyVectorMatrix(t.a);
        Vec3f b=translation.multiplyVectorMatrix(t.b);
        Vec3f c=translation.multiplyVectorMatrix(t.c);
        
        
        
        Point q=ProjectVertex(a);
        Point r=ProjectVertex(b);
        Point s=ProjectVertex(c);
        
        
        DrawWireFrameTriangle(q, r, s, image,t.color);
    }
}

int main()
{
    
    Image K(cw, ch);
    std::cout << "We Work!\n";

    /*
    * Pixel by pixel rendering
    for (int j = h - 1; j > 0; j--)
    {
        for (int i = 0; i < w; i++)
        {
            color pixel_color(0, 0, 0);
           

            //write_color(std::cout, pixel_color, samples_per_pixel);
            auto r = pixel_color.x();
            auto g = pixel_color.y();
            auto b = pixel_color.z();
            K(i, h - j) = Image::Rgb(r, g, b);
            write_color(std::cout, pixel_color);
        }
    }*/
    
    
   
    
    Vec3f p1(1, 1, 1);
    Vec3f p2(-1, 1, 1);
    Vec3f p3(-1, -1, 1);
    Vec3f p4(1, -1, 1);
    Vec3f p5(1, 1, -1);
    Vec3f p6(-1, 1, -1);
    Vec3f p7(-1, -1, -1);
    Vec3f p8(1, -1, -1);

    



    std::vector<Vec3f> vertices;
    vertices.push_back(p1);
    vertices.push_back(p2);
    vertices.push_back(p3);
    vertices.push_back(p4);
    vertices.push_back(p5);
    vertices.push_back(p6);
    vertices.push_back(p7);
    vertices.push_back(p8);
    
    
    /*riangles
 0 = 0, 1, 2, red
 1 = 0, 2, 3, red
 2 = 4, 0, 3, green
 3 = 4, 3, 7, green
 4 = 5, 4, 7, blue
 5 = 5, 7, 6, blue

 6 = 1, 5, 6, yellow
 7 = 1, 6, 2, yellow
 8 = 4, 5, 1, purple
 9 = 4, 1, 0, purple
10 = 2, 6, 7, cyan
11 = 2, 7, 3, cyan*/
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

    vector<Triangle> tris;
    tris.push_back(t1);
    tris.push_back(t2);
    tris.push_back(t3);
    tris.push_back(t4);
    tris.push_back(t5);
    tris.push_back(t6);
    tris.push_back(t7);
    tris.push_back(t8);
    tris.push_back(t9);
    tris.push_back(t10);
    tris.push_back(t11);
    tris.push_back(t12);

    RenderObject(tris, K);
    savePPM(K, "./out.ppm");


    
}