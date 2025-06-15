#include "MenuWindow.hpp"
#include <iostream>

MenuWindow::MenuWindow()
{
    if (!Button::font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
    {
        std::cerr << "Error loading font.\n";
    }

    title_text.setFont(Button::font);
    title_text.setString("E-Gradebook");
    title_text.setCharacterSize(40);
    title_text.setFillColor(sf::Color::White);
    title_text.setStyle(sf::Text::Bold);

    title_background.setFillColor(sf::Color(45, 45, 90));
    title_background.setSize(sf::Vector2f(800.f, 100.f));
    title_background.setPosition(0.f, 0.f);

    menu_background.setFillColor(sf::Color(35, 35, 70));
    menu_background.setSize(sf::Vector2f(800.f, 500.f));
    menu_background.setPosition(0.f, 100.f);

    sf::FloatRect title_bounds = title_text.getLocalBounds();
    title_text.setPosition(
        (800.f - title_bounds.width) / 2.f,
        (100.f - title_bounds.height) / 2.f);

    const int buttonsPerRow = 3;
    const sf::Vector2f buttonSize(220.f, 50.f);
    const float xMargin = 40.f;
    const float yMargin = 150.f;
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
        buttons.back().setColors(
            sf::Color(70, 70, 130),
            sf::Color(90, 90, 160),
            sf::Color(110, 110, 190) 
        );

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
    window.clear(sf::Color(30, 30, 60)); // Darker background color

    // Draw backgrounds
    window.draw(menu_background);
    window.draw(title_background);

    // Draw title
    window.draw(title_text);

    // Draw buttons
    for (const auto &btn : buttons)
        btn.draw(window);

    window.display();
}
