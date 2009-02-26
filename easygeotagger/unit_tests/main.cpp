#include <QApplication>
#include <QObject>

#include "unittestgui.h"

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  UnitTestGui *lvDialog = new UnitTestGui;
  lvDialog->show();
  return app.exec();
}
