#include <iostream>
#include <cstdlib>
#include <ctime>
#include <map>
#include <algorithm>
#include <limits>

#include <SFML/Graphics.hpp>

#include "Symbol.h"
#include "Dictionary.h"
#include "DisplayManager.h"

class environmentData {
public:
    bool manualSeedInput = false;
    Dictionary* currentDictionary = nullptr;
    bool additionalOutput = true;
    int seed;
    DisplayManager* displayManager = nullptr;
};

void settingsMenu(environmentData&);
bool toggleSetting(std::string, bool);
int menu();
int menu(std::map<int, std::string>);
int menu(std::map<int, std::string>, std::string);
void displayDictionarySymbols(environmentData);
void createNewDictioanry(environmentData&);
void regenerateSeed(environmentData);
int uniqueOrderedCombinationsWithDuplicates (int uniqueElements, int maximumSize);
void addPoint (Symbol& symbol, int x, int y);
void addLine (Symbol& symbol, std::vector<int*> coordinates);
Symbol* generateUnfilteredSymbolTest ();

const std::map<std::string, std::string> TITLES_MENU = {{"main", "MAIN MENU"},
                                                        {"settings", "SETTINGS"}};
const std::map<int, std::string> MENU_MAIN = {{1, "Regenerate Seed"},
                                                {2, "Create new dictionary"},
                                                {3, "Display symbols of current dictionary"},
                                                {4, "Change settings"},
                                                {99, "Exit program"}};
const std::map<int, std::string> MENU_SETTINGS = {{1, "Toggle manual seed input"},
                                                    {2, "Toggle additonal output"},
                                                    {99, "Back to main menu"}};

int main () {
    DisplayManager displayManager;

    while (displayManager.window.isOpen())
    {
        while (const std::optional event = displayManager.window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                displayManager.window.close();
            }
        }

        displayManager.window.clear();
        displayManager.window.display();
    }

    /*std::cout << std::boolalpha; // ensure booleans appear readable in text output

    // init environment
    environmentData environment;
    environment.displayManager = new DisplayManager();

	// generate random seed
	regenerateSeed(environment);

    int menuSelection = -1;
    menuSelection = menu();
    while (menuSelection != MENU_MAIN.rbegin()->first) {
        switch(menuSelection) {
            case 1:
                regenerateSeed(environment);
                break;
            case 2:
                createNewDictioanry(environment);
                break;
            case 3:
            	if (environment.currentDictionary) { displayDictionarySymbols(environment); }
                break;
            case 4:
                settingsMenu(environment);
                break;
        }
        menuSelection = menu();
    }*/

	return 0;
}

void settingsMenu(environmentData& environment) {
    int choice = menu(MENU_SETTINGS, TITLES_MENU.at("settings"));
    while (choice != MENU_SETTINGS.rbegin()->first) {
        switch(choice) {
            case 1:
                environment.manualSeedInput = toggleSetting("Manual seed input", environment.manualSeedInput);
                break;
            case 2:
                environment.additionalOutput = toggleSetting("Additional output", environment.additionalOutput);
                break;
        }
        choice = menu(MENU_SETTINGS, TITLES_MENU.at("settings"));
    }
}

bool toggleSetting(std::string settingDisplayName, bool settingState) {
    bool returnValue = settingState;

    std::cout << settingDisplayName << " is currently [" << settingState << "], toggle (y/n)? ";
    std::string input;
    std::cin >> input;
    if (input == "y") {
        returnValue = !(settingState);
    }
    return returnValue;
}

