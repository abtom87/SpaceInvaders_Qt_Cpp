#include "inc/window.h"
#include <QPushButton>

Window::Window(QWidget *parent) : QWidget(parent)
{
    // Set size of the window
    setFixedSize(1000, 500);

    // Create and position the button
    m_button = new QPushButton("Hello World", this);
    m_button->setGeometry(100, 100, 80, 30);
}
