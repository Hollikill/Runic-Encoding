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
void Symbol::displaySymbolText (int resolutionMultiplier, double drawThickness) const {
    //std::cout << "debug ran displaySymbolText()" << std::endl;

    // resize to new resolution
    int modifiedGridSizeX = gridSizeX * resolutionMultiplier;
    int modifiedGridSizeY = gridSizeY * resolutionMultiplier;

    double coordinateAdjustment = (resolutionMultiplier / 2) - 0.5;

    //std::cout << modifiedGridSizeX << "|" << modifiedGridSizeY << "-" << coordinateAdjustment << std::endl;
    //std::cout << strokes.size() << std::endl;

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
                            double differenceX = (double)abs(((px * resolutionMultiplier)+coordinateAdjustment)-x);
                            double differenceY = (double)abs(((py * resolutionMultiplier)+coordinateAdjustment)-y);
                            double distance = (double)sqrt(pow(differenceX, 2) + pow(differenceY, 2));

                            // check if closer than thickness threshold and draw if so
                            if (distance <= drawThickness) { isDrawn = true; pixel = "##"; }

                            // check if there is a line segment, and evaluate if it is on the line
                            if ((p < (pointsSize-1)) && !isDrawn) {
                                distance = distanceToLineSegment(x, y, ((points.at(p)->x * resolutionMultiplier)+coordinateAdjustment), ((points.at(p)->y * resolutionMultiplier)+coordinateAdjustment), ((points.at(p+1)->x * resolutionMultiplier)+coordinateAdjustment), ((points.at(p+1)->y * resolutionMultiplier)+coordinateAdjustment));
                                if (distance <= drawThickness) {
                                    isDrawn = true;
                                    pixel = "##";
                                    //pixel = "[" + std::to_string((int)distance) + "]";
                                }
                                //pixel = "[" + std::to_string((int)distance) + "]";

                                /*// get coordinates of the next point in the line
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
                                }*/
                            }
                        }
                    }
                }
            }

            if (isDrawn) {
                // drawn value
                //pixel = "##";
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

/*
Couldn't figure out this one so I got some code from the internet.
This is a c++ version of the javascript code at the link below.
https://stackoverflow.com/a/6853926

credit to Joshua Perina
https://stackoverflow.com/users/368954/joshua
*/

double Symbol::distanceToLineSegment(int x, int y, int x1, int y1, int x2, int y2) const {
	Point* check = new Point();
	check->x = x;
	check->y = y;
    Point* start = new Point();
    start->x = x1;
    start->y = y1;
    Point* end = new Point();
    end->x = x2;
    end->y = y2;
	double returnValue = distanceToLineSegment(check, start, end);
    delete check;
    delete start;
    delete end;
    check = nullptr;
    start = nullptr;
    end = nullptr;
    return returnValue;
}

double Symbol::distanceToLineSegment(Point* check, Point* start, Point* end) const {
	double a = check->x - start->x;
	double b = check->y - start->y;
	double c = end->x - start->x;
	double d = end->y - start->y;

	double dot = (a*c)+(b*d);
	double len_sq = (c*c) + (d*d);
	double param = -1.0;
	if (len_sq != 0) { //in case of 0 length line
		param = dot / len_sq;
	}

	double xx, yy;

	if (param < 0) {
		xx = start->x;
		yy = start->y;
	}
	else if (param > 1) {
		xx = end->x;
		yy = end->y;
	}
	else {
		xx = start->x + (param * c);
		yy = start->y + (param * d);
	}

	double dx = check->x - xx;
	double dy = check->y - yy;
	return sqrt((dx*dx)+(dy*dy));	
}