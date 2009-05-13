/*
** File: egtgpsdatatable.cpp
** Author( s ): Roberto Garcia Yunta, Peter J. Ersts ( ersts@amnh.org )
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

#include <QMessageBox>
#include <QMenu>
#include <QDate>

EgtGpsDataTable::EgtGpsDataTable( )
{
  cvHeaderSelectionDialog = new QDialog( this );
  cvUiKeySelectionDialog.setupUi( cvHeaderSelectionDialog );

  cvAvailableFields << tr( "( clear )" ) << tr( "Date Time Stamp" );
  QList< EgtExifEngine::KeyMap > lvKeyMap = cvExifEngine.keys();
  QList< EgtExifEngine::KeyMap >::iterator lvIterator = lvKeyMap.begin();
  while( lvIterator != lvKeyMap.end() )
  {
      cvAvailableFields << lvIterator->commonName;
      lvIterator++;
  }


  cvDataProvider = 0;
  cvColumnSelected = 0;
  cvTotalHeadersSet = 0;

  cvOffset= 0;

  connect( horizontalHeader( ), SIGNAL( sectionClicked( int ) ), this, SLOT( horizontalHeader_clicked( int ) ) );

  verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu); //to enable the customContextMenuRequested signal
  connect( verticalHeader(), SIGNAL( customContextMenuRequested(QPoint) ), this, SLOT( popUpMenu( QPoint ) ) );
  connect( verticalHeader(), SIGNAL( sectionClicked(int) ), this, SLOT( verticalHeader_clicked( int ) ) );

  connect( this, SIGNAL( cellClicked( int, int ) ), this, SLOT( cell_selected( int, int ) ) );
  connect( cvUiKeySelectionDialog.pbtnOk, SIGNAL( clicked( ) ), this, SLOT( on_pbtnOk_clicked( ) ) );
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */

QMap<QString,QString> EgtGpsDataTable::getRow( )
{
  EgtDebug( "entered" );

  return cvRowData;
}

bool EgtGpsDataTable::isAnyColumnHeaderSet( )
{
  EgtDebug( "entered" );

  return ( cvTotalHeadersSet > 0 );
}

bool EgtGpsDataTable::hasColumnHeader( QString theFieldName )
{
 QMapIterator<QString, QString> lvMapIterator( cvRowData );
  while ( lvMapIterator.hasNext( ) )
  {
    lvMapIterator.next( );
    if(lvMapIterator.key( ).compare( theFieldName, Qt::CaseInsensitive ) )
    {
      return true;
    }
  }

  return false;
}

/*
 *
 * SIGNAL and SLOTS
 *
 */
