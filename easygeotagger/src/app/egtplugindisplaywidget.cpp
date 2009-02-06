/*
** File: egtplugindisplaywidget.cpp
** Author(s): Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2008-10-06
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
#include "egtplugindisplaywidget.h"

#include <QIcon>
#include <QPushButton>
#include <QVBoxLayout>

EgtPluginDisplayWidget::EgtPluginDisplayWidget( EgtPluginInterface* thePlugin, QWidget* theParent, Qt::WindowFlags theFlags) : QFrame( theParent, theFlags )
{
  //Set some styles for the frame
  setStyleSheet( "QFrame { background-color: white; }" );
  
  //create a new layout for this display item
  setLayout( new QVBoxLayout( this ) );
  
  //Add the launch button
  layout()->addWidget( &cvRunButton );
  
  //Add the description
  layout()->addWidget( &cvDescription );
  
   //Add configuration button if necessary.
  if( thePlugin->isConfigurable() )
  {
    cvConfigureButton.setMaximumSize( 26, 26 );
    cvConfigureButton.setToolTip( tr( "Configure plugin" ) );
    cvConfigureButton.setIcon ( QIcon( ":/22/icons/22/preferences-system.png" ) );
    layout()->addWidget( &cvConfigureButton );
  }
  
  update( thePlugin );
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */
/*!
 * \param thePlugin a pointer to the plugin interface from which to new descriptions and such
 */
void EgtPluginDisplayWidget::update( EgtPluginInterface* thePlugin )
{
  if( 0 == thePlugin ) { return; }
  
  cvRunButton.setText( thePlugin->name() );
  thePlugin->connectRunButton( &cvRunButton );
  
  cvDescription.setText( thePlugin->description() );
  cvDescription.setWordWrap( true );
  cvDescription.setTextFormat( Qt::RichText );
  cvDescription.setStyleSheet( "QLabel { background-color: white; font-size: 10px; }" );
  
  //Add configuration button if necessary.
  if( thePlugin->isConfigurable() )
  {
    thePlugin->connectConfigurationButton( &cvConfigureButton );
  }
}
