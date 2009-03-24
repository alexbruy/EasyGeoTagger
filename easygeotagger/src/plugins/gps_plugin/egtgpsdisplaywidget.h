/*
** File: 
** Author( s ): 
** Creation Date: 
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

#include "egtfilereader.h"
#include "egtreaderfactory.h"
#include "egtgpsdatatablewidget.h"
#include "egtapplicationinterface.h"
#include "egtimagefactory.h"
#include "egtphotoexifengine.h"

#include <QWidget>

namespace Ui
{
    class EgtGpsDisplayWidgetBase;
    class EgtSynchronizeDialog;
}

class EgtGpsDisplayWidget : public QWidget
{
  Q_OBJECT

  public:
    EgtGpsDisplayWidget( QWidget* theParent = 0 );

    void setApplicationInterface(EgtApplicationInterface*);

  signals:
    void keyValuePair( QString, QString );

  private slots:

    void accepted();

    void clicked( const QModelIndex& );

    /*! \brief Slot called to set the file reader object to be used */
    void fileReader_set( EgtFileReader* );

    void loadPreview( bool );

    /*! \brief Slot called to open a gps file */
    void openFile( );
    
    /*! \brief Slot called to tag a picture */
    void sendCoordinates( );
    
    void setOffset();

    void setOffsetPic();

    void setSynchronizing(bool);

  private:

    /*! \brief Instance of the data table which contains the gps data */
    EgtGpsDataTableWidget* cvDataTable; 

    /*! \brief The main dock window that forms the base for the visual component of this plugin */
     QWidget cvMainWidget;

    /*! \brief Instance of the reader factory */
    EgtReaderFactory cvReaderFactory;
    
    /*! \brief Object used to show the picture preview */
    EgtImageFactory cvImageFactory;

    /*! \brief Desginer object of the gps plugin interface */
    Ui::EgtGpsDisplayWidgetBase* ui;

     /*! \brief Desginer object of the synchronize dialog*/
    Ui::EgtSynchronizeDialog* cvSynchronizeUi;

     /*! \brief QDialog that shows the synchronize dialog*/
    QDialog* cvSynchronizeDialog;
  
     /*! \brief Object to comunicate the plugin with the main application*/
    EgtApplicationInterface* cvApplicationInterface;

    /*! \brief Object to generate the path to a picture*/
    EgtPathBuilder cvPathBuilder;
  
    /*! \brief Object used to deal with the photo exif metadata of the picture*/
    EgtPhotoExifEngine cvPhotoExifEngine;

    /*! \brief Contains the offset (in seconds) the user specifies*/
    int cvOffset;
};

#endif // EGTGPSDISPLAYWIDGET_H