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
#include "egtlogger.h"

EgtPythonConsoleBase::EgtPythonConsoleBase( EgtApplicationInterface* theInterface )
{
  EgtDebug( "entered" );

  setupUi(this);

  Py_Initialize();
  cvMainModule = PyImport_AddModule( "__main__" );
  cvDictionary = PyModule_GetDict( cvMainModule );

  runCommand( "import sys" );
  runCommand( "import __main__" );
  runCommand( "from PyEasyGT import *" );
  runCommand( "from sip import wrapinstance" );
  runCommand( "from PyQt4.QtCore import *" );
  runCommand( "from PyQt4.QtGui import *" );
  runCommand( "class redirect:\n"
              "  def __init__( self, textbrowser ):\n"
              "    self.console = textbrowser\n"
              "  def write( self, message ):\n"
              "    if message.__ne__('\\n'):\n"
              "        self.console.append( message )\n"
              "\n"
            );
  runCommand( "outputConsole = wrapinstance(" + QString::number(( unsigned long ) tbOutput ) + ", QTextBrowser)" );
  runCommand( "logger = redirect( outputConsole ) " );
  runCommand( "sys.stderr = sys.stdout = logger" );
  runCommand( "EgtInterface = wrapinstance(" + QString::number(( unsigned long ) theInterface ) + ", EgtApplicationInterface)" );
}

bool  EgtPythonConsoleBase::runCommand( QString theCommand )
{
  PyRun_String( qPrintable( theCommand ), Py_single_input, cvDictionary, cvDictionary );//Py_file_input
  return ( PyErr_Occurred() == 0 );
}

void EgtPythonConsoleBase::on_pbtnRun_clicked()
{
  EgtDebug( QString("run python command\n%1") .arg( teInput->toPlainText() ) );

  tbOutput->append( ">>"+( teInput->toPlainText() ).replace( "\n","\n>>" ) );
  
  if( !runCommand( teInput->toPlainText() ) )
  {
    QString className, errorText;
    getError( className, errorText );
    tbOutput->append( className );
    tbOutput->append( errorText );
  }
  teInput->setText( "" );
}

bool EgtPythonConsoleBase::getError( QString& errorClassName, QString& errorText )
{
  if ( !PyErr_Occurred() )
    return false;

  PyObject* obj_str;
  PyObject* err_type;
  PyObject* err_value;
  PyObject* err_tb;

  // get the exception information and clear error
  PyErr_Fetch( &err_type, &err_value, &err_tb );

  // get exception's class name
  errorClassName = getTypeAsString( err_type );

  // get exception's text
  if ( err_value != NULL && err_value != Py_None )
  {
    obj_str = PyObject_Str( err_value ); // new reference
    errorText = PyString_AS_STRING( obj_str );
    Py_XDECREF( obj_str );
  }
  else
    errorText.clear();

  // cleanup
  Py_XDECREF( err_type );
  Py_XDECREF( err_value );
  Py_XDECREF( err_tb );

  return true;
}


QString EgtPythonConsoleBase::getTypeAsString( PyObject* obj )
{
  if ( obj == NULL )
    return NULL;

  if ( PyClass_Check( obj ) )
  {
    
    return QString( PyString_AsString((( PyClassObject* )obj )->cl_name ) );
  }
  else if ( PyType_Check( obj ) )
  {
    
    return QString((( PyTypeObject* )obj )->tp_name );
  }
  else
  {
    
    PyObject* s = PyObject_Str( obj );
    QString str;
    if ( s && PyString_Check( s ) )
      str = QString( PyString_AsString( s ) );
    Py_XDECREF( s );
    return str;
  }
}