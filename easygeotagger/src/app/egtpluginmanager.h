/*
** File: egtpluginmanager.h
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
#ifndef EGTPLUGINMANAGER_H
#define EGTPLUGINMANAGER_H

#include "egtmainwindow.h"
#include "egtplugininterface.h"
#include "egtplugindisplaywidget.h"
#include "egtapplicationinterface.h"

#include <QMap>
#include <QObject>

#define PLUGINCOLLECTION QMap< QString, QMap< QString, EgtPluginDisplayWidget* >* >

/*! \brief EasyGeoTagger plugin manager
 * 
 * The plugin manager is responsible for loading plugins and updating the gui
 */
class MS_DLL_SPEC EgtPluginManager : public QObject
{
  Q_OBJECT 
  
  public:
    /*! \brief Constructor */
    EgtPluginManager( EgtApplicationInterface*, EgtMainWindow* );

    /*! \brief Update plugin display dock widget in the main window */
    void updateGui();

    /*! \brief Load all plugins in a directory */
    void loadAllPlugins( QString );

    /*! \brief Load a single plugin */
    bool loadSinglePlugin( QString );

  public slots:
    /*! \brief Load a plugin or all plugins in a directory */
    void loadPlugins( QString );

  signals:
    /*! \brief Signal to indicate which plugin was just loaded */
    void pluginLoaded( QString );  
    
  private:
    /*! \brief Collection of available plugins. Collection are build based on category    QMap< Category, QMap< pluginName, pluginDisplay > > */
    PLUGINCOLLECTION cvPluginDisplayCollection;
    
    /*! \brief Default plugin path */
    QString cvDefaultPluginPath;
    
    /*! \brief Pointer to the application interface */
    EgtApplicationInterface* cvApplicationInterface;
    
    /*! \brief Pointer to the main window */
    EgtMainWindow* cvGui;
};
#endif
