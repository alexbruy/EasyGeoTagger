/*
** File: egtgpsdatatable.cpp
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
#include "egtgpsdatatable.h"
#include "egtlogger.h"

#include <QHeaderView>
#include <QMessageBox>
#include <QMenu>
#include <QDate>

EgtGpsDataTable::EgtGpsDataTable( )
{
  cvHeaderSelectionDialog = new QDialog( this );
  cvUiKeySelectionDialog.setupUi( cvHeaderSelectionDialog );

  cvAvailableFields << tr( "( clear )" ) << tr( "Date Time Stamp" );
  QList< EgtExifEngine::KeyMap > lvKeyMap = cvExifEngine.keys( );
  QList< EgtExifEngine::KeyMap >::iterator lvIterator = lvKeyMap.begin( );
  while( lvIterator != lvKeyMap.end( ) )
  {
      cvAvailableFields << lvIterator->commonName;
      lvIterator++;
  }


  cvDataProvider = 0;
  cvColumnSelected = 0;
  cvOffset = 0;

  connect( horizontalHeader( ), SIGNAL( sectionClicked( int ) ), this, SLOT( horizontalHeader_clicked( int ) ) );

  verticalHeader( )->setContextMenuPolicy( Qt::CustomContextMenu ); //to enable the customContextMenuRequested signal
  connect( verticalHeader( ), SIGNAL( customContextMenuRequested( QPoint ) ), this, SLOT( popUpMenu( QPoint ) ) );
  connect( verticalHeader( ), SIGNAL( sectionClicked( int ) ), this, SLOT( verticalHeader_clicked( int ) ) );

  connect( this, SIGNAL( cellClicked( int, int ) ), this, SLOT( cell_selected( int, int ) ) );
  connect( cvUiKeySelectionDialog.pbtnOk, SIGNAL( clicked( ) ), this, SLOT( on_pbtnOk_clicked( ) ) );
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */
void EgtGpsDataTable::broadcastRow( )
{
  EgtDebug( "entered" );

  if( isAnyColumnHeaderSet( ) )
  {
    if( !validDateTimeStamp( ) )
    {
      EgtDebug( "DateTimeStamp was not valid " );
      return;
    }

    QMapIterator<QString, QString> lvMapIterator( cvRowData );
    while ( lvMapIterator.hasNext( ) )
    {
      lvMapIterator.next( );
      if( lvMapIterator.key( ) == tr( "Date Time Stamp" ) )
      {
        QDateTime lvDateTime = applyOffset( lvMapIterator.value( ) );
        if( lvDateTime.isValid( ) )
        {
            emit( keyValuePair( "Egt.GPS.TimeStamp", lvDateTime.toString( "hh:mm:ss" ) ) );
            emit( keyValuePair( "Egt.GPS.DateStamp", lvDateTime.toString( "yyyy:MM:dd" ) ) );
        }
      }
      else
      {
        emit( keyValuePair( cvExifEngine.key( lvMapIterator.key( ) ),lvMapIterator.value( ) ) );
      }

    }
  }
}

void EgtGpsDataTable::broadcastRow( int theRow )
{
  EgtDebug( "entered" );

  selectRow( theRow );
  broadcastRow( );
}

void EgtGpsDataTable::clearColumnHeader( int theColumn )
{
  EgtDebug( "entered" );

  setColumnHeader( theColumn, tr( "( clear )" ) );
}

void EgtGpsDataTable::deleteRow( int theRow )
{
  EgtDebug( "entered" );
  if( rowCount( ) != 0 &&( theRow >= 0 || theRow < rowCount( ) ) )
  {
    removeRow( theRow );
    cvSelectedRow = -1;
    emit rowSelected( false );
  }
}

/*!
 * \returns true of one or more column headers have been set
 */
bool EgtGpsDataTable::isAnyColumnHeaderSet( )
{
  EgtDebug( "entered" );

  return ( cvColumnHeadersSet.size( ) > 0 );
}

/*!
 * theFieldName The string to search for in the column headers
 * \returns true of the string matches and existing column header
 */
bool EgtGpsDataTable::isColumnHeaderSet( QString theFieldName )
{
   EgtDebug( "entered" );

   return cvColumnHeadersSet.contains( theFieldName );
}

