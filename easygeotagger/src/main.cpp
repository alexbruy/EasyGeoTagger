#include "egtapplication.h"

#include <QApplication>

int main(int argc, char* argv[])
{

  QApplication app(argc, argv);
  //TODO: Get this from cmake
  QString lvPluginDirectory = QCoreApplication::instance()->applicationDirPath() + "/easygt/";
  EgtApplication* EasyGT = new EgtApplication( lvPluginDirectory.replace("bin", "lib") );

  return app.exec();
}
