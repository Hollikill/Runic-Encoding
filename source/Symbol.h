#ifndef SYMBOL_
#define SYMBOL_

#include "Stroke.h"
#include <vector>
#include <SFML/Graphics.hpp>

struct BakedTexture {
    sf::Texture texture;
    int scale;
    double strokeThickness;
    bool needsRebake = false;
};

class Symbol
{
private:
    int gridSizeX;
    int gridSizeY;
    std::vector<Stroke*> strokes;

public:
    std::vector<BakedTexture> bakedTextures;

    Symbol();
    Symbol(int sizeX);

    Stroke* addStroke();
    std::vector<Stroke*> getStrokes() const;

    int getGridSize() const;

    bool isBaked(int) const;
    sf::Texture getBakedTexture(int scale);
    void queueRebakeTextures();
    void queueRebakeTexture(int);
    void bakeTexture(int scale, double strokeThickness);

    double distanceToLineSegment(int x, int y, int x1, int y1, int x2, int y2) const;

    ~Symbol();
};

//#include "Symbol.cpp"

#endif