/*
** File: 
** Author(s): 
** Creation Date: 
**
** Copyright (c) 2008, American Museum of Natural History. All rights reserved.
**
** This library/program is free software; you can redistribute it
** and/or modify it under the terms of the GNU Library General Public
** License as published by the Free Software Foundation; either
** version 2 of the License, or (at your option) any later version.
**
** This library/program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.
**
** This work was made possible through a grant by the the John D. and
** Catherine T. MacArthur Foundation with additional support from the
** Center For Biodiversity and Conservation and The Spanish Ministry of
** Science and Innovation's INTEGRANTS program.
**
**/
#ifndef EGTGPSDATATABLE_H
#define EGTGPSDATATABLE_H

#include "egtfilereader.h"

#include <QTableWidget>
#include <QWidget>
#include <QPushButton>
#include <QHeaderView>

class EgtGpsDataTable : public QWidget
{
  Q_OBJECT

  public:
    EgtGpsDataTable( );
    QMap<QString,QString>* getRowItems();

  private slots:
    //void setFileReader();
    void cell_selected(int, int);
    void cvHorizontalHeader_clicked(int);
    void cvVerticalHeader_clicked(int);
    void cvTagButton_clicked();

  signals:
    void rowSelected();

  private:
    QHeaderView *  cvHorizontalHeader;
    QHeaderView *  cvVerticalHeader;
    QMap<QString,QString> * cvMapItems;
    QTableWidget* cvTable;
    QPushButton cvTagButton;

    void populateTable();
};
#endif

