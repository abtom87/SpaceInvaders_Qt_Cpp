
#include "inc/spaceinvaders.h"
#include <QApplication>
#include <QScreen>
#include <QVBoxLayout>
#include <iostream>
#include <memory>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  QWidget window;

  std::unique_ptr<CSpaceInvaders> pGame =
      std::make_unique<CSpaceInvaders>(qApp->screens()[0]->size());

  pGame->showFullScreen();
  pGame->Run();

  return app.exec();

  return 0;
}
