#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "../../components/BaseWindow.hpp"
#include "../../components/Button.hpp"
#include "../../database/Database.hpp"

class AddStudentWindow : public BaseWindow
{
private:
    Button back_button;
    Button add_button;

    sf::RectangleShape input_boxes[3];
    sf::Text input_labels[3];
    sf::Text input_texts[3];
    std::string input_values[3];
    sf::Text date_format_label;

    int selected_box_index = -1;
    Database *db;

    std::string error_message;

    bool show_success_message = false;
    sf::Clock success_message_clock;

public:
    AddStudentWindow(Database *database);

    void handle_events(sf::RenderWindow &window, WindowType &next_window) override;
    void update() override;
    void render(sf::RenderWindow &window) override;

    void add_student();
};
