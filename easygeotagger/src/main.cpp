#include "egtmainwindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{

  QApplication app(argc, argv);
  EgtMainWindow* EasyGT = new EgtMainWindow();
  EasyGT->show();
  
  return app.exec();
}
