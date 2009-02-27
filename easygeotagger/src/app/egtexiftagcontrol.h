/*
** File: egtexiftagcontrol.h
** Author(s): Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2008-12-09
**
** Copyright (c) 2008-2009, American Museum of Natural History. All rights reserved.
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
#ifndef EGTEXIFTAGCORCONTROL_H
#define EGTEXIFTAGCORCONTROL_H

#include <QWidget>
#include <QString>
#include <QObject>
#include <QVariant>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QStringList>

/*! \brief Editor and configuration controls for a exif tag
 *
 */
class MS_DLL_SPEC EgtExifTagControl : public QObject
{

  Q_OBJECT

  public:
    /*! \brief Constructor */
    EgtExifTagControl( QString, QString, QStringList );

    /*! \brief Returns a pointer the configuration control */
    QWidget* configurationControls() { return &cvConfigurationControls; }

    /*! \brief Returns a pointer to the editor controls */
    QWidget* editorControls() { return &cvEditorControls; }

    /*! \brief Returns the current enabeled state of the control */
    bool isEnabled();

    /*! \brief Sets the current enabled state of rhte control */
    void setEnabled( bool );

    /*! \brief Returns the key identifying this control */
    QString key() { return QString( cvKey ); }

    /*! \brief Sets the units */
    void setUnits( QString const &theValue );

    /*! \brief Sets the units */
    void setUnits( QVariant const &theValue );

    /*! \brief Sets the value in the editor's line edit */
    void setValue( QString const &theValue, bool setCachedValue = true );

    /*! \brief Sets the value in the editor's line edit */
    void setValue( QVariant const &theValue, bool setCachedValue = true );

    /*! \brief Sets the visibility of the editor controls */
    void setVisible( bool );

    /*! \brief Get the current associated data selection */
    QString units(){ return cvUnits.itemText( cvUnits.currentIndex() ).trimmed(); }

    /*! \brief Return the current value in the editors line edit */
    QString value() { return cvKeyValue.text(); }


  signals:
    /*! \brief Signal to indicate that a particular control as been enabled, the controls key is passed as an argument */
    void controlEnabled( QString );

    /*! \brief Signal to indicate that a particular control as been disabled, the controls key is passed as an argument */
    void controlDisabled( QString );

  private slots:
    void cvDiscardButton_clicked();
    void cvEnabled_stateChanged( int );
    void cvKeyValue_textChanged( QString );

  private:
    QString cvCachedValue;
    QWidget cvConfigurationControls;
    QPushButton cvDiscardButton;
    QString cvDisplayName;
    QWidget cvEditorControls;
    QCheckBox cvEnabled;
    QString cvKey;
    QLineEdit cvKeyValue;
    QComboBox cvUnits;

};
#endif
