#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Symbol.h"

void addPoint (Symbol& symbol, int x, int y);

void addLine (Symbol& symbol, std::vector<int*> coordinates);

int main () {
	// generate random seed
	int seed;
	
	seed = std::time(0);
	std::cout << "seed is: [" << seed << "]" << std::endl;
	//std::cout << "Enter seed value: ";
	//std::cin >> seed;
	
	srand(seed);
	
	Symbol testSymbol;

	int gridSize = testSymbol.getGridSize();

    int pointsToAdd = (rand() % 3)+1;
    for (int i = 0; i < pointsToAdd; i++) {
        addPoint(testSymbol, rand() % gridSize, rand() % gridSize);
    }

    int linesToAdd = (rand() % 3)+1;
    for (int i = 0; i < linesToAdd; i++) {
        int segments = (rand() % 2) + 2;
        std::vector<int*> coordinates;
        for (int seg = 0; seg < segments; seg++) {
            int* position = new int[2];
            position[0] = rand() % gridSize;
            position[1] = rand() % gridSize;
            coordinates.push_back(position);
        }
        addLine(testSymbol, coordinates);
    }
	
	int resolutionMultiplier;
	double drawThickness;
	
	std::cout << "resolution multiplier: ";
	std::cin >> resolutionMultiplier;
	std::cout << "draw thickness threshold: ";
	std::cin >> drawThickness;

	testSymbol.displaySymbolText(resolutionMultiplier, drawThickness);
	
	return 0;
}

void addPoint (Symbol& symbol, int x, int y) {
    Stroke* newStroke = symbol.addStroke();
    newStroke->addPoint(x, y);
    //std::cout << symbol.getStrokes().size() << "|" << newStroke->getPoints().at(0)->x << std::endl;
}

void addLine (Symbol& symbol, std::vector<int*> coordinates) {
    Stroke* newStroke = symbol.addStroke();
    int segments = coordinates.size();
    for (int i = 0; i < segments; i++) {
        newStroke->addPoint(coordinates.at(i)[0], coordinates.at(i)[1]);
    }
    std::cout << newStroke->getPoints().size() << std::endl;
}