/*
** File: "egtpythonconsole.cpp
** Author(s): Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2008-11-18
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
#include "egtpythonconsole.h"
#include "egtlogger.h"

#include <QtPlugin>
#include <QMessageBox>
static const QString cvCategories = QObject::tr( "Utilities" );
static const QString cvDescription = QObject::tr( "Opens a python console" );
static const QString cvName = QObject::tr( "Python Console" );

EgtPythonConsole::EgtPythonConsole()
{
  cvConsole = 0;
}

QStringList EgtPythonConsole::categories()
{
  return cvCategories.split("|");
}

void EgtPythonConsole::connectConfigurationButton( QPushButton* theButton )
{
  connect( theButton, SIGNAL( clicked() ), this, SLOT( showConfigurationPanel() ) );
}

void EgtPythonConsole::connectRunButton( QPushButton* theButton )
{
  connect( theButton, SIGNAL( clicked() ), this, SLOT( run() ) );
}

QString EgtPythonConsole::description()
{
  return cvDescription;
}

QString EgtPythonConsole::name()
{
  return cvName;
}

void EgtPythonConsole::run()
{
  try
  {
      if( 0 == cvConsole )
      {
        cvConsole = new EgtPythonConsoleBase( cvApplicationInterface );
      }
      cvConsole->show();
  }
  catch(...)
  {
    QMessageBox::warning(0, tr("Python plugin"),
                   tr("Error loading Python plugin, Python is not installed in your system"),
                   QMessageBox::Ok );     
  }
}

 Q_EXPORT_PLUGIN2( pythonconsole, EgtPythonConsole );

