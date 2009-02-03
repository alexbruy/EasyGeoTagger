/***************************************************************************
  easygeotagger.cpp
  Launch EasyGeoTagger and connect to QGIS
  -------------------
         begin                : 2008-10-20
         copyright            : (C) Peter J. Ersts 2008
         email                : ersts at amnh.org

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/*  $Id: plugin.cpp 9327 2008-09-14 11:18:44Z jef $ */

//
// QGIS Specific includes
//

#include <qgisinterface.h>
#include <qgisgui.h>

#include "easygeotagger.h"

//
// Qt4 Related Includes
//

#include <QAction>
#include <QToolBar>
#include <QLibrary>
#include <QMessageBox>


static const char * const sIdent = "$Id: plugin.cpp 9327 2008-09-14 11:18:44Z jef $";
static const QString sName = QObject::tr( "EasyGeoTagger" );
static const QString sDescription = QObject::tr( "Launch EasyGeoTagger and connect to QGIS" );
static const QString sPluginVersion = QObject::tr( "Version 0.1" );
static const QgisPlugin::PLUGINTYPE sPluginType = QgisPlugin::UI;

//////////////////////////////////////////////////////////////////////
//
// THE FOLLOWING METHODS ARE MANDATORY FOR ALL PLUGINS
//
//////////////////////////////////////////////////////////////////////

/**
 * Constructor for the plugin. The plugin is passed a pointer
 * an interface object that provides access to exposed functions in QGIS.
 * @param theQGisInterface - Pointer to the QGIS interface object
 */
EasyGeoTagger::EasyGeoTagger( QgisInterface * theQgisInterface ):
    QgisPlugin( sName, sDescription, sPluginVersion, sPluginType ),
    mQGisIface( theQgisInterface )
{
}

EasyGeoTagger::~EasyGeoTagger()
{

}

/*
 * Initialize the GUI interface for the plugin - this is only called once when the plugin is
 * added to the plugin registry in the QGIS application.
 */
void EasyGeoTagger::initGui()
{
  cvIdTool = 0;
  cvPluginGui = 0;
  cvEasyGeoTaggerApplication =  0;
  
  // Create the action for tool
  mQActionPointer = new QAction( QIcon( ":/easygeotagger/EasyGT.svg" ), tr( "EasyGeoTagger" ), this );
  // Set the what's this text
  mQActionPointer->setWhatsThis( tr( "Launches the EasyGeoTagger application and connects it to QGIS" ) );
  // Connect the action to the run
  connect( mQActionPointer, SIGNAL( triggered() ), this, SLOT( run() ) );
  // Add the icon to the toolbar
  mQGisIface->addToolBarIcon( mQActionPointer );
  mQGisIface->addPluginToMenu( tr( "&EasyGeoTagger" ), mQActionPointer );
}
//method defined in interface
void EasyGeoTagger::help()
{
  //implement me!
}

// Slot called when the menu item is triggered
// If you created more menu items / toolbar buttons in initiGui, you should
// create a separate handler for each action - this single run() method will
// not be enough
void EasyGeoTagger::run()
{

  if( 0 == cvEasyGeoTaggerApplication )
  {
    //Check for the python libs to prevent segfaults if python is not available on the user's system
    QLibrary lvEasyGTLibrary( "easygt" );
    if( lvEasyGTLibrary.load() )
    {
      if( 0 == cvIdTool )
      {
        cvIdTool = new EasyGeoTaggerIdTool( mQGisIface->mapCanvas() );
        if( 0 == cvIdTool) { return; }
      }

      cvEasyGeoTaggerApplication = new EgtApplication( PLUGINPATH );
      if( 0 == cvEasyGeoTaggerApplication) { return; }

      connect( cvIdTool, SIGNAL( keyValuePair( QString, QString ) ), cvEasyGeoTaggerApplication->applicationInterface(), SLOT( acceptKeyValuePair( QString,QString ) ) );
    }
    else
    {
      QMessageBox::warning( mQGisIface->mainWindow(), tr( "Warning" ), tr( "The EasyGeoTagger library could not be found on your system" ) );
      return;
    }
  }
  else
  {
    cvEasyGeoTaggerApplication->show();
  }
  
  if( 0 == cvPluginGui )
  {
    cvPluginGui = new EasyGeoTaggerGui( mQGisIface, cvEasyGeoTaggerApplication, mQGisIface->mainWindow(), QgisGui::ModalDialogFlags );
    if( 0 == cvPluginGui ) { return; }
    connect( cvPluginGui, SIGNAL( setMapTool() ), this, SLOT( setMapTool() ) );
  }
  
  cvPluginGui->show();
  setMapTool();
}

void EasyGeoTagger::setMapTool()
{
  if( 0 == cvIdTool ) { return; }
  
  mQGisIface->mapCanvas()->setMapTool( cvIdTool );
}

// Unload the plugin by cleaning up the GUI
void EasyGeoTagger::unload()
{
  // remove the GUI
  mQGisIface->removePluginMenu( "&EasyGeoTagger", mQActionPointer );
  mQGisIface->removeToolBarIcon( mQActionPointer );
  delete mQActionPointer;
  
  if(0 != cvIdTool)
  {
    delete cvIdTool;
  }
}


//////////////////////////////////////////////////////////////////////////
//
//
//  THE FOLLOWING CODE IS AUTOGENERATED BY THE PLUGIN BUILDER SCRIPT
//    YOU WOULD NORMALLY NOT NEED TO MODIFY THIS, AND YOUR PLUGIN
//      MAY NOT WORK PROPERLY IF YOU MODIFY THIS INCORRECTLY
//
//
//////////////////////////////////////////////////////////////////////////


/**
 * Required extern functions needed  for every plugin
 * These functions can be called prior to creating an instance
 * of the plugin class
 */
// Class factory to return a new instance of the plugin class
QGISEXTERN QgisPlugin * classFactory( QgisInterface * theQgisInterfacePointer )
{
  return new EasyGeoTagger( theQgisInterfacePointer );
}
// Return the name of the plugin - note that we do not user class members as
// the class may not yet be insantiated when this method is called.
QGISEXTERN QString name()
{
  return sName;
}

// Return the description
QGISEXTERN QString description()
{
  return sDescription;
}

// Return the type (either UI or MapLayer plugin)
QGISEXTERN int type()
{
  return sPluginType;
}

// Return the version number for the plugin
QGISEXTERN QString version()
{
  return sPluginVersion;
}

// Delete ourself
QGISEXTERN void unload( QgisPlugin * thePluginPointer )
{
  delete thePluginPointer;
}
