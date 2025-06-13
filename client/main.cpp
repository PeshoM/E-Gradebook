#include <SFML/Graphics.hpp>
#include <memory>
#include "BaseWindow.hpp"
#include "MenuWindow.hpp"
#include "AddStudentWindow.hpp"
#include "WindowType.hpp"

std::unique_ptr<BaseWindow> makeWindow(WindowType type)
{
    switch (type)
    {
        case WindowType::Menu: return std::make_unique<MenuWindow>();
        default: return std::make_unique<MenuWindow>();
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "E-Gradebook");
    window.setFramerateLimit(60);

    WindowType currentType = WindowType::Menu;
    std::unique_ptr<BaseWindow> currentWindow = makeWindow(currentType);

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
