#include "ViewStudentsWindow.hpp"
#include <iostream>

#define BACKSPACE_UNICODE 8

ViewStudentsWindow::ViewStudentsWindow(Database *database)
    : db(database), scroll_offset(0.f), show_modal(false), input_active(false), editing_student_id(-1), show_error(false)
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

    modal_background.setFillColor(sf::Color(0, 0, 0, 128));
    modal_window.setFillColor(sf::Color(60, 60, 100));
    modal_window.setSize({400, 300});
    modal_window.setPosition({200, 150});

    input_box_number.setSize({200, 30});
    input_box_name.setSize({200, 30});
    input_box_date.setSize({200, 30});
    input_box_number.setFillColor(sf::Color(80, 80, 80));
    input_box_name.setFillColor(sf::Color(80, 80, 80));
    input_box_date.setFillColor(sf::Color(80, 80, 80));

    input_text_number.setFont(font);
    input_text_name.setFont(font);
    input_text_date.setFont(font);
    input_text_number.setCharacterSize(18);
    input_text_name.setCharacterSize(18);
    input_text_date.setCharacterSize(18);
    input_text_number.setFillColor(sf::Color::White);
    input_text_name.setFillColor(sf::Color::White);
    input_text_date.setFillColor(sf::Color::White);

    save_button.setSize({100, 30});
    cancel_button.setSize({100, 30});
    save_button.setFillColor(sf::Color(100, 150, 100));
    cancel_button.setFillColor(sf::Color(150, 100, 100));

    save_text.setFont(font);
    cancel_text.setFont(font);
    save_text.setString("Save");
    cancel_text.setString("Cancel");
    save_text.setCharacterSize(18);
    cancel_text.setCharacterSize(18);
    save_text.setFillColor(sf::Color::White);
    cancel_text.setFillColor(sf::Color::White);

    error_text.setFont(font);
    error_text.setString("Class number already exists!");
    error_text.setCharacterSize(16);
    error_text.setFillColor(sf::Color::Red);

    load_students();
}

void ViewStudentsWindow::create_row_buttons()
{
    row_buttons.clear();
    for (size_t i = 0; i < students.size(); ++i)
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

void ViewStudentsWindow::sort_students()
{
    std::sort(students.begin(), students.end(),
              [](const Student &a, const Student &b)
              { return a.number_in_class < b.number_in_class; });

    student_numbers.clear();
    student_names.clear();
    student_dates.clear();

    for (const auto &student : students)
    {
        sf::Text number_text;
        number_text.setFont(font);
        number_text.setCharacterSize(18);
        number_text.setFillColor(sf::Color::White);
        number_text.setString(std::to_string(student.number_in_class));

        sf::Text name_text;
        name_text.setFont(font);
        name_text.setCharacterSize(18);
        name_text.setFillColor(sf::Color::White);
        name_text.setString(student.full_name);

        sf::Text date_text;
        date_text.setFont(font);
        date_text.setCharacterSize(18);
        date_text.setFillColor(sf::Color::White);
        date_text.setString(student.date_of_birth);

        student_numbers.push_back(number_text);
        student_names.push_back(name_text);
        student_dates.push_back(date_text);
    }

    create_row_buttons();
}

void ViewStudentsWindow::load_students()
{
    students = db->get_students();
    sort_students();
}

void ViewStudentsWindow::show_edit_modal(int student_id)
{
    editing_student_id = student_id;
    show_modal = true;
    input_active = true;

    auto it = std::find_if(students.begin(), students.end(),
                           [student_id](const Student &s)
                           { return s.id == student_id; });
    if (it != students.end())
    {
        input_string_number = std::to_string(it->number_in_class);
        input_string_name = it->full_name;
        input_string_date = it->date_of_birth;

        input_text_number.setString(input_string_number);
        input_text_name.setString(input_string_name);
        input_text_date.setString(input_string_date);
    }
}

bool ViewStudentsWindow::is_number_duplicate(int number, int current_id)
{
    for (const auto &student : students)
    {
        if (student.number_in_class == number && student.id != current_id)
        {
            return true;
        }
    }
    return false;
}

void ViewStudentsWindow::save_student_changes()
{
    if (editing_student_id == -1)
        return;

    try
    {
        int new_number = std::stoi(input_string_number);

        if (is_number_duplicate(new_number, editing_student_id))
        {
            show_error = true;
            return;
        }

        if (db->update_student(editing_student_id, new_number, input_string_name, input_string_date))
        {
            show_modal = false;
            input_active = false;
            editing_student_id = -1;
            show_error = false;
            load_students();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to update student: " << e.what() << std::endl;
    }
}

void ViewStudentsWindow::delete_student(int student_id)
{
    if (db->delete_student(student_id))
    {
        load_students();
    }
}

void ViewStudentsWindow::handle_modal_events(sf::RenderWindow &window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::MouseButtonPressed)
        {
            sf::Vector2f mouse_pos(event.mouseButton.x, event.mouseButton.y);

            if (save_button.getGlobalBounds().contains(mouse_pos))
            {
                save_student_changes();
                return;
            }
            else if (cancel_button.getGlobalBounds().contains(mouse_pos))
            {
                show_modal = false;
                input_active = false;
                editing_student_id = -1;
                show_error = false;
                return;
            }
        }

        if (input_active && event.type == sf::Event::TextEntered)
        {
            if (event.text.unicode == BACKSPACE_UNICODE)
            {
                if (!input_string_number.empty())
                {
                    input_string_number.pop_back();
                    input_text_number.setString(input_string_number);
                    show_error = false;
                }
            }
            else if (event.text.unicode >= '0' && event.text.unicode <= '9')
            {
                input_string_number.push_back(static_cast<char>(event.text.unicode));
                input_text_number.setString(input_string_number);
                show_error = false;
            }
        }
    }
}

