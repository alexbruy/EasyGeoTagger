/*
** File: egtplugininterface.h
** Author(s): Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2008-09-30
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
#include "egttestplugin.h"
#include "egtlogger.h"

#include <QtPlugin>
#include <QObject>

static const QString cvCategories = QObject::tr( "Common" );
static const QString cvDescription = QObject::tr( "This plugin also does not do anything, but should only show up in the Common collection" );
static const QString cvName = QObject::tr( "Test Plugin #2" );

QStringList EgtTestPlugin::categories()
{
  return cvCategories.split("|");
}

void EgtTestPlugin::connectConfigurationButton( QPushButton* theButton )
{
  connect( theButton, SIGNAL( clicked() ), this, SLOT( showConfigurationPanel() ) );
}

void EgtTestPlugin::connectRunButton( QPushButton* theButton )
{
  connect( theButton, SIGNAL( clicked() ), this, SLOT( run() ) );
}

QString EgtTestPlugin::description()
{
  return cvDescription;
}

QString EgtTestPlugin::name()
{
  return cvName;
}

void EgtTestPlugin::run()
{
  EgtDebug( "I am test plugin #2!" );
}

void EgtTestPlugin::showConfigurationPanel()
{
  EgtDebug( "You pressed the configuration button" );
}

 Q_EXPORT_PLUGIN2( testplugin, EgtTestPlugin );

