/*
** File: egtsynchronizedialog.cpp
** Author( s ):Roberto Garcia Yunta
** Creation Date: 2009-05-01
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

#include "egtsynchronizedialog.h"
#include "egtlogger.h"

#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QDate>

EgtSynchronizeDialog::EgtSynchronizeDialog( QWidget* theParent )
    : QDialog( theParent ), ui( new Ui::EgtSynchronizeDialogBase )
{
  ui->setupUi( this );
  setWindowIcon( QIcon( ":/icons/internet-web-browser.svg" ) );

  ui->pbtnOpenPic->setIcon( QIcon(":/icons/document-open.svg") );
  ui->rbtnPositive->setChecked(true);

  //Set up the progress bar
  ui->pbarProgressBar->setMinimum( 0 );
  ui->pbarProgressBar->setMaximum( 1 );
  ui->pbarProgressBar->setValue( 0 );

  connect( ui->buttonBox, SIGNAL( accepted() ),this, SLOT( accepted() ) );
  connect( ui->pbtnOpenPic, SIGNAL( clicked() ),this, SLOT( openPic() ) );

  connect( &cvImageFactory, SIGNAL( progress( int, int, int ) ), this, SLOT( updateProgress( int, int, int ) ) );
  connect( &cvImageFactory, SIGNAL( imageLoaded( bool ) ), this, SLOT( updatePreview( bool ) ) );

  connect( ui->sbYear, SIGNAL( valueChanged( int ) ), this, SLOT( updateOffset() ) );
  connect( ui->sbMonth, SIGNAL( valueChanged( int ) ), this, SLOT( updateOffset() ) );
  connect( ui->sbDay, SIGNAL( valueChanged( int ) ), this, SLOT( updateOffset() ) );
  connect( ui->sbUserHours, SIGNAL( valueChanged( int ) ), this, SLOT( updateOffset() ) );
  connect( ui->sbUserMinutes, SIGNAL( valueChanged( int ) ), this, SLOT( updateOffset() ) );
  connect( ui->sbUserSeconds, SIGNAL( valueChanged( int ) ), this, SLOT( updateOffset() ) );
}

void EgtSynchronizeDialog::loadPreview()
{
  EgtDebug( "entered" );

  if( cvValidImage )
  {
    ui->labelPreview->setPixmap( QPixmap::fromImage( cvImageFactory.scaleImage( ui->labelPreview->width( ), ui->labelPreview->height( ) ) ) );

    cvPictureDateTimeStamp = cvPhotoExifEngine.read( "Egt.Photo.DateTimeOriginal" ).toString( );
    ui->labelTimeStamp->setText( cvPictureDateTimeStamp );

    ui->sbYear->setValue( cvPictureDateTimeStamp.mid(0,4).toInt() );
    ui->sbMonth->setValue( cvPictureDateTimeStamp.mid(5,2).toInt() );
    ui->sbDay->setValue( cvPictureDateTimeStamp.mid(8,2).toInt() );
    ui->sbUserHours->setValue( cvPictureDateTimeStamp.mid(11,2).toInt() );
    ui->sbUserMinutes->setValue( cvPictureDateTimeStamp.mid(14,2).toInt() );
    ui->sbUserSeconds->setValue( cvPictureDateTimeStamp.mid(17,2).toInt() );
  }
}

void EgtSynchronizeDialog::accepted()
{ 
  EgtDebug( "entered" );
  
  if( ui->groupBoxPreview->isHidden() ){ cvPictureDateTimeStamp = ""; }
  else{ cvPictureDateTimeStamp = cvPhotoExifEngine.read( "Egt.Photo.DateTimeOriginal" ).toString( ); }

  int lvOffset = ui->sbHours->value()*3600;
  lvOffset+= ui->sbMinutes->value()*60;
  lvOffset+= ui->sbSeconds->value();
  if( 99999 == cvOffset)
  {
    show();
    QMessageBox::critical( this, tr( "Error" ),tr( "The offset is greater than 24 hours" ),QMessageBox::Ok );
    cvOffset = 0;
    return;
  }

  if( ui->rbtnPositive->isChecked() )
  { 
    cvOffset = lvOffset;
  }
  else{ cvOffset = -1*lvOffset; }
  
  emit offsetSet( cvOffset, cvPictureDateTimeStamp );
}

void EgtSynchronizeDialog::closeWindow()
{
  EgtDebug( "entered" );

  close();
}
/*!
 * \param theIndex contains the index of the selected picture in the browser
 */
