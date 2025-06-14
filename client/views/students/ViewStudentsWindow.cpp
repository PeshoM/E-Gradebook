#include "ViewStudentsWindow.hpp"
#include <iostream>

ViewStudentsWindow::ViewStudentsWindow(Database *database)
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

    load_students();
}

void ViewStudentsWindow::load_students()
{
    students = db->get_students();

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
}

void ViewStudentsWindow::handle_events(sf::RenderWindow &window, WindowType &next_window)
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

            float max_scroll = std::max(0.f, static_cast<float>(students.size()) * 30.f - (window.getSize().y - 150.f));
            if (scroll_offset < 0.f)
                scroll_offset = 0.f;
            if (scroll_offset > max_scroll)
                scroll_offset = max_scroll;
        }
    }
}

void ViewStudentsWindow::update() {}

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
    float row_height = 30.f;
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

    window.display();
}
