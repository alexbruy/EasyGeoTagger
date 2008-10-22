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
#include "egtexporttocsv.h"
#include "egtpathbuilder.h"
#include "egtmainwindow.h"
#include "egtexifio.h"
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

static const QString cvCategories = QObject::tr( "Utilities" );
static const QString cvDescription = QObject::tr( "Export a directory of image to a csv file. It will only export latitude and longitude from images with GPS EXIF data." );
static const QString cvName = QObject::tr( "EXIF to CSV" );

QStringList EgtExportToCsv::categories()
{
  return cvCategories.split("|");
}

void EgtExportToCsv::connectConfigurationButton( QPushButton* theButton )
{
  connect( theButton, SIGNAL( clicked() ), this, SLOT( showConfigurationPanel() ) );
}

void EgtExportToCsv::connectRunButton( QPushButton* theButton )
{
  connect( theButton, SIGNAL( clicked() ), this, SLOT( run() ) );
}

QString EgtExportToCsv::description()
{
  return cvDescription;
}

QString EgtExportToCsv::name()
{
  return cvName;
}

void EgtExportToCsv::run()
{
  EgtDebug( "entered" );
  
  //Bail if either the pointer to the gui for the file browser is null
  if( 0 == cvGui || 0 == cvGui->tvFileBrowser )
  {
    EgtDebug( "pointer to gui or the file browser was null" );
    return;
  }
  
  //Check to see if the currently selected item is a file or a directory
  QFileInfo lvFileInfo;
  EgtPathBuilder lvPathBuilder;
  QString lvCurrentFile = lvPathBuilder.buildPath( cvGui->tvFileBrowser->currentIndex() );
  
  //If it is a file, takes is parent which will be the directory whe image is in
  lvFileInfo.setFile( lvCurrentFile );
  if( !lvFileInfo.isDir() )
  {
    EgtDebug( "Current index is pointing to a file, taking parent" );
    cvGui->tvFileBrowser->setCurrentIndex( cvGui->tvFileBrowser->currentIndex().parent() );
  }
  
  //If the directory is writeable, open a file ane export the EXIF data
  lvCurrentFile = lvPathBuilder.buildPath( cvGui->tvFileBrowser->currentIndex() );
  lvFileInfo.setFile( lvCurrentFile );
  if( lvFileInfo.isWritable() )
  {
    QDateTime lvTimestamp = QDateTime::currentDateTime();
    QFile lvOutputFile( QDir::toNativeSeparators( lvCurrentFile + "/" + cvGui->tvFileBrowser->currentIndex().data().toString() + "_Export_" + lvTimestamp.toString( "yyyyMMdd_hhmmss" ) + ".csv" ) );
    if( lvOutputFile.open( QIODevice::WriteOnly | QIODevice::Text ) )
    {
      QTextStream lvOutputWriter( &lvOutputFile );
      lvOutputWriter.setRealNumberPrecision( 10 );
      lvOutputWriter << "Longitude,Latitude,File\n";
      
      int lvExportedImages = 0;
      int lvChildCount = 0;

      EgtExifIO lvEEIO;
      QString lvImageFile;
      QModelIndex lvCurrentIndex = cvGui->tvFileBrowser->currentIndex();
      //Loop through the directory and examine each file
      while( lvCurrentIndex.child( lvChildCount, 0 ).isValid() )
      {
        lvImageFile = lvPathBuilder.buildPath( lvCurrentIndex.child( lvChildCount, 0 ) );
        
        //If the file has exif data,then export it, otherwise skip it
        lvEEIO.setFile( lvImageFile );
        if( lvEEIO.hasGpsExif() )
        {
          lvOutputWriter << lvEEIO.longitude() << "," << lvEEIO.latitude() << "," << lvImageFile << "\n";
          lvExportedImages++;
        }
        lvChildCount++;
      }
    
      lvOutputFile.close();
      QMessageBox::warning( cvGui, tr( "Export Complete" ), tr( "%n images exported.", "", lvExportedImages ) );

    }
    else
    {
      QMessageBox::critical( cvGui, tr( "Write Error" ), tr( "Could create a new file for output" ) );
    }
  }
  else
  {
    QMessageBox::critical( cvGui, tr( "Write Error" ), tr( "The current directory is not writeable" ) );
  }

  //Refresh the file browser once the process is complete
  if( cvGui->tvFileBrowser->model() )
  {
    ( (QDirModel*)cvGui->tvFileBrowser->model() )->refresh();
  }
}

 Q_EXPORT_PLUGIN2( exporttocsv, EgtExportToCsv );
