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
    if (!ImGui::SFML::Init(window)) {
        return -1;
    }

    // generate random seed
    regenerateSeed(environment);

    bool displayingSymbol = false;
    bool confirmCreation = false;

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
            else if (event->is<sf::Event::Resized>())
            {
                sf::FloatRect view({ 0, 0 }, { (float)window.getSize().x, (float)window.getSize().y });
                window.setView(sf::View(view));
            }
            else if (event->is<sf::Event::KeyPressed>()) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C) && environment.currentDictionary != nullptr) {
                    displayingSymbol = true;
                    /*std::cout << environment.currentDictionary->symbols.at(0)->isBaked(10) << "|";
                    environment.currentDictionary->symbols.at(0)->bakeTexture(10, 0.5);
                    std::cout << environment.currentDictionary->symbols.at(0)->isBaked(10) << "|";*/
                }
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        window.clear();

        if (displayingSymbol) {
            environment.currentDictionary->symbols.at(0)->bakeTexture(10, 0.5);
            sf::Texture texture = environment.currentDictionary->symbols.at(0)->getBakedTexture(10);
            sf::Sprite sprite(texture);
            sprite.setPosition({ 10,10 });
            window.draw(sprite);
        }

        //fill screen
        /*ImGuiIO io = ImGui::GetIO();
        ImGui::SetNextWindowSize({ io.DisplaySize.x / 2, io.DisplaySize.y / 2 });
        ImGui::SetNextWindowPos({ 0,0 });*/

        //main menu
        //ImGui::Begin("Main Menu", false, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        ImGui::Begin("Main Menu");

        // regenerate seed
        if (ImGui::Button("Regenerate Seed")) {
            regenerateSeed(environment);
        }

        // creating a dictionary
        if (ImGui::Button("Create new dictionary")) {
            if (environment.currentDictionary != nullptr && !confirmCreation) {
                ImGui::OpenPopup("Confirm Dictionary Creation");
            }
            else {
                ImGui::OpenPopup("Create Dictionary");
            }
        }
        if (ImGui::BeginPopupModal("Confirm Dictionary Creation", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
            ImGui::Text("You already have a dictionary. Creating a new dictionary will delete the previous dictionary.\nIf you wish to keep a previous dictionary, please save it before creating a new dictionary.");
            ImGui::Separator();
            ImGui::Text("Proceed?");
            if (ImGui::Button("OK")) {
                confirmCreation = true;
                std::cout << confirmCreation << "\n";
                ImGui::CloseCurrentPopup();
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
                confirmCreation = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        // displaying a dictionary
        if (ImGui::Button("Display Dictionary")) {
            ImGui::OpenPopup("Dictionary Symbols");
        }
        if (ImGui::BeginPopupModal("Dictionary Symbols")) {
            //displayDictionarySymbols(environment);
            ImGui::SliderInt("Size of symbols", &environment.displayManager->resolutionScale, 5, 25);
            ImGui::SameLine();
            if (ImGui::Button("Close Window")) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::Separator();

            Dictionary* dictionary = environment.currentDictionary;
            std::map<int, Symbol*> symbols = dictionary->symbols;
            int exclusiveLowerBound = -1;
            for (int i = 0; i < symbols.size(); i++) {
                if (i % 6 != 0) {
                    ImGui::SameLine();
                }

                std::string symbolTitle = "Symbol " + std::to_string(i);
                const char* symbolTag = symbolTitle.c_str();


                float symbolDisplaySize = (float)environment.displayManager->resolutionScale * 10;
                ImGui::BeginChild(symbolTag, { std::max<float>(symbolDisplaySize, 70.f), symbolDisplaySize + 10}, NULL, ImGuiWindowFlags_NoTitleBar);
                ImGui::Text(symbolTag);

                // find next symbol in dict
                int nextSymbol = symbols.lower_bound(exclusiveLowerBound)->first;
                exclusiveLowerBound = nextSymbol + 1;

                if (!symbols.at(nextSymbol)->isBaked((float)environment.displayManager->resolutionScale)) {
                    symbols.at(nextSymbol)->bakeTexture((float)environment.displayManager->resolutionScale, 0.5);
                }
                ImGui::Image(symbols.at(nextSymbol)->getBakedTexture((float)environment.displayManager->resolutionScale), {80, 80}, sf::Color::Blue, sf::Color::Magenta);
                /*unsigned int size = symbols.at(nextSymbol)->getGridSize() * environment.displayManager->resolutionScale;
                sf::Image canvas = environment.displayManager->renderSymbol(symbols.at(nextSymbol));
                canvas.setPixel({ 5,5 }, sf::Color::Magenta);
                sf::Texture texture(canvas, false);
                ImGui::Image(texture, sf::Color::Blue, sf::Color::Magenta);*/
                ImGui::EndChild();
            }
            ImGui::EndPopup();
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

        sf::CircleShape circ(300, (rand() % 4) + 30);
        circ.setFillColor(sf::Color::Yellow);
        float radius = (float)((rand()%6)+297);
        circ.setRadius(radius);
        circ.setOrigin({radius, radius});
        circ.setPosition({ 300, 300 });
        window.draw(circ);
        
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
    /*ImGui::Text("All symbols in current dictioanry");
    ImGui::SliderInt("Size of symbols", &environment.displayManager->resolutionScale, 5, 25);
    ImGui::Separator();

    Dictionary* dictionary = environment.currentDictionary;
    std::map<int,Symbol*> symbols = dictionary->symbols;
    int exclusiveLowerBound = -1;
    for (int i = 0; i < symbols.size(); i++) {
        std::string symbolTitle = "Symbol " + std::to_string(i);
        const char* symbolTag = symbolTitle.c_str();


        float symbolDisplaySize = (float)environment.displayManager->resolutionScale * 10;
        ImGui::BeginChild(symbolTag, { symbolDisplaySize, symbolDisplaySize+10 }, NULL, ImGuiWindowFlags_NoTitleBar);
        ImGui::Text(symbolTag);

        // find next symbol in dict
        int nextSymbol = symbols.lower_bound(exclusiveLowerBound)->first;
        exclusiveLowerBound = nextSymbol+1;
        
        unsigned int size = symbols.at(nextSymbol)->getGridSize() * environment.displayManager->resolutionScale;
        sf::Image canvas({size,size}, sf::Color::Black);
        environment.displayManager->renderSymbol(symbols.at(nextSymbol), canvas);
        sf::Texture texture(canvas, false);
        sf::Sprite sprite(texture);
        ImGui::Image(sprite, sf::Color::Blue, sf::Color::Magenta);
        sprite.setPosition({ 1500, 1500 });
        environment.displayManager->window.draw(sprite);
        ImGui::EndChild();
    }*/
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