/*
** File: egtreaderfactory.cpp
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
#include <QFileDialog>

#include "egtreaderfactory.h"

EgtReaderFactory::EgtReaderFactory( )
{
  cvFileReader = new EgtGraphicalDelimitedTextFileReader();

  
  cvUiFileType.setupUi(&cvFileTypeDialog);

  connect( cvUiFileType.pbtnOk, SIGNAL( clicked() ), this, SLOT( on_pbtnOk_clicked() ) ); 
  connect( cvUiFileType.pbtnCancel, SIGNAL( clicked() ), this, SLOT( on_pbtnCancel_clicked() ) );
  connect( cvUiFileType.rbDelimitedText, SIGNAL( toggled(bool) ), this, SLOT( on_rbDelimitedText_toggled(bool) ) );
  connect( cvUiFileType.rbGPSFile, SIGNAL( toggled(bool) ), this, SLOT( on_rbGPSFile_toggled(bool) ) );
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */

void EgtReaderFactory::show()
{
  cvFileTypeDialog.show();

  connect( cvFileReader, SIGNAL( delimiterSelected() ), this, SLOT( reEmitDelimiterSelected() ) );
}


/*
 *
 * PRIVATE FUNCTIONS
 *
 */


/*
 *
 * SIGNAL and SLOTS
 *
 */
void EgtReaderFactory::reEmitDelimiterSelected()
{
  emit(fileReaderCreated( cvFileReader ));
}

void EgtReaderFactory::on_pbtnOk_clicked()
{
qDebug("ok clicked");
cvFileTypeDialog.setVisible(false);
cvFileReader->show();
}

void EgtReaderFactory::on_pbtnCancel_clicked()
{
qDebug("cancel clicked");
}

void EgtReaderFactory::on_rbDelimitedText_toggled( bool theChange )
{
if( theChange )
qDebug("delimited text activated");
}

void EgtReaderFactory::on_rbGPSFile_toggled( bool theChange )
{
if( theChange )
qDebug("gps activated");
}

