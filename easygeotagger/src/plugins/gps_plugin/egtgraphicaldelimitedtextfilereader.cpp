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

  connect( this, SIGNAL( delimiterChanged() ), this, SLOT( on_delimiter_changed() ) ); 
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */

void EgtGraphicalDelimitedTextFileReader::selectDelimiter()
{
  cvSelectDelimiterDialog.show();

}

void EgtGraphicalDelimitedTextFileReader::show()
{
  QString lvFileName = QFileDialog::getOpenFileName(0, tr("Open GPS File"), "/home", tr("GPS Files (*.txt *.gps)"));
  if ( lvFileName != "" )
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

void EgtGraphicalDelimitedTextFileReader::on_delimiter_changed()
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
  if( cvFileData.size() == 0 )
    QMessageBox::critical( &cvSelectDelimiterDialog, tr("Error"),tr("You must have a valid file"),QMessageBox::Ok );
  else
  {
    cvSelectDelimiterDialog.close();
    emit delimiterSelected();
  }
}

void EgtGraphicalDelimitedTextFileReader::on_rbtnBlank_toggled( bool theValue )
{
  if( theValue )
  {
    setDelimiter( " " );
    emit delimiterChanged();
  }
}

void EgtGraphicalDelimitedTextFileReader::on_rbtnComma_toggled( bool theValue )
{
  if( theValue )
  {
    setDelimiter( "," );
    emit delimiterChanged();
  }
}

void EgtGraphicalDelimitedTextFileReader::on_rbtnCustom_toggled( bool theValue )
{
  if( theValue )
  {
    setDelimiter( cvUiTextDelimiter.leCustom->text() );
    emit delimiterChanged();
  }
}

void EgtGraphicalDelimitedTextFileReader::on_rbtnPipe_toggled( bool theValue )
{
  if( theValue )
  {
    setDelimiter( "|" );
    emit delimiterChanged();
  }
}
