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

#include "egtpathbuilder.h"
#include "egtlogger.h"

#include <math.h>
#include <cassert>

#include <QFileInfo>
#include <QDir>


EgtExifIO::EgtExifIO()
{
  Exiv2::DataValue notvalid(Exiv2::invalidTypeId );
  cvNotValidValue=  notvalid;
  cvIsValidImage = false;
  cvHasGpsExif = false;
}

EgtExifIO::EgtExifIO( QString theImageFilename )
{
  EgtExifIO();
  setFile( theImageFilename );
}

EgtExifIO::EgtExifIO( const QModelIndex& theIndex )
{
  EgtExifIO();
  EgtPathBuilder cvPathBuilder;
  setFile( cvPathBuilder.buildPath( theIndex ) );
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */
bool EgtExifIO::hasGpsExif()
{
  return cvHasGpsExif;
}

bool EgtExifIO::isValidImage()
{
  return cvIsValidImage;
}

QString EgtExifIO::lastError() 
{ 
  return cvLastError; 
}

double EgtExifIO::latitude()
{
  EgtDebug( "entered" );
  
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
  
  double lvLatitude = 0.0;
  
  for(int i=lvValue2.count()-1; i>=0; i--)
  {
    lvLatitude+= (double)lvValue2.toFloat(i);
    if(i>0)
      lvLatitude/= 60;
  }
  return lvLatitude*lvNorthing;
}

double EgtExifIO::longitude()
{  
  EgtDebug( "entered" );
  
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
  
  double lvLongitude = 0.0;
  
  for(int i=lvValue2.count()-1; i>=0; i--)
  {
    lvLongitude+= (double)lvValue2.toFloat(i);
    if(i>0)
      lvLongitude/= 60;
  }
  return lvLongitude*lvNorthing;
}

void EgtExifIO::setFile( QString theImageFilename )
{

  cvImageFile = theImageFilename;
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
      EgtDebug( QString( "Error caught ["+ QString( e.what() ) +"]" ) );
    }
  }
  catch ( Exiv2::AnyError& e )
  {
    cvLastError = QString( "Unable to open file: "+ theImageFilename);
    EgtDebug( QString( "Error caught ["+ QString( e.what() ) +"]" ) );
  }
}


bool EgtExifIO::writeLatitude(double theValue)
{
  EgtDebug( "entered writeLatitude(double)" );
  bool ok; 

  if( !ok )
  {
    cvLastError = QString( "Latitude is not in the correct format: "+ QString::number(theValue));
    return false;
  }
    
  if( theValue < 0 )
  {
    ok = write( "Exif.GPSInfo.GPSLatitudeRef", "S", "Ascii" );
    if( !ok )
    {
      cvLastError = QString( "Unable to write file: " + cvImageFile);
      return false;
    }
  }
  else
  {
    ok = write( "Exif.GPSInfo.GPSLatitudeRef", "N", "Ascii" );
    if( !ok )
    {
      cvLastError = QString( "Unable to write file: " + cvImageFile );
      return false;
    }
  }
  
  return write( "Exif.GPSInfo.GPSLatitude", convertToRational( QString::number( theValue ) ), "Rational" );
}


bool EgtExifIO::writeLatitude( QString theValue )
{
  EgtDebug( "entered writeLatitude(QString)" );
  bool ok; 
  double lvLatitude=theValue.toDouble( &ok );
  if( ok )
    return writeLatitude( lvLatitude );
  else 
    return false;
}

bool EgtExifIO::writeLongitude( double theValue )
{
  EgtDebug( "entered writeLatitude(double)" );
  bool ok; 

  if( !ok )
  {
    cvLastError = QString( "Longitude is not in the correct format: "+ QString::number( theValue ) );
    return false;
  }
    
  if( theValue < 0 )
  {
    ok = write( "Exif.GPSInfo.GPSLatitudeRef", "S", "Ascii" );
    if( !ok )
    {
      cvLastError = QString( "Unable to write file: " + cvImageFile );
      return false;
    }
  }
  else
  {
    ok = write( "Exif.GPSInfo.GPSLatitudeRef", "N", "Ascii" );
    if( !ok )
    {
      cvLastError = QString( "Unable to write file: " + cvImageFile );
      return false;
    }
  }
  
  return write( "Exif.GPSInfo.GPSLongitude", convertToRational( QString::number( theValue ) ), "Rational" );
}

bool EgtExifIO::writeLongitude( QString theValue )
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
QString EgtExifIO::convertToRational(QString theDegrees)
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

const Exiv2::Value& EgtExifIO::read(QString theKey)
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
        EgtDebug( QString( "key ["+ theKey + "] no found" ) );
        return cvNotValidValue;
      }

      EgtDebug( QString( "key ["+ theKey + "] found" ) );
      EgtDebug( QString( "Data Type ["+ QString(it->typeName()) +"]" ) );
      
      return it->value();
    }
    catch (Exiv2::AnyError& e)
    {
      EgtDebug( QString( "Error caught ["+ QString( e.what() ) +"]" ) );
      return cvNotValidValue;
    }
  }
  
  EgtDebug( "Image is not valid" );
  return cvNotValidValue; 

}

QString EgtExifIO::readKeyValueAsString(QString theKey)
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
  EgtDebug( "entered" );
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

bool EgtExifIO::write( QString theKey, QString theString, QString theDefaultType )
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
    cvLastError = QString( "Unable to write to file: " + cvImageFile);
    EgtDebug( QString( "Error caught ["+ QString( e.what() ) +"]" ));
    return false;
  }
}
