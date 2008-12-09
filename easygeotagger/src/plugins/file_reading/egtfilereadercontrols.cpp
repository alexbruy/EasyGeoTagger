/*
** File:
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

#include "egtfilereadercontrols.h"
#include "egtlogger.h"


#include <QIcon>
#include <QPushButton>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QtGui>

EgtFileReaderControls::EgtFileReaderControls( EgtExifIoEngine* theExifIoEngine, QWidget* theParent, Qt::WindowFlags theFlags ) : QWidget( theParent, theFlags )
{qDebug("constructor"); 
  setupUi( this );
 connect( pbtnOpenFile, SIGNAL( clicked() ), this, SLOT( onthe_pbtnOpenFile_clicked() ) );  
 connect( pbtnTagPicture, SIGNAL( clicked() ), this, SLOT( onthe_pbtnTagPicture_clicked() ) ); 


  cvExifIoEngine = theExifIoEngine;

  //Set up the file browser window
  QStandardItemModel* lvModel = new QStandardItemModel( );
  tvFileContent->setModel( lvModel );
  QModelIndex lvIndex ;
  for (int lvRow = 0; lvRow < 4; ++lvRow) 
  {
    for (int lvColumn = 0; lvColumn < 2; ++lvColumn) 
    {
      
      lvModel->insertRows(lvModel->rowCount(),1);
      //lvModel->insertColumns(lvColumn,1,lvIndex); 
      //lvModel->setData(lvIndex, QVariant((2) * (2)));
    }
  }

lvModel->insertColumns(lvModel->columnCount(),1);
lvModel->insertColumns(lvModel->columnCount(),1);  
lvModel->insertColumns(lvModel->columnCount(),1); 

lvIndex = lvModel->index(lvModel->rowCount()-2, lvModel->columnCount()-1, QModelIndex());
lvModel->setData(lvIndex, QVariant((2) * (2)));

}

/*
 *
 * PRIVATE SLOTS
 *
 */

void EgtFileReaderControls::open()
{
  QString lvFileName = QFileDialog::getOpenFileName( this, tr( "Open GPS File" ),QDir::currentPath(),
                                          				tr( "GPS Files (*.gps *.xml)" ) );
  if (lvFileName.isEmpty())
    return;

  QFile lvFile(lvFileName);
  if (!lvFile.open(QFile::ReadOnly | QFile::Text)) 
  {
    QMessageBox::warning(this, tr("GPS Files"),
                              tr("Cannot read file %1:\n%2.").arg(lvFileName).arg(lvFile.errorString()));
    return;
  }

  QStandardItemModel* lvModel = new QStandardItemModel( );
  tvFileContent->setModel( lvModel );
  QModelIndex lvIndex ;

  QTextStream stream( &lvFile );
  QString lvLine;
  QStringList lvList;

  lvLine = stream.readLine();
  lvList = lvLine.split(",");
   
  int lvColumnSize = lvList.size();
  lvModel->insertColumns(lvModel->columnCount(),lvColumnSize);
  lvModel->insertRows(lvModel->rowCount(),1);
    
  int lvColumnCount = 0;
  for ( QStringList::Iterator it = lvList.begin(); it != lvList.end(); ++it ) 
  {
    lvIndex = lvModel->index(lvModel->rowCount()-1, lvColumnCount, QModelIndex());
    lvModel->setData(lvIndex, QVariant(*it));
    qDebug((*it).toStdString().c_str());
    lvColumnCount++;
  }

  while( ! stream.atEnd() )
  {
    lvLine = stream.readLine();
    lvList = lvLine.split(",");
    
    lvModel->insertRows(lvModel->rowCount(),1);
    
    lvColumnCount = 0;
    for ( QStringList::Iterator it = lvList.begin(); it != lvList.end(); ++it ) 
    {
      lvIndex = lvModel->index(lvModel->rowCount()-1, lvColumnCount, QModelIndex());
      lvModel->setData(lvIndex, QVariant(*it));
      qDebug((*it).toStdString().c_str());
      lvColumnCount++;
    }
  }
}


void EgtFileReaderControls::onthe_pbtnOpenFile_clicked()
{qDebug("push the button"); 

  if( 0 == cvExifIoEngine ) { return; }
  
  open(); 
 
}

void EgtFileReaderControls::onthe_pbtnTagPicture_clicked()
{
qDebug("push the button"); 

  if( 0 == cvExifIoEngine ) { return; }
  

}
