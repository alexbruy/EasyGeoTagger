#include "egtapplication.h"

#include <QApplication>

int main(int argc, char* argv[])
{

  QApplication app(argc, argv);
  EgtApplication* EasyGT = new EgtApplication();

  return app.exec();
}
