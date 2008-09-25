/***************************************************************************
 *   Copyright (C) 2003 by Tim Sutton                                      *
 *   tim@linfiniti.com                                                     *
 *                                                                         *
 *   This is a plugin generated from the QGIS plugin template              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef ExifProviderTestGUI_H
#define ExifProviderTestGUI_H

#include <QDialog>
#include <QString>

#include <ui_exifprovidertestguibase.h>

/**
@author Tim Sutton
*/
class ExifProviderTestGui : public QDialog, private Ui::ExifProviderTestGuiBase
{
    Q_OBJECT
  public:
    ExifProviderTestGui( QWidget* parent = 0, Qt::WFlags fl = 0 );
    ~ExifProviderTestGui();

  private:
    static const int context_id = 0;
    QString buildPath(const QModelIndex& theIndex);

  private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_clicked(QAbstractButton* theButton);
    void on_buttonBox_rejected();
    void on_buttonBox_helpRequested();
  
  signals:
    void draw(QString, QString);

};

#endif
