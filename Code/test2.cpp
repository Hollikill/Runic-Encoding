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
	
	testSymbol.displaySymbolText(3);
	
	int gridSize = testSymbol.getGridSize();
	
	coordinates[0] = rand() % gridSize;
	coordinates[1] = rand() % gridSize;

    Point testPoint(coordinates);

    testSymbol.addStroke(testPoint);
	
	return 0;
}