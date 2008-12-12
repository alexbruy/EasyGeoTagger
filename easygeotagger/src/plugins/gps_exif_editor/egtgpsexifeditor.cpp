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

  cvDock.setWindowTitle( cvName );
  cvDock.setFeatures( QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable );
  cvDock.setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  cvDock.setMinimumSize( 250,150 );

  cvEditorWidgets = new EgtExifEditorWidgets( &cvExifEngine, &cvDock );
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

    connect( cvApplicationInterface, SIGNAL( coordinatesReceived( double, double ) ), this, SLOT( acceptCoordinates( double, double ) ) );
    connect( cvApplicationInterface, SIGNAL( indexSelected( const QModelIndex& ) ), this, SLOT( updateEditorControls( const QModelIndex& ) ) );
  }
}

/*
 *
 * SIGNAL and SLOTS
 *
 */
void EgtGpsExifEditor::acceptCoordinates( double theLongitude, double theLatitude )
{
  if( cvExifEngine.isValidImage() )
  {
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
  updateEditorControls( cvLastFile );

  EgtDebug( "done" );
}

/*!
 * \param theIndex the QModelIndex representing the file from which to extract exif data
 */
void EgtGpsExifEditor::updateEditorControls( const QModelIndex& theIndex )
{
  EgtDebug( "entered" );
  
  EgtPathBuilder lvPathBuilder;
  QString lvFilename = lvPathBuilder.buildPath( theIndex );
  QFileInfo lvFileInfo( lvFilename );
  
  //If the index points a file, try to extract the exif data
  if( !lvFileInfo.isDir() )
  {
    updateEditorControls( lvFilename );
  }
  else
  {
    updateEditorControls( "" );
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
void EgtGpsExifEditor::updateEditorControls( QString theFilename )
{
  EgtDebug( "entered" );

  if( 0 == cvEditorWidgets) { return; }

  cvExifEngine.setFile( theFilename );
  cvLastFile = theFilename;

  cvEditorWidgets->updateEditorControls( cvExifEngine.hasGpsExif() );
  
}

 Q_EXPORT_PLUGIN2( gpsexifeditor, EgtGpsExifEditor );
