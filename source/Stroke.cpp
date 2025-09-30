#include <vector>

#include "Stroke.h"

void Stroke::addPoint (Point* p) {
    points.push_back(p);
}

void Stroke::addPoint (int x, int y) {
    Point* p = new Point;
    p->x = x;
    p->y = y;
    points.push_back(p);
}

std::vector<Point*> Stroke::getPoints () const {
    return points;
}

Stroke::~Stroke () {
    while (points.size() > 0) {
        delete points.at(0);
        points.at(0) = nullptr;
        points.erase(points.begin());
    }
}