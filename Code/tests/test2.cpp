#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Symbol.h"

int main () {
	// generate random seed
	int seed;
	
	seed = std::time(0);
	std::cout << "seed is: [" << seed << "]" << std::endl;
	//std::cout << "Enter seed value: ";
	//std::cin >> seed;
	
	srand(seed);
	
	Symbol testSymbol;

    int coordinates[2];
	
	//testSymbol.displaySymbolText(3, 1);
	
	int gridSize = testSymbol.getGridSize();
	
	coordinates[0] = rand() % gridSize;
	coordinates[1] = rand() % gridSize;

    Stroke testStroke;
    testStroke.addPoint(coordinates[0], coordinates[1]);

    testSymbol.addStroke(testStroke);
	
	int resolutionMultiplier;
	double drawThickness;
	
	std::cout << "resolution multiplier: ";
	std::cin >> resolutionMultiplier;
	std::cout << "draw thickness threshold: ";
	std::cin >> drawThickness;

	testSymbol.displaySymbolText(resolutionMultiplier, drawThickness);
	
	return 0;
}