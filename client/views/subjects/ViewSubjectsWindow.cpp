#include "ViewSubjectsWindow.hpp"
#include <iostream>

ViewSubjectsWindow::ViewSubjectsWindow(Database *database)
    : db(database), scroll_offset(0.f)
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

    load_subjects();
}

void ViewSubjectsWindow::load_subjects()
{
    subjects = db->get_subjects(); 

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
}

void ViewSubjectsWindow::handle_events(sf::RenderWindow &window, WindowType &next_window)
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

        if (event.type == sf::Event::MouseWheelScrolled)
        {
            scroll_offset -= event.mouseWheelScroll.delta * 30.f;

            float max_scroll = std::max(0.f, static_cast<float>(subjects.size()) * 30.f - (window.getSize().y - 150.f));
            if (scroll_offset < 0.f)
                scroll_offset = 0.f;
            if (scroll_offset > max_scroll)
                scroll_offset = max_scroll;
        }
    }
}

void ViewSubjectsWindow::update() {}

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
    float row_height = 30.f;
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
        subject_teachers[i].setPosition(300.f, y_pos + 5.f);
        subject_rooms[i].setPosition(550.f, y_pos + 5.f);

        window.draw(subject_names[i]);
        window.draw(subject_teachers[i]);
        window.draw(subject_rooms[i]);
    }

    sf::Text header_name, header_teacher, header_room;

    header_name.setFont(font);
    header_name.setCharacterSize(22);
    header_name.setFillColor(sf::Color::Yellow);
    header_name.setString("Subject Name");
    header_name.setPosition(20.f, 80.f);
    header_name.move(scrollMove);

    header_teacher.setFont(font);
    header_teacher.setCharacterSize(22);
    header_teacher.setFillColor(sf::Color::Yellow);
    header_teacher.setString("Teacher");
    header_teacher.setPosition(300.f, 80.f);
    header_teacher.move(scrollMove);

    header_room.setFont(font);
    header_room.setCharacterSize(22);
    header_room.setFillColor(sf::Color::Yellow);
    header_room.setString("Room No.");
    header_room.setPosition(550.f, 80.f);
    header_room.move(scrollMove);

    window.draw(header_name);
    window.draw(header_teacher);
    window.draw(header_room);

    window.display();
}
