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

/*!
 * \param theButton pointer to a QPushButton that is to be connect to the  showConfigureationPanel slot
 */
void EgtMinimalExifEditor::connectConfigurationButton( QPushButton* theButton )
{
  connect( theButton, SIGNAL( clicked() ), this, SLOT( showConfigurationPanel() ) );
}

/*!
 * \param theButton pointer to a QPushButton that is to be connect to the  run slot
 */
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
  //Hook listeners into the application interface
  if( 0 != cvApplicationInterface )
  {
    connect( cvApplicationInterface, SIGNAL( coordinatesReceived( double, double ) ), this, SLOT( acceptCoordinates( double, double ) ) );
    connect( cvApplicationInterface, SIGNAL( indexSelected( const QModelIndex& ) ), this, SLOT( updateExifDisplay( const QModelIndex& ) ) );
  }
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
  if( cvExifIoEngine.isValidImage() )
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
  
  //Build or reshow the plugins GUI component
  if( 0 != cvDock &&  cvDock->isVisible() )
  {
    EgtDebug( "dock is already open and visible" );
    return;
  }
  else if( 0 != cvDock &&  !cvDock->isMinimized() )
  {
    EgtDebug( "dock is already open but not visible" );
    cvDock->showMaximized();
    return;
  }
  else
  {
    EgtDebug( "requesting new dock" );
    cvDock = new QDockWidget( cvName, cvApplicationInterface->gui() );
    if( 0 == cvDock ) { return; }
    
    cvControls = new EgtMinimalExifEditorControls( &cvExifIoEngine, cvDock );
    if( 0 == cvControls ) { return; }
    
    cvDock->setFeatures( QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable );
    cvDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    cvDock->setMinimumSize( 250,150 );
    cvDock->setWidget( cvControls );
    cvControls->show();
    
    EgtDebug( "Adding dock to GUI" );
    cvApplicationInterface->gui()->addDockWidget( Qt::RightDockWidgetArea, cvDock );
  }
  
  //On redisplay of the widget, update the exif data incase the index has changed
  if( "" != cvLastFile )
  {
    updateExifDisplay( cvLastFile );
  }
  
  EgtDebug( "done" );
}

/*!
 * \param theIndex the QModelIndex representing the file from which to extract exif data
 */
void EgtMinimalExifEditor::updateExifDisplay( const QModelIndex& theIndex )
{
  EgtDebug( "entered" );
  
  EgtPathBuilder lvPathBuilder;
  QString lvFilename = lvPathBuilder.buildPath( theIndex );
  QFileInfo lvFileInfo( lvFilename );
  
  //If the index points a file, try to extract the exif data
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
/*!
 * \param theFilename the name of the file from which to extract exif data
 */
void EgtMinimalExifEditor::updateExifDisplay( QString theFilename )
{
  EgtDebug( "entered" );
  cvExifIoEngine.setFile( theFilename );
  cvLastFile = theFilename;

  //check to make sure the dock has been created
  if( 0 == cvDock ) { return; }

  //Get data if it exists
  if( theFilename == "" || !cvExifIoEngine.hasGpsExif() )
  {
    cvControls->leLongitude->setText( "" ); 
    cvControls->leLatitude->setText( "" );
  }
  else
  {
    cvControls->leLongitude->setText( QString::number( cvExifIoEngine.longitude(), 'f', 7 ) ); 
    cvControls->leLatitude->setText( QString::number( cvExifIoEngine.latitude(), 'f', 7 ) );
  }
  
}

 Q_EXPORT_PLUGIN2( minimalexifeditor, EgtMinimalExifEditor );
