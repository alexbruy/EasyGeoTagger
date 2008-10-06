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
#include <QToolBox>
#include <QStringList>
#include <QPluginLoader>
#include <QCoreApplication>

EgtPluginManager::EgtPluginManager( EgtApplicationInterface* theApplicationInterface, EgtMainWindow* theMainWindow )
{
  cvApplicationInterface = theApplicationInterface;
  cvMainWindow = theMainWindow;
  cvDefaultPluginPath = QCoreApplication::instance ()->applicationDirPath() + "/plugin/";
}

bool EgtPluginManager::loadPlugin( QString theLibrary )
{
  EgtDebug( "entered" );
  
  //Check to make sure we have a valid application interface
  if( 0 == cvApplicationInterface || 0 == cvMainWindow )
  {
    EgtDebug( "Application interface or main window was null...bailing" )
    return false;
  }
  
  EgtPluginInterface* lvInterface;
  QPluginLoader lvPluginLoader( theLibrary );
  QObject* lvPlugin = lvPluginLoader.instance();
  if (lvPlugin) 
  {
    lvInterface = qobject_cast<EgtPluginInterface *>( lvPlugin );
    if( lvInterface )
    {
      //Set the pointer to the application interface and gui
      lvInterface->setApplicationInterface( cvApplicationInterface );
      lvInterface->setGui( cvMainWindow );
      
      //Loop through all of the categories and build the display components
      QString lvKey;
      for( int lvIterator = 0; lvIterator < lvInterface->categories().size(); lvIterator++ )
      {
        lvKey = lvInterface->categories().at( lvIterator );
        //Check to make sure the category exists, if not create it
        //TODO break out into spearate function, add sort
        if( !cvPluginDisplayCollection.contains( lvKey ) )
        {
          EgtDebug( "Creating new collection for plugin category: "+ lvKey );
          QMap< QString, EgtPluginDisplayWidget* >* lvPluginDisplayWidgets = new QMap< QString, EgtPluginDisplayWidget* >;
          cvPluginDisplayCollection.insert( lvKey, lvPluginDisplayWidgets );
        }
        
        //TODO break out into spearate function, add sort
        //See if there is aready a display, if not create one.
        if( !cvPluginDisplayCollection[ lvKey ]->contains( lvInterface->name() ) )
        {
          EgtDebug( "Creating new plugin display for plugin: "+ lvInterface->name() );
          EgtPluginDisplayWidget* lvPluginDisplayWidget = new EgtPluginDisplayWidget( lvInterface );
          cvPluginDisplayCollection[ lvKey ]->insert( lvInterface->name(), lvPluginDisplayWidget );
        }
        else
        {
          cvPluginDisplayCollection[ lvKey ]->value( lvInterface->name() )->update( lvInterface );
        }
      }
      
      //Insert plugin into the application interfaces master plugin list
      cvApplicationInterface->cvPlugins.insert( lvInterface->name(), lvInterface );
      EgtDebug( "loaded plugin ["+ lvInterface->name() +"]" );
    }
    else
    {
      EgtDebug( "["+ theLibrary +"] is not a valid plugin" );
      return false;
    }
  }
  else
  {
    EgtDebug( lvPluginLoader.errorString() );
    return false;
  }
  
  return true;
}

void EgtPluginManager::loadPlugins( QString theDirectory )
{
  EgtDebug( "entered" );
  
  //Check to make sure we have a valid application interface
  if( 0 == cvApplicationInterface || 0 == cvMainWindow )
  {
    EgtDebug( "Application interface or main window was null...bailing" )
    return;
  }
  
  //Use the default directory is none is provided
  QString lvPluginDirectory = theDirectory;
  if( theDirectory == "" )
  {
    lvPluginDirectory = cvDefaultPluginPath;
  }
  
  //Get a list of all of the libs in the plugin directory;
  QDir pluginDirectory( lvPluginDirectory );
  if( !pluginDirectory.exists() )
  {
    EgtDebug( "["+ theDirectory +"] was not a valid directory" )
    return;
  }
  QStringList lvFilenames = pluginDirectory.entryList( QStringList() << "lib*" );

  //Loop throught the files in the plugin directory and see if we have any valid plugins
  QStringList::const_iterator lvIterator;
  for( lvIterator = lvFilenames.constBegin(); lvIterator != lvFilenames.constEnd(); ++lvIterator )
  {
    loadPlugin( pluginDirectory.absoluteFilePath( *lvIterator ) );
  }
}

void EgtPluginManager::updateGui()
{
  EgtDebug( "entered" )
  //Update the gui
  QToolBox* lvPluginToolBox = new QToolBox( cvMainWindow );
  QList< QString > lvCollectionKeys = cvPluginDisplayCollection.keys();
  for(int lvIterator = 0; lvIterator < lvCollectionKeys.size(); lvIterator++)
  {
    EgtDebug( "Building display panel for key: "+ lvCollectionKeys.at( lvIterator ) );
    
    //create a new panel
    QWidget* lvPluginPanel = new QWidget( lvPluginToolBox );
    lvPluginPanel->setLayout( new QVBoxLayout );
    
    //Get the keys and loop through the display widget map
    QMap< QString, EgtPluginDisplayWidget*>* lvDisplayItems = cvPluginDisplayCollection.value( lvCollectionKeys.at( lvIterator ) );
    QList< QString > lvDisplayItemKeys = lvDisplayItems->keys();
    qDebug("%d", lvDisplayItemKeys.size());
    for(int lvItemToDisplay = 0; lvItemToDisplay < lvDisplayItemKeys.size(); lvItemToDisplay++)
    {
      EgtDebug( "...Adding plugin "+ lvDisplayItemKeys.at( lvItemToDisplay ) +" to the display panel" );
      lvPluginPanel->layout()->addWidget( lvDisplayItems->value( lvDisplayItemKeys.at( lvItemToDisplay ) ) );
    }
    //Add the panel to the tool box
    lvPluginToolBox->addItem(lvPluginPanel, lvCollectionKeys.at( lvIterator ) );
  }
  cvMainWindow->setPluginToolBox( lvPluginToolBox );
}
