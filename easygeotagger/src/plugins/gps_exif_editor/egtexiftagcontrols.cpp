/*
** File: egtexiftagcontrols.cpp
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
#include "egtexiftagcontrols.h"
#include "egtlogger.h"

#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QHBoxLayout>

EgtExifTagControls::EgtExifTagControls( QString theKey, QString theDisplayName )
{
    cvDisplayName = theDisplayName;
    cvCachedValue = "";
    cvKey = theKey;

    //Build the editor controls
    cvEditorControls.setLayout( new QHBoxLayout() );
    cvEditorControls.layout()->setContentsMargins( 1, 1, 1, 1 );

    QLabel* lvLabel  = new QLabel(cvDisplayName);
    lvLabel->setMinimumWidth( 150 );
    cvEditorControls.layout()->addWidget( lvLabel );

    cvKeyValue.setMinimumWidth( 150 );
    cvEditorControls.layout()->addWidget( &cvKeyValue );

    cvDiscardButton.setMaximumSize( 24,24 );
    cvDiscardButton.setEnabled( false );
    cvDiscardButton.setIcon( QIcon( ":/ExifEditor/cancel.png" ) );
    cvDiscardButton.setToolTip( tr( "Discard changes" ) );
    cvEditorControls.layout()->addWidget( &cvDiscardButton );
    connect( &cvDiscardButton, SIGNAL( clicked() ), this, SLOT( cvDiscardButton_clicked() ) );
    connect( &cvKeyValue, SIGNAL( textEdited( QString ) ), this, SLOT( cvKeyValue_textEdited( QString ) ) );

    //Build the configuration options
    cvConfigurationControls.setLayout( new QHBoxLayout() );
    cvConfigurationControls.setWindowTitle( tr( "Configure" ) );
    cvConfigurationControls.layout()->setContentsMargins( 1, 1, 1, 1 );

    cvEnabled.setChecked( true );
    cvConfigurationControls.layout()->addWidget( &cvEnabled );

    lvLabel  = new QLabel(cvDisplayName);
    lvLabel->setMinimumWidth( 150 );
    cvConfigurationControls.layout()->addWidget( lvLabel );
    connect( &cvEnabled, SIGNAL( stateChanged( int ) ), this, SLOT( cvEnabled_stateChanged( int ) ) );

}

bool EgtExifTagControls::isEnabled()
{
  return cvEnabled.isChecked();
}

void EgtExifTagControls::setValue( QString const &theValue )
{
    cvCachedValue = theValue;
    cvKeyValue.setText( theValue );
}

void EgtExifTagControls::setValue( QVariant const &theValue )
{
  if( QVariant::Double == theValue.type() )
  {
    setValue( QString::number( theValue.toDouble(), 'f', 7 ) );
  }
  else
  {
    setValue( theValue.toString() );
  }

}

void EgtExifTagControls::setVisible( bool show )
{
    cvEditorControls.setVisible( show );
}





void EgtExifTagControls::cvDiscardButton_clicked()
{
    cvKeyValue.setText( cvCachedValue );
    cvDiscardButton.setEnabled( false );
}

void EgtExifTagControls::cvEnabled_stateChanged( int theState )
{
  if( Qt::Checked == theState )
  {
    cvEditorControls.setVisible( true );
  }
  else
  {
    cvEditorControls.setVisible( false );
  }
}

void EgtExifTagControls::cvKeyValue_textEdited( QString theValue )
{
  cvDiscardButton.setEnabled( true );
}
