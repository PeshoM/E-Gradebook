#include "Button.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "E-Gradebook");
    window.setFramerateLimit(60);

    if (!Button::font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
    {
        return -1;
    }

    bool mouse_pressed = false;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
                mouse_pressed = true;
            if (event.type == sf::Event::MouseButtonReleased)
                mouse_pressed = false;
        }

        sf::Vector2f mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        window.clear(sf::Color(40, 40, 40));
        window.display();
    }

    return 0;
}
