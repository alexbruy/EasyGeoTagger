/*
** File: egtgraphicaldelimitedtextfilereader.cpp
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
#include "egtgraphicaldelimitedtextfilereader.h"


#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>

EgtGraphicalDelimitedTextFileReader::EgtGraphicalDelimitedTextFileReader( ):EgtDelimitedTextFileReader( )
{
  cvUiKmlOptions.setupUi( &cvKmlOptionsDialog );
  cvKmlOptionsDialog.setWindowIcon( QIcon( ":/icons/EasyGT.svg" ) );

  connect( cvUiKmlOptions.buttonBox, SIGNAL( accepted( ) ), this, SLOT( accept( ) ) );
  connect( cvUiKmlOptions.buttonBox, SIGNAL( rejected( ) ), this, SLOT( reject( ) ) ); 
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */



void EgtGraphicalDelimitedTextFileReader::init( ) //init function
{
  QWidgetList lvActiveWindows = QApplication::topLevelWidgets( );
  QWidget* lvParent = 0;

  for( int i = 0; i < lvActiveWindows.size( ); i++ )
  {
    if( tr( "Select a file type" ) == lvActiveWindows[i]->windowTitle( ) )
    {
      lvParent = lvActiveWindows[i];
    }
  }

  QString lvFileName = QFileDialog::getOpenFileName( lvParent, tr( "Open KML File" ), "/home", tr( "KML Files ( *.txt *.kml )" ) );
  if ( "" != lvFileName )
  {
    setFileName( lvFileName );
    cvKmlOptionsDialog.show();
  }
} 


/*
 *
 * SIGNAL and SLOTS
 *
 */

v

void EgtGraphicalDelimitedTextFileReader::reject( )
{
  cvKmlOptionsDialog.close( );
}

void EgtGraphicalDelimitedTextFileReader::accept( )
{
  if( 0 == cvFileData.size( ) )
  {
    QMessageBox::critical( &cvSelectDelimiterDialog, tr( "Error" ),tr( "You must have a valid file" ),QMessageBox::Ok );
  }
  else
  {
    cvKmlOptionsDialog.close( );
    emit initializationComplete( );
  }
}




/*
 *
 * PRIVATE FUNCTIONS
 *
 */
