/*
** File: egtplugindisplaywidget.h
** Author(s): Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2008-10-06
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
#ifndef EGTPLUGINDISPLAYWIDGET_H
#define EGTPLUGINDISPLAYWIDGET_H

#include "egtplugininterface.h"

#include <QLabel>
#include <QFrame>
#include <QPushButton>

/*! \brief Plugin display widget for the plugin list
 *
 * The graphical container that holds buttons to launch and configure plugins as well as display
 * the plugin's description.
 */
class MS_DLL_SPEC EgtPluginDisplayWidget : public QFrame
{
  Q_OBJECT

  public:
    /*! \brief Constructor */
    EgtPluginDisplayWidget(  EgtPluginInterface* thePlugin, QWidget* theParent = 0, Qt::WindowFlags theFlags = 0);
    
    /*! \brief Update the widget's components */
    void update( EgtPluginInterface* thePlugin );
    
  private:
    /*! \brief Button that shows up in the plugin collection display and activates or launches the plugin */
    QPushButton cvRunButton;
    
    /*! \brief Button that shows up in the plugin collection display and activates or launches the configuration panel */
    QPushButton cvConfigureButton;
    
    /*! \brief Description that is shown in the plugin collection display */
    QLabel cvDescription;
};
#endif
