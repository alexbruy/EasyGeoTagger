/*
** File: egtgraphicaldelimitedtextfilereader.cpp
** Author(s): Roberto Garcia Yunta
** Creation Date: 2008-12-19
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
#include "egtgraphicaldelimitedtextfilereader.h"


#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>

EgtGraphicalDelimitedTextFileReader::EgtGraphicalDelimitedTextFileReader(  ):EgtDelimitedTextFileReader()
{
  cvUiTextDelimiter.setupUi(&cvSelectDelimiterDialog);

  connect( cvUiTextDelimiter.pbtnOk, SIGNAL( clicked() ), this, SLOT( on_pbtnOk_clicked() ) ); 
  connect( cvUiTextDelimiter.pbtnCancel, SIGNAL( clicked() ), this, SLOT( on_pbtnCancel_clicked() ) ); 
  connect( cvUiTextDelimiter.rbtnComma, SIGNAL( toggled( bool ) ), this, SLOT( on_rbtnComma_toggled( bool ) ) ); 
  connect( cvUiTextDelimiter.rbtnPipe, SIGNAL( toggled( bool ) ), this, SLOT( on_rbtnPipe_toggled( bool ) ) ); 
  connect( cvUiTextDelimiter.rbtnBlank, SIGNAL( toggled( bool ) ), this, SLOT( on_rbtnBlank_toggled( bool ) ) ); 
  connect( cvUiTextDelimiter.rbtnCustom, SIGNAL( toggled( bool ) ), this, SLOT( on_rbtnCustom_toggled( bool ) ) ); 
  connect( cvUiTextDelimiter.leCustom, SIGNAL( textEdited( QString ) ), this, SLOT( on_leCustom_changed() ) ); 
  connect( cvUiTextDelimiter.cbHeader, SIGNAL( stateChanged( int ) ), this, SLOT( on_cbHeader_changed() ) );
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */

void EgtGraphicalDelimitedTextFileReader::selectDelimiter()
{
  QWidgetList lvActiveWindows = QApplication::topLevelWidgets();
  QWidget* lvParent = 0;

  for( int i =0; i < lvActiveWindows.size(); i++ )
  {
    if( tr("GPS Reader") == lvActiveWindows[i]->windowTitle() )
    {
      lvParent = lvActiveWindows[i];
    }
  }

  QPoint lvPosition = lvParent->pos();
  //center the window over the parent
  cvSelectDelimiterDialog.move( lvPosition.x() + lvParent->width()/2 - cvSelectDelimiterDialog.width()/2, lvPosition.y() + lvParent->height()/2 - cvSelectDelimiterDialog.height()/2 );
  cvSelectDelimiterDialog.show();
}

void EgtGraphicalDelimitedTextFileReader::init() //init function
{
  QWidgetList lvActiveWindows = QApplication::topLevelWidgets();
  QWidget* lvParent = 0;

  for( int i =0; i < lvActiveWindows.size(); i++ )
  {
    if( tr("Select a file type") == lvActiveWindows[i]->windowTitle() )
    {
      lvParent = lvActiveWindows[i];
    }
  }

  QString lvFileName = QFileDialog::getOpenFileName( lvParent, tr("Open GPS File"), "/home", tr("GPS Files (*.txt *.gps)"));
  if ( "" != lvFileName )
  {
    setFileName( lvFileName );
    selectDelimiter();
  }
} 


/*
 *
 * SIGNAL and SLOTS
 *
 */

void EgtGraphicalDelimitedTextFileReader::on_cbHeader_changed()
{
  cvHasColumnHeaders = cvUiTextDelimiter.cbHeader->isChecked();
  emit delimiterChanged();
}

void EgtGraphicalDelimitedTextFileReader::on_leCustom_changed()
{
  if( cvUiTextDelimiter.rbtnCustom->isChecked() )
  {
    setDelimiter( cvUiTextDelimiter.leCustom->text() );
    emit delimiterChanged();
  }
}

void EgtGraphicalDelimitedTextFileReader::on_pbtnCancel_clicked()
{
  cvSelectDelimiterDialog.close();
}

void EgtGraphicalDelimitedTextFileReader::on_pbtnOk_clicked()
{
  if( 0 == cvFileData.size() )
  {
    QMessageBox::critical( &cvSelectDelimiterDialog, tr("Error"),tr("You must have a valid file"),QMessageBox::Ok );
  }
  else
  {
    cvSelectDelimiterDialog.close();
    emit initializationComplete();
  }
}

void EgtGraphicalDelimitedTextFileReader::on_rbtnBlank_toggled( bool theValue )
{
  if( theValue )
  {
    setDelimiter( " " );
    delimiterChanged();
  }
}

void EgtGraphicalDelimitedTextFileReader::on_rbtnComma_toggled( bool theValue )
{
  if( theValue )
  {
    setDelimiter( "," );
    delimiterChanged();
  }
}

void EgtGraphicalDelimitedTextFileReader::on_rbtnCustom_toggled( bool theValue )
{
  if( theValue )
  {
    setDelimiter( cvUiTextDelimiter.leCustom->text() );
    delimiterChanged();
  }
}

void EgtGraphicalDelimitedTextFileReader::on_rbtnPipe_toggled( bool theValue )
{
  if( theValue )
  {
    setDelimiter( "|" );
    delimiterChanged();
  }
}

/*
 *
 * PRIVATE FUNCTIONS
 *
 */

void EgtGraphicalDelimitedTextFileReader::delimiterChanged()
{
  cvFileData = read();

  cvUiTextDelimiter.lbStatus->setText( cvLastError );


  QString lvHTML = "<table border=\"1\">";

  int lvNumRows = (cvFileData.size() > 3)? 3: cvFileData.size(); // We just show up to 3 rows

  for( int i = 0; i < lvNumRows; i++ )
  {
    lvHTML +="<tr>";
    for( int j = 0; j < cvFileData[0].size(); j++ )
    {
      lvHTML = lvHTML+"<td>"+cvFileData.at(i).at(j)+"</td>";
    }
    lvHTML +="</tr>";
  }
  lvHTML +="</table>";

  cvUiTextDelimiter.tePreview->setHtml( lvHTML );
}
