/***************************************************************************
 *   Copyright (C) 2003 by Tim Sutton                                      *
 *   tim@linfiniti.com                                                     *
 *                                                                         *
 *   This is a plugin generated from the QGIS plugin template              *
 *   and filled in by Peter J. Ersts                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "loadexifdatasetgui.h"
#include "qgscontexthelp.h"

//qt includes
#include <QDir>
#include <QDirModel>
#include <QFileInfo>

LoadExifDatasetGui::LoadExifDatasetGui( QWidget* parent, Qt::WFlags fl )
    : QDialog( parent, fl )
{
  setupUi( this );
  QDirModel* lvModel = new QDirModel( QStringList(), QDir::AllDirs|QDir::Files|QDir::NoDotAndDotDot, QDir::DirsFirst );
  
  tvDirectoryBrowser->setModel( lvModel );
  tvDirectoryBrowser->setColumnWidth( 0, 400 );
  tvDirectoryBrowser->setCurrentIndex( lvModel->index( QDir::currentPath() ) );
  tvDirectoryBrowser->scrollTo( lvModel->index( QDir::currentPath() ) );
  tvDirectoryBrowser->setStyleSheet( "QTreeView { selection-background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #e7effd, stop: 1 #cbdaf1); }" );
}

LoadExifDatasetGui::~LoadExifDatasetGui()
{
}

// Recursively climb the tree to build the full path to the selected file or directory
QString LoadExifDatasetGui::buildPath( const QModelIndex& theIndex )
{
  if( !theIndex.isValid() ) { return ""; }
  
  if( !theIndex.parent().isValid() )
  {
  #ifdef WIN32
  return theIndex.data().toString();
  #else
  return "";
  #endif
  }
  
  return buildPath( theIndex.parent() ) + QDir::toNativeSeparators ("/") + theIndex.data().toString();
}

void LoadExifDatasetGui::on_buttonBox_accepted()
{
  //close the dialog
  accept();
}

void LoadExifDatasetGui::on_buttonBox_clicked( QAbstractButton* theButton )
{
  if( buttonBox->standardButton ( theButton) == QDialogButtonBox::Ok )
  {
    QFileInfo lvFileInfo;
    QString lvCurrentFile = buildPath( tvDirectoryBrowser->currentIndex() );
    lvFileInfo.setFile( lvCurrentFile );
    if( !lvFileInfo.isDir() )
    {
      tvDirectoryBrowser->setCurrentIndex( tvDirectoryBrowser->currentIndex().parent() );
    }
    emit draw( buildPath( tvDirectoryBrowser->currentIndex() ), tvDirectoryBrowser->currentIndex().data().toString() );
  }
}

void LoadExifDatasetGui::on_buttonBox_rejected()
{
  reject();
}

void LoadExifDatasetGui::on_buttonBox_helpRequested()
{
  QgsContextHelp::run( context_id );
}

