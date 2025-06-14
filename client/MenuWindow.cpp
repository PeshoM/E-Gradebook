#include "MenuWindow.hpp"
#include <iostream>

MenuWindow::MenuWindow()
{
    if (!Button::font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
    {
        std::cerr << "Error loading font.\n";
    }

    const int buttonsPerRow = 3;
    const sf::Vector2f buttonSize(220.f, 50.f);
    const float xMargin = 40.f;
    const float yMargin = 30.f;
    const float xSpacing = 20.f;
    const float ySpacing = 20.f;

    int index = 0;
    for (const auto &[type, label] : windowTypeToLabel)
    {
        if (type == WindowType::Menu)
            continue;

        int row = index / buttonsPerRow;
        int col = index % buttonsPerRow;

        float x = xMargin + col * (buttonSize.x + xSpacing);
        float y = yMargin + row * (buttonSize.y + ySpacing);

        buttons.emplace_back(sf::Vector2f(x, y), buttonSize, label);

        ++index;
    }
}

void MenuWindow::handle_events(sf::RenderWindow &window, WindowType &nextWindow)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

            static std::vector<WindowType> types;
            if (types.empty())
            {
                for (const auto &entry : windowTypeToLabel)
                {
                    if (entry.first == WindowType::Menu)
                        continue;
                    types.push_back(entry.first);
                }
            }

            for (size_t i = 0; i < buttons.size(); ++i)
            {
                if (buttons[i].is_clicked(mousePos, true))
                {
                    nextWindow = types[i];
                    break;
                }
            }
        }
    }
}

void MenuWindow::update() {}

void MenuWindow::render(sf::RenderWindow &window)
{
    window.clear(sf::Color::Blue);
    for (const auto &btn : buttons)
        btn.draw(window);
    window.display();
}
