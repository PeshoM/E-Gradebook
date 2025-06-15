#include "Button.hpp"

Button::Button(sf::Vector2f position, sf::Vector2f size, std::string text)
    : is_hovered(false),
      normal_color(sf::Color(70, 70, 130)),
      hover_color(sf::Color(90, 90, 160)),
      click_color(sf::Color(110, 110, 190))
{
    shape.setPosition(position);
    shape.setSize(size);
    shape.setFillColor(normal_color);
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color(100, 100, 180));

    this->text.setFont(font);
    this->text.setString(text);
    this->text.setCharacterSize(20);
    this->text.setFillColor(sf::Color(220, 220, 255));
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
            shape.setFillColor(hover_color);
            is_hovered = true;
        }
    }
    else
    {
        if (is_hovered)
        {
            shape.setFillColor(normal_color);
            is_hovered = false;
        }
    }
}

bool Button::is_clicked(const sf::Vector2f &mouse_pos, bool mouse_pressed)
{
    if (is_mouse_over(mouse_pos) && mouse_pressed)
    {
        shape.setFillColor(click_color);
        return true;
    }
    else if (is_mouse_over(mouse_pos))
    {
        shape.setFillColor(hover_color);
    }
    else
    {
        shape.setFillColor(normal_color);
    }
    return false;
}

void Button::set_text(const std::string &new_text)
{
    text.setString(new_text);
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.width / 2, textBounds.height / 2 + textBounds.top);
    text.setPosition(shape.getPosition().x + shape.getSize().x / 2,
                     shape.getPosition().y + shape.getSize().y / 2);
}

void Button::setColors(const sf::Color &normal, const sf::Color &hover, const sf::Color &click)
{
    normal_color = normal;
    hover_color = hover;
    click_color = click;
    shape.setFillColor(is_hovered ? hover_color : normal_color);
}
