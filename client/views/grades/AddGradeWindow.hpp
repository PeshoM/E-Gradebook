#pragma once
#include "../../components/BaseWindow.hpp"
#include "../../components/Button.hpp"
#include "../../database/Database.hpp"
#include <SFML/Graphics.hpp>
#include <string>

class AddGradeWindow : public BaseWindow
{
public:
    AddGradeWindow(Database *db);
    void handle_events(sf::RenderWindow &window, WindowType &next_window) override;
    void update() override;
    void render(sf::RenderWindow &window) override;

private:
    Database *db;
    std::vector<Student> students;
    std::vector<Subject> subjects;

    int student_id;
    int subject_id;
    std::string date;

    std::string input_values[3]; // For student number, subject name, and grade
    int selected_box_index;

    Button back_button;
    Button confirm_button;

    sf::Font font;
    sf::RectangleShape input_boxes[3];
    sf::Text input_labels[3];
    sf::Text input_texts[3];
    sf::Text error_text;

    bool show_success_message;
    sf::Clock success_message_clock;

    void submit_grade();
    std::string get_current_date();
    bool student_exists(int class_number);
    bool subject_exists(const std::string &name);
    void reset_inputs();
};
