/*
** File: egtsaveasjpg.h
** Author(s): Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2009-02-26
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
#include "egtsaveasjpg.h"

#include "egtlogger.h"

#include <exiv2/image.hpp>
#include <exiv2/exif.hpp>

#include <QDir>
#include <QtPlugin>
#include <QFileInfo>
#include <QVBoxLayout>
#include <QDialogButtonBox>


EgtSaveAsJpg::EgtSaveAsJpg()
{
  cvCategories = QObject::tr( "Utilities" );
  cvDescription = QObject::tr( "Save the currently selected image as a JPG and copy all EXIF data" );
  cvName = QObject::tr( "Save as JPG" );

  cvProgressDialog.setLayout( new QVBoxLayout() );
  cvProgressDialog.layout()->setSpacing( 0 );
  cvProgressDialog.layout()->setContentsMargins( 2, 2, 2, 2 );
  cvProgressDialog.layout()->setSpacing( 2 );
  cvProgressDialog.setWindowIcon( QIcon( ":/icons/document-save.svg" ) );
  cvProgressDialog.setWindowTitle( tr( "Save as JPG" ) );

  cvOutputConsole.setMaximumHeight( 100 );
  cvOutputConsole.setMinimumWidth( 500 );
  cvProgressDialog.layout()->addWidget( &cvOutputConsole );

  cvProgressBar.setMaximumHeight( 20 );
  cvProgressBar.setMinimumWidth( 300 );
  cvProgressBar.setTextVisible( false );
  cvProgressDialog.layout()->addWidget( &cvProgressBar );

  //Add a close button to the configuration dialog -- probably should be a button box.
  QDialogButtonBox* lvButtonBox = new QDialogButtonBox( QDialogButtonBox::Ok );
  connect( lvButtonBox, SIGNAL( accepted() ), &cvProgressDialog, SLOT( hide() ) );
  cvProgressDialog.layout()->addWidget( lvButtonBox );


  connect( &cvImageFactory, SIGNAL( imageLoaded( bool ) ), this, SLOT( imageLoaded( bool ) ) );
  connect( &cvImageFactory, SIGNAL( progress( int, int, int ) ), this, SLOT( updateProgress( int, int, int ) ) );
}

void EgtSaveAsJpg::connectConfigurationButton( QPushButton* theButton )
{
  connect( theButton, SIGNAL( clicked() ), this, SLOT( showConfigurationPanel() ) );
}

void EgtSaveAsJpg::connectRunButton( QPushButton* theButton )
{
  connect( theButton, SIGNAL( clicked() ), this, SLOT( run() ) );
}

void EgtSaveAsJpg::initPlugin()
{
  //Hook listeners into the application interface
  if( 0 != cvApplicationInterface )
  {
    connect( cvApplicationInterface, SIGNAL( indexSelected( const QModelIndex& ) ), this, SLOT( setIndex( const QModelIndex& ) ) );
  }
}

void EgtSaveAsJpg::imageLoaded( bool theSuccess )
{
  if( cvImageFactory.isValid() )
  {
    QFileInfo lvFileInfo( cvImageFactory.fileName() );
    //TODO: Make plugin configurable and allow the user to over write the destination path
    if( lvFileInfo.isWritable() )
    {
      QString lvNewFileName = cvImageFactory.fileName() + ".to.jpg";

      cvOutputConsole.append( tr( "Saving image..." ) );
      if( cvImageFactory.saveOriginalImageAsJpeg( lvNewFileName ) )
      {
        cvApplicationInterface->refreshFileBrowser();

        cvOutputConsole.append( tr( "Copying EXIF data..." ) );
        //Punt checking for now, we know the images are valid...
        Exiv2::Image::AutoPtr lvSrcImage = Exiv2::ImageFactory::open( cvImageFactory.fileName().toStdString() );
        Exiv2::Image::AutoPtr lvDstImage = Exiv2::ImageFactory::open( lvNewFileName.toStdString() );
        lvSrcImage->readMetadata();
        Exiv2::ExifData &lvExifData = lvSrcImage->exifData();
        lvDstImage->setExifData( lvExifData );
        lvDstImage->writeMetadata();

        cvOutputConsole.append( tr( "Done." ) );
      }
      else
      {
        cvOutputConsole.append( "[" + tr( "ERROR" ) + "] "+ tr( "Failed to save new JPG" ) );
      }
    }
    else
    {
      cvOutputConsole.append( "[" + tr( "ERROR" ) + "] " + lvFileInfo.dir().absolutePath() + tr( " is not writable" ) );
    }
  }
  else
  {
    cvOutputConsole.append( "[" + tr( "ERROR" ) + "] " + tr( "The currently selected item is not a valid image" ) );
  }
}

void EgtSaveAsJpg::run()
{
  if( 0 != cvApplicationInterface )
  {
    cvProgressDialog.move( cvApplicationInterface->positionOfFirstVisibleWidget() );
    cvOutputConsole.clear();
    cvOutputConsole.append( tr("Processing image...") );
    cvProgressDialog.show();
    cvImageFactory.setFile( cvCurrentIndex );
  }
}

 void EgtSaveAsJpg::setIndex( QModelIndex theIndex )
 {
    cvCurrentIndex = theIndex;
 }

 void EgtSaveAsJpg::updateProgress( int theMinimum, int theMaximum, int theProgress )
 {
  //TODO: consider if it is better to set these individually
  cvProgressBar.setMinimum( theMinimum );
  cvProgressBar.setMaximum( theMaximum );
  cvProgressBar.setValue( theProgress );
}

 Q_EXPORT_PLUGIN2( saveasjpg, EgtSaveAsJpg );
