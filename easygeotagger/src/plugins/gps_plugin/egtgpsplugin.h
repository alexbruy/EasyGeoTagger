/*
** File: 
** Author(s): 
** Creation Date: 
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
#ifndef EGTGPSPLUGIN_H
#define EGTGPSPLUGIN_H

#include "egtgpsdatatable.h"
#include "egtplugininterface.h"

#include <QPushButton>
#include <QModelIndex>
#include <QDockWidget>
#include <QString>
#include <QList>

class EgtGpsPlugin: public EgtPluginInterface
{
  Q_OBJECT
  Q_INTERFACES(EgtPluginInterface)
  
  public:
    /*! \brief Constuctor */
    EgtGpsPlugin();
    
    /*! \brief Connect the provided button to the showConfigurationPanel function */
    void connectConfigurationButton( QPushButton* );
    
    /*! \brief Connect the provided button to the run function */
    void connectRunButton( QPushButton* );
    
    /*! \brief Initialization plugin*/
    void initPlugin();
    
    /*! \brief Return the ability of this plugin to be configured */
    bool isConfigurable() { return false; }

  
  public slots:
    
    void acceptRowSelected( );   

    void on_pbtnOpenFile_clicked();

    /*! \brief Slot called to activate or launch the plugin */
    void run();

    /*! \brief Slot to display the condifuration panel */
    void showConfigurationPanel() { }

  signals:
    void keyValuePair( QString, QString );

  private:
    EgtGpsDataTable cvDataTable; 
    /*! \brief The main dock window that forms the base for the visual component of this plugin */
    QDockWidget cvDock;
    

};
#endif

