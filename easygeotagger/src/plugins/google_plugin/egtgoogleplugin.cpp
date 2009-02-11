/*
** File: egtgoogleplugin.h
** Author(s): Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2009-02-10
**
** Copyright (c) 2009, American Museum of Natural History. All rights reserved.
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
#include "egtapplicationinterface.h"
#include "egtgoogleplugin.h"
#include "egtlogger.h"

#include <QtPlugin>

EgtGooglePlugin::EgtGooglePlugin()
{
  cvCategories = tr( "Coordinate Extraction" );
  cvDescription = tr( "This plugin will allow you to capture coordinates from a Google map" );
  cvName = tr( "Google Map" );
}

void EgtGooglePlugin::connectConfigurationButton( QPushButton* theButton )
{
  connect( theButton, SIGNAL( clicked() ), this, SLOT( showConfigurationPanel() ) );
}

void EgtGooglePlugin::connectRunButton( QPushButton* theButton )
{
  connect( theButton, SIGNAL( clicked() ), this, SLOT( run() ) );
}

void EgtGooglePlugin::initPlugin()
{
  //Hook listeners into the application interface
  if( 0 != cvApplicationInterface )
  {
    connect( &cvDisplayWidget, SIGNAL( keyValuePair( QString, QString ) ), cvApplicationInterface, SLOT( acceptKeyValuePair( QString, QString ) ) );
  }
}

void EgtGooglePlugin::run()
{
  if( 0 != cvApplicationInterface )
  {
    cvDisplayWidget.move(  cvApplicationInterface->positionOfFirstVisibleWidget() );
    cvDisplayWidget.show();
  }

}

 Q_EXPORT_PLUGIN2( googlePlugin, EgtGooglePlugin );

