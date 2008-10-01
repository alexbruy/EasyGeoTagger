/*
** File: egtexifio.cpp
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
#include "egtexifio.h"
#include "egtexifio.h"

#include "egtlogger.h"
#include <math.h>

#include <cassert>

#include <QFileInfo>
#include <QDir>


EgtExifIO::EgtExifIO( QString theImageFilename )
{
  setFile( theImageFilename );
}

EgtExifIO::EgtExifIO( const QModelIndex& theIndex )
{
  setFile( buildPath( theIndex ) );
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */

float EgtExifIO::getLatitude()
{
  EgtDebug( "entered getLatitude()" );
  
  const Exiv2::Value& lvValue = read( "Exif.GPSInfo.GPSLatitudeRef" );
        
      
  Exiv2::TypeId lvTypeId = lvValue.typeId ();
  
  if(lvTypeId == Exiv2::invalidTypeId)
    return 0.0;  

  int lvNorthing = 1;

  if( QString::compare( QString( lvValue.toString().c_str() ), "S" ) == 0 )
    lvNorthing = -1;
      
  const Exiv2::Value & lvValue2 = read( "Exif.GPSInfo.GPSLatitude" );
   
  lvTypeId = lvValue2.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
    return 0.0;  
  
  float lvLatitude = 0.0;
  
  for(int i=lvValue2.count()-1; i>=0; i--)
  {
  	lvLatitude+= lvValue2.toFloat(i);
  	if(i>0)
  		lvLatitude/= 60;
  }
  return lvLatitude*lvNorthing;
}


float EgtExifIO::getLongitude()
{  
  EgtDebug( "entered getLongitude()" );
  
  const Exiv2::Value& lvValue = read( "Exif.GPSInfo.GPSLongitudeRef" );   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();

  if(lvTypeId == Exiv2::invalidTypeId)
    return 0.0;  

  int lvNorthing = 1;
  if(QString::compare( QString(lvValue.toString().c_str()), "W" ) == 0)
    lvNorthing = -1;
      
  const Exiv2::Value & lvValue2 = read( "Exif.GPSInfo.GPSLongitude" );
   
  lvTypeId = lvValue2.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
    return 0.0;  
  
  float lvLongitude = 0.0;
  
  for(int i=lvValue2.count()-1; i>=0; i--)
  {
  	lvLongitude+= lvValue2.toFloat(i);
  	if(i>0)
  		lvLongitude/= 60;
  }
  return lvLongitude*lvNorthing;
}

bool EgtExifIO::hasGpsExif()
{
  return cvHasGpsExif;
}

bool EgtExifIO::isValidImage()
{
  return cvIsValidImage;
}

void EgtExifIO::setFile( QString theImageFilename )
{
  EgtDebug( "entered setFile()" );
  cvImageFile = theImageFilename;
  cvIsValidImage = false;
  cvHasGpsExif = false;
  try 
  {
    cvImage = Exiv2::ImageFactory::open( theImageFilename.toStdString() );
    assert( cvImage.get() != 0 );
    cvIsValidImage = true;
    try 
    {
      cvImage->readMetadata();
      
      //Exiv2::ExifData::iterator it = cvImage->exifData().findIfdIdIdx( Exiv2::gpsIfdId, 1 );
      //if( it != cvImage->exifData().end() )
      //{
      //  cvHasGpsExif = true;
      //}
      QString lvKey;

      Exiv2::ExifData::const_iterator end = cvImage->exifData().end();
      for (Exiv2::ExifData::const_iterator i = cvImage->exifData().begin(); i != end; ++i)
      {
        lvKey = QString( i->key().c_str() );
        lvKey = lvKey.left(12);  //Exif.GPSInfo...
        if( QString::compare( lvKey, "Exif.GPSInfo" ,Qt::CaseInsensitive ) == 0 )
        {
	   			cvHasGpsExif = true;
	  			EgtDebug( "it has gps exif" );
	   			break;	
				}
      }
    }
    catch ( Exiv2::AnyError& e )
    {
      EgtDebug( QString( "Error caught ["+ QString( e.what() ) +"]" ) );
    }
  }
  catch ( Exiv2::AnyError& e )
  {
    EgtDebug( QString( "Error caught ["+ QString( e.what() ) +"]" ) );
  }
}

bool EgtExifIO::setLongitude( QString theValue )
{
  EgtDebug( "entered setLongitude()" );
	bool ok;
	float lvLongitude=theValue.toFloat( &ok );
	if( !ok )
		return false;
	if( lvLongitude < 0 )
	{
		ok = write( "Exif.GPSInfo.GPSLongitudeRef", "W", "Ascii" );
		if( !ok )
			return false;
	}
	else
	{
		ok = write( "Exif.GPSInfo.GPSLongitudeRef", "E", "Ascii" );
		if( !ok )
			return false;
	}
	
	return write( "Exif.GPSInfo.GPSLongitude", convertToRational(theValue), "Rational" );
}


