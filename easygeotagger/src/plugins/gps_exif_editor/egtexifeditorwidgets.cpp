/*
** File: egtexifeditorwidgets.cpp
** Author(s): Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2008-12-11
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
#include "egtexifeditorwidgets.h"
#include "egtlogger.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

EgtExifEditorWidgets::EgtExifEditorWidgets( EgtExifEngine* theEngine, QDockWidget* theDock )
{
  cvExifEngine = theEngine;

  if( 0 == theEngine || 0 == theDock ) { return; }

  //Layout editor controls
  cvEditorControls.setParent( theDock );
  cvEditorControls.setLayout( new QVBoxLayout() );
  cvEditorControls.layout()->setSpacing( 0 );
  cvEditorControls.layout()->setContentsMargins( 1, 1, 1, 1 );

  cvConfigurationControls.setWindowTitle( tr( "Configure" ) );
  cvConfigurationControls.setModal( true );
  cvConfigurationControls.setLayout( new QVBoxLayout() );
  cvConfigurationControls.layout()->setSpacing( 0 );
  cvConfigurationControls.layout()->setContentsMargins( 1, 1, 1, 1 );

  //Build Editor and confiuration panels
  QList< EgtExifEngine::KeyMap > lvKeys = cvExifEngine->keys();
  QList< EgtExifEngine::KeyMap >::iterator lvIterator = lvKeys.begin();
  EgtExifTagControl* lvTagControls;
  while( lvIterator != lvKeys.end() )
  {
    lvTagControls = new EgtExifTagControl( lvIterator->key, lvIterator->commonName );
    cvTagControls[ lvIterator->key ] = lvTagControls;
    cvEditorControls.layout()->addWidget( lvTagControls->editorControls() );
    cvConfigurationControls.layout()->addWidget( lvTagControls->configurationControls() );
    connect( lvTagControls, SIGNAL( controlDisabled( QString ) ), this, SLOT( controlDisabled( QString ) ) );
    connect( lvTagControls, SIGNAL( controlEnabled( QString ) ), this, SLOT( controlEnabled( QString ) ) );
    lvIterator++;
  }

  //Add the save button
  QWidget* lvPanel = new QWidget();
  lvPanel->setLayout( new QHBoxLayout() );
  lvPanel->layout()->setContentsMargins( 1, 1, 1, 1 );
  ((QHBoxLayout*)lvPanel->layout())->insertStretch(-1, 1);
  cvSaveButton.setText( tr( "Save" ) );
  lvPanel->layout()->addWidget( &cvSaveButton );
  connect( &cvSaveButton, SIGNAL( clicked() ), this, SLOT( cvSaveButton_clicked() ) );
  cvEditorControls.layout()->addWidget( lvPanel );

  //Add the spacer to push all the objects to the top
  ((QVBoxLayout*)cvConfigurationControls.layout())->insertStretch(-1, 1);
  ((QVBoxLayout*)cvEditorControls.layout())->insertStretch(-1, 1);
  theDock->setWidget( &cvEditorControls );
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */
void EgtExifEditorWidgets::updateEditorControls( bool hasTagData )
{
  EgtDebug( "entered" );

  if( 0 == cvExifEngine ) { return; }

  //Get data if it exists
  QMap< QString, EgtExifTagControl* >::iterator lvIterator = cvTagControls.begin();
  if( !hasTagData )
  {
    QString lvBlank = "";
    while( lvIterator != cvTagControls.end() )
    {
      lvIterator.value()->setValue( lvBlank );
      lvIterator++;
    }
  }
  else
  {
    while( lvIterator != cvTagControls.end() )
    {
      lvIterator.value()->setValue( cvExifEngine->read( lvIterator.value()->key() ) );
      lvIterator++;
    }
  }

}

/*
 *
 * SIGNAL and SLOTS
 *
 */
void EgtExifEditorWidgets::controlDisabled( QString theKey )
{
  if( 0 == cvExifEngine ) { return; }

  QString lvDependency = cvExifEngine->dependency( theKey );
  if( !lvDependency.isNull() )
  {
    cvTagControls[ lvDependency ]->setEnabled( false );
  }
}

void EgtExifEditorWidgets::controlEnabled( QString theKey )
{
  if( 0 == cvExifEngine ) { return; }

  QString lvDependency = cvExifEngine->dependency( theKey );
  if( !lvDependency.isNull() )
  {
    cvTagControls[ lvDependency ]->setEnabled( true );
  }
}

void EgtExifEditorWidgets::cvSaveButton_clicked()
{
  if( 0 == cvExifEngine ) { return; }

  QMap< QString, EgtExifTagControl* >::iterator lvIterator = cvTagControls.begin();
  while( lvIterator != cvTagControls.end() )
  {
    if( lvIterator.value()->isEnabled() && lvIterator.value()->value() != "" )
    {
      EgtDebug( QString("Writing value to image for key: %1") .arg( lvIterator.value()->key() ) );
      cvExifEngine->write( lvIterator.value()->key(),  lvIterator.value()->value() );
      (*lvIterator)->setValue( cvExifEngine->read( lvIterator.value()->key() ) );
    }
    lvIterator++;
  }
}
