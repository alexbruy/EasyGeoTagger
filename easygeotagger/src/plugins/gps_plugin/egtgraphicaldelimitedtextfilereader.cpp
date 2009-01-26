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
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QTextStream>

EgtGraphicalDelimitedTextFileReader::EgtGraphicalDelimitedTextFileReader(  ):EgtDelimitedTextFileReader()
{
  cvUiTextDelimiter.setupUi(&cvSelectDelimiterDialog);
  connect( cvUiTextDelimiter.pbtnOk, SIGNAL( clicked() ), this, SLOT( on_pbtnOk_clicked() ) ); 
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */

void EgtGraphicalDelimitedTextFileReader::selectDelimiter()
{
  cvSelectDelimiterDialog.show();
  QStringList lvFileData = read();
  QString lvString = lvFileData.join(",");
  cvUiTextDelimiter.tePreview->setPlainText( "THIS IS NOT WORKING:"+lvString );
}

void EgtGraphicalDelimitedTextFileReader::show()
{
  QString lvFileName = QFileDialog::getOpenFileName(0, tr("Open GPS File"), "/home", tr("GPS Files (*.txt *.gps)"));
  setFileName( lvFileName );
  selectDelimiter();
} 


/*
 *
 * SIGNAL and SLOTS
 *
 */
void EgtGraphicalDelimitedTextFileReader::on_pbtnOk_clicked()
{
  setDelimiter( cvUiTextDelimiter.leCustom->text() );
  cvSelectDelimiterDialog.close();
  emit delimiterSelected();
}