void ViewStudentsWindow::handle_events(sf::RenderWindow &window, WindowType &next_window)
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
                back_button.setFillColor(sf::Color(100, 100, 150));
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            sf::Vector2f mouse_pos(event.mouseButton.x, event.mouseButton.y);
            if (back_button.getGlobalBounds().contains(mouse_pos))
            {
                next_window = WindowType::Menu;
            }
            else
            {
                for (size_t i = 0; i < row_buttons.size(); ++i)
                {
                    float y_pos = 120.f + i * row_height - scroll_offset;
                    if (y_pos + row_height < 0.f || y_pos > window.getSize().y)
                        continue;

                    row_buttons[i].edit_button.setPosition(600.f, y_pos + 2.5f);
                    row_buttons[i].delete_button.setPosition(670.f, y_pos + 2.5f);
                    row_buttons[i].edit_text.setPosition(610.f, y_pos + 5.f);
                    row_buttons[i].delete_text.setPosition(680.f, y_pos + 5.f);

                    if (row_buttons[i].edit_button.getGlobalBounds().contains(mouse_pos))
                    {
                        show_edit_modal(students[i].id);
                        break;
                    }
                    else if (row_buttons[i].delete_button.getGlobalBounds().contains(mouse_pos))
                    {
                        delete_student(students[i].id);
                        break;
                    }
                }
            }
        }

        if (event.type == sf::Event::MouseWheelScrolled)
        {
            scroll_offset -= event.mouseWheelScroll.delta * 30.f;
            float max_scroll = std::max(0.f, static_cast<float>(students.size()) * row_height - (window.getSize().y - 150.f));
            if (scroll_offset < 0.f)
                scroll_offset = 0.f;
            if (scroll_offset > max_scroll)
                scroll_offset = max_scroll;
        }
    }
}

void ViewStudentsWindow::update() {}

