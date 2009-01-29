/*
** File: egtdelimitedtextfilereader.cpp
** Author(s): Roberto Garcia Yunta
** Creation Date: 2008-12-19
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
#include "egtdelimitedtextfilereader.h"

#include <QFile>
#include <QTextStream>

EgtDelimitedTextFileReader::EgtDelimitedTextFileReader():EgtFileReader()
{
  cvDelimiter = ",";
  cvHasColumnHeaders = false;
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */
/*!
 * \return a QString that contains the column headers
 */
QStringList EgtDelimitedTextFileReader::columnHeaders()
{
  return cvColumnHeaders;
}

/*!
 * \return a boolean that indicates whether the file has column headers or not
 */
bool EgtDelimitedTextFileReader::hasColumnHeaders()
{
  return cvHasColumnHeaders; 
}

bool EgtDelimitedTextFileReader::preprocessFile( QString )
{

}

/*!
 * \param ok pointer to boolean that indicates whether the read was performed correctly or not
 */
QList<QStringList> EgtDelimitedTextFileReader::read( bool* ok)
{
  QFile lvFile( cvFileName );
  QStringList lvStringList;
  QList<QStringList>  lvList;

  bool lvError = false;

  int lvNumFields;

  if (!lvFile.open(QFile::ReadOnly | QFile::Text)) 
  {
    if (ok)
      *ok = false;
    cvLastError = QObject::tr( "Can't read the file") + ": " + cvFileName;
    return lvList;
  }

  QTextStream stream( &lvFile );
  QString lvLine;
  
  if( hasColumnHeaders() )
  {
    lvLine = stream.readLine(); 
    cvColumnHeaders = lvLine.split( cvDelimiter );
    lvNumFields = lvLine.split( cvDelimiter ).size();
  }
  else
  {
    lvLine = stream.readLine();
    lvList << lvLine.split( cvDelimiter );
    lvNumFields = lvLine.split( cvDelimiter ).size();
  }

  while( ! stream.atEnd() )
  {
    lvLine = stream.readLine();
   
    if( lvLine.split( cvDelimiter ).size() == lvNumFields ) 
      lvList << lvLine.split( cvDelimiter );
    else
    {
      lvError = true;
      cvLastError = QObject::tr( "One or more rows have been discarded") + ": " + cvFileName;
    }
  }
  if( !lvError )
    cvLastError = "";
  return lvList;
}

/*!
 * \param theFileName a QString that contains the name of the file to be read
 */
void EgtDelimitedTextFileReader::setFileName( QString theFileName )
{
  cvFileName = theFileName;
}

/*!
 * \param a QString that contains a delimiter
 */
void EgtDelimitedTextFileReader::setDelimiter( QString theDelimiter )
{
  cvDelimiter = theDelimiter;
}
