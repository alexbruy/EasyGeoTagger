<<<<<<< .working
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


=======
/*
** File: egtgpsdisplaywidget.cpp
** Author( s ): Roberto Garcia Yunta
** Creation Date: 2009-03-20
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
#include "egtmainwindow.h"

#include <QMessageBox>
#include <QApplication>
#include <QModelIndex>
#include <QFileDialog>
#include <QDate>

EgtGpsDisplayWidget::EgtGpsDisplayWidget( QWidget* theParent )
    : QWidget( theParent ), ui( new Ui::EgtGpsDisplayWidgetBase )
{
  ui->setupUi( this );
  setWindowIcon( QIcon( ":/icons/internet-web-browser.svg" ) );
  
  cvDataTable = new EgtGpsDataTableWidget();
  cvOffset = 0;
  cvPictureDateTimeStamp = "";
  cvValidImage = false;

  ui->gbTimeOffset->setEnabled( false );
  ui->pbtnSendCoordinates->setEnabled( false );
  ui->pbtnDeleteRow->setEnabled( false );

  QVBoxLayout* lvLayout = new QVBoxLayout;
  ui->widgetTable->setLayout( lvLayout );
  lvLayout->addWidget(cvDataTable);
  lvLayout->setContentsMargins( 1, 1, 1, 1 );

  cvSynchronizeDialog = new Ui::ExtendedQDialog( this );
  cvSynchronizeUi = new Ui::EgtSynchronizeDialog();
  cvSynchronizeUi->setupUi( cvSynchronizeDialog );
  cvSynchronizeUi->pbtnOpenPic->setIcon( QIcon(":/icons/document-open.svg") );
  cvSynchronizeUi->rbtnPositive->setChecked(true);

  //Set up the progress bar
  cvSynchronizeUi->pbarProgressBar->setMinimum( 0 );
  cvSynchronizeUi->pbarProgressBar->setMaximum( 1 );
  cvSynchronizeUi->pbarProgressBar->setValue( 0 );

  connect( ui->pbtnSendCoordinates, SIGNAL( clicked( ) ), this, SLOT( sendCoordinates( ) ) );
  connect( ui->pbtnOpenFile, SIGNAL( clicked( ) ), this, SLOT( openFile( ) ) );
  connect( ui->pbtnOffsetManual, SIGNAL( clicked( ) ), this, SLOT( setOffset( ) ) );
  connect( ui->pbtnOffsetPic, SIGNAL( clicked( ) ), this, SLOT( setOffsetPic( ) ) );
  connect( &cvReaderFactory, SIGNAL( fileReaderCreated( EgtFileReader* ) ),this, SLOT( fileReader_set( EgtFileReader* ) ) );
  connect( cvSynchronizeUi->buttonBox, SIGNAL( accepted() ),this, SLOT( accepted() ) );
  connect( &cvImageFactory, SIGNAL( imageLoaded( bool ) ), this, SLOT( updatePreview( bool ) ) );
  connect( cvDataTable, SIGNAL( timeStampSelected( bool ) ), this, SLOT( setSynchronizing(bool) ) );
  connect( cvSynchronizeUi->pbtnOpenPic, SIGNAL( clicked() ),this, SLOT( openPic() ) );
  connect( ui->pbtnDeleteRow, SIGNAL( clicked() ),cvDataTable, SLOT( deleteRow() ) );

  connect( &cvImageFactory, SIGNAL( progress( int, int, int ) ), this, SLOT( updateProgress( int, int, int ) ) );

  connect( cvDataTable, SIGNAL( rowSelected( bool ) ), this, SLOT( setSendToEditorButton( bool ) ) );

  connect( cvSynchronizeUi->sbYear, SIGNAL( valueChanged( int ) ), this, SLOT( updateOffset() ) );
  connect( cvSynchronizeUi->sbMonth, SIGNAL( valueChanged( int ) ), this, SLOT( updateOffset() ) );
  connect( cvSynchronizeUi->sbDay, SIGNAL( valueChanged( int ) ), this, SLOT( updateOffset() ) );
  connect( cvSynchronizeUi->sbUserHours, SIGNAL( valueChanged( int ) ), this, SLOT( updateOffset() ) );
  connect( cvSynchronizeUi->sbUserMinutes, SIGNAL( valueChanged( int ) ), this, SLOT( updateOffset() ) );
  connect( cvSynchronizeUi->sbUserSeconds, SIGNAL( valueChanged( int ) ), this, SLOT( updateOffset() ) );
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */
/*!
 * \param theEvent event received when the user closes the window
 */
void EgtGpsDisplayWidget::closeEvent(QCloseEvent *theEvent)
 {
    cvSynchronizeDialog->close();
    theEvent->accept();
 }

