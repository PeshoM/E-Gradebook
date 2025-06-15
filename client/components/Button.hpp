#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Button
{
private:
    sf::RectangleShape shape;
    sf::Text text;
    bool is_hovered;
    sf::Color normal_color;
    sf::Color hover_color;
    sf::Color click_color;

public:
    inline static sf::Font font;

    Button(sf::Vector2f position, sf::Vector2f size, std::string text);

    ~Button();

    void draw(sf::RenderWindow &window) const;
    bool is_mouse_over(const sf::Vector2f &mouse_pos);
    void update(const sf::Vector2f &mouse_pos);
    bool is_clicked(const sf::Vector2f &mouse_pos, bool mouse_pressed);
    void set_text(const std::string &new_text);
    void setColors(const sf::Color &normal, const sf::Color &hover, const sf::Color &click);
};