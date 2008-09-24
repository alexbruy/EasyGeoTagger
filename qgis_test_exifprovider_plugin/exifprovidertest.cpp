/***************************************************************************
  exifprovidertest.cpp
  Test EXIF Provider
  -------------------
         begin                : [PluginDate]
         copyright            : [(C) Your Name and Date]
         email                : [Your Email]

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

#include "exifprovidertest.h"
#include "exifprovidertestgui.h"

//
// Qt4 Related Includes
//

#include <QAction>
#include <QToolBar>


static const char * const sIdent = "$Id: plugin.cpp 9327 2008-09-14 11:18:44Z jef $";
static const QString sName = QObject::tr( "ExifProviderTest" );
static const QString sDescription = QObject::tr( "Test EXIF Provider" );
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
ExifProviderTest::ExifProviderTest( QgisInterface * theQgisInterface ):
    QgisPlugin( sName, sDescription, sPluginVersion, sPluginType ),
    mQGisIface( theQgisInterface )
{
}

ExifProviderTest::~ExifProviderTest()
{

}

/*
 * Initialize the GUI interface for the plugin - this is only called once when the plugin is
 * added to the plugin registry in the QGIS application.
 */
void ExifProviderTest::initGui()
{

  // Create the action for tool
  mQActionPointer = new QAction( QIcon( ":/exifprovidertest/exifprovidertest.png" ), tr( "ExifProviderTest" ), this );
  // Set the what's this text
  mQActionPointer->setWhatsThis( tr( "Replace this with a short description of what the plugin does" ) );
  // Connect the action to the run
  connect( mQActionPointer, SIGNAL( triggered() ), this, SLOT( run() ) );
  // Add the icon to the toolbar
  mQGisIface->addToolBarIcon( mQActionPointer );
  mQGisIface->addPluginToMenu( tr( "&ExifProviderTest" ), mQActionPointer );

}
//method defined in interface
void ExifProviderTest::help()
{
  //implement me!
}

// Slot called when the menu item is triggered
// If you created more menu items / toolbar buttons in initiGui, you should
// create a separate handler for each action - this single run() method will
// not be enough
void ExifProviderTest::run()
{
  ExifProviderTestGui *myPluginGui = new ExifProviderTestGui( mQGisIface->mainWindow(), QgisGui::ModalDialogFlags );
  myPluginGui->setAttribute( Qt::WA_DeleteOnClose );
  connect( myPluginGui, SIGNAL( draw( QString ) ), this, SLOT( draw( QString ) ) );
  myPluginGui->show();
}

// Unload the plugin by cleaning up the GUI
void ExifProviderTest::unload()
{
  // remove the GUI
  mQGisIface->removePluginMenu( "&ExifProviderTest", mQActionPointer );
  mQGisIface->removeToolBarIcon( mQActionPointer );
  delete mQActionPointer;
}


void ExifProviderTest::draw(QString theDir)
{
  mQGisIface->addVectorLayer( theDir, "ExifTest", "exif");
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
  return new ExifProviderTest( theQgisInterfacePointer );
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
