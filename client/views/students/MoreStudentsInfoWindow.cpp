#include "MoreStudentsInfoWindow.hpp"
#include <iostream>

MoreStudentsInfoWindow::MoreStudentsInfoWindow(Database *database)
    : db(database), close_window(false), scroll_offset(0.f), showing_list(false)
{
    if (!font.loadFromFile("arial.ttf") &&
        !font.loadFromFile("C:/Windows/Fonts/arial.ttf") &&
        !font.loadFromFile("/System/Library/Fonts/Arial.ttf") &&
        !font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"))
    {
        std::cerr << "Warning: Could not load any system font. Using default rendering.\n";
    }

    setup_buttons();
    setup_text();
}

void MoreStudentsInfoWindow::setup_buttons()
{
    back_button.setSize({150, 40});
    back_button.setPosition({20, 20});
    back_button.setFillColor(sf::Color(70, 70, 130));

    back_button_text.setFont(font);
    back_button_text.setString("Go Back");
    back_button_text.setCharacterSize(20);
    back_button_text.setFillColor(sf::Color(220, 220, 255));
    back_button_text.setPosition(40, 28);

    float button_width = 400;
    float button_height = 50;
    float start_x = 50;
    float start_y = 100;
    float spacing = 20;

    top_students_button.setSize({button_width, button_height});
    top_students_button.setPosition({start_x, start_y});
    top_students_button.setFillColor(sf::Color(70, 70, 130));

    remedial_exam_button.setSize({button_width, button_height});
    remedial_exam_button.setPosition({start_x, start_y + button_height + spacing});
    remedial_exam_button.setFillColor(sf::Color(70, 70, 130));

    failing_students_button.setSize({button_width, button_height});
    failing_students_button.setPosition({start_x, start_y + 2 * (button_height + spacing)});
    failing_students_button.setFillColor(sf::Color(70, 70, 130));

    birthday_students_button.setSize({button_width, button_height});
    birthday_students_button.setPosition({start_x, start_y + 3 * (button_height + spacing)});
    birthday_students_button.setFillColor(sf::Color(70, 70, 130));
}

void MoreStudentsInfoWindow::setup_text()
{
    title_text.setFont(font);
    title_text.setString("More Student Info");
    title_text.setCharacterSize(24);
    title_text.setFillColor(sf::Color(220, 220, 255));
    title_text.setPosition({350, 40});

    top_students_text.setFont(font);
    top_students_text.setString("Top Students");
    top_students_text.setCharacterSize(20);
    top_students_text.setFillColor(sf::Color(220, 220, 255));
    top_students_text.setPosition({70, 115});

    remedial_exam_text.setFont(font);
    remedial_exam_text.setString("Remedial Exam Students");
    remedial_exam_text.setCharacterSize(20);
    remedial_exam_text.setFillColor(sf::Color(220, 220, 255));
    remedial_exam_text.setPosition({70, 185});

    failing_students_text.setFont(font);
    failing_students_text.setString("Failing Students (3+ subjects)");
    failing_students_text.setCharacterSize(20);
    failing_students_text.setFillColor(sf::Color(220, 220, 255));
    failing_students_text.setPosition({70, 255});

    birthday_students_text.setFont(font);
    birthday_students_text.setString("Today's Birthdays");
    birthday_students_text.setCharacterSize(20);
    birthday_students_text.setFillColor(sf::Color(220, 220, 255));
    birthday_students_text.setPosition({70, 325});

    no_students_text.setFont(font);
    no_students_text.setString("No students found");
    no_students_text.setCharacterSize(20);
    no_students_text.setFillColor(sf::Color(220, 220, 255));
    no_students_text.setPosition({350, 200});
}

void MoreStudentsInfoWindow::render_student_list(const std::vector<Student> &students, sf::RenderWindow &window)
{
    if (students.empty())
    {
        window.draw(no_students_text);
        return;
    }

    float start_y = 100.f;
    float row_height = 40.f;
    float visible_area_top = start_y;
    float visible_area_bottom = static_cast<float>(window.getSize().y);

    sf::Text header_number, header_name, header_birthday;

    header_number.setFont(font);
    header_number.setCharacterSize(20);
    header_number.setFillColor(sf::Color::Yellow);
    header_number.setString("Class Number");
    header_number.setPosition(50.f, start_y);

    header_name.setFont(font);
    header_name.setCharacterSize(20);
    header_name.setFillColor(sf::Color::Yellow);
    header_name.setString("Name");
    header_name.setPosition(200.f, start_y);

    header_birthday.setFont(font);
    header_birthday.setCharacterSize(20);
    header_birthday.setFillColor(sf::Color::Yellow);
    header_birthday.setString("Birthday");
    header_birthday.setPosition(500.f, start_y);

    window.draw(header_number);
    window.draw(header_name);
    window.draw(header_birthday);

    for (std::size_t i = 0; i < students.size(); ++i)
    {
        float y_pos = start_y + (i + 1) * row_height - scroll_offset;

        if (y_pos + row_height < visible_area_top || y_pos > visible_area_bottom)
            continue;

        sf::RectangleShape row_bg;
        row_bg.setSize({window.getSize().x - 40.f, row_height});
        row_bg.setPosition(20.f, y_pos);
        row_bg.setFillColor(i % 2 == 0 ? sf::Color(70, 70, 110) : sf::Color(55, 55, 90));
        window.draw(row_bg);

        sf::Text number_text, name_text, birthday_text;

        number_text.setFont(font);
        number_text.setCharacterSize(18);
        number_text.setFillColor(sf::Color::White);
        number_text.setString(std::to_string(students[i].number_in_class));
        number_text.setPosition(50.f, y_pos + 10.f);

        name_text.setFont(font);
        name_text.setCharacterSize(18);
        name_text.setFillColor(sf::Color::White);
        name_text.setString(students[i].full_name);
        name_text.setPosition(200.f, y_pos + 10.f);

        birthday_text.setFont(font);
        birthday_text.setCharacterSize(18);
        birthday_text.setFillColor(sf::Color::White);
        birthday_text.setString(students[i].date_of_birth);
        birthday_text.setPosition(500.f, y_pos + 10.f);

        window.draw(number_text);
        window.draw(name_text);
        window.draw(birthday_text);
    }
}

void MoreStudentsInfoWindow::render_remedial_list(const std::vector<RemedialExamStudent> &students, sf::RenderWindow &window)
{
    if (students.empty())
    {
        window.draw(no_students_text);
        return;
    }

    float start_y = 100.f;
    float row_height = 40.f;
    float visible_area_top = start_y;
    float visible_area_bottom = static_cast<float>(window.getSize().y);

    sf::Text header_number, header_name, header_subject, header_grade;

    header_number.setFont(font);
    header_number.setCharacterSize(20);
    header_number.setFillColor(sf::Color::Yellow);
    header_number.setString("Class Number");
    header_number.setPosition(50.f, start_y);

    header_name.setFont(font);
    header_name.setCharacterSize(20);
    header_name.setFillColor(sf::Color::Yellow);
    header_name.setString("Name");
    header_name.setPosition(200.f, start_y);

    header_subject.setFont(font);
    header_subject.setCharacterSize(20);
    header_subject.setFillColor(sf::Color::Yellow);
    header_subject.setString("Subject");
    header_subject.setPosition(400.f, start_y);

    header_grade.setFont(font);
    header_grade.setCharacterSize(20);
    header_grade.setFillColor(sf::Color::Yellow);
    header_grade.setString("Average");
    header_grade.setPosition(600.f, start_y);

    window.draw(header_number);
    window.draw(header_name);
    window.draw(header_subject);
    window.draw(header_grade);

    for (std::size_t i = 0; i < students.size(); ++i)
    {
        float y_pos = start_y + (i + 1) * row_height - scroll_offset;

        if (y_pos + row_height < visible_area_top || y_pos > visible_area_bottom)
            continue;

        sf::RectangleShape row_bg;
        row_bg.setSize({window.getSize().x - 40.f, row_height});
        row_bg.setPosition(20.f, y_pos);
        row_bg.setFillColor(i % 2 == 0 ? sf::Color(70, 70, 110) : sf::Color(55, 55, 90));
        window.draw(row_bg);

        sf::Text number_text, name_text, subject_text, grade_text;

        number_text.setFont(font);
        number_text.setCharacterSize(18);
        number_text.setFillColor(sf::Color::White);
        number_text.setString(std::to_string(students[i].student.number_in_class));
        number_text.setPosition(50.f, y_pos + 10.f);

        name_text.setFont(font);
        name_text.setCharacterSize(18);
        name_text.setFillColor(sf::Color::White);
        name_text.setString(students[i].student.full_name);
        name_text.setPosition(200.f, y_pos + 10.f);

        subject_text.setFont(font);
        subject_text.setCharacterSize(18);
        subject_text.setFillColor(sf::Color::White);
        subject_text.setString(students[i].subject_name);
        subject_text.setPosition(400.f, y_pos + 10.f);

        grade_text.setFont(font);
        grade_text.setCharacterSize(18);
        grade_text.setFillColor(sf::Color::White);
        char grade_str[10];
        snprintf(grade_str, sizeof(grade_str), "%.2f", students[i].average_grade);
        grade_text.setString(grade_str);
        grade_text.setPosition(600.f, y_pos + 10.f);

        window.draw(number_text);
        window.draw(name_text);
        window.draw(subject_text);
        window.draw(grade_text);
    }
}

void MoreStudentsInfoWindow::render(sf::RenderWindow &window)
{
    window.clear(sf::Color(40, 40, 80));

    window.draw(back_button);
    window.draw(back_button_text);
    window.draw(title_text);

    if (!showing_list)
    {
        window.draw(top_students_button);
        window.draw(remedial_exam_button);
        window.draw(failing_students_button);
        window.draw(birthday_students_button);

        window.draw(top_students_text);
        window.draw(remedial_exam_text);
        window.draw(failing_students_text);
        window.draw(birthday_students_text);
    }
    else
    {
        if (current_list_type == "Remedial Exam Students")
        {
            render_remedial_list(std::get<std::vector<RemedialExamStudent>>(current_students), window);
        }
        else
        {
            render_student_list(std::get<std::vector<Student>>(current_students), window);
        }
    }

    window.display();
}

void MoreStudentsInfoWindow::handle_events(sf::RenderWindow &window, WindowType &next_window)
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
            sf::Vector2f mouse_pos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

            if (back_button.getGlobalBounds().contains(mouse_pos))
            {
                if (showing_list)
                {
                    showing_list = false;
                    title_text.setString("More Student Info");
                }
                else
                {
                    close_window = true;
                    next_window = WindowType::Menu;
                }
                return;
            }

            if (!showing_list)
            {
                if (top_students_button.getGlobalBounds().contains(mouse_pos))
                {
                    current_students = db->get_top_students();
                    current_list_type = "Top Students";
                    title_text.setString("Top Students (>5.50)");
                    showing_list = true;
                }
                else if (remedial_exam_button.getGlobalBounds().contains(mouse_pos))
                {
                    current_students = db->get_remedial_exam_students();
                    current_list_type = "Remedial Exam Students";
                    title_text.setString("Remedial Exam Students");
                    showing_list = true;
                }
                else if (failing_students_button.getGlobalBounds().contains(mouse_pos))
                {
                    current_students = db->get_failing_students();
                    current_list_type = "Failing Students";
                    title_text.setString("Failing Students (3+ subjects)");
                    showing_list = true;
                }
                else if (birthday_students_button.getGlobalBounds().contains(mouse_pos))
                {
                    current_students = db->get_birthday_students();
                    current_list_type = "Today's Birthdays";
                    title_text.setString("Today's Birthdays");
                    showing_list = true;
                }
            }
        }
        else if (event.type == sf::Event::MouseWheelScrolled && showing_list)
        {
            scroll_offset += event.mouseWheelScroll.delta * 20.f;
            if (scroll_offset < 0.f)
                scroll_offset = 0.f;
        }
    }
}

void MoreStudentsInfoWindow::update()
{
    // No updates needed for this window
}
