/*
** File: egtgpsdisplaywidget.cpp
** Author( s ): Roberto Garcia Yunta
** Creation Date: 2009-03-20
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
#ifndef EGTGPSDISPLAYWIDGET_H
#define EGTGPSDISPLAYWIDGET_H

#include "ui_egtsynchronize.h"
#include "egtdataprovider.h"
#include "egtproviderfactory.h"
#include "egtgpsdatatablewidget.h"
#include "egtapplicationinterface.h"
#include "egtimagefactory.h"
#include "egtphotoexifengine.h"

#include <QWidget>
#include <QCloseEvent>

class EgtGpsDisplayWidget;

namespace Ui
{
    class EgtGpsDisplayWidgetBase;
}

class EgtGpsDisplayWidget : public QWidget
{
  Q_OBJECT

  public:
    EgtGpsDisplayWidget( QWidget* theParent = 0 );

    /*! \brief Reimplemented function to close the syncrhonization dialog when closing the plugin window */
    void closeEvent( QCloseEvent * );
 
    /*! \brief Sets the application interface used to comunicate with the main application */
    void setApplicationInterface(EgtApplicationInterface*);

    /*! \brief Desginer object of the gps plugin interface */
    Ui::EgtGpsDisplayWidgetBase* ui;
 
    QPushButton* cvpbtnOffsetManual;
    QPushButton* cvpbtnOffsetPic;
    QPushButton* cvpbtnDeleteRow;
    QPushButton* cvpbtnSendCoordinates;
    
  signals:

    /*! \brief Signal used to comunicate with the application interface */
    void keyValuePair( QString, QString );

  public slots:
    /*! \brief Slot used to enable/disable the group box that contains the offset buttons */
    void setSynchronizing(bool);

     void setDataTable( EgtGpsDataTableWidget* );

     void setButtonsStatus( bool , bool );

  private slots:


    /*! \brief Slot called to open a gps file */
    void openFile( );

    
    /*! \brief Slot called to tag a picture */
    void sendCoordinates( );

    /*! \brief Slot called to set the data provider */
    void setDataProvider( EgtDataProvider* );

  private:

    /*! \brief Instance of the data table which contains the gps data */
    EgtGpsDataTableWidget* cvDataTable; 

    /*! \brief The main dock window that forms the base for the visual component of this plugin */
     QWidget cvMainWidget;

    /*! \brief Instance of the provider factory */
    EgtProviderFactory cvProviderFactory;

     /*! \brief Object to comunicate the plugin with the main application*/
    EgtApplicationInterface* cvApplicationInterface;

};


#endif // EGTGPSDISPLAYWIDGET_H
