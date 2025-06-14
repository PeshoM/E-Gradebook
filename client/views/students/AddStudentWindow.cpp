#include <iostream>
#include "AddStudentWindow.hpp"
#include "../../database/models/student.hpp"

#define BACKSPACE_UNICODE 8

AddStudentWindow::AddStudentWindow(Database *database)
    : back_button({20, 20}, {150, 40}, "Go Back"),
      add_button({20, 80}, {150, 40}, "Add Student"),
      db(database),
      selected_box_index(-1),
      show_success_message(false)
{
    if (!Button::font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
    {
        std::cerr << "Error loading font.\n";
    }

    const std::string labels[] = {
        "Number in Class:", "Full Name:", "Date of Birth:"};

    for (int i = 0; i < 3; ++i)
    {
        input_boxes[i].setSize({300, 40});
        input_boxes[i].setPosition({250, 40 + i * 80});
        input_boxes[i].setFillColor(sf::Color(255, 255, 255, 200));
        input_boxes[i].setOutlineColor(sf::Color::Black);
        input_boxes[i].setOutlineThickness(2);

        input_labels[i].setFont(Button::font);
        input_labels[i].setCharacterSize(18);
        input_labels[i].setFillColor(sf::Color::White);
        input_labels[i].setString(labels[i]);
        input_labels[i].setPosition({250, 10 + i * 80});

        input_texts[i].setFont(Button::font);
        input_texts[i].setCharacterSize(18);
        input_texts[i].setFillColor(sf::Color::Black);
        input_texts[i].setPosition({260, 50 + i * 80});

        input_values[i] = "";
    }

    date_format_label.setFont(Button::font);
    date_format_label.setCharacterSize(14);
    date_format_label.setFillColor(sf::Color(200, 200, 200));
    date_format_label.setString("Format: YYYY-MM-DD");
    date_format_label.setPosition(250, 40 + 2 * 80 + 45);
}

void AddStudentWindow::handle_events(sf::RenderWindow &window, WindowType &next_window)
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
                add_student();
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

void AddStudentWindow::add_student()
{
    error_message.clear();

    try
    {
        int number_in_class = std::stoi(input_values[0]);
        const std::string &full_name = input_values[1];
        const std::string &date_of_birth = input_values[2];

        std::string db_error;

        Student st = { number_in_class, full_name, date_of_birth};

        bool success = db->add_student(st);

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
            error_message = db_error.empty() ? "Failed to add student." : db_error;
        }
    }
    catch (const std::exception &e)
    {
        error_message = std::string("Invalid input!");
    }
}

void AddStudentWindow::update()
{
}

void AddStudentWindow::render(sf::RenderWindow &window)
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

    window.draw(date_format_label);

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
            success_text.setString("Successfully created Student");

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
