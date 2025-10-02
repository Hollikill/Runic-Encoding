#include <SFML/Graphics.hpp>
#include "Symbol.h"
#include "DisplayManager.h"
#include <iostream>

DisplayManager::DisplayManager() {
    window.create(sf::VideoMode({ WINDOW_SIZE_X,WINDOW_SIZE_Y }), WINDOW_TITLE, sf::State::Windowed);
    window.setFramerateLimit(60);
}

DisplayManager::~DisplayManager() {
    //nothing
}