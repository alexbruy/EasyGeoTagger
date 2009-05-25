/*
** File: egtkmlprovider.cpp
** Author( s ): Roberto Garcia Yunta
** Creation Date: 2009-05-22
**
** Copyright ( c ) 2009, American Museum of Natural History. All rights reserved.
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
#include "egtgpxprovider.h"

#include "egtgpsbabelinterface.h"


#include "egtgpxparser.h"
#include "egtlogger.h"

#include <QtPlugin>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QTemporaryFile>

EgtGpxProvider::EgtGpxProvider( ) : EgtGpsProvider( )
{
  cvFileName = ""; 
  cvLastError = "";
  cvName = "GPX";
  cvFileType = "gpx";
  //cvConfigurationDialog = 0;
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */
void EgtGpxProvider::configure( ) //init function
{
  EgtDebug( "entered" );

  QWidgetList lvActiveWindows = QApplication::topLevelWidgets( );
  QWidget* lvParent = 0;

  for( int i = 0; i < lvActiveWindows.size( ); i++ )
  {
    if( tr( "Available Providers" ) == lvActiveWindows[i]->windowTitle( ) )
    {
      lvParent = lvActiveWindows[i];
    }
  }

  QString lvFileName = QFileDialog::getOpenFileName( lvParent, tr( "Open GPS File" ), "/home", tr( "GPX Files ( *.gpx  )" ) );
  if ( "" != lvFileName )
  {
    setFileName( lvFileName );
    
    showConfigurationDialog( );
  }
} 


void EgtGpxProvider::showConfigurationDialog( )
{
  EgtDebug( "entered" );

  notifyInitializationComplete( true );
}


Q_EXPORT_PLUGIN2( gpxprovider, EgtGpxProvider );
