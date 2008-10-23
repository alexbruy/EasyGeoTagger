/*
** File: egtexifioengine.cpp
** Author(s): Roberto Garcia-Yunta, Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2008-09-22
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
#include "egtexifioengine.h"

#include "egtpathbuilder.h"
#include "egtlogger.h"

#include <math.h>
#include <cassert>

#include <QFileInfo>
#include <QDir>


EgtExifIoEngine::EgtExifIoEngine()
{
  Exiv2::DataValue notvalid(Exiv2::invalidTypeId );
  cvNotValidValue=  notvalid;
  cvIsValidImage = false;
  cvHasGpsExif = false;
}

EgtExifIoEngine::EgtExifIoEngine( QString theImageFilename )
{
  EgtExifIoEngine();
  setFile( theImageFilename );
}

EgtExifIoEngine::EgtExifIoEngine( const QModelIndex& theIndex )
{
  EgtExifIoEngine();
  EgtPathBuilder cvPathBuilder;
  setFile( cvPathBuilder.buildPath( theIndex ) );
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */
/*!
 * \returns whether the image contains gps exif metadata or not
 */
bool EgtExifIoEngine::hasGpsExif()
{
  return cvHasGpsExif;
}

/*!
 * \returns whether the image is valid or not
 */
bool EgtExifIoEngine::isValidImage()
{
  return cvIsValidImage;
}

/*!
 * \returns A message with the last error occured
 */
QString EgtExifIoEngine::lastError() 
{ 
  return cvLastError; 
}

/*!
 * \returns the latitude of the current image
 */
double EgtExifIoEngine::latitude( bool * isValid )
{
  EgtDebug( "entered" );
  
  const Exiv2::Value& lvValue = read( "Exif.GPSInfo.GPSLatitudeRef" );
      
      
  Exiv2::TypeId lvTypeId = lvValue.typeId ();
  
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return 0.0;  
  } 

  int lvNorthing = 1;

  if( QString::compare( QString( lvValue.toString().c_str() ), "S" ) == 0 )
    lvNorthing = -1;
      
  const Exiv2::Value & lvValue2 = read( "Exif.GPSInfo.GPSLatitude" );
   
  lvTypeId = lvValue2.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return 0.0;  
  }  
  
  double lvLatitude = 0.0;
  
  for(int i=lvValue2.count()-1; i>=0; i--)
  {
    lvLatitude+= (double)lvValue2.toFloat(i);
    if(i>0)
      lvLatitude/= 60;
  }

  if(isValid)
      *isValid =true;
  
  return lvLatitude*lvNorthing;
}

/*!
 * \returns the longitude of the current image
 */
double EgtExifIoEngine::longitude( bool * isValid )
{  
  EgtDebug( "entered" );
 
  const Exiv2::Value& lvValue = read( "Exif.GPSInfo.GPSLongitudeRef" );   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();

  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return 0.0;  
  }

  int lvNorthing = 1;
  if(QString::compare( QString(lvValue.toString().c_str()), "W" ) == 0)
    lvNorthing = -1;
      
  const Exiv2::Value & lvValue2 = read( "Exif.GPSInfo.GPSLongitude" );
   
  lvTypeId = lvValue2.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return 0.0;  
  }
  
  double lvLongitude = 0.0;
  
  for(int i=lvValue2.count()-1; i>=0; i--)
  {
    lvLongitude+= (double)lvValue2.toFloat(i);
    if(i>0)
      lvLongitude/= 60;
  }

  if(isValid)
    *isValid = true;
 
  return lvLongitude*lvNorthing;
}

/*!
 * \param theImageFilename Absolute path to the image file that is going to be read/written
 */