void ViewStudentsWindow::render_modal(sf::RenderWindow &window)
{
    modal_background.setSize(sf::Vector2f(window.getSize()));
    window.draw(modal_background);

    window.draw(modal_window);

    input_box_number.setPosition(modal_window.getPosition() + sf::Vector2f(20, 50));
    input_box_name.setPosition(modal_window.getPosition() + sf::Vector2f(20, 100));
    input_box_date.setPosition(modal_window.getPosition() + sf::Vector2f(20, 150));

    window.draw(input_box_number);
    window.draw(input_box_name);
    window.draw(input_box_date);

    input_text_number.setPosition(input_box_number.getPosition() + sf::Vector2f(5, 5));
    input_text_name.setPosition(input_box_name.getPosition() + sf::Vector2f(5, 5));
    input_text_date.setPosition(input_box_date.getPosition() + sf::Vector2f(5, 5));

    window.draw(input_text_number);
    window.draw(input_text_name);
    window.draw(input_text_date);

    if (show_error)
    {
        error_text.setPosition(input_box_number.getPosition() + sf::Vector2f(0, -25));
        window.draw(error_text);
    }

    save_button.setPosition(modal_window.getPosition() + sf::Vector2f(50, 200));
    cancel_button.setPosition(modal_window.getPosition() + sf::Vector2f(250, 200));
    save_text.setPosition(save_button.getPosition() + sf::Vector2f(35, 5));
    cancel_text.setPosition(cancel_button.getPosition() + sf::Vector2f(25, 5));

    window.draw(save_button);
    window.draw(cancel_button);
    window.draw(save_text);
    window.draw(cancel_text);
}

void ViewStudentsWindow::render(sf::RenderWindow &window)
{
    window.clear(sf::Color(40, 40, 80));

    sf::Vector2f scrollMove(0.f, -scroll_offset);

    sf::RectangleShape back_button_moved = back_button;
    back_button_moved.move(scrollMove);
    window.draw(back_button_moved);

    sf::Text back_button_text_moved = back_button_text;
    back_button_text_moved.move(scrollMove);
    window.draw(back_button_text_moved);

    float y_start = 120.f - scroll_offset;
    sf::RectangleShape row_bg;
    row_bg.setSize({window.getSize().x - 40.f, row_height});

    for (size_t i = 0; i < students.size(); ++i)
    {
        float y_pos = y_start + i * row_height;

        if (y_pos + row_height < 0.f || y_pos > window.getSize().y)
            continue;

        if (i % 2 == 0)
            row_bg.setFillColor(sf::Color(70, 70, 110));
        else
            row_bg.setFillColor(sf::Color(55, 55, 90));

        row_bg.setPosition(20.f, y_pos);
        window.draw(row_bg);

        student_numbers[i].setPosition(30.f, y_pos + 5.f);
        student_names[i].setPosition(80.f, y_pos + 5.f);
        student_dates[i].setPosition(400.f, y_pos + 5.f);

        window.draw(student_numbers[i]);
        window.draw(student_names[i]);
        window.draw(student_dates[i]);

        row_buttons[i].edit_button.setPosition(600.f, y_pos + 2.5f);
        row_buttons[i].delete_button.setPosition(670.f, y_pos + 2.5f);
        row_buttons[i].edit_text.setPosition(610.f, y_pos + 5.f);
        row_buttons[i].delete_text.setPosition(680.f, y_pos + 5.f);

        window.draw(row_buttons[i].edit_button);
        window.draw(row_buttons[i].delete_button);
        window.draw(row_buttons[i].edit_text);
        window.draw(row_buttons[i].delete_text);
    }

    sf::Text header_no, header_name, header_date;

    header_no.setFont(font);
    header_no.setCharacterSize(22);
    header_no.setFillColor(sf::Color::Yellow);
    header_no.setString("No.");
    header_no.setPosition(20.f, 80.f);
    header_no.move(scrollMove);

    header_name.setFont(font);
    header_name.setCharacterSize(22);
    header_name.setFillColor(sf::Color::Yellow);
    header_name.setString("Full Name");
    header_name.setPosition(80.f, 80.f);
    header_name.move(scrollMove);

    header_date.setFont(font);
    header_date.setCharacterSize(22);
    header_date.setFillColor(sf::Color::Yellow);
    header_date.setString("Date of Birth");
    header_date.setPosition(400.f, 80.f);
    header_date.move(scrollMove);

    window.draw(header_no);
    window.draw(header_name);
    window.draw(header_date);

    if (show_modal)
    {
        render_modal(window);
    }

    window.display();
}