bool EgtExifIO::setLatitude( QString theValue )
{
	EgtDebug( "entered setlatitude()" );
  bool ok; 
	float lvLongitude=theValue.toFloat( &ok );
	if( !ok )
		return false;
		
	if( lvLongitude < 0 )
	{
		ok = write( "Exif.GPSInfo.GPSLatitudeRef", "S", "Ascii" );
		if( !ok )
			return false;
	}
	else
	{
		ok = write( "Exif.GPSInfo.GPSLatitudeRef", "N", "Ascii" );
		if( !ok )
			return false;
	}
	
  return write( "Exif.GPSInfo.GPSLatitude", convertToRational(theValue), "Rational" );
}

bool EgtExifIO::write( QString theKey, QString theString, QString theDefaultType )
{
  EgtDebug( "entered write" );
  
  try 
  {
    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(cvImageFile.toStdString());
    assert (image.get() != 0);
    image->readMetadata();
    Exiv2::ExifData &exifData = image->exifData();

    Exiv2::ExifKey key( theKey.toStdString() );

    QString lvTypeName = exifData[theKey.toStdString()].typeName(); 

		if(QString::compare( lvTypeName, "Invalid" ,Qt::CaseInsensitive ) == 0 ) 
			lvTypeName = theDefaultType;

    EgtDebug( QString( "Data type ["+ lvTypeName +"]" ) );
    
    Exiv2::ExifData::iterator pos = exifData.findKey(key);
    if (pos != exifData.end()) //If the data exist, we first delete it from the Exif data container
			exifData.erase(pos);


    if( QString::compare( lvTypeName, "Ascii" ,Qt::CaseInsensitive ) == 0 )
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create( Exiv2::asciiString );
     rv->read( theString.toStdString() );
     exifData.add( key, rv.get() );
    }
    else if( QString::compare( lvTypeName, "Rational",Qt::CaseInsensitive ) == 0 )
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create( Exiv2::unsignedRational );
     rv->read( theString.toStdString() );
     exifData.add( key, rv.get() );
    }
    else if( QString::compare( lvTypeName, "SRational",Qt::CaseInsensitive ) == 0 )
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create( Exiv2::signedRational );
     rv->read( theString.toStdString() );
     exifData.add( key, rv.get() );
    }
    else if( QString::compare( lvTypeName, "Short" ,Qt::CaseInsensitive ) == 0 )
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create( Exiv2::unsignedShort );
     rv->read( theString.toStdString() );
     exifData.add( key, rv.get() );
    }
    else if( QString::compare( lvTypeName, "SShort" ,Qt::CaseInsensitive ) == 0 )
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create( Exiv2::signedShort );
     rv->read( theString.toStdString() );
     exifData.add( key, rv.get() );
    }
    else if( QString::compare( lvTypeName, "Byte" ,Qt::CaseInsensitive ) == 0 )
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create( Exiv2::unsignedByte );
     rv->read( theString.toStdString() );
     exifData.add( key, rv.get() );
    }
    else if( QString::compare( lvTypeName, "SByte" ,Qt::CaseInsensitive ) == 0 )
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create( Exiv2::signedByte );
     rv->read( theString.toStdString() );
     exifData.add( key, rv.get() );
    }
    else if( QString::compare( lvTypeName, "Long" ,Qt::CaseInsensitive ) == 0)
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create( Exiv2::unsignedLong );
     rv->read( theString.toStdString() );
     exifData.add( key, rv.get() );
    }
    else if( QString::compare( lvTypeName, "SLong" ,Qt::CaseInsensitive ) == 0)
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create( Exiv2::signedLong  );
     rv->read( theString.toStdString() );
     exifData.add( key, rv.get() );
    }
    else //Undefined
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create( Exiv2::undefined );
     rv->read( theString.toStdString() );
     exifData.add( key, rv.get() );
    }
      
    // Writing the exif data to the image file
    image->setExifData( exifData );
    image->writeMetadata();
    return true;      
  }
  catch ( Exiv2::AnyError& e )
  {
    EgtDebug( QString( "Error caught ["+ QString( e.what() ) +"]" ));
    return false;
  }
}

/*
 *
 * PRIVATE FUNCTIONS
 *
 */
QString EgtExifIO::buildPath(const QModelIndex& theIndex)
{
  if(!theIndex.isValid()) { return ""; }
  
  if(!theIndex.parent().isValid())
  {
    #ifdef WIN32
    return theIndex.data().toString();
    #else
    return "";
    #endif
  }
  
  return buildPath( theIndex.parent() ) + QDir::toNativeSeparators( "/" ) + theIndex.data().toString();
}

