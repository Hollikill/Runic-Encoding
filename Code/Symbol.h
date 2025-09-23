#ifndef SYMBOL_
#define SYMBOL_

#include "Stroke.h"
#include <vector>

class Symbol
{
private:
    int gridSizeX;
    int gridSizeY;
    std::vector<Stroke*> strokes;

public:
    Symbol();
    Symbol(int sizeX);
    //Symbol(int sizeX, int sizeY);
    void displaySymbolText(int resolutionMultiplier, double drawThickness) const;
    Stroke* addStroke();
    std::vector<Stroke*> getStrokes() const;
    int getGridSize() const;
    ~Symbol();
    double distanceToLineSegment(int x, int y, int x1, int y1, int x2, int y2) const;
    double distanceToLineSegment(Point* check, Point* start, Point* end) const;
};

#include "Symbol.cpp"

#endif