int menu() { return menu(MENU_MAIN, "MAIN MENU"); }
int menu(std::map<int, std::string> optionNames) { return menu(optionNames, "MENU_TITLE_DEFAULT"); }
int menu(std::map<int, std::string> optionNames, std::string title) {
    std::cout << std::endl << title << std::endl;

    std::vector<int> possibleOptions;
    int exclusiveLowerBound = -1;
    for (int i = 0; i < optionNames.size(); i++) {
        int nextItem = optionNames.lower_bound(exclusiveLowerBound)->first;
        exclusiveLowerBound = nextItem+1;
        std::cout << nextItem << ". " << optionNames.at(nextItem) << std::endl;
        possibleOptions.push_back(nextItem);
    }
    std::cout << "Enter number of chosen option: ";

    int input;
    std::cin >> input;
    while (!input || (std::find(possibleOptions.begin(), possibleOptions.end(), input) == possibleOptions.end())) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "ERROR 1: Invalid menu input value" << std::endl;
        std::cin >> input;
    }

    return input;
}

void displayDictionarySymbols(environmentData environment) {
    int displayScale;
    std::cout << "display scale (min 1, 5 default)? ";
    std::cin >> displayScale;
    while(!displayScale) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> displayScale;
    }

    environment.displayManager->resolutionScale = displayScale;

    Dictionary* dictionary = environment.currentDictionary;
    std::map<int,Symbol*> symbols = dictionary->symbols;
    int exclusiveLowerBound = -1;
    for (int i = 0; i < symbols.size(); i++) {
        int nextSymbol = symbols.lower_bound(exclusiveLowerBound)->first;
        exclusiveLowerBound = nextSymbol+1;
        
        environment.displayManager->renderSymbol(symbols.at(nextSymbol));
    }
}

void createNewDictioanry(environmentData& environment) {
    std::string confirmation = "y";
    if (environment.currentDictionary) {
        std::cout << "WARNING! WARNING! WARNING!" << std::endl
        << "Doing this will delete the previous dictionary." << std::endl
        << "If you wish to keep a previous dictionary, please save it before creating a new dictionary." << std::endl
        << "Proceed (y/n)? ";
        std::cin >> confirmation;
    }
    if (confirmation == "y") {
        environment.currentDictionary = new Dictionary();
        Dictionary* dictionary = environment.currentDictionary;

        dictionary->seed = environment.seed;////

        // ask for generation parameters
        std::cout << "Number of distinct words in dictionary: ";
        std::cin >> dictionary->totalWords;

        std::cout << std::endl << "Maximum composite symbols in a word: ";
        std::cin >> dictionary->compositeSymbolMaximum;

        int symbolsNeededForDictionary = 0;
        while (uniqueOrderedCombinationsWithDuplicates(symbolsNeededForDictionary, dictionary->compositeSymbolMaximum) < dictionary->totalWords) {
            symbolsNeededForDictionary++;
        }
        if (environment.additionalOutput) { std::cout << symbolsNeededForDictionary << " symbols needed for dictionary." << std::endl; }

        for (int i = 0; i < symbolsNeededForDictionary; i++) {
            dictionary->addSymbol(generateUnfilteredSymbolTest());
        }
    }
}

void regenerateSeed(environmentData environment) {
    int seed;
	
    if (!environment.manualSeedInput) {
        seed = std::time(0);
        if (environment.additionalOutput) { std::cout << "seed is: [" << seed << "]" << std::endl; }
    }
    else {
        std::cout << "Enter seed value: ";
	    std::cin >> seed;
        while(!seed) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	        std::cin >> seed;
        }
    }
	
    environment.seed = seed;
    srand(seed);
}

int uniqueOrderedCombinationsWithDuplicates (int uniqueElements, int maximumSize) {
    int uniqueCombinations = 0;
    for (int i = 0; i < maximumSize; i++) {
        uniqueCombinations += pow(uniqueElements, i+1);
    }
    return uniqueCombinations;
}

void addPoint (Symbol& symbol, int x, int y) {
    Stroke* newStroke = symbol.addStroke();
    newStroke->addPoint(x, y);
}

void addLine (Symbol& symbol, std::vector<int*> coordinates) {
    Stroke* newStroke = symbol.addStroke();
    int segments = coordinates.size();
    for (int i = 0; i < segments; i++) {
        newStroke->addPoint(coordinates.at(i)[0], coordinates.at(i)[1]);
    }
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