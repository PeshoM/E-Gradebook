#pragma once

#include "../components/BaseWindow.hpp"
#include "../WindowType.hpp"
#include "../components/Button.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

class MenuWindow : public BaseWindow
{
public:
    std::vector<Button> buttons;
    sf::Text title_text;
    sf::RectangleShape title_background;
    sf::RectangleShape menu_background;

    MenuWindow();
    void handle_events(sf::RenderWindow &window, WindowType &nextWindow) override;
    void update() override;
    void render(sf::RenderWindow &window) override;
};