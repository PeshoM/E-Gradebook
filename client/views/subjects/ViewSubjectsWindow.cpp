#include "ViewSubjectsWindow.hpp"
#include <iostream>

#define BACKSPACE_UNICODE 8

ViewSubjectsWindow::ViewSubjectsWindow(Database *database)
    : db(database), scroll_offset(0.f), show_modal(false), input_active(false), editing_subject_id(-1), show_error(false)
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

    modal_background.setFillColor(sf::Color(0, 0, 0, 180));
    modal_window.setFillColor(sf::Color(70, 70, 130));
    modal_window.setSize({400, 300});
    modal_window.setPosition({200, 150});

    input_box_name.setSize({200, 30});
    input_box_teacher.setSize({200, 30});
    input_box_room.setSize({200, 30});
    input_box_name.setFillColor(sf::Color(90, 90, 160));
    input_box_teacher.setFillColor(sf::Color(90, 90, 160));
    input_box_room.setFillColor(sf::Color(90, 90, 160));

    input_text_name.setFont(font);
    input_text_teacher.setFont(font);
    input_text_room.setFont(font);
    input_text_name.setCharacterSize(18);
    input_text_teacher.setCharacterSize(18);
    input_text_room.setCharacterSize(18);
    input_text_name.setFillColor(sf::Color(220, 220, 255));
    input_text_teacher.setFillColor(sf::Color(220, 220, 255));
    input_text_room.setFillColor(sf::Color(220, 220, 255));

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

    error_text.setFont(font);
    error_text.setString("Subject name already exists!");
    error_text.setCharacterSize(16);
    error_text.setFillColor(sf::Color(255, 100, 100));

    load_subjects();
}

void ViewSubjectsWindow::create_row_buttons()
{
    row_buttons.clear();
    for (size_t i = 0; i < subjects.size(); ++i)
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

void ViewSubjectsWindow::sort_subjects()
{
    std::sort(subjects.begin(), subjects.end(),
              [](const Subject &a, const Subject &b)
              {
                  std::string a_lower = a.name;
                  std::string b_lower = b.name;
                  std::transform(a_lower.begin(), a_lower.end(), a_lower.begin(), ::tolower);
                  std::transform(b_lower.begin(), b_lower.end(), b_lower.begin(), ::tolower);
                  return a_lower < b_lower;
              });

    subject_names.clear();
    subject_teachers.clear();
    subject_rooms.clear();

    for (const auto &subject : subjects)
    {
        sf::Text name_text;
        name_text.setFont(font);
        name_text.setCharacterSize(18);
        name_text.setFillColor(sf::Color::White);
        name_text.setString(subject.name);

        sf::Text teacher_text;
        teacher_text.setFont(font);
        teacher_text.setCharacterSize(18);
        teacher_text.setFillColor(sf::Color::White);
        teacher_text.setString(subject.teacher);

        sf::Text room_text;
        room_text.setFont(font);
        room_text.setCharacterSize(18);
        room_text.setFillColor(sf::Color::White);
        room_text.setString(subject.room_number);

        subject_names.push_back(name_text);
        subject_teachers.push_back(teacher_text);
        subject_rooms.push_back(room_text);
    }

    create_row_buttons();
}

void ViewSubjectsWindow::load_subjects()
{
    subjects = db->get_subjects();
    sort_subjects();
}

void ViewSubjectsWindow::show_edit_modal(int subject_id)
{
    editing_subject_id = subject_id;
    show_modal = true;
    input_active = true;

    auto it = std::find_if(subjects.begin(), subjects.end(),
                           [subject_id](const Subject &s)
                           { return s.id == subject_id; });
    if (it != subjects.end())
    {
        input_string_name = it->name;
        input_string_teacher = it->teacher;
        input_string_room = it->room_number;

        input_text_name.setString(input_string_name);
        input_text_teacher.setString(input_string_teacher);
        input_text_room.setString(input_string_room);
    }
}

bool ViewSubjectsWindow::is_name_duplicate(const std::string &name, int current_id)
{
    for (const auto &subject : subjects)
    {
        if (subject.name == name && subject.id != current_id)
        {
            return true;
        }
    }
    return false;
}

void ViewSubjectsWindow::save_subject_changes()
{
    if (editing_subject_id == -1)
        return;

    if (is_name_duplicate(input_string_name, editing_subject_id))
    {
        show_error = true;
        return;
    }

    if (db->update_subject(editing_subject_id, input_string_name, input_string_teacher, input_string_room))
    {
        show_modal = false;
        input_active = false;
        editing_subject_id = -1;
        show_error = false;
        load_subjects();
    }
}

void ViewSubjectsWindow::delete_subject(int subject_id)
{
    if (db->delete_subject(subject_id))
    {
        load_subjects();
    }
}

void ViewSubjectsWindow::handle_modal_events(sf::RenderWindow &window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::MouseButtonPressed)
        {
            sf::Vector2f mouse_pos(event.mouseButton.x, event.mouseButton.y);

            if (save_button.getGlobalBounds().contains(mouse_pos))
            {
                save_subject_changes();
                return;
            }
            else if (cancel_button.getGlobalBounds().contains(mouse_pos))
            {
                show_modal = false;
                input_active = false;
                editing_subject_id = -1;
                show_error = false;
                return;
            }
            else if (input_box_name.getGlobalBounds().contains(mouse_pos))
            {
                input_active = true;
                active_input = InputField::Name;
            }
            else if (input_box_teacher.getGlobalBounds().contains(mouse_pos))
            {
                input_active = true;
                active_input = InputField::Teacher;
            }
            else if (input_box_room.getGlobalBounds().contains(mouse_pos))
            {
                input_active = true;
                active_input = InputField::Room;
            }
            else
            {
                input_active = false;
            }
        }

        if (input_active && event.type == sf::Event::TextEntered)
        {
            if (event.text.unicode == BACKSPACE_UNICODE)
            {
                switch (active_input)
                {
                case InputField::Name:
                    if (!input_string_name.empty())
                    {
                        input_string_name.pop_back();
                        input_text_name.setString(input_string_name);
                        show_error = false;
                    }
                    break;
                case InputField::Teacher:
                    if (!input_string_teacher.empty())
                    {
                        input_string_teacher.pop_back();
                        input_text_teacher.setString(input_string_teacher);
                    }
                    break;
                case InputField::Room:
                    if (!input_string_room.empty())
                    {
                        input_string_room.pop_back();
                        input_text_room.setString(input_string_room);
                    }
                    break;
                }
            }
            else if (event.text.unicode < 128)
            {
                switch (active_input)
                {
                case InputField::Name:
                    input_string_name.push_back(static_cast<char>(event.text.unicode));
                    input_text_name.setString(input_string_name);
                    show_error = false;
                    break;
                case InputField::Teacher:
                    input_string_teacher.push_back(static_cast<char>(event.text.unicode));
                    input_text_teacher.setString(input_string_teacher);
                    break;
                case InputField::Room:
                    input_string_room.push_back(static_cast<char>(event.text.unicode));
                    input_text_room.setString(input_string_room);
                    break;
                }
            }
        }
    }
}

