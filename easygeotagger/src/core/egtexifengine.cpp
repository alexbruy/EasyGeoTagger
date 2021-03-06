/*
** File: egtexifengine.cpp
** Author( s ): Roberto Garcia-Yunta, Peter J. Ersts ( ersts at amnh.org )
** Creation Date: 2008-09-22
**
** Copyright ( c ) 2008-2009, American Museum of Natural History. All rights reserved.
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
#include "egtexifengine.h"

#include "egtpathbuilder.h"
#include "egtlogger.h"

#include <math.h>
#include <cassert>

#include <QFileInfo>
#include <QDir>


EgtExifEngine::EgtExifEngine( )
{
  Exiv2::DataValue notvalid( Exiv2::invalidTypeId );
  cvNotValidValue = notvalid;
  cvIsValidImage = false;
  cvHasExpectedExif = false;
}

EgtExifEngine::EgtExifEngine( QString theImageFilename )
{
  EgtExifEngine( );
  setFile( theImageFilename );
}

EgtExifEngine::EgtExifEngine( const QModelIndex& theIndex )
{
  EgtExifEngine( );
  EgtPathBuilder cvPathBuilder;
  setFile( cvPathBuilder.buildPath( theIndex ) );
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */

QString EgtExifEngine::commonName( QString theKey )
{
  QList< KeyMap >::iterator lvIterator = cvKeys.begin( );
  while( lvIterator != cvKeys.end( ) )
  {
    if( lvIterator->key.compare( theKey, Qt::CaseInsensitive ) )
    {
      return lvIterator->commonName;
    }
    lvIterator++;
  }

  return "";
}

/*!
 * \param thePartialKey the EXIF key to search for, can be a full key are a left oriented partial key
 * \returns true if the partial key is found
 */
bool EgtExifEngine::hasKey( QString thePartialKey )
{
  if( cvIsValidImage )
  {
    try
    {
      //Read the metadata
      cvImage->readMetadata( );

      //TODO: update this functionality, there has to be a better way to do this
      QString lvKey;
      Exiv2::ExifData::const_iterator end = cvImage->exifData( ).end( );
      for ( Exiv2::ExifData::const_iterator i = cvImage->exifData( ).begin( ); i != end; ++i )
      {
        lvKey = QString( i->key( ).c_str( ) );
        lvKey = lvKey.left( thePartialKey.size( ) );
        if( QString::compare( lvKey, thePartialKey ,Qt::CaseInsensitive ) == 0 )
        {
          return true;
        }
      }
    }
    catch ( Exiv2::AnyError& e )
    {
      cvLastError = QString( "Error caught ["+ QString( e.what( ) ) +"]" );
      EgtDebug( QString( "Error caught ["+ QString( e.what( ) ) +"]" ) );
    }
  }

  return false;
}

/*!
 * \returns whether the image is valid or not
 */
bool EgtExifEngine::isValidImage( )
{
  return cvIsValidImage;
}

/*!
 * \param theCommonName the common name for which you want the key
 * \returns the key associated the theCommonName
 */
QString EgtExifEngine::key( QString theCommonName )
{
    QList< KeyMap >::iterator lvIterator = cvKeys.begin( );
    while( lvIterator != cvKeys.end( ) )
    {
        if( 0 == lvIterator->commonName.compare( theCommonName, Qt::CaseInsensitive ) )
        {
            return lvIterator->key;
        }
        lvIterator++;
    }

    return "";
}

/*!
 * \param theKey the EXIF key or tag name
 * \param isValid flag indicating the return data is valid
 * \returns the data associated with the key
 */
QVariant EgtExifEngine::read( QString theKey, bool* isValid )
{
  EgtDebug( theKey ); //SIP muffler

  if ( 0 != isValid ) { *isValid = false; }

  return QVariant( );
}


/*!
 * \param theKey the EXIF key to be searched within the exif data
 * \returns the Exiv2 Value that has been read from the exif data
 */
