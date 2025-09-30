#include "Symbol.h"
#include <vector>
#include "Stroke.h"
#include <iostream>
#include <cmath>

Symbol::Symbol() {
    gridSizeX = 8;
    gridSizeY = 8;
}

Symbol::Symbol (int sizeX) {
    gridSizeX = sizeX;
    gridSizeY = sizeX;
}

Stroke* Symbol::addStroke () {
    Stroke* newStroke = new Stroke();
    strokes.push_back(newStroke);
    return newStroke;
}

std::vector<Stroke*> Symbol::getStrokes () const {
    return strokes;
}

int Symbol::getGridSize() const {
    return gridSizeX;
}

Symbol::~Symbol () {
    while (strokes.size() > 0) {
        delete strokes.at(0);
        strokes.at(0) = nullptr;
        strokes.erase(strokes.begin());
    }
}