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

// basically just a debug function until I get proper display working
void Symbol::displaySymbolText (int resolutionMutliplier, double drawThickness) const {
    //std::cout << "debug ran displaySymbolText()" << std::endl;

    // resize to new resolution
    int modifiedGridSizeX = gridSizeX * resolutionMutliplier;
    int modifiedGridSizeY = gridSizeY * resolutionMutliplier;

    double coordinateAdjustment = (resolutionMutliplier / 2) - 0.5;

    //std::cout << modifiedGridSizeX << "|" << modifiedGridSizeY << "-" << coordinateAdjustment << std::endl;
    std::cout << strokes.size() << std::endl;

    // loop through pixels
    for (int y = 0; y < modifiedGridSizeX; y++) {
        for (int x = 0; x < modifiedGridSizeY; x++) {
            bool isDrawn = false;

            // clear value
            std::string pixel = "..";

            // loop through strokes
            for (int i = 0; i < strokes.size(); i++) {
                if (!isDrawn) {
                    std::vector<Point*> points = strokes.at(i)->getPoints();
                    int pointsSize = points.size();

                    // loop through points
                    for (int p = 0; p < pointsSize; p++) {
                        if (!isDrawn) {
                            // get coordinates of the point
                            int px = points.at(p)->x;
                            int py = points.at(p)->y;

                            // calculate distance from point coordinate
                            double differenceX = (double)abs(((px * resolutionMutliplier)+coordinateAdjustment)-x);
                            double differenceY = (double)abs(((py * resolutionMutliplier)+coordinateAdjustment)-y);
                            double distance = (double)sqrt(pow(differenceX, 2) + pow(differenceY, 2));

                            // check if closer than thickness threshold and draw if so
                            if (distance <= drawThickness) { isDrawn = true; }

                            // check if there is a line segment, and evaluate if it is on the line
                            if ((p < (pointsSize-1)) && !isDrawn) {
                                // get coordinates of the next point in the line
                                int p2x = points.at(p+1)->x;
                                int p2y = points.at(p+1)->y;

                                // check if in bounding box of the two points
                                if (((px <= x <= p2x) || (px >= x >= p2x)) && ((py <= y <= p2y) || (py >= y >= p2y))) {
                                    // get the slope of the line
                                    double slope = ((py-p2y)/(px-p2x));

                                    // get distance to nearest point on the line
                                    double lineDifferenceY = abs(y - (py + (slope * (x - px))));
                                    double lineDifferenceX = abs(x - (px + ((1/slope) * (y - py))));

                                    double lineDistance = (double)sqrt(pow(lineDifferenceX, 2) + pow(lineDifferenceY, 2));

                                    // check for threshold
                                    if (lineDistance <= drawThickness) { isDrawn = true; }
                                }
                            }
                        }
                    }
                }
            }

            if (isDrawn) {
                // drawn value
                pixel = "##";
            }
            std::cout << pixel;
        }
        std::cout << std::endl;
    }
}

Symbol::~Symbol () {
    while (strokes.size() > 0) {
        delete strokes.at(0);
        strokes.at(0) == nullptr;
        strokes.erase(strokes.begin());
    }
}