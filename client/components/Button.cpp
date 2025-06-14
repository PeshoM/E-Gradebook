#include "Button.hpp"

Button::Button(sf::Vector2f position, sf::Vector2f size, std::string text)
    : is_hovered(false)
{
    shape.setPosition(position);
    shape.setSize(size);
    shape.setFillColor(sf::Color(70, 70, 70));
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::White);

    this->text.setFont(font);
    this->text.setString(text);
    this->text.setCharacterSize(20);
    this->text.setFillColor(sf::Color(200, 200, 200));
    sf::FloatRect textBounds = this->text.getLocalBounds();
    this->text.setOrigin(textBounds.width / 2, textBounds.height / 2 + textBounds.top);
    this->text.setPosition(position.x + size.x / 2, position.y + size.y / 2);
}

Button::~Button()
{
}

void Button::draw(sf::RenderWindow &window) const
{
    window.draw(shape);
    window.draw(text);
}

bool Button::is_mouse_over(const sf::Vector2f &mouse_pos)
{
    return shape.getGlobalBounds().contains(mouse_pos);
}

void Button::update(const sf::Vector2f &mouse_pos)
{
    if (is_mouse_over(mouse_pos))
    {
        if (!is_hovered)
        {
            shape.setFillColor(sf::Color(100, 100, 100));
            is_hovered = true;
        }
    }
    else
    {
        if (is_hovered)
        {
            shape.setFillColor(sf::Color(70, 70, 70));
            is_hovered = false;
        }
    }
}

bool Button::is_clicked(const sf::Vector2f &mouse_pos, bool mouse_pressed)
{
    return is_mouse_over(mouse_pos) && mouse_pressed;
}