void ViewSubjectsWindow::handle_events(sf::RenderWindow &window, WindowType &next_window)
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
                        show_edit_modal(subjects[i].id);
                        break;
                    }
                    else if (row_buttons[i].delete_button.getGlobalBounds().contains(mouse_pos))
                    {
                        delete_subject(subjects[i].id);
                        break;
                    }
                }
            }
        }

        if (event.type == sf::Event::MouseWheelScrolled)
        {
            scroll_offset -= event.mouseWheelScroll.delta * 30.f;
            float max_scroll = std::max(0.f, static_cast<float>(subjects.size()) * row_height - (window.getSize().y - 150.f));
            if (scroll_offset < 0.f)
                scroll_offset = 0.f;
            if (scroll_offset > max_scroll)
                scroll_offset = max_scroll;
        }
    }
}

void ViewSubjectsWindow::update() {}

void ViewSubjectsWindow::render_modal(sf::RenderWindow &window)
{
    modal_background.setSize(sf::Vector2f(window.getSize()));
    window.draw(modal_background);

    window.draw(modal_window);

    input_box_name.setPosition(modal_window.getPosition() + sf::Vector2f(20, 50));
    input_box_teacher.setPosition(modal_window.getPosition() + sf::Vector2f(20, 100));
    input_box_room.setPosition(modal_window.getPosition() + sf::Vector2f(20, 150));

    window.draw(input_box_name);
    window.draw(input_box_teacher);
    window.draw(input_box_room);

    input_text_name.setPosition(input_box_name.getPosition() + sf::Vector2f(5, 5));
    input_text_teacher.setPosition(input_box_teacher.getPosition() + sf::Vector2f(5, 5));
    input_text_room.setPosition(input_box_room.getPosition() + sf::Vector2f(5, 5));

    window.draw(input_text_name);
    window.draw(input_text_teacher);
    window.draw(input_text_room);

    if (show_error)
    {
        error_text.setPosition(input_box_name.getPosition() + sf::Vector2f(0, -25));
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

void ViewSubjectsWindow::render(sf::RenderWindow &window)
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

    for (size_t i = 0; i < subjects.size(); ++i)
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

        subject_names[i].setPosition(30.f, y_pos + 5.f);
        subject_teachers[i].setPosition(200.f, y_pos + 5.f);
        subject_rooms[i].setPosition(400.f, y_pos + 5.f);

        window.draw(subject_names[i]);
        window.draw(subject_teachers[i]);
        window.draw(subject_rooms[i]);

        row_buttons[i].edit_button.setPosition(600.f, y_pos + 2.5f);
        row_buttons[i].delete_button.setPosition(670.f, y_pos + 2.5f);
        row_buttons[i].edit_text.setPosition(610.f, y_pos + 5.f);
        row_buttons[i].delete_text.setPosition(680.f, y_pos + 5.f);

        window.draw(row_buttons[i].edit_button);
        window.draw(row_buttons[i].delete_button);
        window.draw(row_buttons[i].edit_text);
        window.draw(row_buttons[i].delete_text);
    }

    sf::Text header_name, header_teacher, header_room;

    header_name.setFont(font);
    header_name.setCharacterSize(22);
    header_name.setFillColor(sf::Color::Yellow);
    header_name.setString("Subject Name");
    header_name.setPosition(30.f, 80.f);
    header_name.move(scrollMove);

    header_teacher.setFont(font);
    header_teacher.setCharacterSize(22);
    header_teacher.setFillColor(sf::Color::Yellow);
    header_teacher.setString("Teacher");
    header_teacher.setPosition(200.f, 80.f);
    header_teacher.move(scrollMove);

    header_room.setFont(font);
    header_room.setCharacterSize(22);
    header_room.setFillColor(sf::Color::Yellow);
    header_room.setString("Room Number");
    header_room.setPosition(400.f, 80.f);
    header_room.move(scrollMove);

    window.draw(header_name);
    window.draw(header_teacher);
    window.draw(header_room);

    if (show_modal)
    {
        render_modal(window);
    }

    window.display();
}
