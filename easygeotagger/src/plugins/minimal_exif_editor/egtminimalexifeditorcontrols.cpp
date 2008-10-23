/*
** File: egtminimalexifeditorcontrols.cpp
** Author(s): Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2008-10-17
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
#include "egtminimalexifeditorcontrols.h"
#include "egtlogger.h"

#include <QIcon>
#include <QPushButton>
#include <QMessageBox>

EgtMinimalExifEditorControls::EgtMinimalExifEditorControls( EgtExifIoEngine* theExifIoEngine, QWidget* theParent, Qt::WindowFlags theFlags ) : QWidget( theParent, theFlags )
{
  setupUi( this );
  
  cvExifIoEngine = theExifIoEngine;
  
  //setup longitude controls
  pbtnLongitudeSave->setMaximumSize( 20,20 );
  pbtnLongitudeSave->setToolTip( tr( "Save" ) );
  pbtnLongitudeSave->setIcon ( QIcon( ":/MINIMALEDITOR/save.png" ) );
  
  pbtnLongitudeCancel->setMaximumSize( 20,20 );
  pbtnLongitudeCancel->setToolTip( tr( "Discard changes" ) );
  pbtnLongitudeCancel->setIcon ( QIcon( ":/MINIMALEDITOR/cancel.png" ) );
  
  longitudeControls->setEnabled( false );
  
  //setup latitude controls
  pbtnLatitudeSave->setMaximumSize( 20,20 );
  pbtnLatitudeSave->setToolTip( tr( "Save" ) );
  pbtnLatitudeSave->setIcon ( QIcon( ":/MINIMALEDITOR/save.png" ) );
  
  pbtnLatitudeCancel->setMaximumSize( 20,20 );
  pbtnLatitudeCancel->setToolTip( tr( "Discard changes" ) );
  pbtnLatitudeCancel->setIcon ( QIcon( ":/MINIMALEDITOR/cancel.png" ) );
  
  latitudeControls->setEnabled( false );
}

/*
 *
 * PRIVATE SLOTS
 *
 */
 void EgtMinimalExifEditorControls::on_leLatitude_textEdited( QString theString )
{
  latitudeControls->setEnabled( true );
}

void EgtMinimalExifEditorControls::on_leLongitude_textEdited( QString theString )
{
  longitudeControls->setEnabled( true );
}

void EgtMinimalExifEditorControls::on_pbtnLatitudeCancel_clicked()
{
  if( 0 == cvExifIoEngine ) { return; }
  
  leLatitude->setText( QString::number( cvExifIoEngine->latitude(), 'f', 7  ) );
  latitudeControls->setEnabled( false );
}

void EgtMinimalExifEditorControls::on_pbtnLatitudeSave_clicked()
{
  if( 0 == cvExifIoEngine ) { return; }
  
  if( !cvExifIoEngine->writeLatitude( leLatitude->text() ) )
  {
    QMessageBox::critical( this, tr( "Write Error" ), tr( "Unable to write value into exif header" ) ); 
  }
  else
  {
    leLatitude->setText( QString::number( cvExifIoEngine->latitude(), 'f', 7  ) );
    latitudeControls->setEnabled( false );
  }
}

void EgtMinimalExifEditorControls::on_pbtnLongitudeCancel_clicked()
{
  if( 0 == cvExifIoEngine ) { return; }
  
  leLongitude->setText( QString::number( cvExifIoEngine->longitude(), 'f', 7  ) );
  longitudeControls->setEnabled( false );
}

void EgtMinimalExifEditorControls::on_pbtnLongitudeSave_clicked()
{
  if( 0 == cvExifIoEngine ) { return; }
  
  if( !cvExifIoEngine->writeLongitude( leLongitude->text() ) )
  {
    QMessageBox::critical( this, tr( "Write Error" ), tr( "Unable to write value into exif header" ) ); 
  }
  else
  {
    leLongitude->setText( QString::number( cvExifIoEngine->longitude(), 'f', 7  ) );
    longitudeControls->setEnabled( false );
  }
}

