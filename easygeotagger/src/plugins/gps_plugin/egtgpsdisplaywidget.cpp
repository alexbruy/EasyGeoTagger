/*
** File: 
** Author( s ):
** Creation Date:
**
** Copyright ( c ) 2009, American Museum of Natural History. All rights reserved.
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
#include "egtgpsdisplaywidget.h"

#include "egtlogger.h"
#include "ui_egtgpsdisplaywidgetbase.h"
#include "ui_egtsynchronize.h"
#include "egtmainwindow.h"

#include <QMessageBox>
#include <QApplication>
#include <QModelIndex>

EgtGpsDisplayWidget::EgtGpsDisplayWidget( QWidget* theParent )
    : QWidget( theParent ), ui( new Ui::EgtGpsDisplayWidgetBase )
{
  ui->setupUi( this );
  setWindowIcon( QIcon( ":/icons/internet-web-browser.svg" ) );
  cvDataTable = new EgtGpsDataTableWidget();
  cvOffset = 0;
  ui->gbTimeOffset->setEnabled(false);

  QVBoxLayout* lvLayout = new QVBoxLayout;
  ui->widgetTable->setLayout( lvLayout );
  lvLayout->addWidget(cvDataTable);

  cvSynchronizeDialog = new QDialog();
  cvSynchronizeUi = new Ui::EgtSynchronizeDialog();
  cvSynchronizeUi->setupUi( cvSynchronizeDialog );
   
  connect( ui->pbtnSendCoordinates, SIGNAL( clicked( ) ), this, SLOT( sendCoordinates( ) ) );
  connect( ui->pbtnOpenFile, SIGNAL( clicked( ) ), this, SLOT( openFile( ) ) );
  connect( ui->pbtnOffsetManual, SIGNAL( clicked( ) ), this, SLOT( setOffset( ) ) );
  connect( ui->pbtnOffsetPic, SIGNAL( clicked( ) ), this, SLOT( setOffsetPic( ) ) );
  connect( &cvReaderFactory, SIGNAL( fileReaderCreated( EgtFileReader* ) ),this, SLOT( fileReader_set( EgtFileReader* ) ) );
  connect( cvSynchronizeUi->buttonBox, SIGNAL( accepted() ),this, SLOT( accepted() ) );
  connect( &cvImageFactory, SIGNAL( imageLoaded( bool ) ), this, SLOT( loadPreview( bool ) ) );
  connect( cvDataTable, SIGNAL( timeStampSelected( bool ) ), this, SLOT( setSynchronizing(bool) ) );
}

void EgtGpsDisplayWidget::setApplicationInterface( EgtApplicationInterface* theAppInterface )
{
  cvApplicationInterface = theAppInterface;
  connect( cvApplicationInterface, SIGNAL( indexSelected( const QModelIndex& ) ), this, SLOT( clicked( const QModelIndex& ) ) );
}

void EgtGpsDisplayWidget::accepted()
{
  int lvOffset = cvSynchronizeUi->sbHours->value()*3600;
  lvOffset+= cvSynchronizeUi->sbMinutes->value()*60;
  lvOffset+= cvSynchronizeUi->sbSeconds->value();
  if( lvOffset >= 24*3600)
  {
    QMessageBox::critical( cvDataTable, tr( "Error" ),tr( "The offset is more than 24 hours" ),QMessageBox::Ok );
    return;
  }
  cvOffset = lvOffset;
}

void EgtGpsDisplayWidget::clicked( const QModelIndex& theIndex )
{
  EgtDebug( "entered" );
  QString lvFileName = cvPathBuilder.buildPath( theIndex );

  cvImageFactory.setFile( lvFileName );

  cvPhotoExifEngine.setFile( lvFileName );
  cvSynchronizeUi->labelTimeStamp->setText( cvPhotoExifEngine.read( "Egt.Photo.DateTimeOriginal" ).toString( ) );

}

void EgtGpsDisplayWidget::fileReader_set( EgtFileReader* theFileReader )
{
  cvDataTable->setFileReader( theFileReader );
}

void EgtGpsDisplayWidget::loadPreview( bool isValid )
{
  EgtDebug( "entered" );
  
  if( isValid )
  {
    cvSynchronizeUi->labelPreview->setPixmap( QPixmap::fromImage( cvImageFactory.scaleImage( cvSynchronizeUi->labelPreview->width( ), cvSynchronizeUi->labelPreview->height( ) ) ) );
  }
}

void EgtGpsDisplayWidget::openFile( )
{
  cvReaderFactory.show( );
}

void EgtGpsDisplayWidget::sendCoordinates( ) 
{
  if( cvDataTable->isThereAnyColumnSet( ) )
  {
    QMap<QString,QString>* lvMap = cvDataTable->getRowItems( );

    QMapIterator<QString, QString> lvMapIterator( *lvMap );
    while ( lvMapIterator.hasNext( ) )
    {
      lvMapIterator.next( );
      if(lvMapIterator.key( ) == "TimeStamp")
      {
        QString lvValue = lvMapIterator.value();

        int lvHours = lvValue[0].digitValue()*10;
        lvHours+= lvValue[1].digitValue();
        lvHours+= cvOffset/3600;
        int lvMinutes = lvValue[3].digitValue()*10;
        lvMinutes+= lvValue[4].digitValue();
        lvMinutes+=(cvOffset%3600)/60;
        int lvSeconds = lvValue[6].digitValue()*10;
        lvSeconds+= lvValue[7].digitValue();
        lvSeconds+=((cvOffset%3600)%60);

        QString lvStringHours = lvHours < 10? "0"+QString::number( lvHours ):QString::number( lvHours );
        QString lvStringMinutes = lvMinutes < 10? "0"+QString::number( lvMinutes ):QString::number( lvMinutes );
        QString lvStringSeconds= lvSeconds < 10? "0"+QString::number( lvSeconds ):QString::number( lvSeconds );

        emit( keyValuePair( "Egt.GPS."+lvMapIterator.key( ),lvStringHours+":"+lvStringMinutes+":"+lvStringSeconds ) );
      }
      else
      {
        emit( keyValuePair( "Egt.GPS."+lvMapIterator.key( ),lvMapIterator.value( ) ) );
      }
      
    }
  }
  else
  {
    QMessageBox::critical( cvDataTable, tr( "Error" ),tr( "At least one header must be set" ),QMessageBox::Ok );
  }

}

void EgtGpsDisplayWidget::setOffset()
{
  cvSynchronizeUi->groupBoxPreview->setVisible(false);
  cvSynchronizeDialog->layout();
  cvSynchronizeDialog->setMaximumWidth ( 400 );
  cvSynchronizeDialog->setMaximumHeight ( 100 );

  cvSynchronizeDialog->move( cvApplicationInterface->positionOfFirstVisibleWidget( ) );
  cvSynchronizeDialog->show();
}

void EgtGpsDisplayWidget::setOffsetPic()
{
  cvSynchronizeUi->groupBoxPreview->setVisible(true);
  /*cvSynchronizeDialog->setMaximumWidth ( 477 );
  cvSynchronizeDialog->setMaximumHeight ( 580 );*/ 

  cvSynchronizeDialog->move( cvApplicationInterface->positionOfFirstVisibleWidget( ) );
  cvSynchronizeDialog->show();
}

void EgtGpsDisplayWidget::setSynchronizing( bool theStatus )
{
  ui->gbTimeOffset->setEnabled(theStatus);
}


