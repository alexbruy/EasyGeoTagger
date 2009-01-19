/*
** File: 
** Author(s): 
** Creation Date: 
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
 * \param theButton pointer to a QPushButton that is to be connect to the  showConfigureationPanel slot
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

QStringList EgtDelimitedTextFileReader::columnHeaders()
{
  QStringList lvList;
  return lvList;
}

bool EgtDelimitedTextFileReader::hasColumnHeaders()
{
  return false; 
}

/*!
 * \param theFileName A
 */
void EgtDelimitedTextFileReader::setFileName( QString theFileName )
{

qDebug(cvFileName.toStdString().c_str());
  cvFileName = theFileName;
}

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

