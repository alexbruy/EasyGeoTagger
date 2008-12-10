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

#include <QVBoxLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QFileInfo>
#include <QtPlugin>

static const QString cvCategories = QObject::tr( "EXIF Editors" );
static const QString cvDescription = QObject::tr( "Edit/add GPS EXIF entries" );
static const QString cvName = QObject::tr( "GPS EXIF Editor" );

EgtGpsExifEditor::EgtGpsExifEditor()
{
  cvLastFile = "";

  cvDock.setWindowTitle( cvName );
  cvDock.setFeatures( QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable );
  cvDock.setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  cvDock.setMinimumSize( 250,150 );

   //Layout editor controls
  cvEditorControls.setParent( &cvDock );
  cvEditorControls.setLayout( new QVBoxLayout() );
  cvEditorControls.layout()->setSpacing( 0 );
  cvEditorControls.layout()->setContentsMargins( 1, 1, 1, 1 );

  cvConfigurationControls.setLayout( new QVBoxLayout() );
  cvConfigurationControls.layout()->setSpacing( 0 );
  cvConfigurationControls.layout()->setContentsMargins( 1, 1, 1, 1 );

  //TODO: Make this into a loop
    EgtExifTagControls* lvTagControls = new EgtExifTagControls( "Egt.Longitude", "Longitude" );
    cvTagControls << lvTagControls;
    cvEditorControls.layout()->addWidget( lvTagControls->editorControls() );
    cvConfigurationControls.layout()->addWidget( lvTagControls->configurationControls() );

    lvTagControls = new EgtExifTagControls( "Egt.Latitude", "Latitude" );
    cvTagControls << lvTagControls;
    cvEditorControls.layout()->addWidget( lvTagControls->editorControls() );
    cvConfigurationControls.layout()->addWidget( lvTagControls->configurationControls() );
  //end

  ((QVBoxLayout*)cvConfigurationControls.layout())->insertStretch(-1, 1);
  ((QVBoxLayout*)cvEditorControls.layout())->insertStretch(-1, 1);
  cvDock.setWidget( &cvEditorControls );
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */

QStringList EgtGpsExifEditor::categories()
{
  return cvCategories.split("|");
}

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

QString EgtGpsExifEditor::description()
{
  return cvDescription;
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
    connect( cvApplicationInterface, SIGNAL( indexSelected( const QModelIndex& ) ), this, SLOT( updateExifDisplay( const QModelIndex& ) ) );
  }
}

QString EgtGpsExifEditor::name()
{
  return cvName;
}


/*
 *
 * SIGNAL and SLOTS
 *
 */
void EgtGpsExifEditor::acceptCoordinates( double theLongitude, double theLatitude )
{
  if( cvExifIoEngine.isValidImage() )
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
  else if( !cvDock.isVisible() || cvDock.isMinimized() )
  {
    EgtDebug( "dock is already open but not visible" );
    cvDock.showMaximized();
    return;
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
void EgtGpsExifEditor::updateExifDisplay( const QModelIndex& theIndex )
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
void EgtGpsExifEditor::updateExifDisplay( QString theFilename )
{
  EgtDebug( "entered" );
  cvExifIoEngine.setFile( theFilename );
  cvLastFile = theFilename;

  //check to make sure the dock is visible
  if( !cvDock.isVisible() ) { return; }

  //Get data if it exists
  QList<EgtExifTagControls*>::iterator lvIterator = cvTagControls.begin();
  if( theFilename == "" || !cvExifIoEngine.hasGpsExif() )
  {
    QString lvBlank = "";
    while( lvIterator != cvTagControls.end() )
    {
      (*lvIterator)->setValue( lvBlank );
      lvIterator++;
    }
  }
  else
  {
    while( lvIterator != cvTagControls.end() )
    {
      (*lvIterator)->setValue( cvExifIoEngine.readTag( (*lvIterator)->key() ) );
      lvIterator++;
    }
  }
  
}

 Q_EXPORT_PLUGIN2( minimalexifeditor, EgtGpsExifEditor );
