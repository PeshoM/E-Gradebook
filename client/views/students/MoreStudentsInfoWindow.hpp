#pragma once

#include <SFML/Graphics.hpp>
#include "../../database/Database.hpp"
#include "../../components/BaseWindow.hpp"
#include <vector>
#include <string>
#include <variant>

class MoreStudentsInfoWindow : public BaseWindow
{
public:
    MoreStudentsInfoWindow(Database *database);
    void render(sf::RenderWindow &window);
    void handle_events(sf::RenderWindow &window, WindowType &next_window) override;
    void update() override;
    bool should_close() const { return close_window; }

private:
    void render_student_list(const std::vector<Student> &students, sf::RenderWindow &window);
    void render_remedial_list(const std::vector<RemedialExamStudent> &students, sf::RenderWindow &window);
    void setup_buttons();
    void setup_text();

    Database *db;
    bool close_window;
    float scroll_offset;
    sf::Font font;

    sf::RectangleShape back_button;
    sf::Text back_button_text;

    sf::RectangleShape top_students_button;
    sf::RectangleShape remedial_exam_button;
    sf::RectangleShape failing_students_button;
    sf::RectangleShape birthday_students_button;

    sf::Text top_students_text;
    sf::Text remedial_exam_text;
    sf::Text failing_students_text;
    sf::Text birthday_students_text;

    sf::Text title_text;
    sf::Text no_students_text;

    std::variant<std::vector<Student>, std::vector<RemedialExamStudent>> current_students;
    bool showing_list;
    std::string current_list_type;
};
