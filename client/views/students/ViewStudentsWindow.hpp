#pragma once
#include "../../components/BaseWindow.hpp"
#include "../../database/Database.hpp"
#include <SFML/Graphics.hpp>

class ViewStudentsWindow : public BaseWindow
{
public:
    ViewStudentsWindow(Database *database);

    void handle_events(sf::RenderWindow &window, WindowType &next_window) override;
    void update() override;
    void render(sf::RenderWindow &window) override;

private:
    Database *db;

    sf::Font font;
    sf::Text back_button_text;
    sf::RectangleShape back_button;

    std::vector<Student> students;

    std::vector<sf::Text> student_numbers;
    std::vector<sf::Text> student_names;
    std::vector<sf::Text> student_dates;

    float scroll_offset;

    void load_students();
};
