/*
** File: egtexiftagcontrol.cpp
** Author( s ): Peter J. Ersts ( ersts at amnh.org )
** Creation Date: 2008-12-09
**
** Copyright ( c ) 2008, American Museum of Natural History. All rights reserved.
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
#include "egtexiftagcontrol.h"
#include "egtlogger.h"

#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QHBoxLayout>

/*!
 *
 * \param theKey The key for this control
 * \param theDisplayName The common name for the key, used for labels
 * \param theUnits A | delimited list of units if the tag has associated units
 */
EgtExifTagControl::EgtExifTagControl( QString theKey, QString theDisplayName, QStringList theUnits )
{
    cvDisplayName = theDisplayName;
    cvCachedValue = "";
    cvKey = theKey;
    cvUnits.setEnabled( false );

    //Build the editor controls
    cvEditorControls.setLayout( new QHBoxLayout( ) );
    cvEditorControls.layout( )->setContentsMargins( 1, 1, 1, 1 );

    QLabel* lvLabel = new QLabel( cvDisplayName );
    lvLabel->setMinimumWidth( 125 );
    cvEditorControls.layout( )->addWidget( lvLabel );

    cvKeyValue.setMinimumWidth( 100 );
    cvKeyValue.setMaximumHeight( 22 );
    cvEditorControls.layout( )->addWidget( &cvKeyValue );

    if( theUnits.size( ) > 0 )
    {
      cvUnits.setEnabled( true );
      cvUnits.setMinimumWidth( 40 );
      cvUnits.setMaximumHeight( 22 );
      cvEditorControls.layout( )->addWidget( &cvUnits );
      cvUnits.addItems( theUnits );
    }

    cvDiscardButton.setMaximumSize( 22,22 );
    cvDiscardButton.setEnabled( false );
    cvDiscardButton.setIcon( QIcon( ":/16/icons/16/process-stop.png" ) );
    cvDiscardButton.setToolTip( tr( "Discard changes" ) );
    cvEditorControls.layout( )->addWidget( &cvDiscardButton );
    connect( &cvDiscardButton, SIGNAL( clicked( ) ), this, SLOT( cvDiscardButton_clicked( ) ) );
    connect( &cvKeyValue, SIGNAL( textChanged( QString ) ), this, SLOT( cvKeyValue_textChanged( QString ) ) );

    //Build the configuration options
    cvConfigurationControls.setLayout( new QHBoxLayout( ) );
    cvConfigurationControls.layout( )->setContentsMargins( 1, 1, 1, 1 );

    cvEnabled.setChecked( true );
    cvConfigurationControls.layout( )->addWidget( &cvEnabled );

    lvLabel = new QLabel( cvDisplayName );
    lvLabel->setMinimumWidth( 150 );
    cvConfigurationControls.layout( )->addWidget( lvLabel );
    connect( &cvEnabled, SIGNAL( stateChanged( int ) ), this, SLOT( cvEnabled_stateChanged( int ) ) );

}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */
bool EgtExifTagControl::isEnabled( )
{
  return cvEnabled.isChecked( );
}

/*!
 * \param enabled Flag indicating if the control is enabled, associated with the config panel
 */
void EgtExifTagControl::setEnabled( bool enabled )
{
  cvEnabled.setChecked( enabled );
}

/*!
 * \param theValue as pipe delimited list of options
 */
void EgtExifTagControl::setUnits( QString const &theValue )
{
  int lvIndex = cvUnits.findText( theValue );
  if( -1 != lvIndex )
  {
    cvUnits.setCurrentIndex( lvIndex );
  }
  else
  {
    cvUnits.setCurrentIndex( 0 );
  }
}

/*!
 * \param theValue
 */
void EgtExifTagControl::setUnits( QVariant const &theValue )
{
  setUnits( theValue.toString( ) );
}

/*!
 * This function sets the value display in the line edit, replaces the cached value and disabled the discard button
 * If setCache is false, then the value in the line edit is replaced but the cached values is not changed and the discard
 * button is not disbaled, in other words, it acts like someone typing at the console.
 *
 * \param theValue The value to display in the line edit
 * \param setCache A flag to indicate if the cached value should be replaced
 */
void EgtExifTagControl::setValue( QString const &theValue, bool setCache )
{
    cvKeyValue.setText( theValue );
    if( setCache )
    {
      cvCachedValue = theValue;
      cvDiscardButton.setEnabled( false );
    }
}

/*!
 *
 * \param theValue The value to display in the line edit
 * \param setCachedValue A flag to indicate if the cached value should be replaced
 */
void EgtExifTagControl::setValue( QVariant const &theValue, bool setCachedValue )
{
  if( QVariant::Double == theValue.type( ) )
  {
    setValue( QString::number( theValue.toDouble( ), 'f', 7 ), setCachedValue );
  }
  else
  {
    setValue( theValue.toString( ), setCachedValue );
  }

}

/*!
 *
 * \param show Flag to indcate if the this editor control should be visible, i.e., enabled
 */
void EgtExifTagControl::setVisible( bool show )
{
    cvEditorControls.setVisible( show );
}



/*
 *
 * PRIVATE FUNCTIONS
 *
 */
void EgtExifTagControl::cvDiscardButton_clicked( )
{
    cvKeyValue.setText( cvCachedValue );
    cvDiscardButton.setEnabled( false );
}

void EgtExifTagControl::cvEnabled_stateChanged( int theState )
{
  if( Qt::Checked == theState )
  {
    cvEditorControls.setVisible( true );
    emit controlEnabled( cvKey );
  }
  else
  {
    cvEditorControls.setVisible( false );
    emit controlDisabled( cvKey );
  }
}

void EgtExifTagControl::cvKeyValue_textChanged( QString theValue )
{
  cvDiscardButton.setEnabled( true );
}
