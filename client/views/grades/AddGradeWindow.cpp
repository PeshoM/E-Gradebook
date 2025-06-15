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
      back_button({20, 20}, {150, 40}, "Go Back"),
      confirm_button({20, 80}, {150, 40}, "Add Grade"),
      selected_box_index(-1),
      show_success_message(false)
{
    if (!font.loadFromFile("arial.ttf") &&
        !font.loadFromFile("C:/Windows/Fonts/arial.ttf") &&
        !font.loadFromFile("/System/Library/Fonts/Arial.ttf") &&
        !font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"))
    {
        std::cerr << "Warning: Could not load any system font. Using default rendering.\n";
    }

    const std::string labels[] = {
        "Student Class Number:", "Subject Name:", "Grade:"};

    for (int i = 0; i < 3; ++i)
    {
        input_boxes[i].setSize({300, 40});
        input_boxes[i].setPosition({250, 80 + i * 80});
        input_boxes[i].setFillColor(sf::Color(90, 90, 160));
        input_boxes[i].setOutlineColor(sf::Color(100, 100, 180));
        input_boxes[i].setOutlineThickness(2);

        input_labels[i].setFont(font);
        input_labels[i].setCharacterSize(18);
        input_labels[i].setFillColor(sf::Color(220, 220, 255));
        input_labels[i].setString(labels[i]);
        input_labels[i].setPosition({250, 50 + i * 80});

        input_texts[i].setFont(font);
        input_texts[i].setCharacterSize(18);
        input_texts[i].setFillColor(sf::Color(220, 220, 255));
        input_texts[i].setPosition({260, 90 + i * 80});

        input_values[i] = "";
    }

    error_text.setFont(font);
    error_text.setCharacterSize(20);
    error_text.setFillColor(sf::Color(255, 100, 100));
    error_text.setPosition({250, 320});

    students = db->get_students();
    subjects = db->get_subjects();
}

void AddGradeWindow::reset_inputs()
{
    for (int i = 0; i < 3; ++i)
    {
        input_values[i].clear();
        input_texts[i].setString("");
    }
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

void AddGradeWindow::submit_grade()
{
    error_text.setString("");

    // Validate student number
    int student_number;
    try
    {
        student_number = std::stoi(input_values[0]);
        if (!student_exists(student_number))
        {
            error_text.setString("Student not found.");
            return;
        }
    }
    catch (...)
    {
        error_text.setString("Invalid student number.");
        return;
    }

    // Validate subject
    if (!subject_exists(input_values[1]))
    {
        error_text.setString("Subject not found.");
        return;
    }

    // Validate grade
    float grade_value;
    try
    {
        grade_value = std::stof(input_values[2]);
        if (grade_value < 2 || grade_value > 6)
        {
            error_text.setString("Grade must be between 2 and 6");
            return;
        }
    }
    catch (...)
    {
        error_text.setString("Invalid grade value.");
        return;
    }

    // Round grade to 2 decimal places
    grade_value = std::round(grade_value * 100.0f) / 100.0f;

    // Submit grade
    Grade grade = {student_id, subject_id, get_current_date(), grade_value};
    bool success = db->add_grade(grade);
    if (success)
    {
        show_success_message = true;
        success_message_clock.restart();
        reset_inputs();
    }
    else
    {
        error_text.setString("Failed to add grade.");
    }
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
                submit_grade();
            }

            // Check which input box was clicked
            for (int i = 0; i < 3; ++i)
            {
                if (input_boxes[i].getGlobalBounds().contains(mouse_pos))
                {
                    selected_box_index = i;
                    break;
                }
            }
        }

        if (event.type == sf::Event::TextEntered && selected_box_index != -1)
        {
            if (event.text.unicode == BACKSPACE_UNICODE)
            {
                if (!input_values[selected_box_index].empty())
                    input_values[selected_box_index].pop_back();
            }
            else if (event.text.unicode < 128 && std::isprint(event.text.unicode))
            {
                input_values[selected_box_index] += static_cast<char>(event.text.unicode);
            }

            input_texts[selected_box_index].setString(input_values[selected_box_index]);
        }
    }
}

void AddGradeWindow::update()
{
}

void AddGradeWindow::render(sf::RenderWindow &window)
{
    window.clear(sf::Color(40, 40, 80));

    back_button.draw(window);
    confirm_button.draw(window);

    for (int i = 0; i < 3; ++i)
    {
        window.draw(input_boxes[i]);
        window.draw(input_labels[i]);
        window.draw(input_texts[i]);
    }

    if (!error_text.getString().isEmpty())
    {
        // Center the error text
        sf::FloatRect textRect = error_text.getLocalBounds();
        error_text.setOrigin(textRect.left + textRect.width / 2.0f,
                             textRect.top + textRect.height / 2.0f);
        error_text.setPosition(window.getSize().x / 2.0f, 320);
        window.draw(error_text);
    }

    if (show_success_message)
    {
        if (success_message_clock.getElapsedTime().asSeconds() < 1.5f)
        {
            sf::Text success_text;
            success_text.setFont(font);
            success_text.setCharacterSize(22);
            success_text.setFillColor(sf::Color(90, 160, 90));
            success_text.setString("Grade added successfully!");

            sf::FloatRect textRect = success_text.getLocalBounds();
            success_text.setOrigin(textRect.left + textRect.width / 2.0f,
                                   textRect.top + textRect.height / 2.0f);
            success_text.setPosition(window.getSize().x / 2.0f, 400);

            window.draw(success_text);
        }
        else
        {
            show_success_message = false;
        }
    }

    window.display();
}
