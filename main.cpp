
#include "inc/spaceinvaders.h"
#include <QApplication>
#include <QScreen>
#include <QVBoxLayout>
#include <iostream>
#include <memory>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  QWidget window;
  // QVBoxLayout *layout =      new QVBoxLayout(&window); // Create a layout for
  // the window

  // CSpaceInvaders *pGame = new CSpaceInvaders(qApp->screens()[0]->size());
  std::unique_ptr<CSpaceInvaders> pGame =
      std::make_unique<CSpaceInvaders>(qApp->screens()[0]->size());

  // Add the game widget to the layout
  // layout->addWidget(pGame);

  // Show the main window (which will show its child widgets)
  // window.show();
  pGame->showFullScreen();
  pGame->Run();

  return app.exec();

  return 0;
}
