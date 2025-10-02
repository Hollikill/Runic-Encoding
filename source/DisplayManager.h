#ifndef _DISPLAY_MANAGER
#define _DISPLAY_MANAGER

const int WINDOW_SIZE_X = 960;
const int WINDOW_SIZE_Y = 540;
const std::string WINDOW_TITLE = "Runic Encoding";

class DisplayManager {
public:
    sf::RenderWindow window;
    int resolutionScale = 10;
    double strokeThickness = 1;

    DisplayManager();

    sf::Image renderSymbol(Symbol*);

    double distanceToLineSegment(int x, int y, int x1, int y1, int x2, int y2) const;

    ~DisplayManager();
};

//#include "DisplayManager.cpp"

#endif