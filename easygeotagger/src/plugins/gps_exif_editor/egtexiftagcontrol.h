/*
** File: egtexiftagcontrol.h
** Author(s): Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2008-12-09
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
#ifndef EGTEXIFTAGCORCONTROL_H
#define EGTEXIFTAGCORCONTROL_H

#include <QWidget>
#include <QString>
#include <QObject>
#include <QVariant>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>


class EgtExifTagControl : public QObject
{

  Q_OBJECT

  public:
    EgtExifTagControl( QString, QString );

    QWidget* configurationControls() { return &cvConfigurationControls; }
    QWidget* editorControls() { return &cvEditorControls; }
    bool isEnabled();
    void setEnabled( bool );
    QString key() { return QString( cvKey ); }
    void setValue( QString const &theValue );
    void setValue( QVariant const &theValue );
    void setVisible( bool );
    QString value() { return cvKeyValue.text(); }


  signals:
    void controlEnabled( QString );
    void controlDisabled( QString );

  private slots:
    void cvDiscardButton_clicked();
    void cvEnabled_stateChanged( int );
    void cvKeyValue_textEdited( QString );

  private:
    QString cvCachedValue;
    QString cvDisplayName;
    QString cvKey;

    QPushButton cvDiscardButton;
    QWidget cvEditorControls;
    QLineEdit cvKeyValue;

    QWidget cvConfigurationControls;
    QCheckBox cvEnabled;

};
#endif
