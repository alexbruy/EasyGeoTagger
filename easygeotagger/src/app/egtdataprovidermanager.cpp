/*
** File: egtdataprovidermanager.cpp
** Author( s ): Peter J. Ersts ( ersts@amnh.org ), Roberto Garcia-Yunta
** Creation Date: 2009-05-15
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
#include "egtdataprovidermanager.h"
#include "egtlogger.h"

#include <QDir>
#include <QFileInfo>
#include <QPluginLoader>
#include <QCoreApplication>

EgtDataProviderManager::EgtDataProviderManager( )
{
  //Just a guess will not work on Mac with frameworks
#ifdef WIN32
#  ifdef OSGEO4W
     cvDefaultProviderPath = QCoreApplication::instance( )->applicationDirPath( ) + "/../apps/easygt/plugins/";
#  else
   cvDefaultProviderPath = QCoreApplication::instance( )->applicationDirPath( ) + "/plugins/";
#  endif
#else
  cvDefaultProviderPath = QCoreApplication::instance( )->applicationDirPath( ) + "/../lib/easygt/";
#endif
}

EgtDataProviderManager::~EgtDataProviderManager()
{
}


/*
 *
 * PUBLIC FUNCTIONS
 *
 */

QStringList EgtDataProviderManager::availableProviders( )
{
  EgtDebug( "entered" );
  QList < QString > lvKeys = cvProviders.keys();
  qSort( lvKeys );
  return lvKeys;
}

/*!
 * \param theDirectory the fully qualified name of the directory containing providers to load
 */
void EgtDataProviderManager::loadAllProviders( QString theDirectory )
{
  EgtDebug( "entered" );

  //Use the default directory if none is provided or if the string does not actually represent a directory.
  QFileInfo lvFileInfo( theDirectory );
  QString lvPluginDirectory = theDirectory;
  if( theDirectory == "" || !lvFileInfo.isDir( ) )
  {
    lvPluginDirectory = cvDefaultProviderPath;
  }

  EgtDebug( "Plugin directory: " + lvPluginDirectory );

  //Get a list of all of the libs in the plugin directory;
  QDir pluginDirectory( lvPluginDirectory );
  if( !pluginDirectory.exists( ) )
  {
    EgtDebug( "["+ theDirectory +"] was not a valid directory" );
    return;
  }
  #ifdef WIN32
  QStringList lvFilenames = pluginDirectory.entryList( QStringList( ) << "*.dll" );
  #else
  QStringList lvFilenames = pluginDirectory.entryList( QStringList( ) << "lib*" );
  #endif

  //Loop throught the files in the plugin directory and see if we have any valid providers
  QStringList::const_iterator lvIterator;
  for( lvIterator = lvFilenames.constBegin( ); lvIterator != lvFilenames.constEnd( ); ++lvIterator )
  {
    loadSingleProvider( pluginDirectory.absoluteFilePath( *lvIterator ) );
  }
}

/*!
 * \param thePath the fully qualified name of the shared library or directory of providers to load
 */
void EgtDataProviderManager::loadProviders( QString thePath )
{
  EgtDebug( "entered" );
  EgtDebug( "Received: " + thePath );

  //If the path is blank bail
  if( thePath == "" )
  {
    return;
  }

  QFileInfo lvFileInfo( thePath );
  if( lvFileInfo.isDir( ) || !lvFileInfo.exists( ) )
  {
    loadAllProviders( thePath );
  }
  else
  {
    loadSingleProvider( thePath );
  }
}

/*!
 * \param theLibrary the fully qualified filename for the provider library to load
 * \returns true of the provider is successfully loaded, otherwise returns false
 */
bool EgtDataProviderManager::loadSingleProvider( QString theLibrary )
{
  EgtDebug( "entered" );

  //Create an instance of a plugin loader the attempt to open the library
  QPluginLoader lvPluginLoader( theLibrary );
  QObject* lvPlugin = lvPluginLoader.instance( );
  if ( lvPlugin )
  {
    //If the library is a plugin, see if it has an EgtDataProvider
    EgtDataProvider* lvInterface = qobject_cast<EgtDataProvider *>( lvPlugin );
    if( lvInterface )
    {
      cvProviders[ lvInterface->name( ) ] = theLibrary;
      emit providerLoaded( tr( "Provider") +": "+ lvInterface->name( ) );
      EgtDebug( "loaded provider ["+ lvInterface->name( ) +"]" );
    }
    else
    {
      EgtDebug( "["+ theLibrary +"] is not a valid data provider" );
      return false;
    }
  }
  else
  {
    EgtDebug( lvPluginLoader.errorString( ) );
    return false;
  }

  return true;
}

EgtDataProvider* EgtDataProviderManager::provider( QString theProviderName )
{
  EgtDebug( "entered" );

  if( cvProviders.contains( theProviderName ) )
  {
    //Create an instance of a plugin loader the attempt to open the library
    QPluginLoader lvPluginLoader( cvProviders[ theProviderName ] );
    QObject* lvPlugin = lvPluginLoader.instance( );
    if ( lvPlugin )
    {
      //If the library is a plugin, see if it has an EgtDataProvider
      EgtDataProvider* lvInterface = qobject_cast<EgtDataProvider *>( lvPlugin );
      if( lvInterface )
      {
        return lvInterface;
      }
    }
    EgtDebug( "Count not create instance of provider ["+ cvProviders[ theProviderName ] +"]" );
    return new EgtDataProvider();
  }

  EgtDebug( "Request for unknown data provider ["+ cvProviders[ theProviderName ] +"]" ) ;
  return new EgtDataProvider();
}

