#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "../../components/BaseWindow.hpp"
#include "../../database/Database.hpp"
#include "../../WindowType.hpp"

class ViewSubjectsWindow : public BaseWindow
{
public:
    ViewSubjectsWindow(Database *database);

    void handle_events(sf::RenderWindow &window, WindowType &next_window);
    void update();
    void render(sf::RenderWindow &window);

private:
    void load_subjects();

    Database *db;
    sf::Font font;

    sf::RectangleShape back_button;
    sf::Text back_button_text;

    std::vector<Subject> subjects;

    std::vector<sf::Text> subject_names;
    std::vector<sf::Text> subject_teachers;
    std::vector<sf::Text> subject_rooms;

    float scroll_offset;
};
