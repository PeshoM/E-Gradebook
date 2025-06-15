#include "ViewGradesWindow.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include "../../database/models/gradeEntry.hpp"

#define BACKSPACE_UNICODE 8
#define ENTER_UNICODE 13

ViewGradesWindow::ViewGradesWindow(Database *database)
    : db(database), scroll_offset(0.f), input_active(true), class_entered(false),
      show_modal(false), editing_grade_id(-1), show_error(false),
      subject_input_active(false), current_subject_avg(0.0f), current_overall_avg(0.0f)
{
    if (!font.loadFromFile("arial.ttf") &&
        !font.loadFromFile("C:/Windows/Fonts/arial.ttf") &&
        !font.loadFromFile("/System/Library/Fonts/Arial.ttf") &&
        !font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"))
    {
        std::cerr << "Warning: Could not load any system font. Using default rendering.\n";
    }

    back_button.setSize({150, 40});
    back_button.setPosition({20, 20});
    back_button.setFillColor(sf::Color(70, 70, 130));

    back_button_text.setFont(font);
    back_button_text.setString("Go Back");
    back_button_text.setCharacterSize(20);
    back_button_text.setFillColor(sf::Color(220, 220, 255));
    back_button_text.setPosition(40, 28);

    class_input_box.setSize({250, 40});
    class_input_box.setPosition({20, 80});
    class_input_box.setFillColor(sf::Color(90, 90, 160));

    class_input_text.setFont(font);
    class_input_text.setCharacterSize(20);
    class_input_text.setFillColor(sf::Color(220, 220, 255));
    class_input_text.setPosition({25, 85});
    class_input_text.setString("Enter Class Number...");

    subject_input_box.setSize({250, 40});
    subject_input_box.setPosition({290, 80});
    subject_input_box.setFillColor(sf::Color(90, 90, 160));

    subject_input_text.setFont(font);
    subject_input_text.setCharacterSize(20);
    subject_input_text.setFillColor(sf::Color(220, 220, 255));
    subject_input_text.setPosition({295, 85});
    subject_input_text.setString("Enter Subject...");

    subject_avg_text.setFont(font);
    subject_avg_text.setCharacterSize(18);
    subject_avg_text.setFillColor(sf::Color(220, 220, 255));
    subject_avg_text.setPosition({20, 140});
    subject_avg_text.setString("Subject Avg: N/A");

    overall_avg_text.setFont(font);
    overall_avg_text.setCharacterSize(18);
    overall_avg_text.setFillColor(sf::Color(220, 220, 255));
    overall_avg_text.setPosition({290, 140});
    overall_avg_text.setString("Overall Avg: N/A");

    no_grades_text.setFont(font);
    no_grades_text.setCharacterSize(20);
    no_grades_text.setFillColor(sf::Color(220, 220, 255));
    no_grades_text.setString("No grades found for this student.");
    no_grades_text.setPosition({20, 180});

    modal_background.setFillColor(sf::Color(0, 0, 0, 180));
    modal_window.setFillColor(sf::Color(70, 70, 130));
    modal_window.setSize({400, 200});
    modal_window.setPosition({200, 150});

    input_box_grade.setSize({200, 30});
    input_box_grade.setFillColor(sf::Color(90, 90, 160));

    input_text_grade.setFont(font);
    input_text_grade.setCharacterSize(18);
    input_text_grade.setFillColor(sf::Color(220, 220, 255));

    error_text.setFont(font);
    error_text.setString("Invalid grade value!");
    error_text.setCharacterSize(16);
    error_text.setFillColor(sf::Color(255, 100, 100));

    save_button.setSize({100, 30});
    cancel_button.setSize({100, 30});
    save_button.setFillColor(sf::Color(90, 160, 90));
    cancel_button.setFillColor(sf::Color(160, 90, 90));

    save_text.setFont(font);
    cancel_text.setFont(font);
    save_text.setString("Save");
    cancel_text.setString("Cancel");
    save_text.setCharacterSize(18);
    cancel_text.setCharacterSize(18);
    save_text.setFillColor(sf::Color(220, 255, 220));
    cancel_text.setFillColor(sf::Color(255, 220, 220));

    class_input_string.clear();
    subject_input_string.clear();
}

void ViewGradesWindow::create_row_buttons()
{
    row_buttons.clear();
    for (size_t i = 0; i < grades.size(); ++i)
    {
        RowButtons buttons;

        buttons.edit_button.setSize({button_width, button_height});
        buttons.edit_button.setFillColor(sf::Color(100, 150, 100));

        buttons.edit_text.setFont(font);
        buttons.edit_text.setString("Edit");
        buttons.edit_text.setCharacterSize(14);
        buttons.edit_text.setFillColor(sf::Color::White);

        buttons.delete_button.setSize({button_width, button_height});
        buttons.delete_button.setFillColor(sf::Color(150, 100, 100));

        buttons.delete_text.setFont(font);
        buttons.delete_text.setString("Delete");
        buttons.delete_text.setCharacterSize(14);
        buttons.delete_text.setFillColor(sf::Color::White);

        row_buttons.push_back(buttons);
    }
}

void ViewGradesWindow::sort_grades()
{
    std::sort(grades.begin(), grades.end(),
              [](const GradeEntry &a, const GradeEntry &b)
              {
                  float grade_a = std::stof(a.grade_value.getString().toAnsiString());
                  float grade_b = std::stof(b.grade_value.getString().toAnsiString());
                  return grade_a > grade_b;
              });

    create_row_buttons();
}

void ViewGradesWindow::load_grades_for_class(const std::string &class_number)
{
    grades.clear();

    auto student_grades = db->get_grades_by_class_number(std::stoi(class_number));

    for (const auto &grade_record : student_grades)
    {
        GradeEntry entry;
        entry.id = grade_record.id;

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

    sort_grades();
}

void ViewGradesWindow::show_edit_modal(int grade_id)
{
    editing_grade_id = grade_id;
    show_modal = true;
    input_active = true;
    show_error = false;
    subject_input_active = false;

    auto it = std::find_if(grades.begin(), grades.end(),
                           [grade_id](const GradeEntry &g)
                           { return g.id == grade_id; });
    if (it != grades.end())
    {
        input_string_grade = it->grade_value.getString();
        input_text_grade.setString(input_string_grade);
    }
    else
    {
        input_string_grade.clear();
        input_text_grade.setString("");
    }
}

void ViewGradesWindow::save_grade_changes()
{
    if (editing_grade_id == -1)
        return;

    try
    {
        float grade_value = std::stof(input_string_grade);
        if (grade_value < 2.0f || grade_value > 6.0f)
        {
            show_error = true;
            return;
        }

        if (db->update_grade(editing_grade_id, grade_value))
        {
            show_modal = false;
            input_active = false;
            editing_grade_id = -1;
            show_error = false;
            load_grades_for_class(class_input_string);
        }
    }
    catch (...)
    {
        show_error = true;
    }
}

void ViewGradesWindow::delete_grade(int grade_id)
{
    if (db->delete_grade(grade_id))
    {
        load_grades_for_class(class_input_string);
    }
}

void ViewGradesWindow::update_averages()
{
    if (!class_input_string.empty())
    {
        if (!grades.empty())
        {
            current_overall_avg = db->get_overall_average(std::stoi(class_input_string));

            if (!subject_input_string.empty())
            {
                current_subject_avg = db->get_subject_average(std::stoi(class_input_string), subject_input_string);
            }
            else
            {
                current_subject_avg = 0.0f;
            }

            std::ostringstream ss;
            ss << std::fixed << std::setprecision(2);

            ss.str("");
            ss << "Subject Avg: " << current_subject_avg;
            subject_avg_text.setString(ss.str());

            ss.str("");
            ss << "Overall Avg: " << current_overall_avg;
            overall_avg_text.setString(ss.str());
        }
        else
        {
            subject_avg_text.setString("Subject Avg: N/A");
            overall_avg_text.setString("Overall Avg: N/A");
        }
    }
}

void ViewGradesWindow::handle_modal_events(sf::RenderWindow &window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
            return;
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            sf::Vector2f mouse_pos(event.mouseButton.x, event.mouseButton.y);

            if (save_button.getGlobalBounds().contains(mouse_pos))
            {
                save_grade_changes();
                return;
            }
            else if (cancel_button.getGlobalBounds().contains(mouse_pos))
            {
                show_modal = false;
                input_active = false;
                editing_grade_id = -1;
                show_error = false;
                input_string_grade.clear();
                return;
            }
            else if (input_box_grade.getGlobalBounds().contains(mouse_pos))
            {
                input_active = true;
            }
            else if (!modal_window.getGlobalBounds().contains(mouse_pos))
            {
                // Only deactivate input if clicking outside the modal window
                input_active = false;
            }
        }

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                show_modal = false;
                input_active = false;
                editing_grade_id = -1;
                show_error = false;
                input_string_grade.clear();
                return;
            }
            else if (event.key.code == sf::Keyboard::Enter)
            {
                save_grade_changes();
                return;
            }
        }

        if (event.type == sf::Event::TextEntered)
        {
            if (event.text.unicode == BACKSPACE_UNICODE)
            {
                if (!input_string_grade.empty())
                {
                    input_string_grade.pop_back();
                    input_text_grade.setString(input_string_grade);
                    show_error = false;
                }
            }
            else if (event.text.unicode == '.' || (event.text.unicode >= '0' && event.text.unicode <= '9'))
            {
                input_string_grade.push_back(static_cast<char>(event.text.unicode));
                input_text_grade.setString(input_string_grade);
                show_error = false;
            }
        }
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
    if (show_modal)
    {
        handle_modal_events(window);
        return;
    }

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
                back_button.setFillColor(sf::Color(70, 70, 130));
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            sf::Vector2f mouse_pos(event.mouseButton.x, event.mouseButton.y);
            if (back_button.getGlobalBounds().contains(mouse_pos))
            {
                next_window = WindowType::Menu;
            }
            else if (class_input_box.getGlobalBounds().contains(mouse_pos))
            {
                input_active = true;
                subject_input_active = false;
                // Allow re-entering class number by resetting class_entered
                if (class_entered)
                {
                    class_entered = false;
                    grades.clear();
                    scroll_offset = 0.f;
                    subject_avg_text.setString("Subject Avg: N/A");
                    overall_avg_text.setString("Overall Avg: N/A");
                }
            }
            else if (subject_input_box.getGlobalBounds().contains(mouse_pos))
            {
                input_active = false;
                subject_input_active = true;
            }
            else
            {
                // Deactivate input boxes if clicking elsewhere
                input_active = false;
                subject_input_active = false;

                // Check row buttons only if class is entered
                if (class_entered)
                {
                    float table_start_y = 180.f;
                    for (size_t i = 0; i < row_buttons.size(); ++i)
                    {
                        float y_pos = table_start_y + (i + 1) * row_height - scroll_offset;
                        if (y_pos + row_height < table_start_y || y_pos > window.getSize().y)
                            continue;

                        // Set button positions to match render positions
                        row_buttons[i].edit_button.setPosition(650.f, y_pos + 7.5f);
                        row_buttons[i].delete_button.setPosition(720.f, y_pos + 7.5f);

                        if (row_buttons[i].edit_button.getGlobalBounds().contains(mouse_pos))
                        {
                            show_edit_modal(grades[i].id);
                            break;
                        }
                        else if (row_buttons[i].delete_button.getGlobalBounds().contains(mouse_pos))
                        {
                            delete_grade(grades[i].id);
                            break;
                        }
                    }
                }
            }
        }

        if (event.type == sf::Event::TextEntered)
        {
            if (input_active)
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
                        update_averages();
                    }
                }
            }
            else if (subject_input_active)
            {
                if (event.text.unicode == BACKSPACE_UNICODE)
                {
                    if (!subject_input_string.empty())
                    {
                        subject_input_string.pop_back();
                        if (subject_input_string.empty())
                            subject_input_text.setString("Enter Subject...");
                        else
                            subject_input_text.setString(subject_input_string);
                    }
                }
                else if (event.text.unicode == ENTER_UNICODE)
                {
                    subject_input_active = false;
                    update_averages();
                }
                else if (event.text.unicode >= 32 && event.text.unicode <= 126)
                {
                    subject_input_string.push_back(static_cast<char>(event.text.unicode));
                    subject_input_text.setString(subject_input_string);
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

void ViewGradesWindow::render_modal(sf::RenderWindow &window)
{
    modal_background.setSize(sf::Vector2f(window.getSize()));
    window.draw(modal_background);

    window.draw(modal_window);

    input_box_grade.setPosition(modal_window.getPosition() + sf::Vector2f(20, 50));
    window.draw(input_box_grade);

    input_text_grade.setPosition(input_box_grade.getPosition() + sf::Vector2f(5, 5));
    window.draw(input_text_grade);

    if (show_error)
    {
        error_text.setPosition(input_box_grade.getPosition() + sf::Vector2f(0, -25));
        window.draw(error_text);
    }

    save_button.setPosition(modal_window.getPosition() + sf::Vector2f(50, 100));
    cancel_button.setPosition(modal_window.getPosition() + sf::Vector2f(250, 100));
    save_text.setPosition(save_button.getPosition() + sf::Vector2f(35, 5));
    cancel_text.setPosition(cancel_button.getPosition() + sf::Vector2f(25, 5));

    window.draw(save_button);
    window.draw(cancel_button);
    window.draw(save_text);
    window.draw(cancel_text);
}

void ViewGradesWindow::render(sf::RenderWindow &window)
{
    window.clear(sf::Color(40, 40, 80));

    window.draw(back_button);
    window.draw(back_button_text);

    window.draw(class_input_box);
    window.draw(class_input_text);

    window.draw(subject_input_box);
    window.draw(subject_input_text);

    window.draw(subject_avg_text);
    window.draw(overall_avg_text);

    if (class_entered)
    {
        if (grades.empty())
        {
            window.draw(no_grades_text);
        }
        else
        {
            float table_start_y = 180.f;
            float visible_area_top = table_start_y;
            float visible_area_bottom = static_cast<float>(window.getSize().y);

            sf::Text header_subject, header_grade, header_date;

            header_subject.setFont(font);
            header_subject.setCharacterSize(22);
            header_subject.setFillColor(sf::Color::Yellow);
            header_subject.setString("Subject");
            header_subject.setPosition(30.f, table_start_y);

            header_grade.setFont(font);
            header_grade.setCharacterSize(22);
            header_grade.setFillColor(sf::Color::Yellow);
            header_grade.setString("Grade");
            header_grade.setPosition(400.f, table_start_y);

            header_date.setFont(font);
            header_date.setCharacterSize(22);
            header_date.setFillColor(sf::Color::Yellow);
            header_date.setString("Date");
            header_date.setPosition(500.f, table_start_y);

            sf::Text header_actions;
            header_actions.setFont(font);
            header_actions.setCharacterSize(22);
            header_actions.setFillColor(sf::Color::Yellow);
            header_actions.setString("Actions");
            header_actions.setPosition(650.f, table_start_y);

            window.draw(header_subject);
            window.draw(header_grade);
            window.draw(header_date);
            window.draw(header_actions);

            for (std::size_t i = 0; i < grades.size(); ++i)
            {
                float y_pos = table_start_y + (i + 1) * row_height - scroll_offset;

                if (y_pos + row_height < visible_area_top || y_pos > visible_area_bottom)
                    continue;

                sf::RectangleShape row_bg;
                row_bg.setSize({window.getSize().x - 40.f, row_height});
                row_bg.setPosition(20.f, y_pos);
                row_bg.setFillColor(i % 2 == 0 ? sf::Color(70, 70, 110) : sf::Color(55, 55, 90));
                window.draw(row_bg);

                grades[i].subject_name.setPosition(30.f, y_pos + 10.f);
                grades[i].grade_value.setPosition(400.f, y_pos + 10.f);
                grades[i].grade_date.setPosition(500.f, y_pos + 10.f);

                window.draw(grades[i].subject_name);
                window.draw(grades[i].grade_value);
                window.draw(grades[i].grade_date);

                // Set button positions consistently
                row_buttons[i].edit_button.setPosition(650.f, y_pos + 7.5f);
                row_buttons[i].delete_button.setPosition(720.f, y_pos + 7.5f);
                row_buttons[i].edit_text.setPosition(660.f, y_pos + 10.f);
                row_buttons[i].delete_text.setPosition(730.f, y_pos + 10.f);

                window.draw(row_buttons[i].edit_button);
                window.draw(row_buttons[i].delete_button);
                window.draw(row_buttons[i].edit_text);
                window.draw(row_buttons[i].delete_text);
            }
        }
    }

    if (show_modal)
    {
        render_modal(window);
    }

    window.display();
}
