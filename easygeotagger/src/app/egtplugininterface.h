/*
** File: egtplugininterface.h
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
#ifndef EGTPLUGININTERFACE_H
#define EGTPLUGININTERFACE_H

#include <QString>
#include <QObject>
#include <QStringList>
#include <QPushButton>

class EgtApplicationInterface;

/*! \brief EasyGeoTagger plugin interface
 *
 * This interface provide the mechanism by which EasyGT can dynamically load plugins
 */
class MS_DLL_SPEC EgtPluginInterface : public QObject
{
  Q_OBJECT 
  
  public:
    /*! \brief Constuctor */
    EgtPluginInterface() { cvApplicationInterface = 0; }
    
    /*! \brief Virtual Destuctor */
    virtual ~EgtPluginInterface() { }
    
    /*! \brief Pure virtual function to return the list of categories that the plugin will show up in */
    virtual QStringList categories() = 0;
    
    /*! \brief Pure virtual function to connect the provided button to the showConfigurationPanel function */
    virtual void connectConfigurationButton( QPushButton* ) = 0;
    
    /*! \brief Pure virtual function to connect the provided button to the run function */
    virtual void connectRunButton( QPushButton* ) = 0;
    
    /*! \brief Pure virtual function to return the description for this plugin */
    virtual QString description() = 0;
    
    /*! \brief Pure virtual function to return the ability of this plugin to be configured */
    virtual bool isConfigurable() = 0;
    
    /*! \brief Pure virtual function to return the name of this plugin */
    virtual QString name() = 0;
    
    /*! \brief Virtual function to run initialization steps if needed */
    virtual void initPlugin() {}
    
    
    /*! \brief Set the pointer to the EasyGeoTagger application interface */
    void setApplicationInterface( EgtApplicationInterface* theInterface ) { cvApplicationInterface = theInterface; }

  public slots:
    /*! \brief Slot called to activate or launch the plugin */
    virtual void run() { }
    
    /*! \brief Slot to display the condifuration panel */
    virtual void showConfigurationPanel() { }
  
  protected:
    EgtApplicationInterface* cvApplicationInterface;
};

 Q_DECLARE_INTERFACE(EgtPluginInterface, "org.amnh.bif.Plugin.EgtPluginInterface/1.0");

#endif
