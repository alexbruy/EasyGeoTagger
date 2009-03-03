/*
** File: egtpluginmanager.cpp
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
#include "egtlogger.h"
#include "egtpluginmanager.h"
#include "egtplugininterface.h"

#include <QDir>
#include <QToolBox>
#include <QFileInfo>
#include <QStringList>
#include <QPluginLoader>
#include <QCoreApplication>

EgtPluginManager::EgtPluginManager( EgtApplicationInterface* theApplicationInterface, EgtMainWindow* theMainWindow )
{
  cvApplicationInterface = theApplicationInterface;
  cvGui = theMainWindow;

  //Just a guess will not work on Mac with frameworks
#ifdef WIN32
#  ifdef OSGEO4W
     cvDefaultPluginPath = QCoreApplication::instance( )->applicationDirPath( ) + "/../apps/easygt/plugins/";
#  else
	 cvDefaultPluginPath = QCoreApplication::instance( )->applicationDirPath( ) + "/plugins/";
#  endif
#else
  cvDefaultPluginPath = QCoreApplication::instance( )->applicationDirPath( ) + "/../lib/easygt/";
#endif
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */

/*!
 * \param theDirectory the fully qualified name of the directory containing plugins to load
 */
void EgtPluginManager::loadAllPlugins( QString theDirectory )
{
  EgtDebug( "entered" );

  //Check to make sure we have a valid application interface
  if( 0 == cvApplicationInterface || 0 == cvGui )
  {
    EgtDebug( "Application interface or main window was null...bailing" );
    return;
  }

  //Use the default directory if none is provided or if the string does not actually represent a directory.
  QFileInfo lvFileInfo( theDirectory );
  QString lvPluginDirectory = theDirectory;
  if( theDirectory == "" || !lvFileInfo.isDir( ) )
  {
    lvPluginDirectory = cvDefaultPluginPath;
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

  //Loop throught the files in the plugin directory and see if we have any valid plugins
  QStringList::const_iterator lvIterator;
  for( lvIterator = lvFilenames.constBegin( ); lvIterator != lvFilenames.constEnd( ); ++lvIterator )
  {
    loadSinglePlugin( pluginDirectory.absoluteFilePath( *lvIterator ) );
  }
}

/*!
 * \param thePath the fully qualified name of the shared library or directory of plugins to load
 */
void EgtPluginManager::loadPlugins( QString thePath )
{
  EgtDebug( "entered" );
  EgtDebug( "Received: " + thePath );

  //Check to make sure we have a valid application interface
  if( 0 == cvApplicationInterface || 0 == cvGui )
  {
    EgtDebug( "Application interface or main window was null...bailing" );
    return;
  }

  //If the path is blank bail
  if( thePath == "" )
  {
    return;
  }

  QFileInfo lvFileInfo( thePath );
  if( lvFileInfo.isDir( ) || !lvFileInfo.exists( ) )
  {
    loadAllPlugins( thePath );
  }
  else
  {
    loadSinglePlugin( thePath );
  }
  updateGui( );
}

/*!
 * \param theLibrary the fully qualified filename for the plugin library to load
 * \returns true of the plugin is successfully loaded, otherwise returns false
 */
bool EgtPluginManager::loadSinglePlugin( QString theLibrary )
{
  EgtDebug( "entered" );
  
  //Check to make sure we have a valid application interface
  if( 0 == cvApplicationInterface || 0 == cvGui )
  {
    EgtDebug( "Application interface or main window was null...bailing" );
    return false;
  }
  
  //Create an instance of a plugin loader the attempt to open the library
  QPluginLoader lvPluginLoader( theLibrary );
  QObject* lvPlugin = lvPluginLoader.instance( );
  if ( lvPlugin )
  {
    //If the library is a plugin, see if it has an EgtPluginInterface
    EgtPluginInterface* lvInterface = qobject_cast<EgtPluginInterface *>( lvPlugin );
    if( lvInterface )
    {
      //Set the plugin's pointer to the application interface and gui
      lvInterface->setApplicationInterface( cvApplicationInterface );
      lvInterface->initPlugin( );
      
      //Loop through all of the categories and build the display components
      QString lvKey;
      for( int lvIterator = 0; lvIterator < lvInterface->categories( ).size( ); lvIterator++ )
      {
        lvKey = lvInterface->categories( ).at( lvIterator );
        //Check the collection map to see if the category exists, if not create it
        if( !cvPluginDisplayCollection.contains( lvKey ) )
        {
          EgtDebug( "Creating new collection for plugin category: "+ lvKey );
          QMap< QString, EgtPluginDisplayWidget* >* lvPluginDisplayWidgets = new QMap< QString, EgtPluginDisplayWidget* >;
          cvPluginDisplayCollection.insert( lvKey, lvPluginDisplayWidgets );
        }
        
        //Check the collection map to see if there is already a display object, if not create one.
        if( !cvPluginDisplayCollection[ lvKey ]->contains( lvInterface->name( ) ) )
        {
          EgtDebug( "Creating new plugin display for plugin: "+ lvInterface->name( ) );
          EgtPluginDisplayWidget* lvPluginDisplayWidget = new EgtPluginDisplayWidget( lvInterface );
          cvPluginDisplayCollection[ lvKey ]->insert( lvInterface->name( ), lvPluginDisplayWidget );
        }
        else
        {
          //TODO: Look at this a little more, this seems like it is a memory leak,
          // what happens to the original instance of the plugin?

          //If a display object already exists, update it
          cvPluginDisplayCollection[ lvKey ]->value( lvInterface->name( ) )->update( lvInterface );
        }
      }

      emit pluginLoaded( lvInterface->name( ) );
      EgtDebug( "loaded plugin ["+ lvInterface->name( ) +"]" );
    }
    else
    {
      EgtDebug( "["+ theLibrary +"] is not a valid plugin" );
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

/*!
 * Create a new tool box holding all of the plugin displays and update the plugin display dock widget in the main window
 */
void EgtPluginManager::updateGui( )
{
  EgtDebug( "entered" );

  //TODO: See if there is a way to just update the existing display widgets rather than totally rebuilding
  //TODO: Evaluate the memory loss here

  //Create a new tool box to hold the plugins
  QToolBox* lvPluginToolBox = new QToolBox( cvGui );
  QList< QString > lvCollectionKeys = cvPluginDisplayCollection.keys( );
  for( int lvIterator = 0; lvIterator < lvCollectionKeys.size( ); lvIterator++ )
  {
    EgtDebug( "Building display panel for key: "+ lvCollectionKeys.at( lvIterator ) );

    //Create a new planel for the plugin collection
    QWidget* lvPluginPanel = new QWidget( lvPluginToolBox );
    lvPluginPanel->setLayout( new QVBoxLayout );

    //Get the keys and loop through the display widget map
    QMap< QString, EgtPluginDisplayWidget*>* lvDisplayItems = cvPluginDisplayCollection.value( lvCollectionKeys.at( lvIterator ) );
    QList< QString > lvDisplayItemKeys = lvDisplayItems->keys( );
    for( int lvItemToDisplay = 0; lvItemToDisplay < lvDisplayItemKeys.size( ); lvItemToDisplay++ )
    {
      EgtDebug( "...Adding plugin "+ lvDisplayItemKeys.at( lvItemToDisplay ) +" to the display panel" );
      lvPluginPanel->layout( )->addWidget( lvDisplayItems->value( lvDisplayItemKeys.at( lvItemToDisplay ) ) );
    }
    //Add a spacing item to get the buttons to float plugins to the top of the tab
 ( ( QVBoxLayout* ) lvPluginPanel->layout( ) )->insertStretch( -1, 1 );
    //Add the panel to the tool box
    lvPluginToolBox->addItem( lvPluginPanel, lvCollectionKeys.at( lvIterator ) );
  }

  //Set the display dock's main widget top the new toolbox
  cvGui->setPluginToolBox( lvPluginToolBox );
}
