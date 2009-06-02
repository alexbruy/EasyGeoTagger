/*
** File: egtgpsprovider.cpp
** Author( s ): Roberto Garcia Yunta, Peter J. Ersts ( ersts at amnh.org )
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

#include "egtlogger.h"

#include <QtPlugin>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QMessageBox>
#include <QCoreApplication>
#include <QFileDialog>
#include <QLibrary>

EgtGpsProvider::EgtGpsProvider( ) : EgtDataProvider( )
{
  cvFileName = ""; 
  cvFileType ="";
  cvLastError = "";
  cvFeatureType = EgtGpxParser::Tracks;


#ifdef WIN32
#  ifdef OSGEO4W
    QLibrary lvLibrary( QCoreApplication::instance( )->applicationDirPath( ) + "/../apps/easygt/plugins/easygpsbabel.dll" ); //TODO: find out the right path
#  else
     QLibrary lvLibrary( QCoreApplication::instance( )->applicationDirPath( ) + "/plugins/easygpsbabel.dll");
#  endif
#else
   QLibrary lvLibrary( QCoreApplication::instance( )->applicationDirPath( ) + "/../lib/easygt/libgpsbabel" );
#endif

 cvConvert = (ConvertFunction) lvLibrary.resolve("convertWithOptions");

}



/*
 *
 * PUBLIC FUNCTIONS
 *
 */

/*!
 * \param theFileName a QString that contains the name of the file to be read
 */
void EgtGpsProvider::setFileName( QString theFileName )
{
  EgtDebug( "entered" );

  cvFileName = theFileName;
}

/*
 *
 * PROTECTED FUNCTIONS
 *
 */
EgtDataProvider::ErrorType EgtGpsProvider::convert( QString theFileToConvert, QString theConvertedFile, QString& theConvertedFileName)
{
  EgtDebug( "entered" );

  //TODO: This should actually look at the contents of the file to make sure it is a GPX file
  if( cvFileType == "gpx" )
  {
    theConvertedFileName = theFileToConvert;
    return EgtDataProvider::None;
  }

  QTemporaryFile lvErrorFile;
  QTemporaryFile lvInfoFile;

  int lvErrorCode = 0;

  if( lvErrorFile.open() && lvInfoFile.open() )
  {
    if( cvConvert )
    {
      lvErrorCode = cvConvert( theFileToConvert.toStdString().c_str(), cvFileType.toStdString().c_str(), theConvertedFile.toStdString().c_str(), "gpx", lvErrorFile.fileName().toStdString().c_str(),
        lvInfoFile.fileName().toStdString().c_str(),QString("").toStdString().c_str() ); 

      theConvertedFileName = theConvertedFile;
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

      QTextStream lvInput( &lvErrorFile );
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
  return EgtDataProvider::None;
}

EgtDataProvider::ErrorType EgtGpsProvider::read( )
{
  EgtDebug( "entered" );

  cvValid = false;
  cvData.clear();
  cvColumnHeaders.clear();
  cvLastError = "";
  
  QString lvOutputFile;

  if( cvConvertTempFile.open() )
  {
    EgtDataProvider::ErrorType lvErrorCode = convert( cvFileName, cvConvertTempFile.fileName(), lvOutputFile );

    if( EgtDataProvider::None != lvErrorCode ){ return lvErrorCode; }
  }
  else
  {
    cvLastError = QObject::tr( "Can't open temporary file" );
    return EgtDataProvider::Fatal;
  }

  return readGpx( lvOutputFile );
}

EgtDataProvider::ErrorType EgtGpsProvider::readGpx( QString theGpxFile )
{
  EgtDebug( "entered" );

  EgtGpxParser lvParser;
  cvData = lvParser.parse( cvFeatureType, theGpxFile );
  cvColumnHeaders << lvParser.headers();
  cvNumberOfFields= lvParser.headers().size();

  cvValid = true;
  return EgtDataProvider::None;
}
