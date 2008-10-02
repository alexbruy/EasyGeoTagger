/*
** File: egtpluginmanager.cpp
** Author(s): Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2008-09-30
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
#include "egtlogger.h"
#include "egtpluginmanager.h"
#include "egtplugininterface.h"

#include <QDir>
#include <QStringList>
#include <QPluginLoader>
#include <QCoreApplication>

EgtPluginManager::EgtPluginManager( EgtApplicationInterface* theApplicationInterface )
{
  cvApplicationInterface = theApplicationInterface;
}

void EgtPluginManager::loadPlugins()
{
  EgtDebug( "entered" );
  
  //Check to make sure we have a valid application interface
  if( 0 == cvApplicationInterface )
  {
    EgtDebug( "Application interface was null...bailing" )
    return;
  }
  
  //Clear existing plugin map
  cvApplicationInterface->cvPlugins.clear();
  
  //TODO: Consider making this path as a parameter or QStringList so users can have custom plugin directories
  QDir pluginDirectory( QCoreApplication::instance ()->applicationDirPath() + "/plugin/" );
  QStringList lvFilenames = pluginDirectory.entryList( QStringList() << "lib*" );

  //Loop throught the files in the plugin directory and see if we have any valid plugins
  QStringList::const_iterator lvIterator;
  for( lvIterator = lvFilenames.constBegin(); lvIterator != lvFilenames.constEnd(); ++lvIterator )
  {
    EgtPluginInterface* lvInterface;
    QPluginLoader lvPluginLoader( pluginDirectory.absoluteFilePath( *lvIterator ) );
    QObject* lvPlugin = lvPluginLoader.instance();
    if (lvPlugin) 
    {
      lvInterface = qobject_cast<EgtPluginInterface *>( lvPlugin );
      if( lvInterface )
      {
        lvInterface->setApplicationInterface( cvApplicationInterface );
        cvApplicationInterface->cvPlugins.insert( lvInterface->name(), lvInterface );
        EgtDebug( "loading plugin ["+ lvInterface->name() +"]" );
      }
    }
    else
    {
      EgtDebug( lvPluginLoader.errorString() );
    }
  }
}

