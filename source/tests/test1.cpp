#include <iostream>
#include <cstdlib>
#include <ctime>

int main () {
	// generate random seed
	int seed;
	
	seed = std::time(0);
	std::cout << "seed is: [" << seed << "]" << std::endl;
	//std::cout << "Enter seed value: ";
	//std::cin >> seed;
	
	srand(seed);
	
	// create symbol grid
	
	const int gridSizeX = 8;
	const int gridSizeY = 8;
	
	// generate a random point
	
	int coordinates[2];
	
	coordinates[0] = rand() % gridSizeX;
	coordinates[1] = rand() % gridSizeY;
	
	// print image of symbol
	
	for (int y = 0; y < gridSizeY; y++) {
		for (int x = 0; x < gridSizeX; x++) {
			if (y == coordinates[1] && x == coordinates[0]) {
				std::cout << "X";
			}
			else {
				std::cout << "_";
			}
		}
		std::cout << std::endl;
	}
	
	
	return 0;
}