void EgtGpsDisplayWidget::loadPreview()
{
  if( cvValidImage )
  {
    cvSynchronizeUi->labelPreview->setPixmap( QPixmap::fromImage( cvImageFactory.scaleImage( cvSynchronizeUi->labelPreview->width( ), cvSynchronizeUi->labelPreview->height( ) ) ) );

    cvPictureDateTimeStamp = cvPhotoExifEngine.read( "Egt.Photo.DateTimeOriginal" ).toString( );
    cvSynchronizeUi->labelTimeStamp->setText( cvPictureDateTimeStamp );

    cvSynchronizeUi->sbYear->setValue( cvPictureDateTimeStamp.mid(0,4).toInt() );
    cvSynchronizeUi->sbMonth->setValue( cvPictureDateTimeStamp.mid(5,2).toInt() );
    cvSynchronizeUi->sbDay->setValue( cvPictureDateTimeStamp.mid(8,2).toInt() );
    cvSynchronizeUi->sbUserHours->setValue( cvPictureDateTimeStamp.mid(11,2).toInt() );
    cvSynchronizeUi->sbUserMinutes->setValue( cvPictureDateTimeStamp.mid(14,2).toInt() );
    cvSynchronizeUi->sbUserSeconds->setValue( cvPictureDateTimeStamp.mid(17,2).toInt() );
  }
}

/*!
 * \param theAppInterface contains the actual application interface used to comunicate with the main app.
 */
void EgtGpsDisplayWidget::setApplicationInterface( EgtApplicationInterface* theAppInterface )
{
  cvApplicationInterface = theAppInterface;
  connect( cvApplicationInterface, SIGNAL( indexSelected( const QModelIndex& ) ), this, SLOT( clicked( const QModelIndex& ) ) );
}


/*
 *
 * SIGNAL and SLOTS
 *
 */
void EgtGpsDisplayWidget::accepted()
{ 
  if( cvSynchronizeUi->groupBoxPreview->isHidden() ){ cvPictureDateTimeStamp = ""; }
  else{ cvPictureDateTimeStamp = cvPhotoExifEngine.read( "Egt.Photo.DateTimeOriginal" ).toString( ); }

  int lvOffset = cvSynchronizeUi->sbHours->value()*3600;
  lvOffset+= cvSynchronizeUi->sbMinutes->value()*60;
  lvOffset+= cvSynchronizeUi->sbSeconds->value();
  if( 99999 == cvOffset)
  {
    cvSynchronizeDialog->show();
    QMessageBox::critical( cvSynchronizeDialog, tr( "Error" ),tr( "The offset is greater than 24 hours" ),QMessageBox::Ok );
    cvOffset = 0;
    return;
  }

  if( cvSynchronizeUi->rbtnPositive->isChecked() )
  { 
    cvOffset = lvOffset;
  }
  else{ cvOffset = -1*lvOffset; }
}

/*!
 * \param theIndex contains the index of the selected picture in the browser
 */
void EgtGpsDisplayWidget::clicked( const QModelIndex& theIndex )
{
  QString lvFileName = cvPathBuilder.buildPath( theIndex );

  cvImageFactory.setFile( lvFileName );
  cvPhotoExifEngine.setFile( lvFileName );
 
  if( cvSynchronizeUi->groupBoxOffset->isEnabled() ){ updateOffset(); } //only necessary if syncrhonizing with pic
}

/*!
 * \param theFileReader Pointer to the actual file reader
 */
void EgtGpsDisplayWidget::fileReader_set( EgtFileReader* theFileReader )
{
  cvDataTable->setFileReader( theFileReader );
}

void EgtGpsDisplayWidget::openFile( )
{
  cvReaderFactory.show( );
}

void EgtGpsDisplayWidget::openPic()
{
  QString lvFileName = QFileDialog::getOpenFileName( cvSynchronizeDialog, tr( "Open Image File" ), "/home", tr( "Image Files (*)" ) );
  
  cvImageFactory.setFile( lvFileName );
  cvPhotoExifEngine.setFile( lvFileName );
  
  if( cvSynchronizeUi->groupBoxOffset->isEnabled() ){ updateOffset(); } //only necessary if syncrhonizing with pic
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
    QMessageBox::critical( cvDataTable, tr( "Error" ),tr( "At least one header must be set" ),QMessageBox::Ok );
  }

}

void EgtGpsDisplayWidget::setOffset()
{
  cvSynchronizeUi->groupBoxPreview->setVisible( false );
  cvSynchronizeUi->groupBoxOffset->setEnabled( true );

  cvSynchronizeDialog->move( cvApplicationInterface->positionOfFirstVisibleWidget( ) );
  cvSynchronizeDialog->show();
  cvSynchronizeDialog->adjustSize();
}