const Exiv2::Value& EgtExifEngine::readTag( QString theKey )
{
  EgtDebug( "entered" );
  
  if( isValidImage( ) )
  {
    try 
    {
      EgtDebug( "Valid image, reading..." );
      cvImage->readMetadata( );
      Exiv2::ExifKey lvKey( theKey.toStdString( ) );

      Exiv2::ExifData::iterator it = cvImage->exifData( ).findKey( lvKey );

      if( it == cvImage->exifData( ).end( ) )
      {
        cvLastError = QString( "key ["+ theKey + "] no found" );
        EgtDebug( QString( "key ["+ theKey + "] no found" ) );
        return cvNotValidValue;
      }

      EgtDebug( QString( "key ["+ theKey + "] found" ) );
      EgtDebug( QString( "Data Type ["+ QString( it->typeName( ) ) +"]" ) );
         
      if( 0 == it->count( ) ) { return cvNotValidValue; }     //The tag exists, but not the data ( it is corrupted or something )
      return it->value( );
    }
    catch ( Exiv2::AnyError& e )
    {
      cvLastError = QString( "Error caught ["+ QString( e.what( ) ) +"]" ) ;
      EgtDebug( QString( "Error caught ["+ QString( e.what( ) ) +"]" ) );
      return cvNotValidValue;
    }
  }
  
  EgtDebug( "Image is not valid" );
  return cvNotValidValue; 
}

/*!
 * \param theKey the EXIF key to be searched within the exif data
 * \returns A QString that represents the value read
 */
QString EgtExifEngine::readKeyValueAsString( QString theKey )
{
  EgtDebug( "entered" );
  if( isValidImage( ) )
  {
    try 
    {
      EgtDebug( "Valid image, reading..." );
      cvImage->readMetadata( );
      Exiv2::ExifKey lvKey( theKey.toStdString( ) );
      Exiv2::ExifData::iterator it = cvImage->exifData( ).findKey( lvKey );
      if( it == cvImage->exifData( ).end( ) )
      {
        cvLastError = QString( "key ["+ theKey + "] no found" );
        EgtDebug( QString( "key ["+ theKey + "] no found" ) );
        return "";
      }

      EgtDebug( QString( "key ["+ theKey + "] found" ) );
      if( 0 == it->count( ) ) { return ""; }     //The tag exists, but not the data ( it is corrupted or something )
      QString lvIteratorValue( it->value( ).toString( ).c_str( ) );
      return lvIteratorValue;
    }
    catch ( Exiv2::AnyError& e )
    {
      cvLastError = QString( "Error caught ["+ QString( e.what( ) ) +"]" );
      EgtDebug( QString( "Error caught ["+ QString( e.what( ) ) +"]" ) );
      return "";
    }
  }
  
  EgtDebug( "Image is not valid" );
  return "";
}

/*!
 * \param theFileName Absolute path to the image file that is going to be read/written
 */
void EgtExifEngine::setFile( QString theFileName )
{
  openFile( theFileName );
  cvHasExpectedExif = true; //set true in base class because we can access any all/tag
}

/*!
 * \param theKey the EXIF key to be written to
 * \param theString String containing the data to be written
 * \param theDefaultType The data type of what it is going to be written
 * \returns Whether the operation was succesful or not 
 */
