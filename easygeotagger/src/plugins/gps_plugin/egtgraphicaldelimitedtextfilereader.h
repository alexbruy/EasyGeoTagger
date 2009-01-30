/*
** File: egtgraphicaldelimitedtextfilereader.h
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
#ifndef EGTGRAPHICALDELIMITEDTEXFILEREADER_H
#define EGTGRAPHICALDELIMITEDTEXFILEREADER_H

#include "egtdelimitedtextfilereader.h"
#include "ui_textdelimiter.h"

class EgtGraphicalDelimitedTextFileReader: public EgtDelimitedTextFileReader
{
  Q_OBJECT

  public:
    /*! \brief Constuctor */ 
    EgtGraphicalDelimitedTextFileReader();

    /*! \brief Selects the character that will be considered as delimiter when reading a text file */
    void selectDelimiter();

    /*! \brief Shows the file selection dialog. It also perfoms all the associated tasks*/
    void show();

  public slots:
    
    void on_cbHeader_changed();

    /*! \brief Slot used to perform the needed actions when the user selects a delimiter */
    void on_delimiter_changed();

    void on_leCustom_changed();

    void on_pbtnCancel_clicked();

    void on_pbtnOk_clicked();

    void on_rbtnBlank_toggled( bool );

    void on_rbtnComma_toggled( bool );

    void on_rbtnCustom_toggled( bool );

    void on_rbtnPipe_toggled( bool );

  private:

    /*! \brief Dialog where the user specifies a delimiter */
    QDialog cvSelectDelimiterDialog;

    /*! \brief Text edit where the user specifies the delimiter*/
    QTextEdit* cvDelimiterText;

    /*! \brief QList that contains the data from the file to be read*/
    QList<QStringList> cvFileData;

    /*! \brief QDesigner object*/
    Ui::TextDelimiterDialog cvUiTextDelimiter;

  signals:

    /*! \brief Signal emited when the user changes the delimiter (not when she/he selects it)*/
    void delimiterChanged();

    /*! \brief Signal emited when the user selects a delimiter*/
    void delimiterSelected();
};
#endif
