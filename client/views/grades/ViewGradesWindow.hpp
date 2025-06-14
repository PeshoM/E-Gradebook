#pragma once

#include <SFML/Graphics.hpp>
#include "../../database/Database.hpp"
#include "../../WindowType.hpp"
#include "../../components/BaseWindow.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

class ViewGradesWindow : public BaseWindow
{
public:
    ViewGradesWindow(Database *database);

    void handle_events(sf::RenderWindow &window, WindowType &next_window);
    void update();
    void render(sf::RenderWindow &window);

private:
    Database *db;

    sf::Font font;

    sf::RectangleShape back_button;
    sf::Text back_button_text;

    // Input for class number
    sf::RectangleShape class_input_box;
    sf::Text class_input_text;
    std::string class_input_string;
    bool input_active;

    // Grades and subjects display
    std::vector<GradeEntry> grades;

    // Scrolling
    float scroll_offset;
    bool class_entered;
    const float row_height = 40.0f;
    const float visible_rows = 10.0f;

    void load_grades_for_class(const std::string &class_number);
    void reset_input();
    void handle_scroll(float delta);
};
