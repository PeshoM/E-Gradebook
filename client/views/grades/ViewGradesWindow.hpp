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

    sf::RectangleShape class_input_box;
    sf::Text class_input_text;
    std::string class_input_string;
    bool input_active;

    sf::RectangleShape subject_input_box;
    sf::Text subject_input_text;
    std::string subject_input_string;
    bool subject_input_active;

    sf::Text subject_avg_text;
    sf::Text overall_avg_text;
    sf::Text no_grades_text;
    float current_subject_avg;
    float current_overall_avg;

    std::vector<GradeEntry> grades;

    float scroll_offset;
    bool class_entered;
    const float row_height = 40.0f;
    const float visible_rows = 10.0f;

    bool show_modal;
    sf::RectangleShape modal_background;
    sf::RectangleShape modal_window;
    sf::RectangleShape input_box_grade;
    sf::Text input_text_grade;
    std::string input_string_grade;
    int editing_grade_id;
    bool show_error;
    sf::Text error_text;
    sf::RectangleShape save_button;
    sf::RectangleShape cancel_button;
    sf::Text save_text;
    sf::Text cancel_text;

    struct RowButtons
    {
        sf::RectangleShape edit_button;
        sf::RectangleShape delete_button;
        sf::Text edit_text;
        sf::Text delete_text;
    };
    std::vector<RowButtons> row_buttons;
    const float button_width = 60.f;
    const float button_height = 25.f;

    void load_grades_for_class(const std::string &class_number);
    void update_averages();
    void reset_input();
    void handle_scroll(float delta);
    void create_row_buttons();
    void handle_modal_events(sf::RenderWindow &window);
    void render_modal(sf::RenderWindow &window);
    void save_grade_changes();
    void delete_grade(int grade_id);
    void show_edit_modal(int grade_id);
    void sort_grades();
};
