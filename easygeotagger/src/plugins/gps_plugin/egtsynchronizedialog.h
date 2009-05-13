/*
** File: egtsynchronize.h
** Author( s ):Roberto Garcia Yunta
** Creation Date: 2009-05-01
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

#ifndef EGTSYNCHRONIZEDIALOG_H
#define EGTSYNCHRONIZEDIALOG_H

#include "ui_egtsynchronize.h"
#include "egtimagefactory.h"
#include "egtphotoexifengine.h"
#include "egtapplicationinterface.h"

namespace Ui
{
  class EgtSynchronizeDialogBase;
}

class EgtSynchronizeDialog : public QDialog
{
  Q_OBJECT
  
  public:

    EgtSynchronizeDialog( QWidget* theParent = 0 );

    /*! \brief Sets the picture selected on the preview label */
    void loadPreview();

    /*! \brief Slot called to show the dialog that sets the offset manually */
    void showDialog( bool );

  signals:
     /*! \brief Signal used to emit the offset */
     void offsetSet( int );

  public slots:
    
    /*! \brief Slot called when the user accepts the changes made in the synchronize dialog */
    void accepted();

    /*! \brief Slot called when the user clicks on the file browser */
   // void clicked( const QModelIndex& );


    /*! \brief Slot called to open a picture */
    void openImage();

    /*! \brief Slot called when the image is ready to be shown */
    void updatePreview( bool );

    /*! \brief Slot used to set the status of the preview progress bar */
    void updateProgress( int, int, int );

    /*! \brief Slot used to update the offset when the user modifies the datetimestamp that sees in the synchronization picture */
    void updateOffset();

    /*! \brief Overloaded method, reimplemented to resize the preview picture */
    void resizeEvent ( QResizeEvent * event ); 

  private:

    /*! \brief  Pointer to the application  interface*/
    EgtApplicationInterface* cvApplicationInterface;

    /*! \brief Object used to show the picture preview */
    EgtImageFactory cvImageFactory;

    /*! \brief Object to generate the path to a picture*/
    EgtPathBuilder cvPathBuilder;
  
    /*! \brief Object used to deal with the photo exif metadata of the picture*/
    EgtPhotoExifEngine cvPhotoExifEngine;

    /*! \brief  Qt Designer object*/
    Ui::EgtSynchronizeDialogBase* ui;

};
#endif // EGTSYNCHRONIZEDIALOG
