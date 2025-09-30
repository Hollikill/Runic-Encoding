#include "DisplayManager.h"

DisplayManager::DisplayManager() {
    window = sf::RenderWindow(sf::VideoMode({ WINDOW_SIZE_X,WINDOW_SIZE_Y }), WINDOW_TITLE);
}

void DisplayManager::renderSymbol(Symbol* symbol) {

    // resize to new resolution
    int canvasSizeX = symbol->getGridSize() * resolutionScale;
    int canvasSizeY = symbol->getGridSize() * resolutionScale;

    sf::Image canvas({ canvasSizeX, canvasSizeY }, sf::Color::Black);

    // calculate adjustment for stroke coordinates basaed on scale of image
    double coordinateAdjustment = (resolutionScale / 2) - 0.5;

    // loop through pixels
    for (int y = 0; y < canvasSizeX; y++) {
        for (int x = 0; x < canvasSizeY; x++) {
            // loop through strokes
            for (int i = 0; i < symbol->getStrokes().size(); i++) {
                if (canvas.getPixel({x,y}) == sf::Color::Black) {
                    std::vector<Point*> points = symbol->getStrokes().at(i)->getPoints();
                    int pointsSize = points.size();

                    // loop through points
                    for (int p = 0; p < pointsSize; p++) {
                        if (canvas.getPixel({ x,y }) == sf::Color::Black) {
                            // get coordinates of the point
                            int px = points.at(p)->x;
                            int py = points.at(p)->y;

                            // calculate distance from point coordinate
                            double differenceX = (double)abs(((px * resolutionScale)+coordinateAdjustment)-x);
                            double differenceY = (double)abs(((py * resolutionScale)+coordinateAdjustment)-y);
                            double distance = (double)sqrt(pow(differenceX, 2) + pow(differenceY, 2));

                            // check if closer than thickness threshold and draw if so
                            if (distance <= strokeThickness) { canvas.setPixel({x, y}, sf::Color::White);
                        }

                            // check if there is a line segment, and evaluate if it is on the line
                            if ((p < (pointsSize-1)) && canvas.getPixel({x,y}) == sf::Color::Black) {
                                distance = distanceToLineSegment(x, y, ((points.at(p)->x * resolutionScale)+coordinateAdjustment), ((points.at(p)->y * resolutionScale)+coordinateAdjustment), ((points.at(p+1)->x * resolutionScale)+coordinateAdjustment), ((points.at(p+1)->y * resolutionScale)+coordinateAdjustment));
                                if (distance <= strokeThickness) { canvas.setPixel({x, y}, sf::Color::White);
                            }
                            }
                        }
                    }
                }
            }
        }
    }

    // convert symbol to drawable object and draw
    sf::Texture texture(canvas, false);
    sf::Sprite sprite(texture);
    window.draw(sprite);
}

/*
Couldn't figure out this one so I got some code from the internet.
This is a c++ version of the javascript code at the link below.
https://stackoverflow.com/a/6853926

credit to Joshua Perina
https://stackoverflow.com/users/368954/joshua
*/

double DisplayManager::distanceToLineSegment(int x, int y, int x1, int y1, int x2, int y2) const {
	double a = x - x1;
	double b = y - y1;
	double c = x2 - x1;
	double d = y2 - y1;

	double dot = (a*c)+(b*d);
	double len_sq = (c*c) + (d*d);
	double param = -1.0;
	if (len_sq != 0) { //in case of 0 length line
		param = dot / len_sq;
	}

	double xx, yy;

	if (param < 0) {
		xx = x1;
		yy = y1;
	}
	else if (param > 1) {
		xx = x2;
		yy = y2;
	}
	else {
		xx = x1 + (param * c);
		yy = y1 + (param * d);
	}

	double dx = x - xx;
	double dy = y - yy;
	return sqrt((dx*dx)+(dy*dy));	
}