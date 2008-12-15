/*
** File: egtgpsexifeditor.cpp
** Author(s): Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2008-12-09
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
#include "egtgpsexifeditor.h"
#include "egtexiftaggroup.h"
#include "egtpathbuilder.h"
#include "egtlogger.h"

#include <QMainWindow>
#include <QPushButton>
#include <QFileInfo>
#include <QtPlugin>
#include <QMap>

EgtGpsExifEditor::EgtGpsExifEditor()
{
  cvCategories = QObject::tr( "EXIF Editors" );
  cvDescription = QObject::tr( "Edit/add GPS EXIF entries" );
  cvName = QObject::tr( "GPS EXIF Editor" );
  cvLastFile = "";

  cvEditor = new EgtExifEditor( &cvExifEngine );
  //Create custom tag groups
  EgtExifTagGroup* lvTagGroup = new EgtExifTagGroup( "Clear all" );
  cvEditor->addTagGroup( lvTagGroup );

  lvTagGroup = new EgtExifTagGroup( "X, Y" );
  lvTagGroup->addKey("Egt.GPS.Longitude" );
  cvEditor->addTagGroup( lvTagGroup );

  lvTagGroup = new EgtExifTagGroup( "X, Y, Z" );
  lvTagGroup->addKey("Egt.GPS.Longitude" );
  lvTagGroup->addKey("Egt.GPS.Altitude" );
  cvEditor->addTagGroup( lvTagGroup );

  cvDock.setWindowTitle( cvName );
  cvDock.setFeatures( QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable );
  cvDock.setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  cvDock.setMinimumSize( 250,150 );
  cvDock.setWidget( cvEditor->editorWidget() );
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */

/*!
 * \param theButton pointer to a QPushButton that is to be connect to the  showConfigureationPanel slot
 */
void EgtGpsExifEditor::connectConfigurationButton( QPushButton* theButton )
{
  connect( theButton, SIGNAL( clicked() ), this, SLOT( showConfigurationPanel() ) );
}

/*!
 * \param theButton pointer to a QPushButton that is to be connect to the  run slot
 */
void EgtGpsExifEditor::connectRunButton( QPushButton* theButton )
{
  connect( theButton, SIGNAL( clicked() ), this, SLOT( run() ) );
}

void EgtGpsExifEditor::initPlugin()
{
  //Hook listeners into the application interface
  if( 0 != cvApplicationInterface )
  {
    //Add the dock but keep it hidden until the user askes for it
    cvApplicationInterface->gui()->addDockWidget( Qt::RightDockWidgetArea, &cvDock );
    cvDock.setVisible( false );

    connect( cvApplicationInterface, SIGNAL( keyValuePair( QString, QString ) ), this, SLOT( acceptKeyValuePair( QString, QString ) ) );
    connect( cvApplicationInterface, SIGNAL( indexSelected( const QModelIndex& ) ), this, SLOT( loadExifData( const QModelIndex& ) ) );
  }
}

/*
 *
 * SIGNAL and SLOTS
 *
 */
void EgtGpsExifEditor::acceptKeyValuePair( QString theKey, QString theData )
{
  if( cvExifEngine.isValidImage() )
  {
    cvEditor->setTagData( theKey, theData, false );
  }
}

void EgtGpsExifEditor::run()
{
  EgtDebug( "entered" );
  
  //Build or reshow the plugins GUI component
  if( cvDock.isVisible() )
  {
    EgtDebug( "dock is already open and visible" );
    return;
  }

  EgtDebug( "dock is already open but not visible" );
  cvDock.showMaximized();
  loadExifData( cvLastFile );

  EgtDebug( "done" );
}

/*!
 * \param theIndex the QModelIndex representing the file from which to extract exif data
 */
void EgtGpsExifEditor::loadExifData( const QModelIndex& theIndex )
{
  EgtDebug( "entered" );
  
  EgtPathBuilder lvPathBuilder;
  QString lvFilename = lvPathBuilder.buildPath( theIndex );
  QFileInfo lvFileInfo( lvFilename );
  
  //If the index points a file, try to extract the exif data
  if( !lvFileInfo.isDir() )
  {
    loadExifData( lvFilename );
  }
  else
  {
    loadExifData( "" );
  }
} 

/*
 *
 * PRIVATE FUNCTIONS
 *
 */
/*!
 * \param theFilename the name of the file from which to extract exif data
 */
void EgtGpsExifEditor::loadExifData( QString theFilename )
{
  EgtDebug( "entered" );

  if( 0 == cvEditor) { return; }

  cvExifEngine.setFile( theFilename );
  cvLastFile = theFilename;

  cvEditor->loadExifData( cvExifEngine.hasGpsExif() );
  
}

 Q_EXPORT_PLUGIN2( gpsexifeditor, EgtGpsExifEditor );
