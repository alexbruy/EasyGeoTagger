/*
** File: egtproviderfactory.cpp
** Author( s ): Roberto Garcia Yunta
** Creation Date: 2008-12-19
**
** Copyright ( c ) 2008, American Museum of Natural History. All rights reserved.
**
** This library/program is free software; you can redistribute it
** and/or modify it under the terms of the GNU Library General Public
** License as published by the Free Software Foundation; either
** version 2 of the License, or ( at your option ) any later version.
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
#include "egtproviderfactory.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>

EgtProviderFactory::EgtProviderFactory( )
{
  cvDataProvider = 0;

  cvUiFileType.setupUi( &cvFileTypeDialog );
  cvFileTypeDialog.setWindowIcon( QIcon( ":/icons/document-open.svg" ) );

  connect( cvUiFileType.buttonBox, SIGNAL( accepted( ) ), this, SLOT( accept( ) ) );
  connect( cvUiFileType.buttonBox, SIGNAL( rejected( ) ), this, SLOT( reject( ) ) );
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */

void EgtProviderFactory::show( )
{
  QWidget* lvActiveWindow = QApplication::activeWindow ( );
  QPoint lvPosition = lvActiveWindow->pos( );
  //center the window over the parent
  cvFileTypeDialog.move( lvPosition.x( ) + lvActiveWindow->width( )/2 - cvFileTypeDialog.width( )/2, lvPosition.y( ) + lvActiveWindow->height( )/2 - cvFileTypeDialog.height( )/2 );
  cvFileTypeDialog.show( ); 
}

/*
 *
 * SIGNAL and SLOTS
 *
 */
void EgtProviderFactory::accept( )
{ 
  cvFileTypeDialog.setVisible( false );
  if( cvUiFileType.rbDelimitedText ->isChecked( ) )
  {
    cvDataProvider = new EgtGraphicalDelimitedTextProvider( );
  }
  else if( cvUiFileType.rbGPSFile ->isChecked( ) )
  {
    //Nothing so far
    cvDataProvider = new EgtGraphicalDelimitedTextProvider( );
  }
  else
  {
    QMessageBox::warning( &cvFileTypeDialog, tr( "Error" ),tr( "You must select a file type" ),QMessageBox::Ok );
    cvFileTypeDialog.setVisible( true );
    return;
  }
  
  connect( cvDataProvider, SIGNAL( initializationComplete( ) ), this, SLOT( dataProviderInitialized( ) ) );
  cvDataProvider->init( );
}

void EgtProviderFactory::dataProviderInitialized( )
{
  emit( dataProviderCreated( cvDataProvider ) );
}


void EgtProviderFactory::reject( )
{
  cvFileTypeDialog.setVisible( false );
}