void EgtGpsDisplayWidget::setOffsetPic()
{
  cvSynchronizeUi->groupBoxPreview->setVisible( true );
  cvSynchronizeUi->groupBoxOffset->setEnabled( false );

  cvSynchronizeDialog->move( cvApplicationInterface->positionOfFirstVisibleWidget( ) );
  cvSynchronizeDialog->show();
  cvSynchronizeDialog->adjustSize();
}

/*!
 * \param theStatus boolean that indicates whether the "send to editor" button must be enabled or not
 */
void EgtGpsDisplayWidget::setSendToEditorButton( bool theStatus )
{ 
  ui->pbtnSendCoordinates->setEnabled( theStatus );
  ui->pbtnDeleteRow->setEnabled( theStatus );

  QMap<QString,QString>* lvMap = cvDataTable->getRowItems( );
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
          ui->pbtnSendCoordinates->setEnabled( false );
          QMessageBox::critical( cvDataTable, tr( "Error" ),tr( "The dates of the picture and the row don't match" ),QMessageBox::Ok );
        }
      }
    }
  }
}

/*!
 * \param theStatus boolean that indicates whether the syncrhonize group box must be enabled or not
 */
void EgtGpsDisplayWidget::setSynchronizing( bool theStatus )
{
  ui->gbTimeOffset->setEnabled(theStatus);
}

void EgtGpsDisplayWidget::updateOffset()
{
  QDate lvExifDate( cvPictureDateTimeStamp.mid(0,4).toInt(),cvPictureDateTimeStamp.mid(5,2).toInt(),cvPictureDateTimeStamp.mid(8,2).toInt() );
  QDate lvUserDate(cvSynchronizeUi->sbYear->value(),cvSynchronizeUi->sbMonth->value() , cvSynchronizeUi->sbDay->value() );

  QTime lvExifTime( cvPictureDateTimeStamp.mid(11,2).toInt(),cvPictureDateTimeStamp.mid(14,2).toInt(),cvPictureDateTimeStamp.mid(17,2).toInt() );
  QTime lvUserTime(cvSynchronizeUi->sbUserHours->value(),cvSynchronizeUi->sbUserMinutes->value() , cvSynchronizeUi->sbUserSeconds->value() );

  int lvOffset = 99999;
  
    if( 1 == lvExifDate.daysTo( lvUserDate) )
    {
      if( lvExifTime.secsTo( lvUserTime ) < 0){ lvOffset =86400+ lvExifTime.secsTo( lvUserTime ); }
      else{ lvOffset = 99999; }
    }
    else
    if( -1 == lvExifDate.daysTo( lvUserDate) )
    {
      if( lvExifTime.secsTo( lvUserTime ) > 0 ){ lvOffset = 86400 - lvExifTime.secsTo( lvUserTime ); }
      else{ lvOffset = 99999; }
    }
    else
    if( 0 == lvExifDate.daysTo( lvUserDate) )
    {
      lvOffset = lvExifTime.secsTo( lvUserTime );
    }
    else{ lvOffset = 99999; }

    if( lvOffset != 99999 )
    {
      cvOffset = 0;

      if(lvOffset < 0)
      {
        cvSynchronizeUi->rbtnPositive->setChecked(false);
        cvSynchronizeUi->rbtnNegative->setChecked(true);
        lvOffset = -1*lvOffset;
      }
      else
      {
        cvSynchronizeUi->rbtnPositive->setChecked(true);
        cvSynchronizeUi->rbtnPositive->setChecked(false);
      }
      cvSynchronizeUi->sbHours->setValue(lvOffset/3600);
      cvSynchronizeUi->sbMinutes->setValue((lvOffset%3600)/60);
      cvSynchronizeUi->sbSeconds->setValue(((lvOffset%3600)%60));
      


    }
    else{ cvOffset = 99999; }
    
    
}

/*!
 * \param isValid boolean that indicates if the image to preview is valid or not
 */
void EgtGpsDisplayWidget::updatePreview( bool isValid )
{
  cvValidImage = isValid;
  loadPreview();
}

/*!
 * \param theMinimum the minumum value for the progress bar
 * \param theMaximum the maximum value for the progress bar
 * \param theProgress the current progress
 */
void EgtGpsDisplayWidget::updateProgress( int theMinimum, int theMaximum, int theProgress )
{
  //TODO: consider if it is better to set these individually
  cvSynchronizeUi->pbarProgressBar->setMinimum( theMinimum );
  cvSynchronizeUi->pbarProgressBar->setMaximum( theMaximum );
  cvSynchronizeUi->pbarProgressBar->setValue( theProgress );
}



>>>>>>> .merge-right.r262
