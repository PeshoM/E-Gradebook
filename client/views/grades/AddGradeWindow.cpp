#include "AddGradeWindow.hpp"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <cmath>
#include "../../database/models/student.hpp"
#include "../../database/models/subject.hpp"
#include "../../database/models/grade.hpp"

#define BACKSPACE_UNICODE 8

AddGradeWindow::AddGradeWindow(Database *db)
    : db(db),
      back_button({20, 20}, {120, 40}, "Back"),
      confirm_button({250, 250}, {150, 40}, "Confirm"),
      current_step(EnterStudentNumber),
      show_success_message(false)
{

    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
    {
        std::cerr << "Failed to load font.\n";
    }

    input_box.setSize({300, 40});
    input_box.setPosition({250, 150});
    input_box.setFillColor(sf::Color(255, 255, 255, 200));
    input_box.setOutlineColor(sf::Color::Black);
    input_box.setOutlineThickness(2);

    input_label.setFont(font);
    input_label.setCharacterSize(18);
    input_label.setFillColor(sf::Color::White);
    input_label.setPosition({250, 120});
    input_label.setString("Student Class Number:");

    input_text.setFont(font);
    input_text.setCharacterSize(18);
    input_text.setFillColor(sf::Color::Black);
    input_text.setPosition({260, 160});

    error_text.setFont(font);
    error_text.setCharacterSize(16);
    error_text.setFillColor(sf::Color::Red);
    error_text.setPosition({250, 200});

    students = db->get_students();
    subjects = db->get_subjects();
}

void AddGradeWindow::reset_input()
{
    input_value.clear();
    input_text.setString("");
    error_text.setString("");
}

bool AddGradeWindow::student_exists(int class_number)
{
    return std::any_of(students.begin(), students.end(),
                       [&](const Student &s)
                       { 
                        student_id = s.id;
                        return s.number_in_class == class_number; });
}

bool AddGradeWindow::subject_exists(const std::string &name)
{
    return std::any_of(subjects.begin(), subjects.end(),
                       [&](const Subject &s)
                       { 
                        subject_id = s.id;
                        return s.name == name; });
}

std::string AddGradeWindow::get_current_date()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm *local_tm = std::localtime(&now_time);

    std::ostringstream oss;
    oss << std::put_time(local_tm, "%Y-%m-%d");
    return oss.str();
}

void AddGradeWindow::try_advance_step(WindowType &next_window)
{
    error_text.setString("");

    switch (current_step)
    {
    case EnterStudentNumber:
        try
        {
            student_number = std::stoi(input_value);
            if (!student_exists(student_number))
            {
                error_text.setString("Student not found.");
                return;
            }
            current_step = EnterSubjectName;
            input_label.setString("Subject Name:");
            reset_input();
        }
        catch (...)
        {
            error_text.setString("Invalid number.");
        }
        break;

    case EnterSubjectName:
        subject_name = input_value;
        if (!subject_exists(subject_name))
        {
            error_text.setString("Subject not found.");
            return;
        }
        current_step = EnterGrade;
        input_label.setString("Grade:");
        reset_input();
        break;

    case EnterGrade:
        grade_value = std::stof(input_value);
        if (grade_value < 2 || grade_value > 6)
        {
            error_text.setString("Grade must be between 2 and 6");
            return;
        }
        submit_grade();
        current_step = Done;
        confirm_button.set_text("Done");
        break;

    case Done:
        next_window = WindowType::Menu;
        break;
    }
}

void AddGradeWindow::submit_grade()
{
    grade_value = std::round(grade_value * 100.0f) / 100.0f;
    Grade grade = {student_id, subject_id, get_current_date(), grade_value};
    bool success = db->add_grade(grade);
    if (success)
    {
        show_success_message = true;
        success_message_clock.restart();
    }
    else
    {
        error_text.setString("Failed to add grade.");
    }
    reset_input();
}

void AddGradeWindow::handle_events(sf::RenderWindow &window, WindowType &next_window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseButtonPressed)
        {
            sf::Vector2f mouse_pos(event.mouseButton.x, event.mouseButton.y);

            if (back_button.is_clicked(mouse_pos, true))
            {
                next_window = WindowType::Menu;
                return;
            }

            if (confirm_button.is_clicked(mouse_pos, true))
            {
                try_advance_step(next_window);
            }
        }

        if (event.type == sf::Event::TextEntered)
        {
            if (event.text.unicode == BACKSPACE_UNICODE)
            {
                if (!input_value.empty())
                    input_value.pop_back();
            }
            else if (event.text.unicode < 128 && std::isprint(event.text.unicode))
            {
                input_value += static_cast<char>(event.text.unicode);
            }

            input_text.setString(input_value);
        }
    }
}

void AddGradeWindow::update()
{
}

void AddGradeWindow::render(sf::RenderWindow &window)
{
    window.clear(sf::Color(50, 50, 90));

    back_button.draw(window);
    confirm_button.draw(window);
    window.draw(input_box);
    window.draw(input_label);
    window.draw(input_text);

    if (!error_text.getString().isEmpty())
    {
        window.draw(error_text);
    }

    if (show_success_message)
    {
        if (success_message_clock.getElapsedTime().asSeconds() < 1.5f)
        {
            sf::Text success_text;
            success_text.setFont(font);
            success_text.setCharacterSize(22);
            success_text.setFillColor(sf::Color::Green);
            success_text.setString("Grade added successfully!");

            sf::FloatRect textRect = success_text.getLocalBounds();
            success_text.setOrigin(textRect.left + textRect.width / 2.0f,
                                   textRect.top + textRect.height / 2.0f);
            success_text.setPosition(window.getSize().x / 2.0f, 80);

            window.draw(success_text);
        }
        else
        {
            show_success_message = false;
        }
    }

    window.display();
}
