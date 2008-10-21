/*
** File: egtminimalexifeditor.cpp
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
#include "egtminimalexifeditor.h"
#include "egtpathbuilder.h"
#include "egtmainwindow.h"
#include "egtlogger.h"

#include <QPushButton>
#include <QFileInfo>
#include <QtPlugin>

static const QString cvCategories = QObject::tr( "EXIF Editors" );
static const QString cvDescription = QObject::tr( "Edit/add latitude longitude entries" );
static const QString cvName = QObject::tr( "Minimal EXIF Editor" );

EgtMinimalExifEditor::EgtMinimalExifEditor()
{
  cvDock = 0;
  cvLastFile = "";
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */

QStringList EgtMinimalExifEditor::categories()
{
  return cvCategories.split("|");
}

void EgtMinimalExifEditor::connectConfigurationButton( QPushButton* theButton )
{
  connect( theButton, SIGNAL( clicked() ), this, SLOT( showConfigurationPanel() ) );
}

void EgtMinimalExifEditor::connectRunButton( QPushButton* theButton )
{
  connect( theButton, SIGNAL( clicked() ), this, SLOT( run() ) );
}

QString EgtMinimalExifEditor::description()
{
  return cvDescription;
}

void EgtMinimalExifEditor::initPlugin()
{
  connect( cvApplicationInterface, SIGNAL( coordinatesReceived( double, double ) ), this, SLOT( acceptCoordinates( double, double ) ) );
}

QString EgtMinimalExifEditor::name()
{
  return cvName;
}


/*
 *
 * SIGNAL and SLOTS
 *
 */
void EgtMinimalExifEditor::acceptCoordinates( double theLongitude, double theLatitude )
{
  if( cvExifIO.isValidImage() )
  {
    cvControls->leLongitude->setText( QString::number( theLongitude, 'f', 7) );
    cvControls->longitudeControls->setEnabled( true );
    cvControls->leLatitude->setText( QString::number( theLatitude, 'f', 7) );
    cvControls->latitudeControls->setEnabled( true );
  }
}

void EgtMinimalExifEditor::run()
{
  EgtDebug( "entered" );
  //if the gui pointer is null, bail
  if( 0 == cvGui ) { return; }
  

  if( 0 != cvDock &&  cvDock->isVisible() )
  {
    EgtDebug( "dock is already open and visible" );
    return;
  }
  
  if( 0 != cvDock &&  !cvDock->isMinimized() )
  {
    EgtDebug( "dock is already open but not visible" );
    cvDock->showMaximized();
    return;
  }
  else
  {
    EgtDebug( "requesting new dock" );
    cvDock = new QDockWidget( cvName, cvGui );
    if( 0 == cvDock ) { return; }
    cvControls = new EgtMinimalExifEditorControls( &cvExifIO, cvDock );
    if( 0 == cvControls ) { return; }
    
    cvDock->setFeatures( QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable );
    cvDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    cvDock->setMinimumSize( 300,150 );
    cvDock->setWidget( cvControls );
    cvControls->show();
    
    EgtDebug( "Connecting to GUI signals" );
    connect( cvGui->tvFileBrowser, SIGNAL( clicked( const QModelIndex& ) ), this, SLOT( updateExifDisplay(const QModelIndex& ) ) );
    
    EgtDebug( "Adding dock to GUI" );
    cvGui->addDockWidget( Qt::RightDockWidgetArea, cvDock );
  }
  
  if( "" != cvLastFile )
  {
    updateExifDisplay( cvLastFile );
  }
  else
  {
    updateExifDisplay( cvGui->tvFileBrowser->currentIndex() );
  }
  
  
  EgtDebug( "done" );
}

void EgtMinimalExifEditor::updateExifDisplay( const QModelIndex& theIndex )
{
  EgtDebug( "entered" );
  
  EgtPathBuilder lvPathBuilder;
  QString lvFilename = lvPathBuilder.buildPath( theIndex );
  QFileInfo lvFileInfo( lvFilename );
  if( !lvFileInfo.isDir() )
  {
    updateExifDisplay( lvFilename ); 
  }
  else
  {
    updateExifDisplay( "" ); 
  }
} 

/*
 *
 * PRIVATE FUNCTIONS
 *
 */
void EgtMinimalExifEditor::updateExifDisplay( QString theFilename )
{
  EgtDebug( "entered" );
  cvExifIO.setFile( theFilename );
  cvLastFile = theFilename;

  //Get data if it exists
  if( theFilename == "" || !cvExifIO.hasGpsExif() )
  {
    cvControls->leLongitude->setText( "" ); 
    cvControls->leLatitude->setText( "" );
  }
  else
  {
    cvControls->leLongitude->setText( QString::number( cvExifIO.longitude(), 'f', 7 ) ); 
    cvControls->leLatitude->setText( QString::number( cvExifIO.latitude(), 'f', 7 ) );
  }
  
}

 Q_EXPORT_PLUGIN2( minimalexifeditor, EgtMinimalExifEditor );
