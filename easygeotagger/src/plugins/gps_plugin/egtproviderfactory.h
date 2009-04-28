/*
** File: egtproviderfactory.h
** Author( s ): Roberto Garcia Yunta
** Creation Date: 2008-12-19
**
** Copyright ( c ) 2008, American Museum of Natural History. All rights reserved.
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
#ifndef EGTPROVIDERFACTORY_H
#define EGTPROVIDERFACTORY_H

#include "egtgraphicaldelimitedtextprovider.h"
#include "ui_filetypedialog.h"

class EgtProviderFactory: public QObject
{
  Q_OBJECT

  public:

    /*! \brief Constructor */
    EgtProviderFactory( );

    /*! \brief Shows the windows to open a file ( delimited text or gps ) */
    void show( );

  private slots:

    /*! \brief Slot used when the user clicks on the ok button of the file type dialog */
    void accept( );

    /*! \brief Slot used to signal the factory that a file provider has been created */
    void dataProviderInitialized( );

    /*! \brief Slot used when the user clicks on the cancel button of the file type dialog */
    void reject( );

  private:

    /*! \brief Instance of the concrete file provider to be used */
    EgtDataProvider* cvDataProvider;

    /*! \brief QDialog to let the user select the type of file to be read */
    QDialog cvFileTypeDialog;

    /*! \brief Desginer object */
    Ui::FileTypeDialog cvUiFileType;

  signals:
    /*! \brief Signal to notify that the data provider has been created */
    void dataProviderCreated( EgtDataProvider* );
};
#endif