/*!
 * \returns a map of all key value pairs for columns that have been set for the current row in the table
 */
QMap<QString,QString> EgtGpsDataTable::rowData( )
{
  EgtDebug( "entered" );

  if( !validDateTimeStamp( ) )
  {
    QMap< QString, QString > lvTemp;
    return lvTemp;
  }

  return cvRowData;
}

/*!
 * \returns a map of all key value pairs for columns that have been set for the specified row in the table
 */
QMap<QString,QString> EgtGpsDataTable::rowData( int theRow )
{
  EgtDebug( "entered" );

  selectRow( theRow );

  return cvRowData;
}

void EgtGpsDataTable::selectRow( int theRow )
{
  EgtDebug( "entered" );

  cvRowData.clear( );
  cvSelectedRow = -1;
  if( theRow < 0 || theRow > rowCount( ) )
  {
    emit rowSelected( false );
    return;
  }

  cvSelectedRow = theRow;
  if( isAnyColumnHeaderSet( ) )
  {
    QTableWidgetItem* lvHeaderItem;
    QMap< QString, int >::iterator lvIterator = cvColumnHeadersSet.begin( );
    while( lvIterator != cvColumnHeadersSet.end( ) )
    {
      cvRowData.insert( lvIterator.key( ), item( theRow, lvIterator.value( ) )->text( ) );
      lvIterator++;
    }
  }

  emit rowSelected( true );
}

/*!
 * \param theColumn The column number
 * \param theHeader The new header
 * \returns true of theHeader does not match a header that has already been set
 */
bool EgtGpsDataTable::setColumnHeader( int theColumn, QString theHeader )
{
  EgtDebug( "entered" );

  if( theColumn < 0 || theColumn > columnCount( ) ) { return false; }

  QString lvTheHeader = theHeader;
  if( isColumnHeaderSet( lvTheHeader ) )
  {
   return false;
  }

  if( tr( "( clear )" ) == lvTheHeader )
  {
    if( cvDataProvider->hasColumnHeaders( ) )
    {//revert to the original header
      lvTheHeader = cvDataProvider->columnHeaders( ).at( theColumn );
    }
    else
    {
      lvTheHeader = QString::number( theColumn +1 );
    }

    cvColumnHeadersSet.remove( cvColumnHeadersSet.key( theColumn ) );
  }
  else
  {
    cvColumnHeadersSet.insert( theHeader, theColumn );
    if( tr( "Date Time Stamp" ) == lvTheHeader )
    {
      emit timeStampSelected( true );
    }
    else
    {
      emit timeStampSelected( false );
    }
  }

  //Deleting the previous header
  QTableWidgetItem* lvCurrentHeader = horizontalHeaderItem ( theColumn );
  if( 0 != lvCurrentHeader )
  {
    delete lvCurrentHeader;
  }

  setHorizontalHeaderItem( theColumn, new QTableWidgetItem( lvTheHeader ) );
  return true;
}

/*!
 * \param theOffset The time offset in seconds
 */
void EgtGpsDataTable::setOffset( int theOffset )
{
  EgtDebug( "entered" );

  EgtDebug( "Offset = " + QString::number( theOffset ) );
  cvOffset = theOffset;
}

/*!
 * \param theProvider A pointer to a new provider
 */
void EgtGpsDataTable::setProvider( EgtDataProvider* theProvider )
{
  EgtDebug( "entered" );

  if( 0 != cvDataProvider ) { delete cvDataProvider; }

  cvDataProvider = theProvider;
  populateTable( );
}

/*
 *
 * PRIVATE FUNCTIONS
 *
 */

QDateTime EgtGpsDataTable::applyOffset( QString theDateTimeStamp )
{
    EgtDebug( "entered" );

    QDateTime lvNewDateTime = QDateTime::fromString( theDateTimeStamp, "yyyy:MM:dd hh:mm:ss" );
    return lvNewDateTime.addSecs( cvOffset );
}

void EgtGpsDataTable::cell_selected( int row, int column )
{
  EgtDebug( "entered" );

  QTableWidgetItem * lvItem = item ( row, column );
  double x = lvItem->data( 0 ).toDouble( );
  emit rowSelected( false );
}

