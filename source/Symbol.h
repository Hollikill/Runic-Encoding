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
    Stroke* addStroke();
    std::vector<Stroke*> getStrokes() const;
    int getGridSize() const;
    ~Symbol();
};

//#include "Symbol.cpp"

#endif