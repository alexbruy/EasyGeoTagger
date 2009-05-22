/*
** File: egtgpsprovider.cpp
** Author( s ): Roberto Garcia Yunta
** Creation Date: 2009-05-22
**
** Copyright ( c ) 2009, American Museum of Natural History. All rights reserved.
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
#include "egtgpsprovider.h"

#include "egtgpsbabelinterface.h"


#include "egtgpxparser.h"
#include "egtlogger.h"

#include <QtPlugin>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QMessageBox>
#include <QTemporaryFile>
#include <QFileDialog>
#include <QLibrary>

EgtGpsProvider::EgtGpsProvider( ) : EgtDataProvider( )
{
  cvFileName = ""; 
  cvFileType ="";
  cvLastError = "";


#ifdef WIN32
#  ifdef OSGEO4W
    QLibrary lvLibrary( "/../apps/easygt/easygpsbabel.dll" ); //TODO: find out the right path
#  else
     QLibrary lvLibrary( "/plugins/easygpsbabel.dll");
#  endif
#else
   QLibrary lvLibrary( "/usr/local/lib/easygt/easygpsbabel" );
#endif

 cvConvert = (ConvertFunction) lvLibrary.resolve("convert");

}



/*
 *
 * PUBLIC FUNCTIONS
 *
 */


void EgtGpsProvider::notifyInitializationComplete( bool isComplete )
{ 
  EgtDebug( "entered" );

  if( !isComplete )
  {
    cvData.clear();
  }
  emit dataProviderReady();
}


/*!
 * \param theFileName a QString that contains the name of the file to be read
 */
EgtDataProvider::ErrorType EgtGpsProvider::setFileName( QString theFileName )
{
  EgtDebug( "entered" );

  cvFileName = theFileName;
  return read();
}

/*
 *
 * PROTECTED FUNCTIONS
 *
 */
EgtDataProvider::ErrorType EgtGpsProvider::read( )
{
  EgtDebug( "entered" );

  cvData.clear();
  cvLastError = "";
  cvHasColumnHeaders = true; /*A GPX file always has headers*/

  QFile lvFile( cvFileName );
  QStringList lvStringList;

  QTemporaryFile lvErrorFile;
  QTemporaryFile lvInfoFile;
  QTemporaryFile lvTempFile;

  int lvErrorCode = 0;

  if ( !lvFile.open( QFile::ReadOnly | QFile::Text ) ) 
  {
    cvLastError = QObject::tr( "Can't read the file" ) + ": " + cvFileName;
    return EgtDataProvider::Fatal;
  }

  if( lvErrorFile.open() && lvInfoFile.open() && lvTempFile.open() && cvFileType != "gpx" )
  {
    if( cvConvert )
    {
      lvErrorCode = cvConvert(cvFileName.toStdString().c_str(), cvFileType.toStdString().c_str(), lvTempFile.fileName().toStdString().c_str(), "gpx", lvErrorFile.fileName().toStdString().c_str(),
        lvInfoFile.fileName().toStdString().c_str());
    }
    else
    {
      cvLastError = QObject::tr( "Couldn't load gpsbabel library" ); 
      return EgtDataProvider::Fatal;
    }
  }

  switch( lvErrorCode )
  {
    case -1:
    { 
      cvLastError = QObject::tr( "Error converting with gpsbabel" ) + ": " + cvFileName +"\n"; 

      QTextStream lvInput(&lvErrorFile);
      while ( !lvInput.atEnd() )
      {
        QString lvLine = lvInput.readLine();
        cvLastError+= lvLine;
        cvLastError+= "\n";
      }
      return EgtDataProvider::Fatal;
    }
    break;
    case 0:{}
    break;
    /*TODO: Create more error codes if needed and handle them*/
  }

  EgtGpxParser lvHandler;

  if( cvFileType != "gpx" )
  {
    QXmlInputSource lvXmlSource( &lvTempFile );

    QXmlSimpleReader lvReader;
    lvReader.setContentHandler( &lvHandler );

    lvReader.parse( lvXmlSource );
  }
  else //we just use as input the original file
  {
    QXmlInputSource lvXmlSource( &lvFile );

    QXmlSimpleReader lvReader;
    lvReader.setContentHandler( &lvHandler );

    lvReader.parse( lvXmlSource );
  }

  cvData = lvHandler.data();
  cvNumberOfFields= lvHandler.numberOfFields();

  cvColumnHeaders = lvHandler.headers();

  return EgtDataProvider::None;
}
