#ifndef UNITTESTGUI_H
#define UNITTESTGUI_H

#include <QDialog>
#include "egtphotoexifengine.h"

namespace Ui
{
    class UnitTestGuiBase;
}

class UnitTestGui : public QDialog
{
  Q_OBJECT

  public:
    UnitTestGui();

  private slots:
    void on_pbtnStartTest_clicked();

  private:
    Ui::UnitTestGuiBase *ui;
    EgtPhotoExifEngine* cvPhotoExifEngine;
};

#endif //UNITTESTGUI_H