bool EgtExifEngine::writeTag( QString theKey, QString theString, QString theDefaultType )
{
  EgtDebug( "entered" );
  
  try 
  {
    cvImage->readMetadata( );
    Exiv2::ExifData &exifData = cvImage->exifData( );

    Exiv2::ExifKey key( theKey.toStdString( ) );

    QString lvTypeName = exifData[theKey.toStdString( )].typeName( ); 

    if( QString::compare( lvTypeName, "Invalid" ,Qt::CaseInsensitive ) == 0 ) 
    {
      lvTypeName = theDefaultType;
    }

    EgtDebug( QString( "Data type ["+ lvTypeName +"]" ) );
    
    Exiv2::ExifData::iterator pos = exifData.findKey( key );
    if ( pos != exifData.end( ) ) //If the data exist, we first delete it from the Exif data container
    {
      exifData.erase( pos );
    }

    if( !theString.isEmpty( ) )
    {
      if( QString::compare( lvTypeName, "Ascii" ,Qt::CaseInsensitive ) == 0 )
      {
       Exiv2::Value::AutoPtr lvNewValue = Exiv2::Value::create( Exiv2::asciiString );
       lvNewValue->read( theString.toStdString( ) );
       exifData.add( key, lvNewValue.get( ) );
      }
      else if( QString::compare( lvTypeName, "Rational",Qt::CaseInsensitive ) == 0 )
      {
       Exiv2::Value::AutoPtr lvNewValue = Exiv2::Value::create( Exiv2::unsignedRational );
       lvNewValue->read( theString.toStdString( ) );
       exifData.add( key, lvNewValue.get( ) );
      }
      else if( QString::compare( lvTypeName, "SRational",Qt::CaseInsensitive ) == 0 )
      {
       Exiv2::Value::AutoPtr lvNewValue = Exiv2::Value::create( Exiv2::signedRational );
       lvNewValue->read( theString.toStdString( ) );
       exifData.add( key, lvNewValue.get( ) );
      }
      else if( QString::compare( lvTypeName, "Short" ,Qt::CaseInsensitive ) == 0 )
      {
       Exiv2::Value::AutoPtr lvNewValue = Exiv2::Value::create( Exiv2::unsignedShort );
       lvNewValue->read( theString.toStdString( ) );
       exifData.add( key, lvNewValue.get( ) );
      }
      else if( QString::compare( lvTypeName, "SShort" ,Qt::CaseInsensitive ) == 0 )
      {
       Exiv2::Value::AutoPtr lvNewValue = Exiv2::Value::create( Exiv2::signedShort );
       lvNewValue->read( theString.toStdString( ) );
       exifData.add( key, lvNewValue.get( ) );
      }
      else if( QString::compare( lvTypeName, "Byte" ,Qt::CaseInsensitive ) == 0 )
      {
       Exiv2::Value::AutoPtr lvNewValue = Exiv2::Value::create( Exiv2::unsignedByte );
       lvNewValue->read( theString.toStdString( ) );
       exifData.add( key, lvNewValue.get( ) );
      }
      else if( QString::compare( lvTypeName, "SByte" ,Qt::CaseInsensitive ) == 0 )
      {
       Exiv2::Value::AutoPtr lvNewValue = Exiv2::Value::create( Exiv2::signedByte );
       lvNewValue->read( theString.toStdString( ) );
       exifData.add( key, lvNewValue.get( ) );
      }
      else if( QString::compare( lvTypeName, "Long" ,Qt::CaseInsensitive ) == 0 )
      {
       Exiv2::Value::AutoPtr lvNewValue = Exiv2::Value::create( Exiv2::unsignedLong );
       lvNewValue->read( theString.toStdString( ) );
       exifData.add( key, lvNewValue.get( ) );
      }
      else if( QString::compare( lvTypeName, "SLong" ,Qt::CaseInsensitive ) == 0 )
      {
       Exiv2::Value::AutoPtr lvNewValue = Exiv2::Value::create( Exiv2::signedLong );
       lvNewValue->read( theString.toStdString( ) );
       exifData.add( key, lvNewValue.get( ) );
      }
      else //Undefined
      {
       Exiv2::Value::AutoPtr lvNewValue = Exiv2::Value::create( Exiv2::undefined );
       lvNewValue->read( theString.toStdString( ) );
       exifData.add( key, lvNewValue.get( ) );
      }
    }
    // Writing the exif data to the image file
    cvImage->setExifData( exifData );
    cvImage->writeMetadata( );
    return true;      
  }
  catch ( Exiv2::AnyError& e )
  {
    cvLastError = QString( "Unable to write to file: " + cvImageFileName );
    EgtDebug( QString( "Error caught ["+ QString( e.what( ) ) +"]" ) );
    return false;
  }
}

/*
 *
 * PROTECTED FUNCTIONS
 *
 */

/*!
 * \param theKey the Egt key name
 * \param theCommonName the name that should be used to display to the user
 * \param theUnitString a pipe delimited string of units ( e.g., meeters feet ) assoicated with the key
 */
void EgtExifEngine::addKey( QString theKey, QString theCommonName, QString theUnitString )
{
  KeyMap lvMap;
  lvMap.key = theKey;
  lvMap.commonName = theCommonName;
  if( "" == theUnitString )
  {
    lvMap.units = QStringList( );
  }
  else
  {
    lvMap.units = theUnitString.split( "|" );
  }
  cvKeys.append( lvMap );
}

/*!
 * \param theFileName the absolute path and file name of the image to open
 */
void EgtExifEngine::openFile( QString theFileName )
{
  cvImageFileName = theFileName;

  //Reset the flags
  cvIsValidImage = false;
  cvHasExpectedExif = false;

  //If the file is a directory just bail no need to try an open it as an image
  QFileInfo lvFileToTest( theFileName );

  if( lvFileToTest.isDir( ) ) { return; }

  try
  {
    //Try to open the image
    cvImage = Exiv2::ImageFactory::open( theFileName.toStdString( ) );
    assert( cvImage.get( ) != 0 );

    //Assert passed so we have a valid image
    cvIsValidImage = true;
  }
  catch ( Exiv2::AnyError& e )
  {
    cvLastError = QString( "Unable to open file: "+ theFileName );
    EgtDebug( QString( "Error caught ["+ QString( e.what( ) ) +"]" ) );
  }
}