void EgtExifIoEngine::setFile( QString theImageFilename )
{

  cvImageFileName = theImageFilename;
  cvIsValidImage = false;
  cvHasGpsExif = false;
  
  //If the file is a directory just bail no need to try an open it as an image
  QFileInfo lvFileToTest( theImageFilename );
  if( lvFileToTest.isDir() ) { return; }
  
  try 
  {
    //Try to open the image
    cvImage = Exiv2::ImageFactory::open( theImageFilename.toStdString() );
    assert( cvImage.get() != 0 );
    
    //Assert passed so we have an image
    cvIsValidImage = true;
    try 
    {
      //Read the metadata
      cvImage->readMetadata();
      
      //Search through the exif data looking for gps header
      //TODO: update this functionality, there has to be a better way to do this, if not make it a function
      QString lvKey;
      Exiv2::ExifData::const_iterator end = cvImage->exifData().end();
      for (Exiv2::ExifData::const_iterator i = cvImage->exifData().begin(); i != end; ++i)
      {
        lvKey = QString( i->key().c_str() );
        lvKey = lvKey.left(12);  //Exif.GPSInfo...
        if( QString::compare( lvKey, "Exif.GPSInfo" ,Qt::CaseInsensitive ) == 0 )
        {
	   cvHasGpsExif = true;
	   EgtDebug( "["+ theImageFilename +"] has gps exif" );
	   break;	
	}  
      }
    }
    catch ( Exiv2::AnyError& e )
    {
      cvLastError = QString( "Error caught ["+ QString( e.what() ) +"]" );
      EgtDebug( QString( "Error caught ["+ QString( e.what() ) +"]" ) );
    }
  }
  catch ( Exiv2::AnyError& e )
  {
    cvLastError = QString( "Unable to open file: "+ theImageFilename);
    EgtDebug( QString( "Error caught ["+ QString( e.what() ) +"]" ) );
  }
}

/*!
 * \param theValue Latitude represented as a double value, to be written in the exif metadata
 * \returns Whether the operation was successful or not
 */
bool EgtExifIoEngine::writeLatitude(double theValue)
{
  EgtDebug( "entered writeLatitude(double)" );
  bool ok; 
    
  if( theValue < 0 )
  {
    ok = write( "Exif.GPSInfo.GPSLatitudeRef", "S", "Ascii" );
    if( !ok )
    {
      cvLastError = QString( "Unable to write file: " + cvImageFileName);
      return false;
    }
  }
  else
  {
    ok = write( "Exif.GPSInfo.GPSLatitudeRef", "N", "Ascii" );
    if( !ok )
    {
      cvLastError = QString( "Unable to write file: " + cvImageFileName );
      return false;
    }
  }
  
  return write( "Exif.GPSInfo.GPSLatitude", convertToRational( QString::number( theValue, 'f', 7 ) ), "Rational" );
}

/*!
 * \param theValue QString representing the Latitude to be written in the exif metadata
 * \returns Whether the operation was successful or not
 */
bool EgtExifIoEngine::writeLatitude( QString theValue )
{
  EgtDebug( "entered writeLatitude(QString)" );
  bool ok; 
  double lvLatitude=theValue.toDouble( &ok );
  if( ok )
    return writeLatitude( lvLatitude );
  else 
    return false;
}


/*!
 * \param theValue Longitude represented as a double value, to be written in the exif metadata
 * \returns Whether the operation was successful or not
 */
bool EgtExifIoEngine::writeLongitude( double theValue )
{
  EgtDebug( "entered writeLatitude(double)" );
  bool ok; 

  if( theValue < 0 )
  {
    ok = write( "Exif.GPSInfo.GPSLatitudeRef", "S", "Ascii" );
    if( !ok )
    {
      cvLastError = QString( "Unable to write file: " + cvImageFileName );
      return false;
    }
  }
  else
  {
    ok = write( "Exif.GPSInfo.GPSLatitudeRef", "N", "Ascii" );
    if( !ok )
    {
      cvLastError = QString( "Unable to write file: " + cvImageFileName );
      return false;
    }
  }
  
  return write( "Exif.GPSInfo.GPSLongitude", convertToRational( QString::number( theValue, 'f', 7 ) ), "Rational" );
}


/*!
 * \param theValue QString representing the Longitude to be written in the exif metadata
 * \returns Whether the operation was successful or not
 */
bool EgtExifIoEngine::writeLongitude( QString theValue )
{
  EgtDebug( "entered writeLongitude(QString)" );
  bool ok; 
  double lvLongitude=theValue.toDouble( &ok );
  if( ok )
    return writeLongitude( lvLongitude );
  else 
    return false;
}


/*
 *
 * PRIVATE FUNCTIONS
 *
 */

/*!
 * \param theDegrees A QString that represents the decimal degrees to be converted to rational format
 * \returns A QString containing the rational value obtained
 */
