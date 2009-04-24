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
#include "egtfilereader.h"
#include "egtreaderfactory.h"
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
    class EgtSynchronizeDialog;
    class ExtendedQDialog;
}

class EgtGpsDisplayWidget : public QWidget
{
  Q_OBJECT

  public:
    EgtGpsDisplayWidget( QWidget* theParent = 0 );

    /*! \brief Reimplemented function to close the syncrhonization dialog when closing the plugin window */
    void closeEvent( QCloseEvent * );
 
    /*! \brief Sets the picture selected on the preview label */
    void loadPreview();

    /*! \brief Sets the application interface used to comunicate with the main application */
    void setApplicationInterface(EgtApplicationInterface*);
    
  signals:

    /*! \brief Signal used to comunicate with the application interface */
    void keyValuePair( QString, QString );

  private slots:

     /*! \brief Slot that sets the offset when the user clicks "OK" on the synchronize dialog */
    void accepted();

    /*! \brief Slot called when the user clicks on the file browser */
    void clicked( const QModelIndex& );

    /*! \brief Slot called to set the file reader object to be used */
    void fileReader_set( EgtFileReader* );

    /*! \brief Slot called to open a gps file */
    void openFile( );

    /*! \brief Slot called to open a picture */
    void openPic();
    
    /*! \brief Slot called to tag a picture */
    void sendCoordinates( );

    /*! \brief Slot used to enable/disable the "Send to editor" button and "delete row" button */
    void setSendToEditorButton( bool );
    
    /*! \brief Slot called to show the dialog that sets the offset manually */
    void setOffset();

    /*! \brief Slot called to show the dialog that sets the offset with the help of a picture */
    void setOffsetPic();

    /*! \brief Slot used to enable/disable the group box that contains the offset buttons */
    void setSynchronizing(bool);
 
    /*! \brief Slot used to update the offset when the user modifies the datetimestamp that sees in the synchronization picture */
    void updateOffset();

    /*! \brief Slot called when the image is ready to be shown */
    void updatePreview( bool );

    /*! \brief Slot used to set the status of the preview progress bar */
    void updateProgress( int, int, int );

   
  private:

    /*! \brief Instance of the data table which contains the gps data */
    EgtGpsDataTableWidget* cvDataTable; 

    /*! \brief The main dock window that forms the base for the visual component of this plugin */
     QWidget cvMainWidget;

    /*! \brief Instance of the reader factory */
    EgtReaderFactory cvReaderFactory;
    
    /*! \brief Object used to show the picture preview */
    EgtImageFactory cvImageFactory;

    /*! \brief Specifies whether the image to show is valid or not */
    bool cvValidImage;

    /*! \brief Desginer object of the gps plugin interface */
    Ui::EgtGpsDisplayWidgetBase* ui;

     /*! \brief Desginer object of the synchronize dialog*/
    Ui::EgtSynchronizeDialog* cvSynchronizeUi;

     /*! \brief QDialog that shows the synchronize dialog*/
    Ui::ExtendedQDialog* cvSynchronizeDialog;
  
     /*! \brief Object to comunicate the plugin with the main application*/
    EgtApplicationInterface* cvApplicationInterface;

    /*! \brief Contains the offset (in seconds) the user specifies*/
    int cvOffset;

    /*! \brief Object to generate the path to a picture*/
    EgtPathBuilder cvPathBuilder;
  
    /*! \brief Object used to deal with the photo exif metadata of the picture*/
    EgtPhotoExifEngine cvPhotoExifEngine;

    /*! \brief QString that contains the EXIF datetimestamp of the picture*/
    QString cvPictureDateTimeStamp;

    
};

namespace Ui
{
    class ExtendedQDialog :public QDialog
    {
       public: 
         ExtendedQDialog(EgtGpsDisplayWidget* theWidget)
         {
           cvWidget = theWidget;
         }
         /*! \brief Overloaded method. Reimplemented to resize the image preview*/
         void resizeEvent ( QResizeEvent * event ) 
         {
           cvWidget->loadPreview();
         }

         private:
           EgtGpsDisplayWidget* cvWidget;
    };
}
#endif // EGTGPSDISPLAYWIDGET_H