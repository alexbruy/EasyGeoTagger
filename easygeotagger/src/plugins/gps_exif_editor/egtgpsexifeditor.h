/*
** File: egtgpsexifeditor.h
** Author(s): Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2008-12-09
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
#ifndef EGTGPSEXIFEDITOR_H
#define EGTGPSEXIFEDITOR_H

#include "egtgpsexifengine.h"
#include "egtexiftagcontrol.h"
#include "egtplugininterface.h"

#include <QPushButton>
#include <QModelIndex>
#include <QDockWidget>
#include <QDialog>
#include <QString>
#include <QList>

class EgtGpsExifEditor: public EgtPluginInterface
{
  Q_OBJECT
  Q_INTERFACES(EgtPluginInterface)
  
  public:
    /*! \brief Constuctor */
    EgtGpsExifEditor();
    
    /*! \brief Return the list of categories that the plugin will show up in */
    QStringList categories();
    
    /*! \brief Connect the provided button to the showConfigurationPanel function */
    void connectConfigurationButton( QPushButton* );
    
    /*! \brief Connect the provided button to the run function */
    void connectRunButton( QPushButton* );
    
    /*! \brief Return the description for this plugin */
    QString description();

    /*! \brief Initialization plugin*/
    void initPlugin();
    
    /*! \brief Return the ability of this plugin to be configured */
    bool isConfigurable() { return true; }

    
    /*! \brief Return the name of this plugin */
    QString name();

  public slots:
    /*! \brief Slot that allows external components to pass coordinates to the plugin */
    void acceptCoordinates( double, double );
    
    /*! \brief Slot that will update the fields with data from the currently selected object in the file browser */
    void updateExifDisplay( const QModelIndex& );
    
    /*! \brief Slot called to activate or launch the plugin */
    void run();
    
    /*! \brief Slot to display the condifuration panel */
    void showConfigurationPanel() { cvConfigurationControls.setVisible( true ); }

  private slots:
    void cvSaveButton_clicked();
    void controlDisabled( QString );
    void controlEnabled( QString );
  
  private:
    /*! \brief The main dock window that forms the base for the visual component of this plugin */
    QDockWidget cvDock;
    
    /*! \brief An instance of the EXIF reader writer */
    EgtGPSExifEngine cvExifEngine;
    
    /*! \brief Last file that sucessfully had GPS exif data extraced */
    QString cvLastFile;

    /*! \brief Slot that will update the fields with data a image file */
    void updateExifDisplay( QString );

    QMap< QString, EgtExifTagControl* > cvTagControls;
    QWidget cvEditorControls;
    QDialog cvConfigurationControls;

    QPushButton cvSaveButton;

};
#endif
