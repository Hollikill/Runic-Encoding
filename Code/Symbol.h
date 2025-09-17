#ifndef SYMBOL_
#define SYMBOL_

#include "Stroke.h"
#include <vector>

class Symbol
{
private:
    int gridSizeX;
    int gridSizeY;
    std::vector<Stroke> strokes;

public:
    Symbol();
    Symbol(int sizeX);
    //Symbol(int sizeX, int sizeY);
    void displaySymbolText(int resolutionMutliplier, double drawThickness) const;
    void addStroke(Stroke& stroke);
    int getGridSize() const;
};

#include "Symbol.cpp"

#endif