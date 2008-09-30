#ifndef EGTMAINWINDOW_H
#define EGTMAINWINDOW_H

#include "ui_egtmainwindowgui.h"

#include "egtpathbuilder.h"

#include <QFileInfo>
#include <QMainWindow>
#include <QModelIndex>

class EgtMainWindow : public QMainWindow, Ui::EgtMainWindowGui
{

  Q_OBJECT
  
public:
  EgtMainWindow();
  
private slots:
  void clicked(const QModelIndex&);
  void expanded(const QModelIndex&);
  
private:
  QFileInfo cvFileInfo;
  EgtPathBuilder cvPathBuilder;
};
#endif
