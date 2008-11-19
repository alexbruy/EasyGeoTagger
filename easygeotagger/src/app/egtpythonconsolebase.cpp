/*
** File: egtpythonconsolebase.cpp
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
#include "egtpythonconsolebase.h"

EgtPythonConsoleBase::EgtPythonConsoleBase( EgtApplicationInterface* theInterface )
{
  setupUi(this);

  Py_Initialize();
  cvMainModule = PyImport_AddModule( "__main__" );
  cvDictionary = PyModule_GetDict( cvMainModule );

  runCommand( "import sys" );
  //runCommand( "from PyEasyGT" );
  runCommand( "from sip import wrapinstance" );
  runCommand( "from PyQt4.QtCore import *" );
  runCommand( "from PyQt4.QtGui import *" );
  runCommand( "class redirect:\n"
              "  def __init__( self, textbrowser ):\n"
              "    self.console = textbrowser\n"
              "  def write( self, message ):\n"
              "    self.console.append( message )\n"
              "\n"
            );
  runCommand( "outputConsole = wrapinstance(" + QString::number(( unsigned long ) tbOutput ) + ", QTextBrowser)" );
  runCommand( "logger = redirect( outputConsole ) " );
  runCommand( "sys.stdout = logger" );
  //runCommand( "EgtInterface = wrapinstance(" + QString::number(( unsigned long ) theInterface ) + ", EgtApplicationInterface)" );

}

bool  EgtPythonConsoleBase::runCommand( QString theCommand )
{
  PyRun_String( qPrintable( theCommand ), Py_file_input, cvDictionary, cvDictionary );
  return ( PyErr_Occurred() == 0 );
}

void EgtPythonConsoleBase::on_pbtnRun_clicked()
{
    runCommand( teInput->toPlainText() );
}
