/*
** File: egtgpsdatatablewidget.h
** Author(s): Roberto Garcia Yunta
** Creation Date: 2008-12-19
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
#ifndef EGTGPSDATATABLEWIDGET_H
#define EGTGPSDATATABLEWIDGET_H

#include "egtfilereader.h"
#include "ui_columnmeaningdialog.h"


#include <QTableWidget>
#include <QHeaderView>

class EgtGpsDataTableWidget : public QTableWidget
{
  Q_OBJECT

  public:

     /*! \brief Constructor */
    EgtGpsDataTableWidget( );

    /*! \brief Indicates if the user has set all the columns with a valid value */
    bool isThereAnyColumnSet();

    /*! \brief Returns a QMap that contains all the items of the selected row */
    QMap<QString,QString>* getRowItems();

  public slots:

    /*! \brief Slot used to set the specific file reader */
    void setFileReader( EgtFileReader* );

  private slots:

     /*! \brief Slot used when a cell of the table is selected */
    void cell_selected(int, int);

    /*! \brief Slot used when the horizontal header is clicked */
    void cvHorizontalHeader_clicked(int);

    /*! \brief Slot used when the tag button is clicked*/
    void cvTagButton_clicked();
 
    /*! \brief Slot used when the vertical header is clicked */
    void cvVerticalHeader_clicked(int);

    void on_pbtnOk_clicked();

  private:
    
    /*! \brief Contains all the possible fields that can be used for taggin a picture */
    QStringList cvAvailableFields;

    /*! \brief Contains the last column that the user selected */
    int cvColumnSelected;

    /*! \brief Dialog to show the user all the available fields */
    QDialog* cvColumnMeaningDialog;

    /*! \brief Pointer to the concrete instance of the file reader */
    EgtFileReader* cvFileReader;

    /*! \brief Indicates if all the headers are set */
    bool cvHeadersAreSet;

    /*! \brief A QList with all the headers that are set so far */
    QList<int> cvHeadersThatAreSet;

    /*! \brief Pointer to the horizontal header */
    QHeaderView *  cvHorizontalHeader;

    /*! \brief QMap to store the items of the selected row */
    QMap<QString,QString> * cvMapItems;

    /*! \brief QDesigner object of the dialog with the available fields*/
    Ui::ColumnMeaningDialog cvUiColumnMeaning;

    /*! \brief Pointer to the vertical header */
    QHeaderView *  cvVerticalHeader;

    /*! \brief Function used to populate the table */
    void populateTable();

};
#endif

