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
  QStandardItemModel* lvModel = new QStandardItemModel( 4,2 );
  tvFileContent->setModel( lvModel );
  
  for (int lvRow = 0; lvRow < 4; ++lvRow) 
  {
    for (int lvColumn = 0; lvColumn < 2; ++lvColumn) 
    {
      QModelIndex lvIndex = lvModel->index(lvRow, lvColumn, QModelIndex());
      lvModel->setData(lvIndex, QVariant((lvRow+1) * (lvColumn+1)));
    }
  }

}

/*
 *
 * PRIVATE SLOTS
 *
 */

void EgtFileReaderControls::onthe_pbtnOpenFile_clicked()
{qDebug("push the button"); 

  if( 0 == cvExifIoEngine ) { return; }
  
  /*leLongitude->setText( QString::number( cvExifIoEngine->longitude(), 'f', 7  ) );
  longitudeControls->setEnabled( false );*/
}

void EgtFileReaderControls::onthe_pbtnTagPicture_clicked()
{
qDebug("push the button"); 

  if( 0 == cvExifIoEngine ) { return; }
  
  /*if( !cvExifIoEngine->writeLongitude( leLongitude->text() ) )
  {
    QMessageBox::critical( this, tr( "Write Error" ), tr( "Unable to write value into exif header" ) ); 
  }
  else
  {
    leLongitude->setText( QString::number( cvExifIoEngine->longitude(), 'f', 7  ) );
    longitudeControls->setEnabled( false );
  }*/
}
