/*
** File: 
** Author( s ): 
** Creation Date:
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
#include "egtdelimitedtextfilereader.h"

#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QMessageBox>

EgtDelimitedTextFileReader::EgtDelimitedTextFileReader( ):EgtFileReader( )
{
  cvFileName = ""; 
  cvLastError = "";
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */
/*!
 * \return a QString that contains the column headers
 */
QStringList EgtDelimitedTextFileReader::columnHeaders( )
{
  return cvColumnHeaders;
}



/*!
 * \param ok pointer to boolean that indicates whether the read was performed correctly or not
 */
QList<QStringList> EgtDelimitedTextFileReader::read( bool* ok )
{
  QFile lvFile( cvFileName );
  QStringList lvStringList;
  QList<QStringList>  lvList;

  bool lvError = false;

  int lvNumFields;

  if ( !lvFile.open( QFile::ReadOnly | QFile::Text ) ) 
  {
    if ( ok ) { *ok = false; }
    cvLastError = QObject::tr( "Can't read the file" ) + ": " + cvFileName;
    return lvList;
  }



  EgtGpxParser handler;


  QXmlInputSource source( lvFile );

  QXmlSimpleReader lvReader;
  lvReader.setContentHandler( &handler );

 lvReader.parse( source );

  return handler.data();

}

/*!
 * \param theFileName a QString that contains the name of the file to be read
 */
void EgtDelimitedTextFileReader::setFileName( QString theFileName )
{
  cvFileName = theFileName;
}


