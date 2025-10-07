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

bool toggleSetting(std::string, bool);
void displayDictionarySymbols(environmentData, std::vector<sf::Texture*>&, bool&);
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

    // ensure delete protection for dictionaries
    bool confirmCreation = false;

    // flags for keeping popup windows open
    bool popupDisplaySymbols = false;

    ImGuiStyle* style = &ImGui::GetStyle();
    style->WindowMinSize = { 300, 150 };

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
                    // example keypress
                }
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        window.clear();

        std::vector<sf::Texture*> texRefs;

        //fill screen
        ImGuiIO io = ImGui::GetIO();
        ImGui::SetNextWindowSize({ io.DisplaySize.x / 2, io.DisplaySize.y / 2 });
        ImGui::SetNextWindowPos({ 0,0 });

        //main menu
        ImGui::Begin("Main Menu", false, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);

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
        if (ImGui::Button("Display Dictionary") && environment.currentDictionary) {
            ImGui::OpenPopup("Dictionary Symbols");
            popupDisplaySymbols = true;
        }
        if (popupDisplaySymbols) {
            displayDictionarySymbols(environment, texRefs, popupDisplaySymbols);
            
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
        
        ImGui::SFML::Render(window); // This should be last render step most of the time

        // cleanup step required for imgui::image
        for (auto item : texRefs) {
            delete item;
            item = nullptr;
        }
        texRefs.clear();

        window.display();
    }

    ImGui::SFML::Shutdown();

	return 0;
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

void displayDictionarySymbols(environmentData environment, std::vector<sf::Texture*>& texRefs, bool& popupDisplaySymbols) {
    if (ImGui::BeginPopupModal("Dictionary Symbols", NULL, ImGuiWindowFlags_MenuBar)) {
        float itemWidth = ((float)environment.displayManager->resolutionScale * 8) + 30;
        int itemsPerRow = floor(ImGui::GetWindowSize().x / itemWidth);
        ImVec2 windowPos = ImGui::GetWindowPos();
        float windowEndPosY = windowPos.y + (ImGui::GetWindowSize().y);
        //ImGui::SetWindowSize({ (((float)environment.displayManager->resolutionScale*8)+30)*7, io.DisplaySize.y-ImGui::GetWindowPos().y});

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Close")) {
                    popupDisplaySymbols = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Symbol Size")) {
                if (ImGui::MenuItem("Small")) {
                    environment.displayManager->resolutionScale = 5;
                }
                if (ImGui::MenuItem("Medium", "recommened option")) {
                    environment.displayManager->resolutionScale = 10;
                }
                if (ImGui::MenuItem("Large Symbols")) {
                    environment.displayManager->resolutionScale = 20;
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Line Thickness")) {
                if (ImGui::MenuItem("Very Thin")) {
                    environment.displayManager->strokeThickness = 0.5;
                }
                if (ImGui::MenuItem("Normal")) {
                    environment.displayManager->strokeThickness = 1.0;
                }
                if (ImGui::MenuItem("Thick")) {
                    environment.displayManager->strokeThickness = 3;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        Dictionary* dictionary = environment.currentDictionary;
        std::map<int, Symbol*> symbols = dictionary->symbols;
        int exclusiveLowerBound = -1;
        for (int i = 0; i < symbols.size(); i++) {
            if (itemsPerRow > 0) {
                if (i % itemsPerRow != 0) {
                    ImGui::SameLine();
                }
            }

            std::string symbolTitle = "ID: {" + std::to_string(i) + "}";
            const char* symbolTag = symbolTitle.c_str();


            float symbolDisplaySize = (float)environment.displayManager->resolutionScale * 10;
            ImGui::BeginChild(symbolTag, { std::max<float>(symbolDisplaySize, 70.f), symbolDisplaySize + 10 }, NULL, ImGuiWindowFlags_NoTitleBar);
            ImGui::Text(symbolTag);

            // find next symbol in dict
            int nextSymbol = symbols.lower_bound(exclusiveLowerBound)->first;
            exclusiveLowerBound = nextSymbol + 1;

            ImVec2 minBound = { ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y + 20 };
            if (windowPos.y-symbolDisplaySize < minBound.y && minBound.y < windowEndPosY) {
                if (!symbols.at(nextSymbol)->isBaked(environment.displayManager->resolutionScale, environment.displayManager->strokeThickness)) {
                    symbols.at(nextSymbol)->bakeTexture(environment.displayManager->resolutionScale, environment.displayManager->strokeThickness);
                }

                sf::Texture* tex = new sf::Texture(symbols.at(nextSymbol)->getBakedTexture(environment.displayManager->resolutionScale));
                texRefs.push_back(tex); // for later cleanup
                ImGui::Image(*tex);
            }
            else {
                ImGuiStyle* style = &ImGui::GetStyle();
                style->Colors[ImGuiCol_Text] = ImVec4(1, 0, 0, 1);
                ImGui::Text("No Display");
                style->Colors[ImGuiCol_Text] = ImVec4(1, 1, 1, 1);
            }
            
            ImGui::EndChild();
        }
        ImGui::EndPopup();
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