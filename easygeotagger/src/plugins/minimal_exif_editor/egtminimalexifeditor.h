/*
** File: egtminimalexifeditor.h
** Author(s): Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2008-10-15
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
#ifndef EGTMINIMALEXIFEDITOR_H
#define EGTMINIMALEXIFEDITOR_H

#include "egtplugininterface.h"
#include "egtminimalexifeditorcontrols.h"
#include "ui_egtminimalexifeditorcontrolsgui.h"
#include "egtexifio.h"

#include <QModelIndex>
#include <QDockWidget>
#include <QString>
#include <QObject>

class EgtMinimalExifEditor: public QObject, EgtPluginInterface
{
  Q_OBJECT
  Q_INTERFACES(EgtPluginInterface)
  
  public:
    EgtMinimalExifEditor();
    
    QStringList categories();
    void connectConfigurationButton( QPushButton* );
    void connectRunButton( QPushButton* );
    QString description();
    bool isConfigurable() { return false; }
    QString name();

  public slots:
    void updateExifDisplay( const QModelIndex& );
    void run();
    void showConfigurationPanel() { }
  
  private:
    QDockWidget* cvDock;
    EgtExifIO cvExifIO;
    QString cvLastFile;
    
    EgtMinimalExifEditorControls* cvControls;

    void updateExifDisplay( QString );

};
#endif
