#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Symbol.h"

int uniqueOrderedCombinationsWithDuplicates (int uniqueElements, int maximumSize);
void debugDisplaySymbol(Symbol symbol);
void addPoint (Symbol& symbol, int x, int y);
void addLine (Symbol& symbol, std::vector<int*> coordinates);
Symbol* generateUnfilteredSymbolTest ();

int main () {
while (true) {
	// generate random seed
	int seed;
	
	seed = std::time(0);
	std::cout << "seed is: [" << seed << "]" << std::endl;
	//std::cout << "Enter seed value: ";
	//std::cin >> seed;
	
	srand(seed);

    // ask for generation parameters
    
    int dictionarySize;
    int compositeSymbolMaximum;

    std::cout << "Number of distinct words in dictionary: ";
    std::cin >> dictionarySize;

    std::cout << std::endl << "Maximum composite symbols in a word: ";
    std::cin >> compositeSymbolMaximum;

    // determine how many distinct symbols are needed
    int symbolsNeededForDictionary = 0;
    while (uniqueOrderedCombinationsWithDuplicates(symbolsNeededForDictionary, compositeSymbolMaximum) < dictionarySize) {
        //std::cout << symbolsNeededForDictionary << " symbols needed, " << uniqueOrderedCombinationsWithDuplicates(symbolsNeededForDictionary, compositeSymbolMaximum) << " total symbol combinations, " << dictionarySize << " dictionary size" << std::endl;
        symbolsNeededForDictionary++;
    }
    std::cout << symbolsNeededForDictionary << " symbols needed" << std::endl;

    // generate enough symbols
    std::vector<Symbol*> testPartialDictionary;

    for (int i = 0; i < symbolsNeededForDictionary; i++) {
        testPartialDictionary.push_back(generateUnfilteredSymbolTest());
    }

    std::string input;
    std::cout << "display symbols (y/n)? ";
    std::cin >> input;
    if (input == "y") {
        for (int i = 0; i < testPartialDictionary.size(); i++) {
            std::cout << "=========" << std::endl;
            std::cout << "Symbol #" << i << std::endl;
            testPartialDictionary.at(i)->displaySymbolText(5, 0.5);
        }
    }

}
	return 0;
}

int uniqueOrderedCombinationsWithDuplicates (int uniqueElements, int maximumSize) {
    int uniqueCombinations = 0;
    for (int i = 0; i < maximumSize; i++) {
        uniqueCombinations += pow(uniqueElements, i+1);
    }
    return uniqueCombinations;
}

void debugDisplaySymbol(Symbol symbol) {
    symbol.displaySymbolText(10, 1);
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
    //std::cout << newStroke->getPoints().size() << std::endl;
}

Symbol* generateUnfilteredSymbolTest () {
    Symbol* testSymbol = new Symbol();

	int gridSize = testSymbol->getGridSize();

    int pointsToAdd = (rand() % 3)+1;
    for (int i = 0; i < pointsToAdd; i++) {
        addPoint(*testSymbol, rand() % gridSize, rand() % gridSize);
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
        addLine(*testSymbol, coordinates);
    }

    return testSymbol;
}