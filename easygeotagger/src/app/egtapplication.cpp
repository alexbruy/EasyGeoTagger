/*
** File: egtapplication.cpp
** Author(s): Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2008-09-30
**
** Copyright (c) 2008-2009, American Museum of Natural History. All rights reserved.
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
#include "egtapplication.h"

#include "egtlogger.h"
#include "egtmainwindow.h"
#include "egtpluginmanager.h"

EgtApplication::EgtApplication()
{
  init();
}

EgtApplication::EgtApplication( QString thePluginDirectory )
{
  init( thePluginDirectory );
}

/*
 *
 * PRIVATE FUNCTIONS
 *
 */
/*!
 * \param thePluginDirectory a fully qualified path to the directory with EasyGeoTagger plugins
 */
void EgtApplication::init( QString thePluginDirectory )
{
  EgtDebug( "entered" );
  
  //TODO: Add slash screen showing progress, i.e. showing plugin

  //Create a new main window
  cvGui = new EgtMainWindow();
  
  //Create a new application interface and gui
  cvApplicationInterface = new EgtApplicationInterface( cvGui );
  
  //Create a new plugin manager and load plugins from the main plugin archive
  cvPluginManager = new EgtPluginManager( cvApplicationInterface, cvGui );

  connect( cvGui, SIGNAL( loadPlugins( QString ) ), cvPluginManager, SLOT( loadPlugins( QString ) ) );
  connect( cvApplicationInterface, SIGNAL( loadPluginRequest( QString ) ), cvPluginManager, SLOT( loadPlugins( QString ) ) );
  cvPluginManager->loadPlugins( thePluginDirectory );
  
  //Display the main window
  cvGui->show();
}

void EgtApplication::show()
{
  cvGui->show();
}
