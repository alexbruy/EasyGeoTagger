/*
** File: egtapplication.cpp
** Author( s ): Peter J. Ersts ( ersts at amnh.org )
** Creation Date: 2008-09-30
**
** Copyright ( c ) 2008-2009, American Museum of Natural History. All rights reserved.
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
#include "egtapplication.h"

#include "egtlogger.h"
#include "egtmainwindow.h"
#include "egtpluginmanager.h"
#include "egtdataprovidermanager.h"

#include <QApplication>
#include <QDesktopWidget>

EgtApplication::EgtApplication( bool displaySplash )
{
  init( "", displaySplash );
}

EgtApplication::EgtApplication( QString thePluginDirectory, bool displaySplash )
{
  init( thePluginDirectory, displaySplash );
}
/*
 *
 * PUBLIC FUNCTIONS
 *
 */
void EgtApplication::show( )
{
  if( 0 != cvGui )
  {
    cvGui->show( );
  }
}
/*
 *
 * PRIVATE FUNCTIONS
 *
 */
/*!
 * \param thePluginDirectory a fully qualified path to the directory with EasyGeoTagger plugins
 * \param displaySplash a flag to indicate if the splash images should be displayed on startup
 */
void EgtApplication::init( QString thePluginDirectory, bool displaySplash )
{
  EgtDebug( "entered" );
  cvSplashScreen = 0;
  cvApplicationInterface = 0;

  //Show splash screen
  if( displaySplash )
  {  
    cvSplashScreen = new QSplashScreen( QPixmap( ":/splash/EasyGT-Logo.png" ) );
    cvSplashScreen->show( );
    cvSplashScreen->showMessage( tr( "Loading plugins..." ), Qt::AlignHCenter | Qt::AlignBottom );
  }

  //Create a new application interface and gui
  cvApplicationInterface = new EgtApplicationInterface( );
  if( 0 == cvApplicationInterface ) { exit( 0 ); }

  //Create a new main window
  cvGui = new EgtMainWindow( );
  cvApplicationInterface->setGui( cvGui );
  
  //Create a new plugin manager and load plugins from the main plugin archive
  cvPluginManager = new EgtPluginManager( cvApplicationInterface, cvGui );
  cvApplicationInterface->setPluginManager( cvPluginManager );
  if( displaySplash && cvPluginManager )
  {
    connect( cvPluginManager, SIGNAL( pluginLoaded( QString ) ), this, SLOT( showSplashScreenMessage( QString ) ) );
  }

  //Create a new provider manager and load providers from the main plugin archive
  cvProviderManager = new EgtDataProviderManager( );
  cvApplicationInterface->setDataProviderManager( cvProviderManager );
  if( displaySplash && cvPluginManager )
  {
    connect( cvProviderManager, SIGNAL( providerLoaded( QString ) ), this, SLOT( showSplashScreenMessage( QString ) ) );
  }

  if( cvGui && cvApplicationInterface && cvPluginManager && cvProviderManager )
  {
    connect( cvGui, SIGNAL( loadPlugins( QString ) ), cvPluginManager, SLOT( loadPlugins( QString ) ) );

    cvPluginManager->loadPlugins( thePluginDirectory );
    cvProviderManager->loadProviders( thePluginDirectory );
  }
  
  if( displaySplash )
  {
    cvSplashScreen->finish( cvGui );
  }

  QDesktopWidget* lvDesktop = QApplication::desktop( );
  if( 0 != lvDesktop )
  {
    QRect lvScreen = lvDesktop->screenGeometry( );
    cvGui->move( lvScreen.center( ).x( ) - ( int )( cvGui->width( ) / 2 ), lvScreen.center( ).y( ) - ( int )( cvGui->height( ) / 2 ) );
  }
  
  //Display the main window
  cvGui->show( );

  if( displaySplash )
  {
    delete cvSplashScreen;
    cvSplashScreen = 0;
  }
}

void EgtApplication::showSplashScreenMessage( QString theMessage )
{
  if( 0 != cvSplashScreen )
  {
    cvSplashScreen->showMessage( theMessage, Qt::AlignHCenter | Qt::AlignBottom );
  }
}
