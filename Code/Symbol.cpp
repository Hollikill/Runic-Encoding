#include "Symbol.h"
#include <vector>
#include "Stroke.h"
#include <iostream>

Symbol::Symbol() {
    gridSizeX = 8;
    gridSizeY = 8;
}

Symbol::Symbol (int sizeX) {
    gridSizeX = sizeX;
    gridSizeY = sizeX;
}

/*
Symbol::Symbol (int sizeX, int sizeY) {
    gridSizeX = sizeX;
    gridSizeY = sizeY;
}
*/

void Symbol::addStroke (Stroke& stroke) {
    strokes.push_back(stroke);
}

int Symbol::getGridSize() const {
    return gridSizeX;
}

void Symbol::displaySymbolText (int mutliplier) const {
    std::cout << "temp" << std::endl;
}