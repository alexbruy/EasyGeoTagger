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

#include <image.hpp>
#include <exif.hpp>
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
  connect(tvFileBrowser, SIGNAL(clicked(const QModelIndex&)), this, SLOT(clicked(const QModelIndex&)));  
  connect(tvFileBrowser, SIGNAL(expanded(const QModelIndex&)), this, SLOT(expanded(const QModelIndex&)));
  
  QDirModel* lvModel = new QDirModel(QStringList(), QDir::AllDirs|QDir::Files|QDir::NoDotAndDotDot, QDir::DirsFirst);
  
  tvFileBrowser->setModel(lvModel);
  tvFileBrowser->setColumnWidth(0, 400);
  tvFileBrowser->setCurrentIndex(lvModel->index(QDir::currentPath()));
  tvFileBrowser->scrollTo(lvModel->index(QDir::currentPath()));
  
  EgtItemDelegate* lvItemDelegate =  new EgtItemDelegate();
  connect(chkbColorCodeFilenames, SIGNAL(stateChanged(int)), lvItemDelegate, SLOT(displayGpsExifAvailability(int)));
  tvFileBrowser->setItemDelegate(lvItemDelegate);
  tvFileBrowser->setStyleSheet("QTreeView { selection-background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #e7effd, stop: 1 #cbdaf1); }");
  
  cvPluginDock = new EgtPluginDock( "Plugins", this );
  addDockWidget( Qt::LeftDockWidgetArea, cvPluginDock );
}

/*
 *
 * SIGNAL and SLOTS
 *
 */

void EgtMainWindow::clicked(const QModelIndex& theIndex)
{
  EgtDebug( "entered" );
  cvImageEngine.setFile( cvPathBuilder.buildPath( theIndex ) );
  labelThumbnail->setPixmap( QPixmap::fromImage( cvImageEngine.image( labelThumbnail->width(), labelThumbnail->height() ) ) );
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


/*
void EgtMainWindow::on_pbtnCreateLayerFromDir_clicked()
{
  EgtDebug("on_pbtnCreateLayerFromDir_clicked()");
  QFileInfo lvFileInfo;
  QString lvCurrentFile = cvPathBuilder.buildPath(tvFileBrowser->currentIndex());
  lvFileInfo.setFile(lvCurrentFile);
  if(!lvFileInfo.isDir())
  {
    tvFileBrowser->setCurrentIndex(tvFileBrowser->currentIndex().parent());
  }
  
  lvCurrentFile = cvPathBuilder.buildPath(tvFileBrowser->currentIndex());
  lvFileInfo.setFile(lvCurrentFile);
  if(lvFileInfo.isWritable())
  {
    QDateTime lvTimestamp = QDateTime::currentDateTime();
    QFile lvOutputFile(QDir::toNativeSeparators (lvCurrentFile + "/" + tvFileBrowser->currentIndex().data().toString() + "Export_" + lvTimestamp.toString("yyyyMMdd_hhmmss") + ".csv"));
    if(lvOutputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      QTextStream lvOutputWriter(&lvOutputFile);
      lvOutputWriter.setRealNumberPrecision(10);
      lvOutputWriter << "Longitude,Latitude,File\n";
      
      int lvExportedImages = 0;
      int lvChildCount = 0;

      EgtExifIO lvEEIO;
      QModelIndex lvCurrentIndex = tvFileBrowser->currentIndex();
      while(lvCurrentIndex.child(lvChildCount, 0).isValid())
      {
        QString lvImageFile = buildPath(lvCurrentIndex.child(lvChildCount, 0));
        EgtDebug(qPrintable(lvImageFile));
        if(lvEEIO.hasGpsExif(lvImageFile))
        {
          lvOutputWriter << lvEEIO.getLongitude(lvImageFile) << "," << lvEEIO.getLatitude(lvImageFile) << "," << lvImageFile << "\n";
          lvExportedImages++;
        }
        lvChildCount++;
      }
    
      lvOutputFile.close();
      QMessageBox::warning(this, tr("Export Complete"), tr("%n images exported.", "", lvExportedImages) );

    }
    else
    {
      QMessageBox::critical(this, tr("Write Error"), tr("Could create a new file for output") );
    }
  }
  else
  {
    QMessageBox::critical(this, tr("Write Error"), tr("The current directory is not writeable") );
  }

  if(tvFileBrowser->model())
  {
    ((QDirModel*)tvFileBrowser->model())->refresh();
  }
}
*/
void EgtMainWindow::setPluginToolBox( QToolBox* theToolBox)
{
  cvPluginDock->setWidget( theToolBox );
}
