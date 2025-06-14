#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "../../components/BaseWindow.hpp"
#include "../../database/Database.hpp"
#include "../../WindowType.hpp"
#include <algorithm>

class ViewSubjectsWindow : public BaseWindow
{
public:
    ViewSubjectsWindow(Database *database);

    void handle_events(sf::RenderWindow &window, WindowType &next_window) override;
    void update() override;
    void render(sf::RenderWindow &window) override;

private:
    enum class InputField
    {
        Name,
        Teacher,
        Room
    };

    Database *db;

    sf::Font font;
    sf::Text back_button_text;
    sf::RectangleShape back_button;

    std::vector<Subject> subjects;

    std::vector<sf::Text> subject_names;
    std::vector<sf::Text> subject_teachers;
    std::vector<sf::Text> subject_rooms;

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

    sf::RectangleShape input_box_name;
    sf::RectangleShape input_box_teacher;
    sf::RectangleShape input_box_room;
    sf::Text input_text_name;
    sf::Text input_text_teacher;
    sf::Text input_text_room;
    std::string input_string_name;
    std::string input_string_teacher;
    std::string input_string_room;
    bool input_active;
    InputField active_input;
    int editing_subject_id;

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

    void load_subjects();
    void create_row_buttons();
    void handle_modal_events(sf::RenderWindow &window);
    void render_modal(sf::RenderWindow &window);
    void save_subject_changes();
    void delete_subject(int subject_id);
    void show_edit_modal(int subject_id);
    bool is_name_duplicate(const std::string &name, int current_id = -1);
    void sort_subjects();
};
