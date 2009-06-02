/*
** File: egtgpxprovider.h
** Author( s ): Roberto Garcia Yunta, Peter J. Ersts ( ersts at amnh.org )
** Creation Date: 2009-05-22
**
** Copyright ( c ) 2009, American Museum of Natural History. All rights reserved.
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
#ifndef EGTGPXPROVIDER_H
#define EGTGPXPROVIDER_H

#include <QDialog>

#include "egtgpsprovider.h"
#include "ui_configurationdialog.h"

namespace Ui
{
  class ConfigurationDialog;
}

class EgtGpxProvider: public EgtGpsProvider
{
  Q_OBJECT
  Q_INTERFACES( EgtDataProvider )

  public:

    /*! \brief Constuctor */
    EgtGpxProvider( );

    void configure( QPoint );

    EgtDataProvider* instance( ) { return new EgtGpxProvider(); }

  private slots:
    void cancel();
    void finishConfiguration();

  private:
    QDialog cvConfigurationDialog;
    Ui::ConfigurationDialog* ui;
 
};
#endif
