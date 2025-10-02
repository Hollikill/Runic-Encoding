#include <iostream>
#include <cstdlib>
#include <ctime>
#include <map>
#include <algorithm>
#include <limits>

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

#include "Symbol.h"
#include "Dictionary.h"
#include "DisplayManager.h"

class environmentData {
public:
    bool manualSeedInput = false;
    Dictionary* currentDictionary = nullptr;
    bool additionalOutput = true;
    int seed = 0;
    DisplayManager* displayManager = nullptr;
};

void settingsMenu(environmentData&);
bool toggleSetting(std::string, bool);
int menu();
int menu(std::map<int, std::string>);
int menu(std::map<int, std::string>, std::string);
void displayDictionarySymbols(environmentData);
void createNewDictioanry(environmentData&, int, int);
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

int main() {
    std::cout << std::boolalpha; // ensure booleans appear readable in text output

    // init environment
    environmentData environment;
    environment.displayManager = new DisplayManager();
    auto& window = environment.displayManager->window;
    ImGui::SFML::Init(window);

    // generate random seed
    regenerateSeed(environment);

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, event.value());
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        //fill screen
        auto io = ImGui::GetIO();
        ImGui::SetNextWindowSize({ io.DisplaySize.x, io.DisplaySize.y });
        ImGui::SetNextWindowPos({ 0,0 });

        //main menu
        ImGui::Begin("Main Menu", false, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        // regenerate seed
        if (ImGui::Button("Regenerate Seed")) {
            regenerateSeed(environment);
        }

        // creating a dictionary
        if (ImGui::Button("Create new dictionary")) {
            if (environment.currentDictionary == nullptr) {
                ImGui::OpenPopup("Create Dictionary");
            }
            else {
                ImGui::OpenPopup("Confirm Dictionary Creation");
            }
        }
        if (ImGui::BeginPopupModal("Confirm Dictionary Creation", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
            ImGui::Text("You already have a dictionary. Creating a new dictionary will delete the previous dictionary.\nIf you wish to keep a previous dictionary, please save it before creating a new dictionary.");
            ImGui::Separator();
            ImGui::Text("Proceed?");
            if (ImGui::Button("OK")) {
                ImGui::CloseCurrentPopup();
                ImGui::OpenPopup("Create Dictionary");
            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (ImGui::BeginPopup("Create Dictionary")) {
            static char buf1[10] = "1"; ImGui::InputText("Total words in dictionary", buf1, sizeof(buf1), ImGuiInputTextFlags_CharsDecimal); int totalWords = atoi(buf1);
            static char buf2[3] = "1"; ImGui::InputText("Maximum symbols per word", buf2, sizeof(buf2), ImGuiInputTextFlags_CharsDecimal); int maxSymbols = atoi(buf2);
            if (ImGui::Button("Finalize parameters")) {
                createNewDictioanry(environment, totalWords, maxSymbols);
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        // displaying a dictionary
        if (ImGui::Button("Display Dictionary")) {
            // do shit
        }

        // changing settings
        if (ImGui::Button("Settings")) {
            ImGui::OpenPopup("Settings");
        }
        if (ImGui::BeginPopup("Settings")) {
            
            ImGui::Checkbox("Toggle manual seed input", &environment.manualSeedInput);
            ImGui::Checkbox("Toggle additional console output", &environment.additionalOutput);
            ImGui::EndPopup();
        }

        ImGui::End();

        
        

        /*int menuSelection = -1;
        menuSelection = menu();
        switch (menuSelection) {
            case 1:
                regenerateSeed(environment);
                menuSelection = menu();
                break;
            case 2:
                createNewDictioanry(environment);
                menuSelection = menu();
                break;
            case 3:
                if (environment.currentDictionary) { displayDictionarySymbols(environment); }
                menuSelection = menu();
                break;
            case 4:
                settingsMenu(environment);
                menuSelection = menu();
                break;
            case 99:
                environment.displayManager->window.close();
                break;
        }*/
        
        window.clear();

        ImGui::SFML::Render(window); // This should be last render step most of the time
        window.display();
    }

    ImGui::SFML::Shutdown();

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

    int input = 0;
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

void createNewDictioanry(environmentData& environment, int totalWords, int maxSymbols) {
        environment.currentDictionary = new Dictionary();
        Dictionary* dictionary = environment.currentDictionary;

        dictionary->seed = environment.seed;////

        // set generation parameters
        dictionary->totalWords = totalWords;
        dictionary->compositeSymbolMaximum = maxSymbols;

        int symbolsNeededForDictionary = 0;
        while (uniqueOrderedCombinationsWithDuplicates(symbolsNeededForDictionary, dictionary->compositeSymbolMaximum) < dictionary->totalWords) {
            symbolsNeededForDictionary++;
        }
        if (environment.additionalOutput) { std::cout << symbolsNeededForDictionary << " symbols needed for dictionary." << std::endl; }

        for (int i = 0; i < symbolsNeededForDictionary; i++) {
            dictionary->addSymbol(generateUnfilteredSymbolTest());
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