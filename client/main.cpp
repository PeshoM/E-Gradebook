#include <SFML/Graphics.hpp>
#include "WindowType.hpp"
#include "components/BaseWindow.hpp"
#include "views/MenuWindow.hpp"
#include "database/Database.hpp"
#include "views/students/AddStudentWindow.hpp"
#include "views/students/ViewStudentsWindow.hpp"
#include "views/subjects/AddSubjectWindow.hpp"
#include "views/subjects/ViewSubjectsWindow.hpp"
#include "views/grades/AddGradeWindow.hpp"
#include <iostream>

std::unique_ptr<BaseWindow> makeWindow(WindowType type, Database *db)
{
    switch (type)
    {
    case WindowType::Menu:
        return std::make_unique<MenuWindow>();
    case WindowType::AddStudent:
        return std::make_unique<AddStudentWindow>(db);
    case WindowType::AddSubject:
        return std::make_unique<AddSubjectWindow>(db);
    case WindowType::ViewStudents:
        return std::make_unique<ViewStudentsWindow>(db);
    case WindowType::ViewSubjects:
        return std::make_unique<ViewSubjectsWindow>(db);
    case WindowType::AddGrade:
        return std::make_unique<AddGradeWindow>(db);
    default:
        return std::make_unique<MenuWindow>();
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "E-Gradebook");
    window.setFramerateLimit(60);

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

    WindowType currentType = WindowType::Menu;
    std::unique_ptr<BaseWindow> currentWindow = makeWindow(currentType, &db);

    while (window.isOpen())
    {
        WindowType nextType = currentType;

        currentWindow->handle_events(window, nextType);
        currentWindow->update();

        if (nextType != currentType)
        {
            currentType = nextType;
            currentWindow = makeWindow(currentType, &db);
        }

        currentWindow->render(window);
    }

    db.disconnect();

    return 0;
}
