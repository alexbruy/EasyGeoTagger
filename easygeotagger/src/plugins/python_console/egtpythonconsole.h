/*
** File: egtpythonconsole.h
** Author( s ): Peter J. Ersts ( ersts at amnh.org )
** Creation Date: 2008-11-18
**
** Copyright ( c ) 2008-2009, American Museum of Natural History. All rights reserved.
**
** This library/program is free software; you can redistribute it
** and/or modify it under the terms of the GNU Library General Public
** License as published by the Free Software Foundation; either
** version 2 of the License, or ( at your option ) any later version.
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
#ifndef EGTPYTHONCONSOLE_H
#define EGTPYTHONCONSOLE_H

#include "egtpythonconsolebase.h"
#include "egtplugininterface.h"

/*! \brief Simple Python Console
 *
 */
class EgtPythonConsole : public EgtPluginInterface
{
  Q_OBJECT
  Q_INTERFACES( EgtPluginInterface )

  public:
    EgtPythonConsole( );

    /*! \brief Return the list of categories that the plugin will show up in */
    QStringList categories( );

    /*! \brief Connects the provided button to the showConfigurationPanel function */
    void connectConfigurationButton( QPushButton* );

    /*! \brief Connects the provided button to the run function */
    void connectRunButton( QPushButton* );

    /*! \brief Returns the description for this plugin */
    QString description( );

    /*! \brief Returns the ability of this plugin to be configured */
    bool isConfigurable( ) { return false; }

    /*! \brief Returns the name of this plugin */
    QString name( );

  public slots:
    /*! \brief Slot called to activate or launch the plugin */
    void run( );

    /*! \brief Slot to display the condifuration panel */
    void showConfigurationPanel( ) { }

  private:
    EgtPythonConsoleBase* cvConsole;
};
#endif // EGTPYTHONCONSOLE_H