QString EgtExifIoEngine::convertToRational(QString theDegrees)
{
	bool ok;

	double lvTheDegrees =fabs(theDegrees.toDouble(&ok));
	if(!ok)
		return QString( "" );
	
	double lvDegrees, lvMinutes, lvSeconds, lvAux;
	/*decomposes num into its integer and fractional parts.*/
	lvAux = modf(lvTheDegrees, &lvDegrees);
	/*lvAux contains the decimal part*/
	lvAux = modf(lvAux * 60, &lvMinutes);
	
	lvSeconds = lvAux * 60;
	
	double lvSecondsInt;
        lvAux = modf(lvSeconds*99999, &lvSecondsInt);
	
	QString lvTextDegrees, lvTextMinutes, lvTextSeconds;
	
	lvTextDegrees = lvTextDegrees.setNum(lvDegrees); 
	lvTextMinutes = lvTextMinutes.setNum(lvMinutes); 
	lvTextSeconds = lvTextSeconds.setNum((int)lvSecondsInt);
	
	return QString(lvTextDegrees + "/1" + " " +lvTextMinutes+ "/1"+ " " +lvTextSeconds+ "/99999");
}


/*!
 * \param theKey the key to be searched within the exif data
 * \returns the Value that has been readen from the exif data 
 */
const Exiv2::Value& EgtExifIoEngine::read(QString theKey)
{
  EgtDebug( "entered" );
  
  if( isValidImage() )
  {
    try 
    {
      EgtDebug( "Valid image, reading..." );
      cvImage->readMetadata();
      Exiv2::ExifKey lvKey( theKey.toStdString() );
      Exiv2::ExifData::iterator it = cvImage->exifData().findKey( lvKey );
      if( it == cvImage->exifData().end() )
      {
        cvLastError = QString( "key ["+ theKey + "] no found" );
        EgtDebug( QString( "key ["+ theKey + "] no found" ) );
        return cvNotValidValue;
      }

      EgtDebug( QString( "key ["+ theKey + "] found" ) );
      EgtDebug( QString( "Data Type ["+ QString(it->typeName()) +"]" ) );
      
      return it->value();
    }
    catch (Exiv2::AnyError& e)
    {
      cvLastError = QString( "Error caught ["+ QString( e.what() ) +"]" ) ;
      EgtDebug( QString( "Error caught ["+ QString( e.what() ) +"]" ) );
      return cvNotValidValue;
    }
  }
  
  EgtDebug( "Image is not valid" );
  return cvNotValidValue; 

}
/*!
 * \param theKey the key to be searched within the exif data
 * \returns A QString that represents the readen value 
 */
QString EgtExifIoEngine::readKeyValueAsString(QString theKey)
{
  EgtDebug( "entered" );
  if( isValidImage() )
  {
    try 
    {
      EgtDebug( "Valid image, reading..." );
      cvImage->readMetadata();
      Exiv2::ExifKey lvKey( theKey.toStdString() );
      Exiv2::ExifData::iterator it = cvImage->exifData().findKey( lvKey );
      if( it == cvImage->exifData().end() )
      {
        cvLastError = QString( "key ["+ theKey + "] no found" );
        EgtDebug( QString( "key ["+ theKey + "] no found" ) );
        return "";
      }

      EgtDebug( QString( "key ["+ theKey + "] found" ) );
      QString lvIteratorValue( it->value().toString().c_str() );
      return lvIteratorValue;
    }
    catch (Exiv2::AnyError& e)
    {
      cvLastError = QString( "Error caught ["+ QString( e.what() ) +"]" );
      EgtDebug( QString( "Error caught ["+ QString( e.what() ) +"]" ) );
      return "";
    }
  }
  
  EgtDebug( "Image is not valid" );
  return "";
}

/*!
 * \param theKey the key to be searched within the exif data
 * \param theString String containing the data to be written
 * \param theDefaultType The data type of what it is going to be written
 * \returns Whether the operation was succesful or not 
 */
