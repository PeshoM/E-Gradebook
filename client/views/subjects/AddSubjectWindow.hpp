#pragma once
#include <SFML/Graphics.hpp>
#include "../../components/Button.hpp"
#include "../../database/Database.hpp"
#include "../../WindowType.hpp"
#include "../../components/BaseWindow.hpp"
#include <string>

class AddSubjectWindow : public BaseWindow
{
public:
    AddSubjectWindow(Database *database);

    void handle_events(sf::RenderWindow &window, WindowType &next_window);
    void update();
    void render(sf::RenderWindow &window);

private:
    Button back_button;
    Button add_button;

    Database *db;

    sf::RectangleShape input_boxes[3];
    sf::Text input_labels[3];
    sf::Text input_texts[3];
    std::string input_values[3];
    int selected_box_index;

    std::string error_message;
    bool show_success_message;
    sf::Clock success_message_clock;

    void add_subject();
};
