#include <SFML/Graphics.hpp>
#include <memory>
#include "WindowType.hpp"
#include "BaseWindow.hpp"
#include "MenuWindow.hpp"
#include "Database.hpp"
#include <iostream>

std::unique_ptr<BaseWindow> makeWindow(WindowType type)
{
    switch (type)
    {
    case WindowType::Menu:
        return std::make_unique<MenuWindow>();
    default:
        return std::make_unique<MenuWindow>();
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "E-Gradebook");
    window.setFramerateLimit(60);

    WindowType currentType = WindowType::Menu;
    std::unique_ptr<BaseWindow> currentWindow = makeWindow(currentType);

    Database db;
    nanodbc::connection conn_string("Driver={ODBC Driver 17 for SQL Server};Server=127.0.0.1,1433;Database=master;Uid=sa;Pwd=Pesho12345;");

    if (!db.connect(conn_string))
    {
        return 1;
    }

    if (db.create_tables())
    {
        std::cout << "Tables created successfully!" << std::endl;
    }
    else
    {
        std::cout << "Failed to create tables (maybe they already exist?)" << std::endl;
    }

    db.disconnect();

    while (window.isOpen())
    {
        WindowType nextType = currentType;

        currentWindow->handleEvents(window, nextType);
        currentWindow->update();

        if (nextType != currentType)
        {
            currentType = nextType;
            currentWindow = makeWindow(currentType);
        }

        currentWindow->render(window);
    }

    return 0;
}
