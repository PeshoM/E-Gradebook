#pragma once

#include <SFML/Graphics.hpp>
#include "WindowType.hpp"

class BaseWindow
{
public:
    virtual ~BaseWindow() = default;
    virtual void handleEvents(sf::RenderWindow &window, WindowType &nextWindow) = 0;
    virtual void update() = 0;
    virtual void render(sf::RenderWindow &window) = 0;
};

