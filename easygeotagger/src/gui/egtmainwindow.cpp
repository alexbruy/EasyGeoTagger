/*
** File: egtmainwindow.cpp
** Author(s): Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2008-09-22
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
#include "egtmainwindow.h"

#include "egtlogger.h"
#include "egtexifio.h"
#include "egtitemdelegate.h"

#include <exiv2/image.hpp>
#include <exiv2/exif.hpp>
#include <cassert>

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDirModel>
#include <QDateTime>
#include <QTextStream>
#include <QMessageBox>

EgtMainWindow::EgtMainWindow()
{
  EgtDebug( "entered" );
  setupUi(this);
  connect( tvFileBrowser, SIGNAL( clicked( const QModelIndex& ) ), this, SLOT( clicked(const QModelIndex& ) ) );  
  connect( tvFileBrowser, SIGNAL( expanded( const QModelIndex& ) ), this, SLOT( expanded(const QModelIndex& ) ) );
  
  QDirModel* lvModel = new QDirModel( QStringList(), QDir::AllDirs|QDir::Files|QDir::NoDotAndDotDot, QDir::DirsFirst );
  
  tvFileBrowser->setModel( lvModel );
  tvFileBrowser->setColumnWidth( 0, 400) ;
  tvFileBrowser->setCurrentIndex( lvModel->index( QDir::currentPath() ) );
  tvFileBrowser->scrollTo( lvModel->index( QDir::currentPath() ) );
  
  EgtItemDelegate* lvItemDelegate =  new EgtItemDelegate();
  connect( chkbColorCodeFilenames, SIGNAL( stateChanged( int) ), lvItemDelegate, SLOT( displayGpsExifAvailability( int) ) );
  tvFileBrowser->setItemDelegate( lvItemDelegate );
  tvFileBrowser->setStyleSheet( "QTreeView { selection-background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #e7effd, stop: 1 #cbdaf1); }" );
  
  cvPluginDock = new EgtPluginDock( "Plugins", this );
  addDockWidget( Qt::LeftDockWidgetArea, cvPluginDock );
  
  pbarProgressBar->setMinimum( 0 );
  pbarProgressBar->setMaximum( 1 );
  pbarProgressBar->setValue( 0 );
  
  connect(&cvImageEngine, SIGNAL( progress( int, int, int ) ), this, SLOT( updateProgress( int, int, int ) ) );
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */
void EgtMainWindow::setPluginToolBox( QToolBox* theToolBox)
{
  //TODO: Need to free all of the panels and the tool box if it exists?
  
  cvPluginDock->setWidget( theToolBox );
}

/*
 *
 * SIGNAL and SLOTS
 *
 */

void EgtMainWindow::clicked( const QModelIndex& theIndex )
{
  EgtDebug( "entered" );
  cvImageEngine.setFile( cvPathBuilder.buildPath( theIndex ) );
  labelPreview->setPixmap( QPixmap::fromImage( cvImageEngine.image( labelPreview->width(), labelPreview->height() ) ) );
}

void EgtMainWindow::expanded(const QModelIndex& theIndex)
{
  EgtDebug( "entered" );
  int lvChildCount = 0;
  while(theIndex.child(lvChildCount, 0).isValid())
  {
    lvChildCount++;
  }
}

void EgtMainWindow::updateProgress(int theMinimum, int theMaximum, int theProgress )
{
  //TODO: consider if it is better to set these individually
  pbarProgressBar->setMinimum( theMinimum );
  pbarProgressBar->setMaximum( theMaximum );
  pbarProgressBar->setValue( theProgress );
}

