
#include <QApplication>
#include <QScreen>
#include "inc/spaceinvaders.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    CSpaceInvaders *pGame = new CSpaceInvaders(qApp->screens()[0]->size());

    pGame->showFullScreen();
    pGame->Run();

    return app.exec();

    return 0;
}
