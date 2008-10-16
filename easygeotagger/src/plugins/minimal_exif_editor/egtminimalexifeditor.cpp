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
#include "egtminimalexifeditor.h"
#include "egtpathbuilder.h"
#include "egtmainwindow.h"
#include "egtlogger.h"

#include <QFileInfo>
#include <QtPlugin>
#include <QObject>

static const QString cvCategories = QObject::tr( "EXIF Editors" );
static const QString cvDescription = QObject::tr( "Edit/add latitude longitude entries" );
static const QString cvName = QObject::tr( "Minimal EXIF Editor" );

EgtMinimalExifEditor::EgtMinimalExifEditor()
{
  cvDock = 0;
  cvExifTable = 0;
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

QString EgtMinimalExifEditor::name()
{
  return cvName;
}


/*
 *
 * SIGNAL and SLOTS
 *
 */
void EgtMinimalExifEditor::clicked( const QModelIndex& theIndex )
{
  EgtDebug( "entered" );
  
  EgtPathBuilder lvPathBuilder;
  QString lvFilename = lvPathBuilder.buildPath( theIndex );
  QFileInfo lvFileInfo( lvFilename );
  if( !lvFileInfo.isDir() )
  {
    updateTable( lvFilename ); 
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
    
    cvExifTable = new QTableWidget( cvGui );
    if( 0 == cvExifTable ) { return; }
    
    cvExifTable->setColumnCount(2);
    cvExifTable->setColumnWidth(0, 180);
    cvExifTable->setColumnWidth(1, 180);
    cvExifTable->setHorizontalHeaderLabels(QStringList() << "Field" << "Value");
    
    cvDock->setFeatures( QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable );
    cvDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    cvDock->setMinimumSize( 360,150 );
    cvDock->setWidget( cvExifTable );
    
    EgtDebug( "Connecting to GUI signals" );
    connect( cvGui->fileBrowser(), SIGNAL( clicked( const QModelIndex& ) ), this, SLOT( clicked(const QModelIndex& ) ) );
    
    EgtDebug( "Adding dock to GUI" );
    cvGui->addDockWidget( Qt::RightDockWidgetArea, cvDock );
  }
  
  if( "" != cvLastFile )
  {
    updateTable( cvLastFile );
  }
  
  
  EgtDebug( "done" );
}
/*
 *
 * PRIVATE FUNCTIONS
 *
 */
void EgtMinimalExifEditor::updateTable( QString theFilename )
{
  EgtDebug( "entered" );
//   cvExifTable->clear(); //Why does this not work?
  for(int lvIterator = cvExifTable->rowCount() - 1; lvIterator >= 0; lvIterator--)
  {
    cvExifTable->removeRow( lvIterator );
  }

  cvExifIO.setFile( theFilename );
  if( cvExifIO.hasGpsExif() )
  {
    cvExifTable->insertRow( 0 );
    cvExifTable->setItem( 0, 0, new QTableWidgetItem( "Longitude" ) );
    cvExifTable->setItem( 0, 1, new QTableWidgetItem( QString::number( cvExifIO.longitude() ) ) );
    cvExifTable->insertRow( 1 );
    cvExifTable->setItem( 1, 0, new QTableWidgetItem( "Latitude" ) );
    cvExifTable->setItem( 1, 1, new QTableWidgetItem( QString::number( cvExifIO.latitude() ) ) );
  }
}

 Q_EXPORT_PLUGIN2( minimalexifeditor, EgtMinimalExifEditor );