void EgtGpsDataTable::horizontalHeader_clicked( int theIndex )
{
  EgtDebug( "entered" );

  cvColumnSelected = theIndex;
  cvHeaderSelectionDialog->show( );
  emit rowSelected( false );
}

void EgtGpsDataTable::deleteRow( )
{
  EgtDebug( "entered" );
  deleteRow( cvSelectedRow );
}

void EgtGpsDataTable::on_pbtnOk_clicked( )
{
  EgtDebug( "entered" );
  QString lvHeader = cvUiKeySelectionDialog.cbFields->currentText( );

  if( isColumnHeaderSet( lvHeader ) )
  {
    QMessageBox::critical( this, tr( "Error" ),tr( "Another column has already been set to" ) + ":" + lvHeader,QMessageBox::Ok );
    return;
  }

  setColumnHeader( cvColumnSelected, lvHeader );

  cvHeaderSelectionDialog->close( );
}

void EgtGpsDataTable::popUpMenu( QPoint theCoordinates )
{
  EgtDebug( "entered" );

  QMenu menu;
  QAction* lvDeleteAction = new QAction( tr( "&Delete row..." ), this );
  lvDeleteAction->setStatusTip( tr( "Delete the selected row" ) );
  cvSelectedRow = verticalHeader( )->logicalIndexAt( theCoordinates );
  connect( lvDeleteAction, SIGNAL( triggered( ) ), this, SLOT( deleteRow( ) ) );

  menu.addAction( lvDeleteAction );

  menu.exec( mapToGlobal( theCoordinates ) );
}

void EgtGpsDataTable::populateTable( )
{
  EgtDebug( "entered" );

  clear( );
  setRowCount( 0 );
  setColumnCount( 0 );

  //Reset the map that contains which headers are set
  cvColumnHeadersSet.clear( );

  //Make sure the provider is valid
  if( cvDataProvider == 0 || cvDataProvider->numberOfRecords( ) == 0 || cvDataProvider->numberOfFields( ) == 0 ) { return; }

  cvDataProvider->reset( );
  setRowCount( cvDataProvider->numberOfRecords( ) );
  setColumnCount( cvDataProvider->numberOfFields( ) );

  /*Set up the combo box*/
  cvUiKeySelectionDialog.cbFields->clear( );
  cvUiKeySelectionDialog.cbFields->insertItems( 0,cvAvailableFields );

  if( cvDataProvider->hasColumnHeaders( ) )
  {
    QStringList lvTags;
    lvTags = cvDataProvider->columnHeaders( );
    setHorizontalHeaderLabels( lvTags );
  }
  else
  {
    QStringList lvDefaultHeader;

    for( int i = 0; i< cvDataProvider->numberOfFields( ); i++ )
    {
      lvDefaultHeader << QString::number( i+1 );
    }

    setHorizontalHeaderLabels( lvDefaultHeader );
  }

  for( int i = 0; i < cvDataProvider->numberOfRecords( ); i++ )
  {
    QStringList cvRowData = cvDataProvider->next( );

    for( int j = 0; j < cvRowData.size( ); j++ )
    {
      QTableWidgetItem *lvNewItem = new QTableWidgetItem( cvRowData.at( j ) );
       setItem( i, j, lvNewItem );
    }
  }
  emit timeStampSelected( false );
  emit rowSelected( false );
}

bool EgtGpsDataTable::validDateTimeStamp( )
{
  if( cvRowData.contains( tr( "Date Time Stamp" ) ) )
  {
    EgtDebug( cvRowData[ tr( "Date Time Stamp" ) ] );
    QDateTime lvDateTime = QDateTime::fromString( cvRowData[ tr( "Date Time Stamp" ) ], "yyyy:MM:dd hh:mm:ss" );
    if( !lvDateTime.isValid( ) )
    {
      return false;
    }
  }

  return true;
}

void EgtGpsDataTable::verticalHeader_clicked( int theIndex )
{
  selectRow( theIndex );
  if( !validDateTimeStamp( ) )
  {
    QMessageBox::critical( this, tr( "Error" ),tr( "The date time stamp has an incorrect format.\n The expected format is:\n yyyy:mm:dd hh:mm:ss" ),QMessageBox::Ok );
  }
}
