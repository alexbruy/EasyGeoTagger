/*
** File: egtgpsdatatable.h
** Author( s ): Roberto Garcia Yunta, Peter J. Ersts ( ersts at amnh.org )
** Creation Date: 2008-12-19
**
** Copyright ( c ) 2008, American Museum of Natural History. All rights reserved.
**
** This library/program is free software; you can redistribute it
** and/or modify it under the terms of the GNU Library General Public
** License as published by the Free Software Foundation; either
** version 2 of the License, or ( at your option ) any later version.
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

#include "egtdataprovider.h"
#include "egtgpsexifengine.h"
#include "ui_egtkeyselectiondialog.h"


#include <QTableWidget>
#include <QStringList>
#include <QDateTime>
#include <QMap>

class MS_DLL_SPEC EgtGpsDataTable : public QTableWidget
{
  Q_OBJECT

  public:

     /*! \brief Constructor */
    EgtGpsDataTable( );

    /*! \brief Resets a column header */
    void clearColumnHeader( int );

    /*! \brief Indicates if the user has set at least column header */
    bool isAnyColumnHeaderSet( );

    /*! \brief Indicates if a specific column header has been set */
    bool isColumnHeaderSet( QString );

    /*! \brief Returns a QMap that contains all the key-value pairs of the selected row */
    QMap<QString,QString> rowData( );

    /*! \brief Returns a QMap that contains all the key-value pairs of the specified row */
    QMap<QString,QString> rowData( int );

    /*! \brief Set the column header for a specified row */
    bool setColumnHeader( int, QString );

  public slots:

    /*! \brief Broadcasts the key-value pairs in the currently selected row via the application interface */
    void broadcastRow( );

    /*! \brief Broadcasts the key-value pairs in the specified row via the application interface */
    void broadcastRow( int );

    /*! \brief Deletes the current row of the table */
    void deleteRow( );

    /*! \brief Deletes a row of the table */
    void deleteRow( int );

    /*! \brief Select a row and make it the current row */
    void selectRow( int );

    /*! \brief Sets the offset*/
    void setOffset( int );

    /*! \brief Set the table's data provider */
    void setProvider( EgtDataProvider* );

  private slots:

     /*! \brief Slot used when a cell of the table is selected */
    void cell_selected( int, int );

    /*! \brief Slot used when the horizontal header is clicked */
    void horizontalHeader_clicked( int );

    /*! \brief Slot used when the user sets the header of a column */
    void on_pbtnOk_clicked( );

    /*! \brief Slot used to show up a pop up menu, when user right clicks on the vertical header */
    void popUpMenu( QPoint );

    /*! \brief Slot used when the vertical header is clicked */
    void verticalHeader_clicked( int );

  signals:

    /*! \brief Signal used to set the status of the sendCoordinates and deleteRow buttons */
    void displayButtonsStatus( bool, bool );

    /*! \brief Signal used to comunicate with the application interface */
    void keyValuePair( QString, QString );

    /*! \brief Signal indicating if a row has been selected or deselected */
    void rowSelected( bool );

    /*! \brief Signal used to notify that one column contains a date timestamp */
    void timeStampSelected( bool );

  private:

    /*! \brief Apply the offset to the date time stamp passed in as a QString */
    QDateTime applyOffset( QString );

    /*! \brief Function used to populate the table */
    void populateTable( );

    /*! \brief Check to see if the DateTimeStamp is valid
     *  \returns False if the stamp is not valid and true if it is or no DataTimeStamp was found
     */
    bool validDateTimeStamp( );



    /*! \brief Contains all the possible fields that can be used for tagging a picture */
    QStringList cvAvailableFields;

    /*! \brief Contains the last column that the user selected */
    int cvColumnSelected;

    /*! \brief Pointer to the concrete instance of the file reader */
    EgtDataProvider* cvDataProvider;

    /*! \brief Instance of a GPS EXIF engine */
    EgtGpsExifEngine cvExifEngine;

    /*! \brief A QMap with all the headers that are set so far */
    QMap< QString, int > cvColumnHeadersSet;

    /*! \brief Dialog to show the user all the available fields */
    QDialog* cvHeaderSelectionDialog;

    /*! \brief Offset specified by the user in the synchronization dialog */
    int cvOffset;

    /*! \brief Date time stamp of picture used in the synchronization dialog */
    QString cvPictureDateTimeStamp;

    /*! \brief QMap to store the items of the selected row */
    QMap<QString,QString>  cvRowData;

    /*! \brief Row the user selected */  
    int cvSelectedRow;

    /*! \brief QDesigner object of the dialog with the available fields*/
    Ui::EgtKeySelectionDialog cvUiKeySelectionDialog;
  
};

#endif

