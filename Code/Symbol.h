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
    void displaySymbolText(int resolutionMutliplier, double drawThickness) const;
    Stroke* addStroke();
    std::vector<Stroke*> getStrokes() const;
    int getGridSize() const;
    ~Symbol();
};

#include "Symbol.cpp"

#endif