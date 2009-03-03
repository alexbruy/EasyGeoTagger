/*
** File: egtexifeditor.cpp
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
#include "egtexiftaggroup.h"
#include "egtexifeditor.h"
#include "egtlogger.h"

#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSettings>

/*!
 *
 * \param theId A unique string to that identifies this editor or storing preferences between sessions
 * \param theEngine Pointer to the Exif engine that will be used to build the editor
 */
EgtExifEditor::EgtExifEditor( QString theId, EgtExifEngine* theEngine )
{
  cvId = theId;
  cvExifEngine = theEngine;

  if( 0 == theEngine ) { return; }

  //Layout editor controls
  cvEditorWidget.setLayout( new QVBoxLayout( ) );
  cvEditorWidget.layout( )->setSpacing( 0 );
  cvEditorWidget.layout( )->setContentsMargins( 1, 1, 1, 1 );

  //Setup the Configure Dialog
  cvConfigurationDialog.setWindowTitle( tr( "Configure" ) );
  cvConfigurationDialog.setWindowIcon( QIcon( ":/22/icons/22/preferences-system.png" ) );
  cvConfigurationDialog.setModal( true );
  cvConfigurationDialog.setLayout( new QVBoxLayout( ) );
  cvConfigurationDialog.layout( )->setSpacing( 5 );
  cvConfigurationDialog.layout( )->setContentsMargins( 1, 1, 1, 1 );

  //Add group boxes to the configuration dialog
  cvTagGroups.setTitle( tr( "Tag Groups" ) );
  cvTagGroups.setLayout( new QVBoxLayout( ) );
  cvTagGroups.layout( )->setSpacing( 0 );
  cvTagGroups.layout( )->setContentsMargins( 1, 1, 1, 1 );
  cvConfigurationDialog.layout( )->addWidget( &cvTagGroups );
  cvTagGroups.setVisible( false );

  QGroupBox* lvGroupBox = new QGroupBox( tr( "Individual Tags" ) );
  lvGroupBox->setLayout( new QVBoxLayout( ) );
  lvGroupBox->layout( )->setSpacing( 0 );
  lvGroupBox->layout( )->setContentsMargins( 1, 1, 1, 1 );


  //Build Editor and confiuration panels
  QList< EgtExifEngine::KeyMap > lvKeys = cvExifEngine->keys( );
  QList< EgtExifEngine::KeyMap >::iterator lvIterator = lvKeys.begin( );
  EgtExifTagControl* lvTagControls;
  while( lvIterator != lvKeys.end( ) ) //Loop through the keys and create the control objects
  {
    lvTagControls = new EgtExifTagControl( lvIterator->key, lvIterator->commonName, lvIterator->units );
    cvTagControls[ lvIterator->key ] = lvTagControls;
    cvEditorWidget.layout( )->addWidget( lvTagControls->editorControls( ) );
    lvGroupBox->layout( )->addWidget( lvTagControls->configurationControls( ) );
    connect( lvTagControls, SIGNAL( controlDisabled( QString ) ), this, SLOT( controlDisabled( QString ) ) );
    connect( lvTagControls, SIGNAL( controlEnabled( QString ) ), this, SLOT( controlEnabled( QString ) ) );
    lvIterator++;
  }

  cvConfigurationDialog.layout( )->addWidget( lvGroupBox );

  //Add the save button to editor panel
  QWidget* lvPanel = new QWidget( );
  lvPanel->setLayout( new QHBoxLayout( ) );
  lvPanel->layout( )->setContentsMargins( 1, 1, 1, 1 );
 ( ( QHBoxLayout* )lvPanel->layout( ) )->insertStretch( -1, 1 );
  cvSaveButton.setText( tr( "Save" ) );
  lvPanel->layout( )->addWidget( &cvSaveButton );
  connect( &cvSaveButton, SIGNAL( clicked( ) ), this, SLOT( cvSaveButton_clicked( ) ) );
  cvEditorWidget.layout( )->addWidget( lvPanel );

  //Add the spacer to push all the objects to the top
 ( ( QVBoxLayout* )cvConfigurationDialog.layout( ) )->insertStretch( -1, 1 );
 ( ( QVBoxLayout* )cvEditorWidget.layout( ) )->insertStretch( -1, 1 );

  //Add a close button to the configuration dialog -- probably should be a button box.
  QDialogButtonBox* lvButtonBox = new QDialogButtonBox( QDialogButtonBox::Ok );
  connect( lvButtonBox, SIGNAL( accepted( ) ), &cvConfigurationDialog, SLOT( accept( ) ) );
  cvConfigurationDialog.layout( )->addWidget( lvButtonBox );

  //Loop through the controls and restore setting from last session
  QSettings lvSettings;

  QMap< QString, EgtExifTagControl* >::iterator lvTagIterator = cvTagControls.begin( );
  while( lvTagIterator != cvTagControls.end( ) )
  {
    lvTagIterator.value( )->setEnabled( lvSettings.value( cvId + "/" +  lvTagIterator.value( )->key( ), true ).toBool( ) );
    lvTagIterator++;
  }
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */

/*!
 * \param theTagGroup A pointer to a tag group to add to the configuration widget
 */
void EgtExifEditor::addTagGroup( EgtExifTagGroup* theTagGroup )
{
  cvTagGroups.setVisible( true );
  cvTagGroups.layout( )->addWidget( theTagGroup );
  connect( theTagGroup, SIGNAL( tagGroupActivated( QStringList ) ), this, SLOT( tagGroupActivated( QStringList ) ) );
}

/*!
  * \param hasTagData A flag to indicate that the current selected file has appropriate exif data
 */
void EgtExifEditor::loadExifData( bool hasTagData )
{
  EgtDebug( "entered" );

  if( 0 == cvExifEngine ) { return; }

  QString lvBlank = "";
  QMap< QString, EgtExifTagControl* >::iterator lvIterator = cvTagControls.begin( );
  if( !hasTagData ) //If the selected file does not have tag data clear the line edits
  {
    while( lvIterator != cvTagControls.end( ) )
    {
      lvIterator.value( )->setValue( lvBlank );
      lvIterator.value( )->setUnits( lvBlank );
      lvIterator++;
    }
  }
  else //If the selected file does have tag data try to load it into the line edits
  {
    bool isValid;
    QVariant lvResult;
    QVariant lvInvalidResult( "" );
    while( lvIterator != cvTagControls.end( ) )
    {
      if( lvIterator.value( )->isEnabled( ) )
      {
        lvResult = cvExifEngine->read( lvIterator.value( )->key( ), &isValid );
        if( isValid )
        {
       ( *lvIterator )->setValue( lvResult );
        }
        else
        {
       ( *lvIterator )->setValue( lvInvalidResult );
        }

        if( lvIterator.value( )->hasUnits( ) )
        {
          lvResult = cvExifEngine->read( lvIterator.value( )->key( )+"Ref", &isValid );
          if( isValid )
          {
         ( *lvIterator )->setUnits( lvResult );
          }
          else
          {
         ( *lvIterator )->setUnits( lvInvalidResult );
          }
        }
      }
      else
      {
        lvIterator.value( )->setValue( lvBlank );
        lvIterator.value( )->setUnits( lvBlank );
      }

      lvIterator++;
    }
  }
}

void EgtExifEditor::showConfigurationDialog( QPoint thePoint )
{
   cvConfigurationDialog.move( thePoint );
   cvConfigurationDialog.show( );
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

  QSettings lvSettings;
  lvSettings.setValue( cvId + "/" + theKey, false );

  QString lvDependency = cvExifEngine->dependency( theKey );
  if( !lvDependency.isNull( ) )
  {
    cvTagControls[ lvDependency ]->setEnabled( false );
    lvSettings.setValue( cvId + "/" + lvDependency, false );
  }
}

/*!
 *
 * \param theKey The key for the control that was enabled
 */
void EgtExifEditor::controlEnabled( QString theKey )
{
  if( 0 == cvExifEngine ) { return; }

  //Try to populate the controls data
  bool isValid;
  QVariant lvResult;
  QVariant lvInvalidResult( "" );
  lvResult = cvExifEngine->read( theKey, &isValid );
  if( isValid )
  {
    cvTagControls[ theKey ]->setValue( lvResult );
  }
  else
  {
    cvTagControls[ theKey ]->setValue( lvInvalidResult );
  }

  if( cvTagControls[ theKey ]->hasUnits( ) )
  {
    lvResult = cvExifEngine->read( theKey+"Ref", &isValid );
    if( isValid )
    {
      cvTagControls[ theKey ]->setUnits( lvResult );
    }
    else
    {
      cvTagControls[ theKey ]->setUnits( lvInvalidResult );
    }
  }

  //Update persistent settings
  QSettings lvSettings;
  lvSettings.setValue( cvId + "/" + theKey, true );

  //Enabled any dependency
  QString lvDependency = cvExifEngine->dependency( theKey );
  if( !lvDependency.isNull( ) && !cvTagControls[ lvDependency ]->isEnabled( ) )
  {
    cvTagControls[ lvDependency ]->setEnabled( true );
    lvSettings.setValue( cvId + "/" + lvDependency, true );
  }
}

void EgtExifEditor::cvSaveButton_clicked( )
{
  if( 0 == cvExifEngine ) { return; }

  bool isValid = false;
  QVariant lvResult;
  QVariant lvInvalidResult( "" );
  QMap< QString, EgtExifTagControl* >::iterator lvIterator = cvTagControls.begin( );
  while( lvIterator != cvTagControls.end( ) )
  {
    if( lvIterator.value( )->isEnabled( ) && lvIterator.value( )->value( ) != "" )
    {
      cvExifEngine->write( lvIterator.value( )->key( ),  lvIterator.value( )->value( ) );
      lvResult = cvExifEngine->read( lvIterator.value( )->key( ), &isValid );
      if( isValid )
      {
     ( *lvIterator )->setValue( lvResult );
      }
      else
      {
     ( *lvIterator )->setValue( lvInvalidResult );
      }

      if( lvIterator.value( )->hasUnits( ) )
      {
        cvExifEngine->write( lvIterator.value( )->key( )+"Ref",  lvIterator.value( )->units( ) );
        lvResult = cvExifEngine->read( lvIterator.value( )->key( )+"Ref", &isValid );
        if( isValid )
        {
       ( *lvIterator )->setUnits( lvResult );
        }
        else
        {
       ( *lvIterator )->setUnits( lvInvalidResult );
        }
      }
    }
    lvIterator++;
  }
}

/*!
 * \param theKeys the Egt keys in the tag group which was just activated
 */
void EgtExifEditor::tagGroupActivated( QStringList theKeys )
{
  QMap< QString, EgtExifTagControl* >::iterator lvIterator = cvTagControls.begin( );
  while( lvIterator != cvTagControls.end( ) )
  {
    cvTagControls[ lvIterator.value( )->key( ) ]->setEnabled( false );
    lvIterator++;
  }
  QStringList::iterator lvTagRunner = theKeys.begin( );
  while( lvTagRunner != theKeys.end( ) )
  {
    cvTagControls[ *lvTagRunner ]->setEnabled( true );
    lvTagRunner++;
  }
}

/*!
 * \param theKey Egt key to receive the incoming data
 * \param theValue the data for the Egt key
 * \param setCachedValue Flag to indicate if the new data should be considered the fall back data if the user should change the value in the editor
 */
void EgtExifEditor::setTagData( QString theKey, QString theValue, bool setCachedValue )
{
  if( cvTagControls.contains( theKey ) )
  {
    cvTagControls[ theKey ]->setValue( theValue, setCachedValue );
  }
}

/*!
 * \param theKey Egt key to receive the incoming data
 * \param theValue the data for the Egt key
 * \param setCachedValue Flag to indicate if the new data should be considered the fall back data if the user should change the value in the editor
 */
void EgtExifEditor::setTagData( QString theKey, QVariant theValue, bool setCachedValue )
{
  if( cvTagControls.contains( theKey ) )
  {
    cvTagControls[ theKey ]->setValue( theValue, setCachedValue );
  }
}