void EgtGpsDataTable::broadcastRow( )
{
  EgtDebug( "entered" );

  if( isAnyColumnHeaderSet( ) )
  {
    QMapIterator<QString, QString> lvMapIterator( cvRowData );
    while ( lvMapIterator.hasNext( ) )
    {
      lvMapIterator.next( );
      if(lvMapIterator.key( ) == tr( "Date Time Stamp" ))
      {
        QDateTime lvDateTime = applyOffset( lvMapIterator.value() );
        if( lvDateTime.isValid() )
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

void EgtGpsDataTable::cell_selected( int row, int column )
{ 
  EgtDebug( "entered" );

  QTableWidgetItem * lvItem = item ( row, column );
  double x = lvItem->data( 0 ).toDouble( );
  emit rowSelected(false);
}

void EgtGpsDataTable::horizontalHeader_clicked( int theIndex )
{
  EgtDebug( "entered" );

  cvColumnSelected = theIndex;

  QTableWidgetItem* lvHeaderItem;
      
  QString lvText;
  lvHeaderItem = horizontalHeaderItem ( theIndex );
  lvText = lvHeaderItem->text( );

  bool ok;
  lvText.toInt( &ok ); 

  cvHeaderSelectionDialog->show( );
  emit rowSelected( false );
}

void EgtGpsDataTable::deleteRow()
{ 
  EgtDebug( "entered" );
  deleteRow( cvSelectedRow );
}

void EgtGpsDataTable::deleteRow( int theRow )
{
  EgtDebug( "entered" );
  removeRow( cvSelectedRow );
  cvSelectedRow = -1;
  emit rowSelected( false );
}

void EgtGpsDataTable::on_pbtnOk_clicked( )
{  
  EgtDebug( "entered" );
  QString lvSelectedItem = cvUiKeySelectionDialog.cbFields->currentText( );
  
  int lvIndex = cvUiKeySelectionDialog.cbFields->currentIndex( );

  if( tr( "( clear )" ) != lvSelectedItem )
  {
    //TODO: check to make sure the value has changed
    cvTotalHeadersSet++;
    cvHeadersThatAreSet<<cvColumnSelected;
    if( tr( "Date Time Stamp" ) == lvSelectedItem )
    {
      emit timeStampSelected(true);
    }
    else { emit timeStampSelected(false); }
  }
  else
  {
    //TODO: check to make sure the value has changed
    cvTotalHeadersSet--;
    if( cvDataProvider->hasColumnHeaders( ) )
    {//revert to the original header
      QStringList lvTags;
      lvTags = cvDataProvider->columnHeaders( );
      lvSelectedItem = lvTags.at( cvColumnSelected );
    }
    else
    {
      lvSelectedItem = QString::number( cvColumnSelected +1 );
      cvHeadersThatAreSet.removeAll( cvColumnSelected );
    }
  }

  if( cvUiKeySelectionDialog.cbFields->itemText( cvUiKeySelectionDialog.cbFields->count()-1 ) == tr( "Date Time Stamp" ) && lvSelectedItem != tr( "Date Time Stamp" ) )
  {//We don't have a timestamp anymore
    emit timeStampSelected(false); 
  }
  //Deleting the previous header
  QTableWidgetItem* lvCurrentHeader = horizontalHeaderItem ( cvColumnSelected );
  if( 0 != lvCurrentHeader )
  {
    delete lvCurrentHeader;
  }
  
  setHorizontalHeaderItem( cvColumnSelected, new QTableWidgetItem( lvSelectedItem ) );
  
  cvHeaderSelectionDialog->close( );
}

void EgtGpsDataTable::popUpMenu(QPoint theCoordinates)
{
  EgtDebug( "entered" );

  QMenu menu;
  QAction* lvDeleteAction = new QAction( tr("&Delete row..."), this );
  lvDeleteAction->setStatusTip(tr("Delete the selected row"));
  cvSelectedRow = verticalHeader()->logicalIndexAt( theCoordinates );
  connect( lvDeleteAction, SIGNAL( triggered() ), this, SLOT( deleteRow() ) );
     
  menu.addAction(lvDeleteAction);

  menu.exec(mapToGlobal(theCoordinates));
}

void EgtGpsDataTable::setOffset( int theOffset )
{
  EgtDebug( "entered" ); 
 
  EgtDebug( "Offset = " + QString::number( theOffset ) );
  cvOffset = theOffset;
}

void EgtGpsDataTable::setProvider( EgtDataProvider* theProvider )
{
  EgtDebug( "entered" );

  if( 0 != cvDataProvider ) { delete cvDataProvider; }

  cvDataProvider = theProvider;
  populateTable( );
}

void EgtGpsDataTable::verticalHeader_clicked( int theIndex )
{
  EgtDebug( "entered" );

  cvSelectedRow = theIndex;
  cvRowData.clear();

  if( isAnyColumnHeaderSet( ) )
  {
    QTableWidgetItem* lvHeaderItem;
    for( int lvColumnCount = 0; lvColumnCount < columnCount( ); lvColumnCount++ )
    {
      QString lvText;
      lvHeaderItem = horizontalHeaderItem ( lvColumnCount );
      lvText = lvHeaderItem->text( );
      if( lvText == tr( "Date Time Stamp" ) )
      {
         QString lvContent = item( theIndex, lvColumnCount )->text();
         //Format checking of the timestamp
        if( lvContent.size() != 19 )
        {
          QMessageBox::critical( this, tr( "Error" ),tr( "The date time stamp has an incorrect format.\n The expected format is:\n yyyy:mm:dd hh:mm:ss" ),QMessageBox::Ok );
          emit timeStampSelected(false);
          return;
        }
        bool lvExpectedFormat = true;
        lvExpectedFormat = lvExpectedFormat && ':' == lvContent[4];
        lvExpectedFormat = lvExpectedFormat && ':' == lvContent[7];
        lvExpectedFormat = lvExpectedFormat && ' ' == lvContent[10];
        lvExpectedFormat = lvExpectedFormat && ':' == lvContent[13];
        lvExpectedFormat = lvExpectedFormat && ':' == lvContent[16];

        for( int lvIterator = 0; lvIterator < lvContent.size( ); lvIterator++ )
        {
          if( !lvContent[ lvIterator ].isNumber( ) && ':' != lvContent[ lvIterator ] && ' ' != lvContent[ lvIterator ] )
          {
            QMessageBox::critical( this, tr( "Error" ),tr( "The date time stamp has an incorrect format.\n The expected format is:\n yyyy:mm:dd hh:mm:ss" ),QMessageBox::Ok );
            emit timeStampSelected(false);
            return;
          }
          else { emit timeStampSelected(true);  }
        }
        //format checking end
      }

      bool ok;
      lvText.toInt( &ok );

      if( !ok )
      {
        cvRowData.insert( lvText, item( theIndex, lvColumnCount )->text( ) );
      }
    }
  }

  emit rowSelected(true);
}

/*
 *
 * PRIVATE FUNCTIONS
 *
 */

QDateTime EgtGpsDataTable::applyOffset( QString theDateTimeStamp )
{
    EgtDebug( "enetered" );

    QDateTime lvNewDateTime = QDateTime::fromString( theDateTimeStamp, "yyyy:MM:dd hh:mm:ss" );
    return lvNewDateTime.addSecs( cvOffset );
}

void EgtGpsDataTable::populateTable( )
{
  EgtDebug( "entered" );

  clear();
  setRowCount( 0 );
  setColumnCount( 0 );

  /*Reset the Qlist that contains which headers are set*/
  cvHeadersThatAreSet.clear( );

  if( cvDataProvider == 0 || cvDataProvider->numberOfRecords( ) == 0 || cvDataProvider->numberOfFields( ) == 0 ) { return; }
  cvDataProvider->reset();
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
    QStringList cvRowData = cvDataProvider->next();

    for( int j = 0; j < cvRowData.size( ); j++ )
    {
      QTableWidgetItem *lvNewItem = new QTableWidgetItem( cvRowData.at( j ) );
       setItem( i, j, lvNewItem );
    }
  }
  emit timeStampSelected( false );
  emit rowSelected( false );
}