QString EgtExifIO::convertToRational(QString theDegrees)
{
	EgtDebug( "entered read()" );
	
	const int lvInitialPrecision = 1000000;
	bool ok;

	double lvTheDegrees =fabs(theDegrees.toDouble(&ok));
	if(!ok)
		return QString( "" );
	
	double lvDegrees, lvMinutes, lvSeconds, lvAux;
	/*decomposes num into its integer and fractional parts.*/
	lvAux = modf(lvTheDegrees, &lvDegrees);
	
	lvAux = modf(lvAux * 60, &lvMinutes);
	
	lvSeconds = lvAux * 60;
	
	int lvDegreesInt = round(lvDegrees * lvInitialPrecision);
	int lvMinutesInt = round(lvMinutes * lvInitialPrecision);
	int lvSecondsInt = round(lvSeconds * lvInitialPrecision);
	
	int lvDegreesPrecision = lvInitialPrecision;
	int lvMinutesPrecision = lvInitialPrecision;
	int lvSecondsPrecision = lvInitialPrecision;
	
	while (lvDegreesInt % 10 == 0)
	{
		lvDegreesInt/= 10;
		lvDegreesPrecision/= 10;
	}
	
	while (lvMinutesInt % 10 == 0)
	{
		lvMinutesInt/= 10;
		lvMinutesPrecision/= 10;
	}
	
	while (lvSecondsInt % 10 == 0)
	{
		lvSecondsInt/= 10;
		lvSecondsPrecision/= 10;
	}
	
	QString lvTextDegrees, lvTextDegreesPrecision, lvTextMinutes, lvTextMinutesPrecision;
	QString lvTextSeconds, lvTextSecondsPrecision;
	
	lvTextDegrees = lvTextDegrees.setNum(lvDegreesInt); 
	lvTextDegreesPrecision = lvTextDegreesPrecision.setNum(lvDegreesPrecision); 
	lvTextMinutes = lvTextMinutes.setNum(lvMinutesInt); 
	lvTextMinutesPrecision = lvTextMinutesPrecision.setNum(lvMinutesPrecision);
	lvTextSeconds = lvTextSeconds.setNum(lvSecondsInt);
	lvTextSecondsPrecision = lvTextSecondsPrecision.setNum(lvSecondsPrecision);
	
	return QString(lvTextDegrees + "/" +lvTextDegreesPrecision+ " " +lvTextMinutes+ "/" +lvTextMinutesPrecision+ " " +lvTextSeconds+ "/" +lvTextSecondsPrecision);
}


const Exiv2::Value& EgtExifIO::read(QString theKey)
{
  EgtDebug( "entered read()" );
  
  Exiv2::DataValue lvNotValidValue(Exiv2::invalidTypeId );
  Exiv2::Value& lvNotValid = lvNotValidValue;
  Exiv2::TypeId lvTypeId = lvNotValidValue.typeId ();

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
        EgtDebug( QString( "key ["+ theKey + "] no found" ) );
        return lvNotValid;
      }

      EgtDebug( QString( "key ["+ theKey + "] found" ) );
      EgtDebug( QString( "Data Type ["+ QString(it->typeName()) +"]" ) );
      
      return it->value();
    }
    catch (Exiv2::AnyError& e)
    {
      EgtDebug( QString( "Error caught ["+ QString( e.what() ) +"]" ) );
      return lvNotValid;
    }
  }
  
  EgtDebug( "Image is not valid" );
  return lvNotValid; 

}


QString EgtExifIO::readKeyValueAsString(QString theKey)
{
  EgtDebug( "entered readKeyValueAsString()" );
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
        EgtDebug( QString( "key ["+ theKey + "] no found" ) );
        return "";
      }

      EgtDebug( QString( "key ["+ theKey + "] found" ) );
      QString lvIteratorValue( it->value().toString().c_str() );
      return lvIteratorValue;
    }
    catch (Exiv2::AnyError& e)
    {
      EgtDebug( QString( "Error caught ["+ QString( e.what() ) +"]" ) );
      return "";
    }
  }
  
  EgtDebug( "Image is not valid" );
  return "";
}

float EgtExifIO::tokenizeCoordinate(QString theString)
{
  EgtDebug( "entered tokenizeCoordinate()" );
  int lvValue1 = 0;
  bool lvValue1Done = false;
  int lvValue2 = 0;
  bool lvNumberFound = false;
  float lvCoordinate = 0.0;
  bool lvFirstRationalDone = false;

  for(int i=0; i<theString.length(); i++)
  {
    if( theString[i].digitValue()!= -1 )
    {
      if(!lvValue1Done)
      {
        lvNumberFound = true;
        lvValue1=theString[i].digitValue()+lvValue1*10;
      }
      else
      {
        lvValue2=theString[i].digitValue()+lvValue2*10;
      }   
    }
    
    else if( theString[i] =='/' )
    {
      lvValue1Done = true;
    }
    
    else if( theString[i] == ' ' && lvNumberFound )
    {
      lvCoordinate = lvCoordinate + lvValue1/lvValue2;
      if(lvFirstRationalDone)
      {
        lvCoordinate = lvCoordinate / 60.0;
      }
      lvFirstRationalDone = true;
      lvValue1 = 0;
      lvValue2 = 0;
      lvValue1Done = false;
    }
  }

  lvCoordinate = lvCoordinate + lvValue1/lvValue2;
  lvCoordinate = lvCoordinate / 60.0;
          
  return lvCoordinate;
}