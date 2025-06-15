#include <iostream>
#include "AddSubjectWindow.hpp"
#include "../../database/models/subject.hpp"

#define BACKSPACE_UNICODE 8

AddSubjectWindow::AddSubjectWindow(Database *database)
    : back_button({20, 20}, {150, 40}, "Go Back"),
      add_button({20, 80}, {150, 40}, "Add Subject"),
      db(database),
      selected_box_index(-1),
      show_success_message(false)
{
    if (!Button::font.loadFromFile("arial.ttf") &&
        !Button::font.loadFromFile("C:/Windows/Fonts/arial.ttf") &&
        !Button::font.loadFromFile("/System/Library/Fonts/Arial.ttf") &&
        !Button::font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"))
    {
        std::cerr << "Warning: Could not load any system font. Using default rendering.\n";
    }

    const std::string labels[] = {
        "Subject Name:", "Teacher:", "Room Number:"};

    for (int i = 0; i < 3; ++i)
    {
        input_boxes[i].setSize({300, 40});
        input_boxes[i].setPosition({250, 40 + i * 80});
        input_boxes[i].setFillColor(sf::Color(90, 90, 160));
        input_boxes[i].setOutlineColor(sf::Color(100, 100, 180));
        input_boxes[i].setOutlineThickness(2);

        input_labels[i].setFont(Button::font);
        input_labels[i].setCharacterSize(18);
        input_labels[i].setFillColor(sf::Color(220, 220, 255));
        input_labels[i].setString(labels[i]);
        input_labels[i].setPosition({250, 10 + i * 80});

        input_texts[i].setFont(Button::font);
        input_texts[i].setCharacterSize(18);
        input_texts[i].setFillColor(sf::Color(220, 220, 255));
        input_texts[i].setPosition({260, 50 + i * 80});

        input_values[i] = "";
    }
}

void AddSubjectWindow::handle_events(sf::RenderWindow &window, WindowType &next_window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseButtonPressed)
        {
            sf::Vector2f mouse_pos(event.mouseButton.x, event.mouseButton.y);
            selected_box_index = -1;

            for (int i = 0; i < 3; ++i)
            {
                if (input_boxes[i].getGlobalBounds().contains(mouse_pos))
                {
                    selected_box_index = i;
                    break;
                }
            }

            if (back_button.is_clicked(mouse_pos, true))
                next_window = WindowType::Menu;

            if (add_button.is_clicked(mouse_pos, true))
                add_subject();
        }

        if (event.type == sf::Event::TextEntered && selected_box_index != -1)
        {
            if (event.text.unicode == BACKSPACE_UNICODE)
            {
                if (!input_values[selected_box_index].empty())
                    input_values[selected_box_index].pop_back();
            }
            else if (event.text.unicode < 128)
            {
                input_values[selected_box_index] += static_cast<char>(event.text.unicode);
            }

            input_texts[selected_box_index].setString(input_values[selected_box_index]);
        }
    }
}

void AddSubjectWindow::add_subject()
{
    error_message.clear();

    const std::string &name = input_values[0];
    const std::string &teacher = input_values[1];
    const std::string &room_number = input_values[2];

    if (name.empty() || teacher.empty() || room_number.empty())
    {
        error_message = "All fields are required!";
        return;
    }

    Subject subject = {0, name, teacher, room_number};

    bool success = db->add_subject(subject);

    if (success)
    {
        show_success_message = true;
        success_message_clock.restart();

        for (int i = 0; i < 3; ++i)
        {
            input_values[i].clear();
            input_texts[i].setString("");
        }
    }
    else
    {
        error_message = "Failed to add subject (duplicate or error).";
    }
}

void AddSubjectWindow::update()
{
}

void AddSubjectWindow::render(sf::RenderWindow &window)
{
    window.clear(sf::Color(50, 50, 90));

    back_button.draw(window);
    add_button.draw(window);

    for (int i = 0; i < 3; ++i)
    {
        window.draw(input_boxes[i]);
        window.draw(input_labels[i]);
        window.draw(input_texts[i]);
    }

    if (!error_message.empty())
    {
        sf::Text error_text;
        error_text.setFont(Button::font);
        error_text.setCharacterSize(18);
        error_text.setFillColor(sf::Color::Red);
        error_text.setString(error_message);
        error_text.setPosition(250, 300);
        window.draw(error_text);
    }

    if (show_success_message)
    {
        if (success_message_clock.getElapsedTime().asSeconds() < 1.0f)
        {
            sf::Text success_text;
            success_text.setFont(Button::font);
            success_text.setCharacterSize(22);
            success_text.setFillColor(sf::Color::Green);
            success_text.setString("Successfully added Subject");

            sf::FloatRect textRect = success_text.getLocalBounds();
            success_text.setOrigin(textRect.left + textRect.width / 2.0f,
                                   textRect.top + textRect.height / 2.0f);
            success_text.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

            window.draw(success_text);
        }
        else
        {
            show_success_message = false;
        }
    }

    window.display();
}
