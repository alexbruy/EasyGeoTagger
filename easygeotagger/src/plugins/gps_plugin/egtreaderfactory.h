/*
** File: egtfilereaderfactory.h
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
#ifndef EGTREADERFACTORY_H
#define EGTREADERFACTORY_H

#include "egtgraphicaldelimitedtextfilereader.h"
#include "ui_filetypedialog.h"

#include <QObject>
#include <QWidget>
class EgtReaderFactory: public QObject
{
  Q_OBJECT

  public:

    /*! \brief Constructor */
    EgtReaderFactory( );

    /*! \brief Shows the windows to open a file (delimited text or gps) */
    void show();

  private slots:

    /*! \brief Slot used to re-emit the signal emited from the graphical file reader */
    void reEmitDelimiterSelected();

    void on_pbtnOk_clicked();

    void on_pbtnCancel_clicked();

    void on_rbDelimitedText_toggled( bool );

    void on_rbGPSFile_toggled( bool );

  private:
    /*! \brief Instance of the concrete file reader to be used */
    EgtGraphicalDelimitedTextFileReader* cvFileReader;

    QDialog cvFileTypeDialog;
    Ui::FileTypeDialog cvUiFileType;

  signals:
    /*! \brief Signal to notify that the file reader has been created */
    void fileReaderCreated( EgtFileReader* );
};
#endif
