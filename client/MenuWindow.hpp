#pragma once

#include "BaseWindow.hpp"
#include "WindowType.hpp"
#include "Button.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

class MenuWindow : public BaseWindow
{
public:
    std::vector<Button> buttons;

    MenuWindow();
    void handleEvents(sf::RenderWindow &window, WindowType &nextWindow) override;
    void update() override;
    void render(sf::RenderWindow &window) override;
};