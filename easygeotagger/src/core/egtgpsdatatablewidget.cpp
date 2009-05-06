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
#include "egtlogger.h"

#include <QMessageBox>
#include <QMenu>
#include <QDate>

EgtGpsDataTableWidget::EgtGpsDataTableWidget( )
{
  cvColumnMeaningDialog = new QDialog( this );
  cvUiKeySelectionDialog.setupUi( cvColumnMeaningDialog );

  cvAvailableFields<<"( clear )"<<"Altitude"<<"DateTimeStamp"<<"Latitude"<<"Longitude";

  cvDataProvider = 0;
  cvColumnSelected = 0;
  cvHeadersAreSet = false;

  cvOffset= 0;
  cvPictureDateTimeStamp = "";
  cvMapItems = new QMap<QString,QString>;

  connect( this, SIGNAL( cellClicked( int, int ) ), this, SLOT( cell_selected( int, int ) ) );  

  cvHorizontalHeader = horizontalHeader( );
  connect( cvHorizontalHeader, SIGNAL( sectionClicked( int ) ), this, SLOT( cvHorizontalHeader_clicked( int ) ) ); 

  cvVerticalHeader = verticalHeader() ;
  
  cvVerticalHeader->setContextMenuPolicy(Qt::CustomContextMenu); //to enable the customContextMenuRequested signal
  connect( cvVerticalHeader, SIGNAL( customContextMenuRequested(QPoint) ), this, SLOT( popUpMenu( QPoint ) ) ); 

  connect( cvVerticalHeader, SIGNAL( sectionClicked(int) ), this, SLOT( cvVerticalHeader_clicked( int ) ) ); 

  connect( cvUiKeySelectionDialog.pbtnOk, SIGNAL( clicked( ) ), this, SLOT( on_pbtnOk_clicked( ) ) );
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */

QMap<QString,QString>* EgtGpsDataTableWidget::getRowItems( )
{
  EgtDebug( "entered" );

  return cvMapItems;
}

/*
 *
 * PRIVATE FUNCTIONS
 *
 */

bool EgtGpsDataTableWidget::isThereAnyColumnSet( )
{
  EgtDebug( "entered" );

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
  sendToEditor( false );
}


/*
 *
 * SIGNAL and SLOTS
 *
 */

void EgtGpsDataTableWidget::cell_selected( int row, int column )
{ 
  EgtDebug( "entered" );

  QTableWidgetItem * lvItem = item ( row, column );
  double x = lvItem->data( 0 ).toDouble( );
   sendToEditor(false);
}

void EgtGpsDataTableWidget::cvHorizontalHeader_clicked( int theIndex )
{
  EgtDebug( "entered" );

  cvColumnSelected = theIndex;

  QTableWidgetItem* lvHeaderItem;
      
  QString lvText;
  lvHeaderItem = horizontalHeaderItem ( theIndex );
  lvText = lvHeaderItem->text( );

  bool ok;
  lvText.toInt( &ok ); 

 /* if( !ok ) //It's not a number, therefore, it is a valid field
  {
    cvUiKeySelectionDialog.cbFields->addItem( lvText );
  }*/

  cvColumnMeaningDialog->show( );
}

void EgtGpsDataTableWidget::cvVerticalHeader_clicked( int theIndex )
{ 
  EgtDebug( "entered" );

  cvSelectedRow = theIndex;
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
      if( lvText == "DateTimeStamp" )
      {  
         QString lvContent = item( theIndex, lvColumnCount )->text();
         //Format checking of the timestamp
        if( lvContent.size() != 19 )
        {
          QMessageBox::critical( this, tr( "Error" ),tr( "The datetimestamp has an incorrect format.\n The right format is:\n yyyy:mm:dd hh:mm:ss" ),QMessageBox::Ok );
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
            QMessageBox::critical( this, tr( "Error" ),tr( "The datetimestamp has an incorrect format.\n The right format is:\n yyyy:mm:dd hh:mm:ss" ),QMessageBox::Ok ); 
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
        cvMapItems->insert( lvText, item( theIndex, lvColumnCount )->text( ) );
      }
    }
    sendToEditor(true);
  }
}

