#ifndef STROKE_
#define STROKE_

#include <vector>

class Point {
    public:
    int x;
    int y;
};

class Stroke
{
public:
    std::vector<Point*> points;

    void addPoint (Point* p);
    void addPoint (int x, int y);

    std::vector<Point*> getPoints() const; // get anchor points

    ~Stroke (); //destructor
};

//#include "Stroke.cpp"

#endif