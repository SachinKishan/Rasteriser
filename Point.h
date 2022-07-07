#pragma once


class Point
{
public:
	int x, y;

	Point() { x = 0; y = 0; }
	Point(int _x,int _y)
	{
		x = _x;
		y = _y;
	}
	~Point(){}

private:
	

};

void PointSwap(Point &p1, Point &p2)
{
	Point temp = p1;
	p1 = p2;
	p2 = temp;
}