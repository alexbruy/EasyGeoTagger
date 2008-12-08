/*
** File: egtminimalexifeditor.cpp
** Author(s): 
** Creation Date: 
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
#include "egtfilereader.h"
#include "egtpathbuilder.h"
#include "egtmainwindow.h"
#include "egtlogger.h"

#include <QPushButton>
#include <QFileInfo>
#include <QtPlugin>

static const QString cvCategories = QObject::tr( "File Readers" );
static const QString cvDescription = QObject::tr( "Read files with gps data" );
static const QString cvName = QObject::tr( "GPS file reader" );

EgtFileReader::EgtFileReader()
{
  cvDock = 0;
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */

QStringList EgtFileReader::categories()
{
  return cvCategories.split("|");
}

/*!
 * \param theButton pointer to a QPushButton that is to be connect to the  showConfigureationPanel slot
 */
void EgtFileReader::connectConfigurationButton( QPushButton* theButton )
{
  connect( theButton, SIGNAL( clicked() ), this, SLOT( showConfigurationPanel() ) );
}

/*!
 * \param theButton pointer to a QPushButton that is to be connect to the  run slot
 */
void EgtFileReader::connectRunButton( QPushButton* theButton )
{
  connect( theButton, SIGNAL( clicked() ), this, SLOT( run() ) );
}

QString EgtFileReader::description()
{
  return cvDescription;
}

void EgtFileReader::initPlugin()
{
  //Hook listeners into the application interface
  if( 0 != cvApplicationInterface )
  {
  }
}

QString EgtFileReader::name()
{
  return cvName;
}


/*
 *
 * SIGNAL and SLOTS
 *
 */


void EgtFileReader::run()
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
    
    cvControls = new EgtFileReaderControls( &cvExifIoEngine, cvDock );
    if( 0 == cvControls ) { return; }
    
    cvDock->setFeatures( QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable );
    cvDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    cvDock->setMinimumSize( 250,150 );
    cvDock->setWidget( cvControls );
    cvControls->show();
    
    EgtDebug( "Adding dock to GUI" );
    cvApplicationInterface->gui()->addDockWidget( Qt::RightDockWidgetArea, cvDock );
  }
  
  
  EgtDebug( "done" );
}

Q_EXPORT_PLUGIN2( filereader, EgtFileReader );
