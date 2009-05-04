/*
** File: egtgraphicaldelimitedtextfilereader.h
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
#ifndef EGTGRAPHICALKMLREADER_H
#define EGTGRAPHICALKMLREADER_H

#include "egtkmlreader.h"
#include "ui_kmldialog.h"

class EgtGraphicalKmlReader: public EgtKmlReader
{
  Q_OBJECT

  public:
    /*! \brief Constuctor */ 
    EgtGraphicalKmlReader( );

    /*! \brief Shows the file selection dialog. It also perfoms all the associated tasks*/
    void init( );
   
  public slots:
    void reject( );

    void accept( );

  private:

    /*! \brief Dialog where the user specifies a delimiter */
    QDialog cvKmlOptionsDialog;

    /*! \brief Text edit where the user specifies the delimiter*/
    QTextEdit* cvDelimiterText;

    /*! \brief QList that contains the data from the file to be read*/
    QList<QStringList> cvFileData;

    /*! \brief QDesigner object*/
    Ui::KmlDialogBase cvUiKmlOptions;

  signals:
    /*! \brief Signal emited when the user selects a delimiter*/
    void initializationComplete( );
};
#endif