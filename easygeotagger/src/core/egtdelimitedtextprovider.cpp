/*
** File: egtdelimitedtextprovider.cpp
** Author( s ): Roberto Garcia Yunta
** Creation Date: 2008-12-19
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
#include "egtdelimitedtextprovider.h"

#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QMessageBox>

EgtDelimitedTextProvider::EgtDelimitedTextProvider( ) : EgtDataProvider( )
{
  cvDelimiter = ",";
  cvFileName = ""; 
  cvLastError = "";
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */

/*!
 * \param theFileName a QString that contains the name of the file to be read
 */
EgtDataProvider::ErrorType EgtDelimitedTextProvider::setFileName( QString theFileName )
{
  cvFileName = theFileName;
  return read();
}

/*!
 * \param a QString that contains a delimiter
 */
void EgtDelimitedTextProvider::setDelimiter( QString theDelimiter )
{
  cvDelimiter = theDelimiter;
}

/*
 *
 * PROTECTED FUNCTIONS
 *
 */
/*!
 * \param ok pointer to boolean that indicates whether the read was performed correctly or not
 */
EgtDataProvider::ErrorType EgtDelimitedTextProvider::read( )
{
  cvData.clear();
  cvLastError = "";

  QFile lvFile( cvFileName );
  QStringList lvStringList;
  bool lvError = false;

  if ( !lvFile.open( QFile::ReadOnly | QFile::Text ) )
  {
    cvLastError = QObject::tr( "Can't read the file" ) + ": " + cvFileName;
    return EgtDataProvider::Fatal;
  }

  QTextStream stream( &lvFile );
  QString lvLine;

  if( hasColumnHeaders( ) )
  {
    lvLine = stream.readLine( );
    cvColumnHeaders = lvLine.split( cvDelimiter );
    cvNumberOfFields = lvLine.split( cvDelimiter ).size( );
  }
  else
  {
    lvLine = stream.readLine( );
    cvData << lvLine.split( cvDelimiter );
    cvNumberOfFields = lvLine.split( cvDelimiter ).size( );
  }

  while( ! stream.atEnd( ) )
  {
    lvLine = stream.readLine( );

    if( lvLine.split( cvDelimiter ).size( ) == cvNumberOfFields )
    {
      cvData << lvLine.split( cvDelimiter );
    }
    else
    {
      lvError = true;
      cvLastError = QObject::tr( "One or more rows have been discarded" ) + ": " + cvFileName;
    }
  }

  if( lvError ) { return EgtDataProvider::Warning; }

  return EgtDataProvider::None;
}
