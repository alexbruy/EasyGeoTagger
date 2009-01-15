/*
** File: egtphotoexifengine.cpp
** Author(s): Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2009-01-13
**
** Copyright (c) 2009, American Museum of Natural History. All rights reserved.
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
#include "egtphotoexifengine.h"
#include "egtlogger.h"

#include <QObject>

EgtPhotoExifEngine::EgtPhotoExifEngine() : EgtExifEngine()
{
    addKey("Egt.Photo.DateTimeOriginal", QObject::tr( "Dates Time" ) );
}

EgtPhotoExifEngine::EgtPhotoExifEngine( QString theImageFilename ) : EgtExifEngine( theImageFilename )
{
    EgtPhotoExifEngine();
}

EgtPhotoExifEngine::EgtPhotoExifEngine( const QModelIndex& theIndex ) : EgtExifEngine( theIndex )
{
    EgtPhotoExifEngine();
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */
QString EgtPhotoExifEngine::dateTimeOriginal( bool* isValid )
{
  EgtDebug( "entered dateTimeOriginal(bool)" );

  if( 0 != isValid ) { *isValid =false; }
  if( !isValidImageWithExpectedExif() ) { return ""; }

  const Exiv2::Value &lvValue = readTag( "Exif.Photo.DateTimeOriginal" );
  if( lvValue.typeId() == Exiv2::invalidTypeId )
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return "";
  }

  if( 0 != isValid ) { *isValid = true; }
  return QString( lvValue.toString( 0 ).c_str() );
}

QVariant EgtPhotoExifEngine::read( QString theTag, bool* isValid )
{
  EgtDebug( "entered" );

  if( 0 != isValid ) { *isValid = false; }
  if( !isValidImageWithExpectedExif() ) { return QVariant( "" ); }

  if( QString::compare( theTag, "Egt.Photo.DateTimeOriginal", Qt::CaseInsensitive ) == 0)
  {
    return QVariant( dateTimeOriginal( isValid ) );
  }

  return QVariant( "" );
}

/*!
 * \param theFileName Absolute path to the image file that is going to be read/written
 */
void EgtPhotoExifEngine::setFile( QString theFileName )
{
  EgtDebug( "entered" );

  openFile( theFileName );
  cvHasExpectedExif = hasKey( "Exif.Photo" );
}

bool EgtPhotoExifEngine::write( QString theTag, QString theValue )
{
  EgtDebug( "entered" );

  if( !isValidImage() ) { return false; }

  if( QString::compare( theTag, "Egt.Photo.DateTimeOriginal", Qt::CaseInsensitive ) == 0)
  {
    return writeDateTimeOriginal( theValue );
  }

  return false;
}

bool EgtPhotoExifEngine::writeDateTimeOriginal( QString theValue )
{
  EgtDebug( "entered" );
  if( !isValidImage() ) { return false; }

  //Check format
  if( theValue.size() != 19 )
  {
    cvLastError = "[ writeDateTimeOriginal ] "+ QObject::tr( "unexpected format" );
    return false;
  }
  bool lvExpectedFormat = true;
  lvExpectedFormat = lvExpectedFormat && ':' == theValue[4];
  lvExpectedFormat = lvExpectedFormat && ':' == theValue[7];
  lvExpectedFormat = lvExpectedFormat && ' ' == theValue[10];
  lvExpectedFormat = lvExpectedFormat && ':' == theValue[13];
  lvExpectedFormat = lvExpectedFormat && ':' == theValue[16];
  if( !lvExpectedFormat )
  {
    cvLastError = "[ writeDateTimeOriginal ] "+ QObject::tr( "unexpected format" );
    return false;
  }
  for( int lvIterator = 0; lvIterator < theValue.size(); lvIterator++ )
  {
    if( !theValue[ lvIterator ].isNumber() && ':' != theValue[ lvIterator ] && ' ' != theValue[ lvIterator ] )
    {
      cvLastError = "[ writeDateTimeOriginal ] "+ QObject::tr( "unexpected characters found in datetime stamp" );
      return false;
    }
  }

  //Format passed, try to write tag
  if( !writeTag( "Exif.Photo.DateTimeOriginal", theValue, "Ascii" ) )
  {
    return false;
  }

  //Write passed so set the has expected exif flag and return true
  cvHasExpectedExif = true;
  return true;
}
