#ifndef EGTMAINWINDOW_H
#define EGTMAINWINDOW_H

#include "ui_egtmainwindowgui.h"

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
  void on_pbtnCreateLayerFromDir_clicked();
  
private:
  QFileInfo cvFileInfo;
  QString buildPath(const QModelIndex& theIndex);
};
#endif
