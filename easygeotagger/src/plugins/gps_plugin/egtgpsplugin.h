/*
** File: egtgpsplugin.h
** Author(s): Roberto Garcia Yunta
** Creation Date: 2008-12-19
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

#include "egtgpsdatatablewidget.h"
#include "egtplugininterface.h"
#include "egtfilereader.h"
#include "egtreaderfactory.h"

#include <QPushButton>
#include <QModelIndex>
#include <QDialog>
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
    
    /*! \brief Slot called to activate or launch the plugin */
    void run();

    /*! \brief Slot to display the condifuration panel */
    void showConfigurationPanel() { }

  private slots:
    /*! \brief Slot called to open a gps file */
    void cvOpenFile_clicked();
    
    /*! \brief Slot called to tag a picture */
    void cvTagButton_clicked();
    
    /*! \brief Slot called to set the file reader object to be used */
    void fileReader_set( EgtFileReader* );
  signals:
    /*! \brief Slot called to tag a picture */
    void keyValuePair( QString, QString );

  private:
    /*! \brief Instance of the data table which contains the gps data */
    EgtGpsDataTableWidget cvDataTable; 

    /*! \brief Instance of the reader factory */
    EgtReaderFactory cvReaderFactory;

    /*! \brief The main dock window that forms the base for the visual component of this plugin */
    QDialog cvDialog;
  
    /*! \brief Push button to tag a picture */
    QPushButton cvTagButton;
   
    /*! \brief Push button to open a file */
    QPushButton cvOpenFileButton;
    

};
#endif

