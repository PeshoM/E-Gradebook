#pragma once

#include <SFML/Graphics.hpp>
#include "WindowType.hpp"

class BaseWindow
{
public:
    virtual ~BaseWindow() = default;
    virtual void handle_events(sf::RenderWindow &window, WindowType &next_window) = 0;
    virtual void update() = 0;
    virtual void render(sf::RenderWindow &window) = 0;
};

