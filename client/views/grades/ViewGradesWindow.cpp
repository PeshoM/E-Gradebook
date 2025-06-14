#include "ViewGradesWindow.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include "../../database/models/gradeEntry.hpp"

#define BACKSPACE_UNICODE 8
#define ENTER_UNICODE 13

ViewGradesWindow::ViewGradesWindow(Database *database)
    : db(database), scroll_offset(0.f), input_active(true), class_entered(false)
{
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
    {
        std::cerr << "Failed to load font.\n";
    }

    back_button.setSize({150, 40});
    back_button.setPosition({20, 20});
    back_button.setFillColor(sf::Color(100, 100, 150));

    back_button_text.setFont(font);
    back_button_text.setString("Go Back");
    back_button_text.setCharacterSize(20);
    back_button_text.setFillColor(sf::Color::White);
    back_button_text.setPosition(40, 28);

    class_input_box.setSize({300, 40});
    class_input_box.setPosition({20, 80});
    class_input_box.setFillColor(sf::Color(80, 80, 80));

    class_input_text.setFont(font);
    class_input_text.setCharacterSize(24);
    class_input_text.setFillColor(sf::Color::White);
    class_input_text.setPosition({25, 85});
    class_input_text.setString("Enter Class Number...");

    class_input_string.clear();
}

void ViewGradesWindow::reset_input()
{
    class_input_string.clear();
    class_input_text.setString("Enter Class Number...");
    input_active = true;
    class_entered = false;
    grades.clear();
    scroll_offset = 0.f;
}

void ViewGradesWindow::load_grades_for_class(const std::string &class_number)
{
    grades.clear();

    auto student_grades = db->get_grades_by_class_number(std::stoi(class_number));

    for (const auto &grade_record : student_grades)
    {
        GradeEntry entry;

        entry.subject_name.setFont(font);
        entry.subject_name.setCharacterSize(18);
        entry.subject_name.setFillColor(sf::Color::White);
        entry.subject_name.setString(grade_record.subject_name);

        entry.grade_value.setFont(font);
        entry.grade_value.setCharacterSize(18);
        entry.grade_value.setFillColor(sf::Color::White);
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << grade_record.grade_value;
        entry.grade_value.setString(ss.str());

        entry.grade_date.setFont(font);
        entry.grade_date.setCharacterSize(18);
        entry.grade_date.setFillColor(sf::Color::White);
        entry.grade_date.setString(grade_record.date);

        grades.push_back(entry);
    }
}

void ViewGradesWindow::handle_scroll(float delta)
{
    if (!class_entered)
        return;

    float max_scroll = std::max(0.f, static_cast<float>(grades.size()) * row_height - visible_rows * row_height);
    scroll_offset -= delta * 20.f;
    scroll_offset = std::max(0.f, std::min(scroll_offset, max_scroll));
}

void ViewGradesWindow::handle_events(sf::RenderWindow &window, WindowType &next_window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseMoved)
        {
            sf::Vector2f mouse_pos(event.mouseMove.x, event.mouseMove.y);
            if (back_button.getGlobalBounds().contains(mouse_pos))
                back_button.setFillColor(sf::Color(130, 130, 180));
            else
                back_button.setFillColor(sf::Color(100, 100, 150));
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            sf::Vector2f mouse_pos(event.mouseButton.x, event.mouseButton.y);
            if (back_button.getGlobalBounds().contains(mouse_pos))
            {
                next_window = WindowType::Menu;
            }
        }

        if (input_active && event.type == sf::Event::TextEntered)
        {
            if (event.text.unicode >= '0' && event.text.unicode <= '9')
            {
                class_input_string.push_back(static_cast<char>(event.text.unicode));
                class_input_text.setString(class_input_string);
            }
            else if (event.text.unicode == BACKSPACE_UNICODE) 
            {
                if (!class_input_string.empty())
                {
                    class_input_string.pop_back();
                    if (class_input_string.empty())
                        class_input_text.setString("Enter Class Number...");
                    else
                        class_input_text.setString(class_input_string);
                }
            }
            else if (event.text.unicode == ENTER_UNICODE)
            {
                if (!class_input_string.empty())
                {
                    input_active = false;
                    class_entered = true;
                    load_grades_for_class(class_input_string);
                }
            }
        }

        if (event.type == sf::Event::MouseWheelScrolled && class_entered)
        {
            handle_scroll(event.mouseWheelScroll.delta);
        }
    }
}

void ViewGradesWindow::update()
{
}

void ViewGradesWindow::render(sf::RenderWindow &window)
{
    window.clear();

    window.draw(back_button);
    window.draw(back_button_text);

    window.draw(class_input_box);
    window.draw(class_input_text);

    if (class_entered)
    {
        float table_start_y = 160.f;
        float visible_area_top = table_start_y;
        float visible_area_bottom = static_cast<float>(window.getSize().y);

        for (std::size_t i = 0; i < grades.size(); ++i)
        {
            float y_pos = table_start_y + i * row_height - scroll_offset;

            if (y_pos + row_height < visible_area_top || y_pos > visible_area_bottom)
                continue;

            grades[i].subject_name.setPosition(30.f, y_pos);
            window.draw(grades[i].subject_name);

            grades[i].grade_value.setPosition(400.f, y_pos);
            window.draw(grades[i].grade_value);

            grades[i].grade_date.setPosition(550.f, y_pos);
            window.draw(grades[i].grade_date);
        }
    }

    window.display();
}
