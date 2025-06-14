#pragma once

#include "../../components/BaseWindow.hpp"
#include "../../database/Database.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>

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

    struct RowButtons
    {
        sf::RectangleShape edit_button;
        sf::RectangleShape delete_button;
        sf::Text edit_text;
        sf::Text delete_text;
    };
    std::vector<RowButtons> row_buttons;

    bool show_modal;
    sf::RectangleShape modal_background;
    sf::RectangleShape modal_window;

    sf::RectangleShape input_box_number;
    sf::RectangleShape input_box_name;
    sf::RectangleShape input_box_date;
    sf::Text input_text_number;
    sf::Text input_text_name;
    sf::Text input_text_date;
    std::string input_string_number;
    std::string input_string_name;
    std::string input_string_date;
    bool input_active;
    int editing_student_id;

    bool show_error;
    sf::Text error_text;

    sf::RectangleShape save_button;
    sf::RectangleShape cancel_button;
    sf::Text save_text;
    sf::Text cancel_text;

    float scroll_offset;
    const float row_height = 30.f;
    const float button_width = 60.f;
    const float button_height = 25.f;

    void load_students();
    void create_row_buttons();
    void handle_modal_events(sf::RenderWindow &window);
    void render_modal(sf::RenderWindow &window);
    void save_student_changes();
    void delete_student(int student_id);
    void show_edit_modal(int student_id);
    bool is_number_duplicate(int number, int current_id = -1);
    void sort_students();
};
