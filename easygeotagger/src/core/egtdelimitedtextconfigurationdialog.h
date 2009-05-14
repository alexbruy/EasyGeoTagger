/*
** File: egtdelimitedtextconfigurationdialog.h
** Author( s ): Roberto Garcia Yunta
** Creation Date: 2009-05-13
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
#ifndef EGTDELIMITEDTEXTCONFIGURATIONDIALOG_H
#define EGTDELIMITEDTEXTCONFIGURATIONDIALOG_H

#include "egtdelimitedtextprovider.h"

#include <QDialog>

namespace Ui
{
    class EgtDelimitedTextConfigurationDialogBase;
}

class EgtDelimitedTextProvider;

class EgtDelimitedTextConfigurationDialog : public QDialog
{
  Q_OBJECT

  public:

    EgtDelimitedTextConfigurationDialog( EgtDelimitedTextProvider* ,QWidget* theParent = 0 );

  private slots:

    void accepted( );

    void on_cbHeader_stateChanged( );

    void on_leCustom_textEdited( );

    void on_rbtnBlank_toggled( bool );

    void on_rbtnComma_toggled( bool );

    void on_rbtnCustom_toggled( bool );

    void on_rbtnPipe_toggled( bool );
    
    void rejected( );


  private:

    EgtDelimitedTextProvider* cvProvider;

    Ui::EgtDelimitedTextConfigurationDialogBase* ui;

    void delimiterChanged( );
};

#endif // EGTDELIMITEDTEXTCONFIGURATIONDIALOG_H
