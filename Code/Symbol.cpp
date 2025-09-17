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

void Symbol::displaySymbolText (int resolutionMutliplier, double drawThickness) const {
    //std::cout << "debug ran displaySymbolText()" << std::endl;

    // resize to new resolution
    int modifiedGridSizeX = gridSizeX * resolutionMutliplier;
    int modifiedGridSizeY = gridSizeY * resolutionMutliplier;

    double coordinateAdjustment = (resolutionMutliplier / 2) - 0.5;

    //std::cout << modifiedGridSizeX << "|" << modifiedGridSizeY << "-" << coordinateAdjustment << std::endl;

    // loop through pixels
    for (int y = 0; y < modifiedGridSizeX; y++) {
        for (int x = 0; x < modifiedGridSizeY; x++) {
            bool isDrawn = false;

            // clear value
            std::string pixel = " ";

            // loop through strokes
            for (int i = 0; i < strokes.size(); i++) {
                if (!isDrawn) {
                    std::vector<Point*> points = strokes.at(i).getPoints();

                    // loop through points
                    for (int p = 0; p < points.size(); p++) {
                        if (!isDrawn) {
                            // calculate distance from point coordinate
                            int differenceX = abs(((points.at(p)->x * resolutionMutliplier)+coordinateAdjustment)-x);
                            int differenceY = abs(((points.at(p)->y * resolutionMutliplier)+coordinateAdjustment)-y);
                            double distance = (double)sqrt(pow(differenceX, 2) + pow(differenceY, 2));

                            // check if closer than thickness threshold and draw if so
                            if (distance <= drawThickness) { isDrawn = true; }
                        }
                    }
                }
            }

            if (isDrawn) {
                // drawn value
                pixel = "@";
            }
            std::cout << pixel;
        }
        std::cout << std::endl;
    }
}