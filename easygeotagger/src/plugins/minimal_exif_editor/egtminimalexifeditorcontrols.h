/*
** File: egtminimalexifeditorcontrols.h
** Author(s): Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2008-10-17
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
#ifndef EGTMINIMALEXIFEDITORCONTROLS_H
#define EGTMINIMALEXIFEDITORCONTROLS_H

#include "ui_egtminimalexifeditorcontrolsgui.h"
#include "egtexifioengine.h"

#include <QWidget>
#include <QString>

class EgtMinimalExifEditorControls : public QWidget, public Ui::EgtMinimalExifEditorControlsGui
{
  Q_OBJECT
  
  public:
    EgtMinimalExifEditorControls( EgtExifIoEngine* theExifIoEngine = 0, QWidget* theParent = 0, Qt::WindowFlags theFlags = 0 );
  
  private slots:
    void on_leLatitude_textEdited( QString );
    void on_leLongitude_textEdited( QString );
    void on_pbtnLatitudeSave_clicked();
    void on_pbtnLatitudeCancel_clicked();
    void on_pbtnLongitudeSave_clicked();
    void on_pbtnLongitudeCancel_clicked();
    
  private:
    EgtExifIoEngine* cvExifIoEngine;

};
#endif
