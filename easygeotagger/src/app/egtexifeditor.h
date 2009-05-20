/*
** File: egtexifeditor.h
** Author( s ): Peter J. Ersts ( ersts at amnh.org )
** Creation Date: 2008-12-11
**
** Copyright ( c ) 2008-2009, American Museum of Natural History. All rights reserved.
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
#ifndef EGTEXIFEDITOR_H
#define EGTEXIFEDITOR_H

#include "egtexiftagcontrol.h"
#include "egtexiftaggroup.h"
#include "egtexifengine.h"

#include <QPushButton>
#include <QGroupBox>
#include <QObject>
#include <QDialog>
#include <QWidget>
#include <QMap>

/*! \brief Generic EXIF editor
 *
 * This class will build an basic editor for any exif engine.
 */
class MS_DLL_SPEC EgtExifEditor : public QObject
{
  Q_OBJECT

  public:
    /*! \brief Constructor */
    EgtExifEditor( QString, EgtExifEngine* );

    /*! \brief Add a tag group to the editor */
    void addTagGroup( EgtExifTagGroup* );

    /*! \brief Get a pointer to the editing widget */
    QWidget* editorWidget( ) { return &cvEditorWidget; }

    /*! \brief Load all exif tags */
    void loadExifData( bool );

    /*! \brief Show the configuration dialog */
    void showConfigurationDialog( ) { cvConfigurationDialog.show( ); }

    /*! \brief Show the configuration dialog */
    void showConfigurationDialog( QPoint );

  public slots:
    /*! \brief Slot to receive key value pairs from the application interface */
    void setTagData( QString theKey, QString theValue, bool setCachedValue = true );

    /*! \brief Slot to receive key value pairs from the application interface */
    void setTagData( QString theKey, QVariant theValue, bool setCachedValue = true );

  private slots:
    void controlDisabled( QString );
    void controlEnabled( QString );
    void cvSaveButton_clicked( );
    void tagGroupActivated( QStringList );

  private:
    QDialog cvConfigurationDialog;
    QWidget cvEditorWidget;
    EgtExifEngine* cvExifEngine;
    QString cvId;
    QMap< QString, EgtExifTagControl* > cvTagControls;
    QPushButton cvSaveButton;
    QGroupBox cvTagGroups;
  };
#endif
