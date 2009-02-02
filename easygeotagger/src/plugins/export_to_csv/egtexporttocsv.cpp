/*
** File: egtexporttocsv.cpp
** Author(s): Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2008-10-15
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
#include "egtapplicationinterface.h"
#include "egtgpsexifengine.h"
#include "egtexporttocsv.h"
#include "egtpathbuilder.h"
#include "egtlogger.h"

#include <QModelIndex>
#include <QTextStream>
#include <QMessageBox>
#include <QDirModel>
#include <QDateTime>
#include <QTreeView>
#include <QFileInfo>
#include <QtPlugin>
#include <QObject>
#include <QDir>

EgtExportToCsv::EgtExportToCsv()
{
  cvCategories = QObject::tr( "Utilities" );
  cvDescription = QObject::tr( "Export a directory of images to a csv file. Exports all GPS tags from images with GPS EXIF data." );
  cvName = QObject::tr( "EXIF to CSV" );
}

void EgtExportToCsv::connectConfigurationButton( QPushButton* theButton )
{
  connect( theButton, SIGNAL( clicked() ), this, SLOT( showConfigurationPanel() ) );
}

void EgtExportToCsv::connectRunButton( QPushButton* theButton )
{
  connect( theButton, SIGNAL( clicked() ), this, SLOT( run() ) );
}

void EgtExportToCsv::indexSelected( const QModelIndex& theIndex )
{
    cvCurrentIndex = theIndex;
}

void EgtExportToCsv::initPlugin()
{
  //Hook into the application interface and listen for mouse clicks in the file browser
  if( 0 != cvApplicationInterface )
  {
    connect( cvApplicationInterface, SIGNAL( indexSelected( const QModelIndex& ) ), this, SLOT( indexSelected( const QModelIndex& ) ) );
  }
}

void EgtExportToCsv::run()
{
  EgtDebug( "entered" );
  
  //Check to see if the currently selected item is a file or a directory
  QFileInfo lvFileInfo;
  EgtPathBuilder lvPathBuilder;
  QString lvCurrentFile = lvPathBuilder.buildPath( cvCurrentIndex );
  
  //If it is a file, takes is parent which will be the directory whe image is in
  lvFileInfo.setFile( lvCurrentFile );
  if( !lvFileInfo.isDir() )
  {
    EgtDebug( "Current index is pointing to a file, taking parent" );
    cvCurrentIndex = cvCurrentIndex.parent();
  }
  
  //If the directory is writeable, open a file ane export the EXIF data
  lvCurrentFile = lvPathBuilder.buildPath( cvCurrentIndex );
  lvFileInfo.setFile( lvCurrentFile );
  if( lvFileInfo.isWritable() )
  {
    QDateTime lvTimestamp = QDateTime::currentDateTime();
    QFile lvOutputFile( QDir::toNativeSeparators( lvCurrentFile + "/" + cvCurrentIndex.data().toString() + "_Export_" + lvTimestamp.toString( "yyyyMMdd_hhmmss" ) + ".csv" ) );
    if( lvOutputFile.open( QIODevice::WriteOnly | QIODevice::Text ) )
    {
      EgtGpsExifEngine lvExifEngine;
      QList< EgtExifEngine::KeyMap > lvKeyMap = lvExifEngine.keys();
      QTextStream lvOutputWriter( &lvOutputFile );
      lvOutputWriter.setRealNumberPrecision( 7 );

      lvOutputWriter << "File";
      for( int lvIterator = 0; lvIterator < lvKeyMap.size(); lvIterator++ )
      {
        lvOutputWriter << "," + lvKeyMap[ lvIterator ].commonName;
      }
      lvOutputWriter << "\n";
      
      int lvExportedImages = 0;
      int lvChildCount = 0;
      QString lvImageFile;
      //Loop through the directory and examine each file
      while( cvCurrentIndex.child( lvChildCount, 0 ).isValid() )
      {
        lvImageFile = lvPathBuilder.buildPath( cvCurrentIndex.child( lvChildCount, 0 ) );
        
        //If the file has exif data,then export it, otherwise skip it
        lvExifEngine.setFile( lvImageFile );
        if( lvExifEngine.hasExpectedExif() )
        {
          lvOutputWriter << lvImageFile;
          for( int lvIterator = 0; lvIterator < lvKeyMap.size(); lvIterator++ )
          {
            lvOutputWriter << "," + lvExifEngine.read( lvKeyMap[ lvIterator ].key ).toString();
          }
          lvOutputWriter  << "\n";
          lvExportedImages++;
        }
        lvChildCount++;
      }
    
      lvOutputFile.close();
      QMessageBox::warning( cvApplicationInterface->gui(), tr( "Export Complete" ), tr( "%n images exported.", "", lvExportedImages ) );

    }
    else
    {
      QMessageBox::critical( cvApplicationInterface->gui(), tr( "Write Error" ), tr( "Could create a new file for output" ) );
    }
  }
  else
  {
    QMessageBox::critical( cvApplicationInterface->gui(), tr( "Write Error" ), tr( "The current directory is not writeable" ) );
  }

  //Refresh the file browser once the process is complete
  cvApplicationInterface->refreshFileBrowser();
}

 Q_EXPORT_PLUGIN2( exporttocsv, EgtExportToCsv );
