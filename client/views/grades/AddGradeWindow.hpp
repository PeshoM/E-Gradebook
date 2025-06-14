#pragma once
#include "../../components/BaseWindow.hpp"
#include "../../components/Button.hpp"
#include "../../database/Database.hpp"
#include <SFML/Graphics.hpp>
#include <string>

class AddGradeWindow : public BaseWindow {
public:
    AddGradeWindow(Database* db);
    void handle_events(sf::RenderWindow& window, WindowType& next_window) override;
    void update() override;
    void render(sf::RenderWindow& window) override;

private:
    enum Step { EnterStudentNumber, EnterSubjectName, EnterGrade, Done };
    Step current_step;

    Database* db;
    std::vector<Student> students;
    std::vector<Subject> subjects;

    int student_id;
    int subject_id;
    std::string date;

    int student_number;
    std::string subject_name;
    float grade_value;

    std::string input_value;

    Button back_button;
    Button confirm_button;

    sf::Font font;
    sf::RectangleShape input_box;
    sf::Text input_label;
    sf::Text input_text;
    sf::Text error_text;

    bool show_success_message;
    sf::Clock success_message_clock;

    void try_advance_step(WindowType& next_window);
    void reset_input();
    std::string get_current_date();
    void submit_grade();
    bool student_exists(int class_number);
    bool subject_exists(const std::string& name);
};