bool EgtExifIoEngine::write( QString theKey, QString theString, QString theDefaultType )
{
  EgtDebug( "entered" );
  
  try 
  {
    cvImage->readMetadata();
    Exiv2::ExifData &exifData = cvImage->exifData();

    Exiv2::ExifKey key( theKey.toStdString() );

    QString lvTypeName = exifData[theKey.toStdString()].typeName(); 

    if( QString::compare( lvTypeName, "Invalid" ,Qt::CaseInsensitive ) == 0 ) 
      lvTypeName = theDefaultType;

    EgtDebug( QString( "Data type ["+ lvTypeName +"]" ) );
    
    Exiv2::ExifData::iterator pos = exifData.findKey(key);
    if (pos != exifData.end()) //If the data exist, we first delete it from the Exif data container
      exifData.erase(pos);


    if( QString::compare( lvTypeName, "Ascii" ,Qt::CaseInsensitive ) == 0 )
    {
     Exiv2::Value::AutoPtr lvNewValue = Exiv2::Value::create( Exiv2::asciiString );
     lvNewValue->read( theString.toStdString() );
     exifData.add( key, lvNewValue.get() );
    }
    else if( QString::compare( lvTypeName, "Rational",Qt::CaseInsensitive ) == 0 )
    {
     Exiv2::Value::AutoPtr lvNewValue = Exiv2::Value::create( Exiv2::unsignedRational );
     lvNewValue->read( theString.toStdString() );
     exifData.add( key, lvNewValue.get() );
    }
    else if( QString::compare( lvTypeName, "SRational",Qt::CaseInsensitive ) == 0 )
    {
     Exiv2::Value::AutoPtr lvNewValue = Exiv2::Value::create( Exiv2::signedRational );
     lvNewValue->read( theString.toStdString() );
     exifData.add( key, lvNewValue.get() );
    }
    else if( QString::compare( lvTypeName, "Short" ,Qt::CaseInsensitive ) == 0 )
    {
     Exiv2::Value::AutoPtr lvNewValue = Exiv2::Value::create( Exiv2::unsignedShort );
     lvNewValue->read( theString.toStdString() );
     exifData.add( key, lvNewValue.get() );
    }
    else if( QString::compare( lvTypeName, "SShort" ,Qt::CaseInsensitive ) == 0 )
    {
     Exiv2::Value::AutoPtr lvNewValue = Exiv2::Value::create( Exiv2::signedShort );
     lvNewValue->read( theString.toStdString() );
     exifData.add( key, lvNewValue.get() );
    }
    else if( QString::compare( lvTypeName, "Byte" ,Qt::CaseInsensitive ) == 0 )
    {
     Exiv2::Value::AutoPtr lvNewValue = Exiv2::Value::create( Exiv2::unsignedByte );
     lvNewValue->read( theString.toStdString() );
     exifData.add( key, lvNewValue.get() );
    }
    else if( QString::compare( lvTypeName, "SByte" ,Qt::CaseInsensitive ) == 0 )
    {
     Exiv2::Value::AutoPtr lvNewValue = Exiv2::Value::create( Exiv2::signedByte );
     lvNewValue->read( theString.toStdString() );
     exifData.add( key, lvNewValue.get() );
    }
    else if( QString::compare( lvTypeName, "Long" ,Qt::CaseInsensitive ) == 0)
    {
     Exiv2::Value::AutoPtr lvNewValue = Exiv2::Value::create( Exiv2::unsignedLong );
     lvNewValue->read( theString.toStdString() );
     exifData.add( key, lvNewValue.get() );
    }
    else if( QString::compare( lvTypeName, "SLong" ,Qt::CaseInsensitive ) == 0)
    {
     Exiv2::Value::AutoPtr lvNewValue = Exiv2::Value::create( Exiv2::signedLong  );
     lvNewValue->read( theString.toStdString() );
     exifData.add( key, lvNewValue.get() );
    }
    else //Undefined
    {
     Exiv2::Value::AutoPtr lvNewValue = Exiv2::Value::create( Exiv2::undefined );
     lvNewValue->read( theString.toStdString() );
     exifData.add( key, lvNewValue.get() );
    }
      
    // Writing the exif data to the image file
    cvImage->setExifData( exifData );
    cvImage->writeMetadata();
    return true;      
  }
  catch ( Exiv2::AnyError& e )
  {
    cvLastError = QString( "Unable to write to file: " + cvImageFileName);
    EgtDebug( QString( "Error caught ["+ QString( e.what() ) +"]" ));
    return false;
  }
}
