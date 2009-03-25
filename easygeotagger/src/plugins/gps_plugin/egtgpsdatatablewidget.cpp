/*
** File: egtgpsdatatablewidget.cpp
** Author( s ): Roberto Garcia Yunta
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
#include "egtgpsdatatablewidget.h"

#include <QMessageBox>

EgtGpsDataTableWidget::EgtGpsDataTableWidget( )
{
  cvColumnMeaningDialog = new QDialog( this );
  cvUiColumnMeaning.setupUi( cvColumnMeaningDialog );

  cvAvailableFields<<"Longitude"<<"Latitude"<<"Altitude"<<"TimeStamp"<<"( clear )";

  cvFileReader = 0;
  cvColumnSelected = 0;
  cvHeadersAreSet = false;

  cvMapItems = new QMap<QString,QString>;

  connect( this, SIGNAL( cellClicked( int, int ) ), this, SLOT( cell_selected( int, int ) ) );  

  cvHorizontalHeader = horizontalHeader( );
  connect( cvHorizontalHeader, SIGNAL( sectionClicked( int ) ), this, SLOT( cvHorizontalHeader_clicked( int ) ) ); 

  cvVerticalHeader = verticalHeader( );
  connect( cvVerticalHeader, SIGNAL( sectionClicked( int ) ), this, SLOT( cvVerticalHeader_clicked( int ) ) ); 

  connect( cvUiColumnMeaning.pbtnOk, SIGNAL( clicked( ) ), this, SLOT( on_pbtnOk_clicked( ) ) );
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */

QMap<QString,QString>* EgtGpsDataTableWidget::getRowItems( )
{
  return cvMapItems;
}

/*
 *
 * PRIVATE FUNCTIONS
 *
 */

bool EgtGpsDataTableWidget::isThereAnyColumnSet( )
{
  if( cvHeadersAreSet ) { return true; }

  bool lvReturn = false;
  for( int i = 0; i < columnCount( ); i++ )
  {
    lvReturn = lvReturn || cvHeadersThatAreSet.contains( i );
  } 

  return lvReturn;
}

void EgtGpsDataTableWidget::populateTable( )
{
  cvHeadersThatAreSet.clear( );
  QList<QStringList> lvDataFile = cvFileReader->read( );

  setRowCount( lvDataFile.size( ) );
  setColumnCount( lvDataFile[0].size( ) );
  /*Set up the combo box*/
  cvUiColumnMeaning.cbFields->clear( );
  cvUiColumnMeaning.cbFields->insertItems( 0,cvAvailableFields );
  /*Reset the Qlist that contains which headers are set*/
  cvHeadersThatAreSet.clear( );

  if( cvFileReader->hasColumnHeaders( ) ) 
  {
    QStringList lvTags;
    lvTags = cvFileReader->columnHeaders( );
    setHorizontalHeaderLabels( lvTags );
  }
  else
  {
    QStringList lvDefaultHeader;
 
    for( int i = 0; i< lvDataFile[0].size( ); i++ )
    {
      lvDefaultHeader << QString::number( i+1 );
    }

    setHorizontalHeaderLabels( lvDefaultHeader );
  }

  for( int i = 0; i < lvDataFile.size( ); i++ )
  {
    for( int j = 0; j < lvDataFile[0].size( ); j++ )
    {
      QTableWidgetItem *lvNewItem = new QTableWidgetItem( lvDataFile.at( i ).at( j ) );
       setItem( i, j, lvNewItem );
    }
  }
  emit timeStampSelected(false);
}


/*
 *
 * SIGNAL and SLOTS
 *
 */

void EgtGpsDataTableWidget::cell_selected( int row, int column )
{ 
  QTableWidgetItem * lvItem = item ( row, column );
  double x = lvItem->data( 0 ).toDouble( );
}

void EgtGpsDataTableWidget::cvHorizontalHeader_clicked( int theIndex )
{
  cvColumnSelected = theIndex;

  QTableWidgetItem* lvHeaderItem;
      
  QString lvText;
  lvHeaderItem = horizontalHeaderItem ( theIndex );
  lvText = lvHeaderItem->text( );

  bool ok;
  lvText.toInt( &ok ); 

  if( !ok ) //It's not a number, therefore, it is a valid field
  {
    cvUiColumnMeaning.cbFields->addItem( lvText );
  }

  cvColumnMeaningDialog->show( );
}

void EgtGpsDataTableWidget::cvVerticalHeader_clicked( int theIndex )
{ 
  delete cvMapItems;
  cvMapItems = new QMap<QString,QString>;
 
  if( isThereAnyColumnSet( ) )
  {
    QTableWidgetItem* lvHeaderItem;
    for( int lvColumnCount = 0; lvColumnCount < columnCount( ); lvColumnCount++ )
    {
      QString lvText;
      lvHeaderItem = horizontalHeaderItem ( lvColumnCount );
      lvText = lvHeaderItem->text( );
      if( lvText == "TimeStamp" )
      {  
         QString lvContent = item( theIndex, lvColumnCount )->text();
         //Format checking of the timestamp
        if( lvContent.size() != 8 )
        {
          QMessageBox::critical( this, tr( "Error" ),tr( "The timestamp has an incorrect format" ),QMessageBox::Ok );
          emit timeStampSelected(false);
          return;
        }
        bool lvExpectedFormat = true;
        lvExpectedFormat = lvExpectedFormat && ':' == lvContent[2];
        lvExpectedFormat = lvExpectedFormat && ':' == lvContent[5];

        for( int lvIterator = 0; lvIterator < lvContent.size( ); lvIterator++ )
        {
          if( !lvContent[ lvIterator ].isNumber( ) && ':' != lvContent[ lvIterator ] )
          {
            QMessageBox::critical( this, tr( "Error" ),tr( "The timestamp has an incorrect format" ),QMessageBox::Ok );
            emit timeStampSelected(false);
            return;
          }
        }
        //format checking end
      }

      bool ok;
      lvText.toInt( &ok ); 

      if( !ok )
      {
        cvMapItems->insert( lvText, item( theIndex, lvColumnCount )->text( ) );
      }
    }
  }
}

void EgtGpsDataTableWidget::on_pbtnOk_clicked( )
{   
  QString lvSelectedItem = cvUiColumnMeaning.cbFields->currentText( );
  
  int lvIndex = cvUiColumnMeaning.cbFields->currentIndex( );

  if( "( clear )" != lvSelectedItem )
  {
    cvUiColumnMeaning.cbFields->removeItem( lvIndex );
    cvHeadersThatAreSet<<cvColumnSelected;
    if( "TimeStamp" == lvSelectedItem )
    {
      emit timeStampSelected(true);
    }
  }
  else
  {
    lvSelectedItem = QString::number( cvColumnSelected +1 );
    cvHeadersThatAreSet.removeAll( cvColumnSelected );
  }

  if( cvUiColumnMeaning.cbFields->itemText( cvUiColumnMeaning.cbFields->count()-1 ) == "TimeStamp" && lvSelectedItem != "TimeStamp" )
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
  
  cvColumnMeaningDialog->close( );
}

void EgtGpsDataTableWidget::setFileReader( EgtFileReader* theFileReader )
{
  if( 0 != cvFileReader ) { delete cvFileReader; }

  cvFileReader = theFileReader;
  cvHeadersAreSet = cvFileReader->hasColumnHeaders( );

  populateTable( );
}
