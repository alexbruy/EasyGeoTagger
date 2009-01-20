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
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */

/*!
 * \param ok pointer to boolean that indicates whether the read was performed correctly or not
 */
QStringList EgtDelimitedTextFileReader::read( bool* ok)
{
  QFile lvFile( cvFileName );
  QStringList lvList;
  if (!lvFile.open(QFile::ReadOnly | QFile::Text)) 
  {
    if (ok)
      *ok = false;
    return lvList;
  }

  QTextStream stream( &lvFile );
  QString lvLine;
  

  if( hasColumnHeaders() )
  {
    stream.readLine();
  }

  while( ! stream.atEnd() )
  {
    lvLine = stream.readLine();
    lvList << lvLine.split( cvDelimiter );
  }

  return lvList;
}

/*!
 * \return a QString that contains the column headers
 */
QStringList EgtDelimitedTextFileReader::columnHeaders()
{
  QStringList lvList;
  return lvList;
}

/*!
 * \return a boolean that indicates whether the file has column headers or not
 */
bool EgtDelimitedTextFileReader::hasColumnHeaders()
{
  return false; 
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

bool EgtDelimitedTextFileReader::preprocessFile( QString )
{

}


/*
 *
 * PRIVATE FUNCTIONS
 *
 */