void EgtGpsDataTableWidget::deleteRow()
{ 
  EgtDebug( "entered" );
  removeRow(cvSelectedRow);
}

void EgtGpsDataTableWidget::on_pbtnOk_clicked( )
{  
  EgtDebug( "entered" );
  QString lvSelectedItem = cvUiKeySelectionDialog.cbFields->currentText( );
  
  int lvIndex = cvUiKeySelectionDialog.cbFields->currentIndex( );

  if( "( clear )" != lvSelectedItem )
  {
    //cvUiKeySelectionDialog.cbFields->removeItem( lvIndex );
    cvHeadersThatAreSet<<cvColumnSelected;
    if( "DateTimeStamp" == lvSelectedItem )
    {
      emit timeStampSelected(true);
    }
    else { emit timeStampSelected(false); }
  }
  else
  {
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

  if( cvUiKeySelectionDialog.cbFields->itemText( cvUiKeySelectionDialog.cbFields->count()-1 ) == "DateTimeStamp" && lvSelectedItem != "DateTimeStamp" )
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

void EgtGpsDataTableWidget::popUpMenu(QPoint theCoordinates)
{
  EgtDebug( "entered" );

  QMenu menu;
  QAction* lvDeleteAction = new QAction( tr("&Delete row..."), this );
  lvDeleteAction->setStatusTip(tr("Delete the selected row"));
  cvSelectedRow = cvVerticalHeader->logicalIndexAt( theCoordinates );
  connect( lvDeleteAction, SIGNAL( triggered() ), this, SLOT( deleteRow() ) );
     
  menu.addAction(lvDeleteAction);

  menu.exec(mapToGlobal(theCoordinates));
}

void EgtGpsDataTableWidget::sendCoordinates( ) 
{
  EgtDebug( "entered" );

  if( isThereAnyColumnSet( ) )
  {
    QMap<QString,QString>* lvMap = getRowItems( );

    QMapIterator<QString, QString> lvMapIterator( *lvMap );
    while ( lvMapIterator.hasNext( ) )
    {
      lvMapIterator.next( );
      if(lvMapIterator.key( ) == "DateTimeStamp")
      {
        QString lvValue = lvMapIterator.value();

        int lvCarryOut = 0;

        int lvSeconds = lvValue[17].digitValue()*10;
        lvSeconds+= lvValue[18].digitValue();
        lvSeconds+=( ( cvOffset%3600 )%60 );

        lvCarryOut = lvSeconds/60;
        lvSeconds = lvSeconds%60;
        if( lvSeconds< 0 )
        { 
          lvSeconds+= 60; 
          lvCarryOut-= 1; 
        }

        int lvMinutes = lvValue[14].digitValue()*10;
        lvMinutes+= lvValue[15].digitValue();
        lvMinutes+=(cvOffset%3600)/60;
        lvMinutes+= lvCarryOut;

        lvCarryOut = lvMinutes/60;
        lvMinutes = lvMinutes%60;
        if( lvMinutes< 0 )
        { 
          lvMinutes+= 60; 
          lvCarryOut-= 1; 
        }

        int lvHours = lvValue[11].digitValue()*10;
        lvHours+= lvValue[12].digitValue();
        lvHours+= cvOffset/3600;
        lvHours+= lvCarryOut;

        lvCarryOut = lvHours/24;
        lvHours = lvHours%24;
        if( lvHours< 0 )
        { 
          lvHours+= 60; 
          lvCarryOut-= 1; 
        }

        int lvDay = lvValue[8].digitValue()*10;
        lvDay+= lvValue[9].digitValue();

        int lvMonth = lvValue[5].digitValue()*10;
        lvMonth+= lvValue[6].digitValue();

        int lvYear = lvValue[0].digitValue()*1000;
        lvYear+= lvValue[1].digitValue()*100;
        lvYear+= lvValue[2].digitValue()*10;
        lvYear+= lvValue[3].digitValue();

        QDate lvDate( lvYear, lvMonth, lvDay ); 
        lvDate = lvDate.addDays(lvCarryOut);

        QString lvStringHours = lvHours < 10? "0"+QString::number( lvHours ):QString::number( lvHours );
        QString lvStringMinutes = lvMinutes < 10? "0"+QString::number( lvMinutes ):QString::number( lvMinutes );
        QString lvStringSeconds= lvSeconds < 10? "0"+QString::number( lvSeconds ):QString::number( lvSeconds );

        QString lvStringYear = QString::number( lvDate.year() );
        QString lvStringMonth = lvDate.month() < 10? "0"+QString::number( lvDate.month() ): QString::number( lvDate.month() );
        QString lvStringDay = lvDate.day() < 10? "0"+QString::number( lvDate.day() ): QString::number( lvDate.day() );

        emit( keyValuePair( "Egt.GPS.TimeStamp",lvStringHours+":"+lvStringMinutes+":"+lvStringSeconds ) );
        emit( keyValuePair( "Egt.GPS.DateStamp",lvStringYear+":"+lvStringMonth+":"+lvStringDay ) );
      }
      else
      {
        emit( keyValuePair( "Egt.GPS."+lvMapIterator.key( ),lvMapIterator.value( ) ) );
      }
      
    }
  }
  else
  {
    QMessageBox::critical( this, tr( "Error" ),tr( "At least one header must be set" ),QMessageBox::Ok );
  }

}

/*!
 * \param theStatus boolean that indicates whether the "send to editor" button must be enabled or not
 */
void EgtGpsDataTableWidget::sendToEditor( bool theStatus )
{ 
  EgtDebug( "entered" );
  /*ui->pbtnSendCoordinates->setEnabled( theStatus );
  ui->pbtnDeleteRow->setEnabled( theStatus );*/
  emit displayButtonsStatus( theStatus, theStatus );

  QMap<QString,QString>* lvMap = getRowItems( );
  if( theStatus && cvPictureDateTimeStamp != "" )
  {
    QMapIterator<QString, QString> lvMapIterator( *lvMap );
    while ( lvMapIterator.hasNext( ) )
    {
      lvMapIterator.next( );
      if(lvMapIterator.key( ) == "DateTimeStamp" )
      {
        QString lvValue = lvMapIterator.value();
        QDate lvPictureDate( lvValue.mid(0,4).toInt(),lvValue.mid(5,2).toInt(),lvValue.mid(8,2).toInt() );
        QDate lvTableDate( cvPictureDateTimeStamp.mid(0,4).toInt(),cvPictureDateTimeStamp.mid(5,2).toInt(),cvPictureDateTimeStamp.mid(8,2).toInt() );
        if( abs(lvTableDate.daysTo( lvPictureDate )) > 1 )
        {
          //ui->pbtnSendCoordinates->setEnabled( false );
          emit displayButtonsStatus( false, theStatus ); 
          QMessageBox::critical( this, tr( "Error" ),tr( "The dates of the picture and the row don't match" ),QMessageBox::Ok );
        }
      }
    }
  }
}

void EgtGpsDataTableWidget::setOffset( int theOffset )
{
  EgtDebug( "entered" ); 
 
  cvOffset = theOffset;
}

void EgtGpsDataTableWidget::setOffsetAndTimeStamp( int theOffset, QString theDateTimeStamp )
{
  EgtDebug( "entered" );
  
  cvOffset = theOffset;
  cvPictureDateTimeStamp = theDateTimeStamp;
}
void EgtGpsDataTableWidget::setPictureDateTimeStamp( QString theDateTimeStamp )
{
  EgtDebug( "entered" );

  cvPictureDateTimeStamp = theDateTimeStamp;
}

void EgtGpsDataTableWidget::setProvider( EgtDataProvider* theProvider )
{
  EgtDebug( "entered" );

  if( 0 != cvDataProvider ) { delete cvDataProvider; }

  cvDataProvider = theProvider;
  cvHeadersAreSet = cvDataProvider->hasColumnHeaders( );

  populateTable( );
}