/*void EgtSynchronizeDialog::clicked( const QModelIndex& theIndex )
{
  QString lvFileName = cvPathBuilder.buildPath( theIndex );

  cvImageFactory.setFile( lvFileName );
  cvPhotoExifEngine.setFile( lvFileName );
 
  if( ui->groupBoxOffset->isEnabled() ){ updateOffset(); } //only necessary if syncrhonizing with pic
}*/

void EgtSynchronizeDialog::openPic()
{
  EgtDebug( "entered" );

  QString lvFileName = QFileDialog::getOpenFileName( this, tr( "Open Image File" ), "/home", tr( "Image Files (*)" ) );
  
  cvImageFactory.setFile( lvFileName );
  cvPhotoExifEngine.setFile( lvFileName );
  
  if( ui->groupBoxOffset->isEnabled() ){ updateOffset(); } //only necessary if syncrhonizing with pic
}

void EgtSynchronizeDialog::resizeEvent ( QResizeEvent * event ) 
{
  EgtDebug( "entered" );

  loadPreview(); 
}

/*!
 * \param isValid boolean that indicates if the image to preview is valid or not
 */
void EgtSynchronizeDialog::updatePreview( bool isValid )
{
  EgtDebug( "entered" );

  cvValidImage = isValid;
  loadPreview();
}

/*!
 * \param theMinimum the minumum value for the progress bar
 * \param theMaximum the maximum value for the progress bar
 * \param theProgress the current progress
 */
void EgtSynchronizeDialog::updateProgress( int theMinimum, int theMaximum, int theProgress )
{
  EgtDebug( "entered" );

  //TODO: consider if it is better to set these individually
  ui->pbarProgressBar->setMinimum( theMinimum );
  ui->pbarProgressBar->setMaximum( theMaximum );
  ui->pbarProgressBar->setValue( theProgress );
}
void EgtSynchronizeDialog::setApplicationInterface( EgtApplicationInterface* theAppInterface )
{
  EgtDebug( "entered" );

  cvApplicationInterface = theAppInterface;
}

void EgtSynchronizeDialog::setOffset()
{
  EgtDebug( "entered" );

  ui->groupBoxPreview->setVisible( false );
  ui->groupBoxOffset->setEnabled( true );

  move( cvApplicationInterface->positionOfFirstVisibleWidget( ) );
  show();
  adjustSize();
}

void EgtSynchronizeDialog::setOffsetPic()
{
  EgtDebug( "entered" );

  ui->groupBoxPreview->setVisible( true );
  ui->groupBoxOffset->setEnabled( false );

  move( cvApplicationInterface->positionOfFirstVisibleWidget( ) );
  show();
  adjustSize();
}

void EgtSynchronizeDialog::updateOffset()
{
  EgtDebug( "entered" );

  if( ui->groupBoxPreview->isHidden() ){ return; }

  QDate lvExifDate( cvPictureDateTimeStamp.mid(0,4).toInt(),cvPictureDateTimeStamp.mid(5,2).toInt(),cvPictureDateTimeStamp.mid(8,2).toInt() );
  QDate lvUserDate(ui->sbYear->value(),ui->sbMonth->value() , ui->sbDay->value() );

  QTime lvExifTime( cvPictureDateTimeStamp.mid(11,2).toInt(),cvPictureDateTimeStamp.mid(14,2).toInt(),cvPictureDateTimeStamp.mid(17,2).toInt() );
  QTime lvUserTime(ui->sbUserHours->value(),ui->sbUserMinutes->value() , ui->sbUserSeconds->value() );

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

    if( lvOffset != 99999  )
    {
      cvOffset = 0;

      if(lvOffset < 0)
      {
        ui->rbtnPositive->setChecked(false);
        ui->rbtnNegative->setChecked(true);
        lvOffset = -1*lvOffset;
      }
      else
      {
        ui->rbtnPositive->setChecked(true);
        ui->rbtnPositive->setChecked(false);
      }
      ui->sbHours->setValue(lvOffset/3600);
      ui->sbMinutes->setValue((lvOffset%3600)/60);
      ui->sbSeconds->setValue(((lvOffset%3600)%60));
    }
    else{ cvOffset = 99999; }
    
}



