/*
** File: egtexifeditor.cpp
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
#include "egtexiftaggroup.h"
#include "egtexifeditor.h"
#include "egtlogger.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

/*!
 * TODO: This should relaly be moved into a GUI lib so that you could build editor plugins on the fly
 *
 * \param theEngine Pointer to the Exif engine that will be used to build the editor
 */
EgtExifEditor::EgtExifEditor( EgtExifEngine* theEngine )
{
  cvExifEngine = theEngine;

  if( 0 == theEngine ) { return; }

  //Layout editor controls
  cvEditorWidget.setLayout( new QVBoxLayout() );
  cvEditorWidget.layout()->setSpacing( 0 );
  cvEditorWidget.layout()->setContentsMargins( 1, 1, 1, 1 );

  //Setup the Configure Dialog
  cvConfigurationDialog.setWindowTitle( tr( "Configure" ) );
  cvConfigurationDialog.setWindowIcon( QIcon( ":/ExifEditor/wrench.png" ) );
  cvConfigurationDialog.setModal( true );
  cvConfigurationDialog.setLayout( new QVBoxLayout() );
  cvConfigurationDialog.layout()->setSpacing( 5 );
  cvConfigurationDialog.layout()->setContentsMargins( 1, 1, 1, 1 );

  //Add group boxes to the configuration dialog
  cvTagGroups.setTitle( tr( "Tag Groups" ) );
  cvTagGroups.setLayout( new QVBoxLayout() );
  cvTagGroups.layout()->setSpacing( 0 );
  cvTagGroups.layout()->setContentsMargins( 1, 1, 1, 1 );
  cvConfigurationDialog.layout()->addWidget( &cvTagGroups );
  cvTagGroups.setVisible( false );

  QGroupBox* lvGroupBox = new QGroupBox( tr( "Individual Tags" ) );
  lvGroupBox->setLayout( new QVBoxLayout() );
  lvGroupBox->layout()->setSpacing( 0 );
  lvGroupBox->layout()->setContentsMargins( 1, 1, 1, 1 );


  //Build Editor and confiuration panels
  QList< EgtExifEngine::KeyMap > lvKeys = cvExifEngine->keys();
  QList< EgtExifEngine::KeyMap >::iterator lvIterator = lvKeys.begin();
  EgtExifTagControl* lvTagControls;
  while( lvIterator != lvKeys.end() ) //Loop through the keys and create the control objects
  {
    lvTagControls = new EgtExifTagControl( lvIterator->key, lvIterator->commonName, lvIterator->hasUnits );//////////////////////////
    cvTagControls[ lvIterator->key ] = lvTagControls;
    cvEditorWidget.layout()->addWidget( lvTagControls->editorControls() );
    lvGroupBox->layout()->addWidget( lvTagControls->configurationControls() );
    connect( lvTagControls, SIGNAL( controlDisabled( QString ) ), this, SLOT( controlDisabled( QString ) ) );
    connect( lvTagControls, SIGNAL( controlEnabled( QString ) ), this, SLOT( controlEnabled( QString ) ) );
    lvIterator++;
  }

  cvConfigurationDialog.layout()->addWidget( lvGroupBox );

  //Add the save button to editor panel
  QWidget* lvPanel = new QWidget();
  lvPanel->setLayout( new QHBoxLayout() );
  lvPanel->layout()->setContentsMargins( 1, 1, 1, 1 );
  ((QHBoxLayout*)lvPanel->layout())->insertStretch(-1, 1);
  cvSaveButton.setText( tr( "Save" ) );
  lvPanel->layout()->addWidget( &cvSaveButton );
  connect( &cvSaveButton, SIGNAL( clicked() ), this, SLOT( cvSaveButton_clicked() ) );
  cvEditorWidget.layout()->addWidget( lvPanel );

  //Add the spacer to push all the objects to the top
  ((QVBoxLayout*)cvConfigurationDialog.layout())->insertStretch(-1, 1);
  ((QVBoxLayout*)cvEditorWidget.layout())->insertStretch(-1, 1);

  //Add a close button to the configuration dialog -- probably should be a button box.
  QPushButton* lvCloseButton = new QPushButton( tr( "Close" ) );
  lvPanel = new QWidget();
  lvPanel->setLayout( new QHBoxLayout() );
  lvPanel->layout()->setContentsMargins( 1, 1, 1, 1 );
  ((QHBoxLayout*)lvPanel->layout())->insertStretch(-1, 1);
  lvPanel->layout()->addWidget( lvCloseButton );
  connect( lvCloseButton, SIGNAL( clicked() ), &cvConfigurationDialog, SLOT( accept() ) );
  cvConfigurationDialog.layout()->addWidget( lvPanel );
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */

void EgtExifEditor::addTagGroup( EgtExifTagGroup* theTagGroup )
{
  cvTagGroups.setVisible( true );
  cvTagGroups.layout()->addWidget( theTagGroup );
  connect( theTagGroup, SIGNAL( tagGroupActivated( QStringList ) ), this, SLOT( tagGroupActivated( QStringList ) ) );
}

/*!
 *
 * \param hasTagData A flag to indicate that the current selected file has appropriate exif data
 */
void EgtExifEditor::loadExifData( bool hasTagData )
{
  EgtDebug( "entered" );

  if( 0 == cvExifEngine ) { return; }

  QMap< QString, EgtExifTagControl* >::iterator lvIterator = cvTagControls.begin();
  if( !hasTagData ) //If the selected file does not have tag data clear the line edits
  {
    QString lvBlank = "";
    while( lvIterator != cvTagControls.end() )
    {
      lvIterator.value()->setValue( lvBlank );
      lvIterator.value()->setValueAssociatedData( lvBlank );
      lvIterator++;
    }
  }
  else //If the selected file does have tag data try to load it into the line edits
  {
    while( lvIterator != cvTagControls.end() )
    {
      lvIterator.value()->setValue( cvExifEngine->read( lvIterator.value()->key() ) );
      lvIterator.value()->setValueAssociatedData( cvExifEngine->read( lvIterator.value()->key()+"Ref" ) );
      lvIterator++;
    }
  }

}

/*
 *
 * SIGNAL and SLOTS
 *
 */

/*!
 *
 * \param theKey The key for the control that was disabed
 */
void EgtExifEditor::controlDisabled( QString theKey )
{
  if( 0 == cvExifEngine ) { return; }

  QString lvDependency = cvExifEngine->dependency( theKey );
  if( !lvDependency.isNull() )
  {
    cvTagControls[ lvDependency ]->setEnabled( false );
  }
}

/*!
 *
 * \param theKey The key for the control that was enabled
 */
void EgtExifEditor::controlEnabled( QString theKey )
{
  if( 0 == cvExifEngine ) { return; }

  QString lvDependency = cvExifEngine->dependency( theKey );
  if( !lvDependency.isNull() )
  {
    cvTagControls[ lvDependency ]->setEnabled( true );
  }
}

void EgtExifEditor::cvSaveButton_clicked()
{
  if( 0 == cvExifEngine ) { return; }

  QMap< QString, EgtExifTagControl* >::iterator lvIterator = cvTagControls.begin();
  while( lvIterator != cvTagControls.end() )
  {
    if( lvIterator.value()->isEnabled() && lvIterator.value()->value() != "" )
    {
      cvExifEngine->write( lvIterator.value()->key(),  lvIterator.value()->value() );
      (*lvIterator)->setValue( cvExifEngine->read( lvIterator.value()->key() ) );

      cvExifEngine->write( lvIterator.value()->key()+"Ref",  lvIterator.value()->AssociatedDataValue() );
      (*lvIterator)->setValueAssociatedData( cvExifEngine->read( lvIterator.value()->key()+"Ref" ) );
    }
    lvIterator++;
  }
}

void EgtExifEditor::tagGroupActivated( QStringList theKeys )
{
  QMap< QString, EgtExifTagControl* >::iterator lvIterator = cvTagControls.begin();
  while( lvIterator != cvTagControls.end() )
  {
    lvIterator.value()->setEnabled( false );
    lvIterator++;
  }
  QStringList::iterator lvTagRunner = theKeys.begin();
  while( lvTagRunner != theKeys.end() )
  {
    cvTagControls[ *lvTagRunner ]->setEnabled( true );
    lvTagRunner++;
  }
}

void EgtExifEditor::setTagData( QString theKey, QString theValue, bool setCachedValue )
{
  if( cvTagControls.contains( theKey ) )
  {
    cvTagControls[ theKey ]->setValue( theValue, setCachedValue );
  }
}

void EgtExifEditor::setTagData( QString theKey, QVariant theValue, bool setCachedValue )
{
  if( cvTagControls.contains( theKey ) )
  {
    cvTagControls[ theKey ]->setValue( theValue, setCachedValue );
  }
}
