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
 * \param isValid if the access to the picture was successful
 * \returns the altitude where the picture was taken
 */
double EgtExifIoEngine::altitude( bool * isValid )
{
  EgtDebug( "entered altitude(bool)" );
  
  const Exiv2::Value& lvValue = read( "Exif.GPSInfo.GPSAltitudeRef" );
      
      
  Exiv2::TypeId lvTypeId = lvValue.typeId ();
  
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return 0.0;  
  } 

  int lvSeaLevel = 1;

  if( QString::compare( QString( lvValue.toString().c_str() ), "1" ) == 0 )
    lvSeaLevel = -1;
      
  const Exiv2::Value & lvValue2 = read( "Exif.GPSInfo.GPSAltitude" );
   
  lvTypeId = lvValue2.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return 0.0;  
  }  
  
  double lvAltitude = 0.0;

  for(int i=lvValue2.count()-1; i>=0; i--)
  {
    lvAltitude+= (double)lvValue2.toFloat(i);
  }

  if(isValid)
      *isValid =true;
  
  return lvAltitude*lvSeaLevel;
}

/*!
 * \param isValid if the access to the picture was successful
 * \returns A QString recording the name of the GPS area. The first byte indicates the character code used, and this is 
 * followed by the name of the GPS area.
 */
QString EgtExifIoEngine::areaInformation( bool * isValid )
{
  EgtDebug( "entered areaInformation(bool)" );

  const Exiv2::Value & lvValue = read( "Exif.GPSInfo.GPSAreaInformation" );
   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return "";  
  }  
  
  QString lvAreaInformation;
  lvAreaInformation= QString(lvValue.toString(0).c_str());
  

  if(isValid)
      *isValid =true;
  
  return lvAreaInformation;
}

/*!
 * \param isValid if the access to the picture was successful
 * \returns A Qstring recording date and time information relative to UTC (Coordinated Universal Time). The format is
 * "YYYY:MM:DD.".
 */
QString EgtExifIoEngine::dateStamp( bool * isValid )
{
  EgtDebug( "entered dateStamp(bool)" );

  const Exiv2::Value & lvValue = read( "Exif.GPSInfo.GPSDateStamp" );
   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return "";  
  }  
  
  QString lvDateStamp;
  lvDateStamp= QString(lvValue.toString(0).c_str());
  

  if(isValid)
      *isValid =true;
  
  return lvDateStamp;
}

/*!
 * \param isValid if the access to the picture was successful
 * \param theRefference a char pointer containing the units of the measure (Kilometers, miles or knots)
 * \returns The bearing to the destination point. The range of values is from 0.00 to 359.99.
 */
double EgtExifIoEngine::destBearing( char * theRefference, bool * isValid )
{
  EgtDebug( "entered destBearing(bool)" );
  
  const Exiv2::Value& lvValue = read( "Exif.GPSInfo.GPSDestBearingRef" );
      
  Exiv2::TypeId lvTypeId = lvValue.typeId ();
  
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return 0.0;  
  } 

  if( QString::compare( QString( lvValue.toString().c_str() ), "M" ) == 0 )
    *theRefference = 'M';
  else
    *theRefference = 'T';
      
  const Exiv2::Value & lvValue2 = read( "Exif.GPSInfo.GPSDestBearing" );
   
  lvTypeId = lvValue2.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return 0.0;  
  }  
  
  double lvBearing = (double)lvValue2.toFloat(0);

  if(isValid)
      *isValid =true;
  
  return lvBearing;
}

/*!
 * \param isValid if the access to the picture was successful
 * \returns whether differential correction is applied to the GPS receiver.
 */
int EgtExifIoEngine::differential( bool * isValid )
{
  EgtDebug( "entered differential(bool)" );

  const Exiv2::Value & lvValue = read( "Exif.GPSInfo.GPSDifferential" );
   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return -1;  
  }  
  
  int lvDifferential = int(lvValue.toFloat(0));
  
  if(isValid)
      *isValid =true;
  
  return lvDifferential;
}

/*!
 * \param isValid if the access to the picture was successful
 * \returns the direction of the image when it was captured. The range of values is from 0.00 to 359.99.
 */
float EgtExifIoEngine::direction( char * theRefference, bool * isValid )
{
  EgtDebug( "entered direction(char, bool)" );
  
  const Exiv2::Value& lvValue = read( "Exif.GPSInfo.GPSImgDirectionRef" );
      
  Exiv2::TypeId lvTypeId = lvValue.typeId ();
  
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return 0.0;  
  } 

  if( QString::compare( QString( lvValue.toString().c_str() ), "M" ) == 0 )
    *theRefference = 'M';
  else
    *theRefference = 'T';
      
  const Exiv2::Value & lvValue2 = read( "Exif.GPSInfo.GPSImgDirection" );
   
  lvTypeId = lvValue2.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return 0.0;  
  }  
  
  int lvDirection = int(lvValue2.toFloat(0));
  
  if(isValid)
      *isValid =true;
  
  return lvDirection;
}

/*!
 * \param isValid if the access to the picture was successful
 * \returns the distance to the destination point
 */
double EgtExifIoEngine::destDistance( char * theRefference, bool * isValid )
{
  EgtDebug( "entered destDistance(char, bool)" );
  
  const Exiv2::Value& lvValue = read( "Exif.GPSInfo.GPSDestDistanceRef" );
      
  Exiv2::TypeId lvTypeId = lvValue.typeId ();
  
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return 0.0;  
  } 

  if( QString::compare( QString( lvValue.toString().c_str() ), "N" ) == 0 )
    *theRefference = 'N';
  else
    if( QString::compare( QString( lvValue.toString().c_str() ), "M" ) == 0 )
      *theRefference = 'M';
    else
      *theRefference = 'K';
      
  const Exiv2::Value & lvValue2 = read( "Exif.GPSInfo.GPSDestDistance" );
   
  lvTypeId = lvValue2.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return 0.0;  
  }  
  
  double lvDistance = double(lvValue2.toFloat(0));
  
  if(isValid)
      *isValid =true;
  
  return lvDistance;
}

/*!
 * \param isValid if the access to the picture was successful
 * \returns the latitude of the destination point
 */
double EgtExifIoEngine::destLatitude( bool * isValid )
{
  EgtDebug( "entered destLatitude()" );
 
  const Exiv2::Value& lvValue = read( "Exif.GPSInfo.GPSDestLatitudeRef" );   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();

  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return 0.0;  
  }

  int lvNorthing = 1;
  if(QString::compare( QString(lvValue.toString().c_str()), "S" ) == 0)
    lvNorthing = -1;
      
  const Exiv2::Value & lvValue2 = read( "Exif.GPSInfo.GPSDestLatitude" );
   
  lvTypeId = lvValue2.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return 0.0;  
  }
  
  double lvDestLatitude = 0.0;
  
  for(int i=lvValue2.count()-1; i>=0; i--)
  {
    lvDestLatitude+= (double)lvValue2.toFloat(i);
    if(i>0)
      lvDestLatitude/= 60;
  }

  if(isValid)
    *isValid = true;
 
  return lvDestLatitude*lvNorthing;
}

/*!
 * \param isValid if the access to the picture was successful
 * \returns the longitude of the destination point
 */
double EgtExifIoEngine::destLongitude( bool * isValid )
{
  EgtDebug( "entered destLongitude()" );
 
  const Exiv2::Value& lvValue = read( "Exif.GPSInfo.GPSDestLongitudeRef" );   
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
      
  const Exiv2::Value & lvValue2 = read( "Exif.GPSInfo.GPSDestLongitude" );
   
  lvTypeId = lvValue2.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return 0.0;  
  }
  
  double lvDestLongitude = 0.0;
  
  for(int i=lvValue2.count()-1; i>=0; i--)
  {
    lvDestLongitude+= (double)lvValue2.toFloat(i);
    if(i>0)
      lvDestLongitude/= 60;
  }

  if(isValid)
    *isValid = true;
 
  return lvDestLongitude*lvNorthing;
}

/*!
 * \param isValid if the access to the picture was successful
 * \returns the GPS DOP (data degree of precision). An HDOP value is written during two-dimensional measurement, and PDOP during
 * three-dimensional measurement.
 */
double EgtExifIoEngine::gpsDOP( bool * isValid )
{
  EgtDebug( "entered gpsDOP(bool)" );

  const Exiv2::Value & lvValue = read( "Exif.GPSInfo.GPSDOP" );
   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return 0.0;  
  }  
  
  double lvDop = double(lvValue.toFloat(0));
  
  if(isValid)
      *isValid =true;
  
  return lvDop;
}

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
 *\param a boolean that indicates whether the operation was successful or not.
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
 *\param isValid if the access to the picture was successful
 * \returns the geodetic survey data used by the GPS receiver. If the survey data is restricted to Japan, the value of this tag is
 * "TOKYO" or "WGS-84".
 */
QString EgtExifIoEngine::mapDatum( bool * isValid )
{
  EgtDebug( "entered mapDatum(bool)" );

  const Exiv2::Value & lvValue = read( "Exif.GPSInfo.GPSMapDatum" );
   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return "";  
  }  

  if(isValid)
      *isValid =true;
  
  return QString(lvValue.toString(0).c_str());
}

/*!
 *\param isValid if the access to the picture was successful
 *\returns the GPS measurement mode. "2" means two-dimensional measurement and "3" means three-dimensional measurement is in
 * progress.
 */
QString EgtExifIoEngine::measureMode( bool * isValid )
{
  EgtDebug( "entered measureMode(bool)" );

  const Exiv2::Value & lvValue = read( "Exif.GPSInfo.GPSMeasureMode" );
   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return "";  
  }  
  
  if(isValid)
      *isValid =true;
  
  return QString(lvValue.toString(0).c_str());
}

/*!
 *\param isValid if the access to the picture was successful
 *\returns A character string recording the name of the method used for location finding. The first byte indicates the character
 * code used, and this is followed by the name of the method.
 */
QString EgtExifIoEngine::processingMethod( bool * isValid )
{
  EgtDebug( "entered processingMethod(bool)" );

  const Exiv2::Value & lvValue = read( "Exif.GPSInfo.GPSProcessingMethod" );
   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return "";  
  }  
  
  if(isValid)
      *isValid =true;
  
  return QString(lvValue.toString(0).c_str());
}

/*!
 *\param isValid if the access to the picture was successful
 *\returns the GPS satellites used for measurements. This tag can be used to describe the number of satellites, their ID number,
 * angle of elevation, azimuth, SNR and other information in ASCII notation. The format is not specified. If the GPS receiver is
 * incapable of taking measurements, value of the tag is set to NULL.
 */
QString EgtExifIoEngine::satellites( bool * isValid )
{
  EgtDebug( "entered satellites(bool)" );

  const Exiv2::Value & lvValue = read( "Exif.GPSInfo.GPSSatellites" );
   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return "";  
  }  
  
  if(isValid)
      *isValid =true;
  
  return QString(lvValue.toString(0).c_str());
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
 *\param isValid if the access to the picture was successful
 *\param theUnit A char pointer that indicates the unit of the measurement(Kilometers, Miles or Knots)
 *\returns the GPS satellites used for measurements. This tag can be used to describe the number of satellites, their ID number,
 * angle of elevation, azimuth, SNR and other information in ASCII notation. The format is not specified. If the GPS receiver is
 * incapable of taking measurements, value of the tag is set to NULL.
 */
double EgtExifIoEngine::speed( char *theUnit, bool * isValid )
{
  EgtDebug( "entered speed(char, bool)" );
  
  const Exiv2::Value& lvValue = read( "Exif.GPSInfo.GPSSpeedRef" );
      
  Exiv2::TypeId lvTypeId = lvValue.typeId ();
  
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return 0.0;  
  } 

  if( QString::compare( QString( lvValue.toString().c_str() ), "N" ) == 0 )
    *theUnit = 'N';
  else
    if( QString::compare( QString( lvValue.toString().c_str() ), "M" ) == 0 )
      *theUnit = 'M';
    else
      *theUnit = 'K';
      
  const Exiv2::Value & lvValue2 = read( "Exif.GPSInfo.GPSSpeed" );
   
  lvTypeId = lvValue2.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return 0.0;  
  }  

  if(isValid)
      *isValid =true;
  
  return double(lvValue2.toFloat(0));
}

/*!
 *\param isValid if the access to the picture was successful
 *\returns the status of the GPS receiver when the image is recorded. "A" means measurement is in progress, and "V" means the
 * measurement is Interoperability.
 */
QString EgtExifIoEngine::status( bool * isValid )
{
  EgtDebug( "entered satellites(bool)" );

  const Exiv2::Value & lvValue = read( "Exif.GPSInfo.GPSStatus" );
   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return "";  
  }  
  
  if(isValid)
      *isValid =true;
  
  return QString(lvValue.toString(0).c_str());
}

/*!
 *\param isValid if the access to the picture was successful
 *\returns the timStamp
 */
QString EgtExifIoEngine::timeStamp( bool * isValid )
{
  EgtDebug( "entered timeStamp(bool)" );
 
  const Exiv2::Value& lvValue = read( "Exif.GPSInfo.GPSTimeStamp" );   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();

  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return "";  
  }

    QString lvTimeStamp = QString(QString::number(int(lvValue.toFloat(0))) +":"+ QString::number(int(lvValue.toFloat(1))) +":"+ QString::number(int(lvValue.toFloat(2))) );

  if(isValid)
    *isValid = true;
 
  return lvTimeStamp;
}

/*!
 *\param isValid if the access to the picture was successful
 *\returns the direction of GPS receiver movement. The range of values is from 0.00 to 359.99.
 */
double EgtExifIoEngine::track(char * theRefference, bool * isValid )
{
  EgtDebug( "entered track(char, bool)" );
  
  const Exiv2::Value& lvValue = read( "Exif.GPSInfo.GPSTrackRef" );
      
  Exiv2::TypeId lvTypeId = lvValue.typeId ();
  
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return 0.0;  
  } 

  if( QString::compare( QString( lvValue.toString().c_str() ), "M" ) == 0 )
    *theRefference = 'M';
  else
    *theRefference = 'T';
      
  const Exiv2::Value & lvValue2 = read( "Exif.GPSInfo.GPSTrack" );
   
  lvTypeId = lvValue2.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return 0.0;  
  }  
  
  double lvDirection = double(lvValue2.toFloat(0));
  
  if(isValid)
      *isValid =true;
  
  return lvDirection;
}

/*!
 *\param isValid if the access to the picture was successful
 *\returns Indicates the version of <GPSInfoIFD>. The version is given as 2.0.0.0. This tag is mandatory when <GPSInfo> tag is
 * present. (Note: The <GPSVersionID> tag is given in bytes, unlike the <ExifVersion> tag. When the version is 2.0.0.0, the tag
 * value is 02000000.H).
 */
int EgtExifIoEngine::versionID( bool * isValid )
{
  EgtDebug( "entered versionID(bool)" );

  const Exiv2::Value & lvValue = read( "Exif.GPSInfo.GPSVersionID" );
   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QString( "Unable to read exif data from file: " + cvImageFileName );
    if(isValid)
      *isValid =false;
    return -1;  
  }  
  
  int lvDifferential = int(lvValue.toFloat(0));
  
  if(isValid)
      *isValid =true;
  
  return lvDifferential;
}

/*!
 *\param theValue a double containing the altitude to be written to the exif metadata
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtExifIoEngine::writeAltitude( double theValue )
{
  EgtDebug( "entered writeAltitude(double)" );
  bool ok; 
    
  if( theValue < 0 )
  {
    ok = write( "Exif.GPSInfo.GPSAltitudeRef", "1", "Byte" );
  }
  else
  {
    ok = write( "Exif.GPSInfo.GPSAltitudeRef", "0", "Byte" );
  }
  
  if( !ok )
  {
    cvLastError = QString( "Unable to write file: " + cvImageFileName );
    return false;
  }  

  double lvValueInt;
  modf( theValue*99999, &lvValueInt );
  return write( "Exif.GPSInfo.GPSAltitude", QString( QString::number( (int)fabs( lvValueInt ) )+"/99999" ), "Rational" );
}

/*!
 *\param theValue a double containing the area information to be written to the exif metadata
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtExifIoEngine::writeAreaInformation( QString theValue )
{
  return write( "Exif.GPSInfo.GPSAreaInformation", theValue, "Undefined" );
}

/*!
 *\param theValue a double containing the date stamp to be written to the exif metadata.The format is "YYYY:MM:DD"
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtExifIoEngine::writeDateStamp( QString theValue )
{
  return write( "Exif.GPSInfo.GPSDateStamp", theValue, "Ascii" );
}

/*!
 *\param theValue a float containing the bearing of the destination point to be written to the exif metadata
 *\param theRefference a char that indicates the refference(geographic north pole(T) or magnetic noth pole(M))
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtExifIoEngine::writeDestBearing( float theValue, char theRefference )
{
  EgtDebug( "entered writeDestBearing(float)" );
  bool ok; 
    
  if( theRefference=='M' )
  {
    ok = write( "Exif.GPSInfo.GPSDestBearingRef", "M", "Ascii" );
    
  }
  else
  {
    ok = write( "Exif.GPSInfo.GPSDestBearingRef", "T", "Ascii" );
  }

  if( !ok )
  {
    cvLastError = QString( "Unable to write file: " + cvImageFileName);
    return false;
  }
 
  double lvValueInt;
  modf( theValue*99999, &lvValueInt );
  return write( "Exif.GPSInfo.GPSDestBearing", QString( QString::number( (int)fabs( lvValueInt ) )+"/99999" ), "Rational" );
}

/*!
 *\param theValue an integer containing the differential correction to be written to the exif metadata
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtExifIoEngine::writeDifferential( int theValue )
{
  return write( "Exif.GPSInfo.GPSDifferential", QString( QString::number( theValue ) ), "Short" );
}

/*!
 *\param theValue a float containing the direction of the gps receiver to be written to the exif metadata
 *\param theRefference a char that indicates the refference(geographic north pole(T) or magnetic noth pole(M))
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtExifIoEngine::writeDirection( float theValue, char theRefference )
{
  EgtDebug( "entered writeDirection(float)" );
  bool ok; 
    
  if( theRefference=='M' )
  {
    ok = write( "Exif.GPSInfo.GPSImgDirectionRef", "M", "Ascii" );
  }
  else
  {
    ok = write( "Exif.GPSInfo.GPSImgDirectionRef", "T", "Ascii" );
  }
 
  if( !ok )
  {
    cvLastError = QString( "Unable to write file: " + cvImageFileName);
    return false;
  }

  double lvValueInt;
  modf( theValue*99999, &lvValueInt );
  return write( "Exif.GPSInfo.GPSImgDirection", QString( QString::number( (int)fabs( lvValueInt ) )+"/99999" ), "Rational" );
}

/*!
 *\param theValue a double containing the distance to the destination point to be written to the exif metadata
 *\param theUnit a char that indicates the unit used(Kilometers, Miles or Knots)
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtExifIoEngine::writeDestDistance( double theValue, char theUnit )
{
  EgtDebug( "entered :writeDestDistance(double)" );
  bool ok; 
  
  switch( theUnit )
  {
    case 'M':
    {
      ok = write( "Exif.GPSInfo.GPSDestDistanceRef", "M", "Ascii" );
    }
    break;
    case 'N':
    {
      ok = write( "Exif.GPSInfo.GPSDestDistanceRef", "N", "Ascii" );
    }
    break;
    case 'K':
    {
      ok = write( "Exif.GPSInfo.GPSDestDistanceRef", "K", "Ascii" );
    }
    break;
    default:
    {
      ok = write( "Exif.GPSInfo.GPSDestDistanceRef", "K", "Ascii" );
    }
    break;
  }
  
  if( !ok )
  {
    cvLastError = QString( "Unable to write file: " + cvImageFileName );
    return false;
  }
    
  double lvValueInt;
  modf( theValue*99999, &lvValueInt );
  return write( "Exif.GPSInfo.GPSDestDistance", QString( QString::number( (int)fabs( lvValueInt ) )+"/99999" ), "Rational" );
}

/*!
 *\param theValue a double containing the latitude of the destination point to be written to the exif metadata
 *\param theRefference a char that indicates the refference(geographic north pole(T) or magnetic noth pole(M))
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtExifIoEngine::writeDestLatitude( double theValue, char theRefference )
{
  EgtDebug( "entered writeDestLatitude(double)" );
  bool ok; 
    
  if( theRefference=='N' )
  {
    ok = write( "Exif.GPSInfo.GPSDestLatitudeRef", "N", "Ascii" );
  }
  else
  {
    ok = write( "Exif.GPSInfo.GPSDestLatitudeRef", "S", "Ascii" );
  }
 
  if( !ok )
  {
    cvLastError = QString( "Unable to write file: " + cvImageFileName);
    return false;
  }

  double lvValueInt;
  modf( theValue*99999, &lvValueInt );
  return write( "Exif.GPSInfo.GPSDestLatitude", QString( QString::number( (int)fabs( lvValueInt ) )+"/99999" ), "Rational" );
}

/*!
 *\param theValue a double containing the longitude of the destination point to be written to the exif metadata
 *\param theRefference a char that indicates the refference(geographic north pole(T) or magnetic noth pole(M))
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtExifIoEngine::writeDestLongitude( double theValue, char theRefference )
{
  EgtDebug( "entered writeDestLongitude(double)" );
  bool ok; 
    
  if( theRefference=='N' )
  {
    ok = write( "Exif.GPSInfo.GPSDestLongitudeRef", "N", "Ascii" );
  }
  else
  {
    ok = write( "Exif.GPSInfo.GPSDestLongitudeRef", "S", "Ascii" );
  }
 
  if( !ok )
  {
    cvLastError = QString( "Unable to write file: " + cvImageFileName);
    return false;
  }

  double lvValueInt;
  modf( theValue*99999, &lvValueInt );
  return write( "Exif.GPSInfo.GPSDestLongitude", QString( QString::number( (int)fabs( lvValueInt ) )+"/99999" ), "Rational" );
}

/*!
 *\param theValue a double containing the GPS DOP to be written to the exif metadata
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtExifIoEngine::writeGpsDOP( double theValue )
{
  double lvValueInt;
  modf( theValue*99999, &lvValueInt );
  return write( "Exif.GPSInfo.GPSDOP", QString( QString::number( (int)fabs( lvValueInt ) )+"/99999" ), "Rational" );
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

/*!
 *\param theValue a QString containing the geodetic survey data used by the GPS receiver to be written to the exif metadata
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtExifIoEngine::writeMapDatum( QString theValue )
{
  return write( "Exif.GPSInfo.GPSMapDatum", theValue, "Ascii" );
}

/*!
 *\param theValue a QString containing the measure mode to be written to the exif metadata
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtExifIoEngine::writeMeasureMode( QString theValue)
{
  return write( "Exif.GPSInfo.GPSMeasureMode", theValue, "Ascii" );
}

/*!
 *\param theValue a QString containing the processing method to be written to the exif metadata
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtExifIoEngine::writeProcessingMethod( QString theValue)
{
  return write( "Exif.GPSInfo.GPSProcessingMethod", theValue, "Undefined" );
}

/*!
 *\param theValue a QString containing the satellites used for measurements to be written to the exif metadata
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtExifIoEngine::writeSatellites( QString theValue )
{
  return write( "Exif.GPSInfo.GPSSatellites", theValue, "Ascii" );
}

/*!
 *\param theValue a QString containing the speed of the gps receiver to be written to the exif metadata
  *\param theUnit a char that indicates the units of the measurement(Kilometers, Miles or Knots)
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtExifIoEngine::writeSpeed( double theValue, char theUnit )
{
  
  EgtDebug( "entered :writeSpeed(double)" );
  bool ok; 
  
  switch( theUnit )
  {
    case 'M':
    {
      ok = write( "Exif.GPSInfo.GPSSpeedRef", "M", "Ascii" );
    }
    break;
    case 'N':
    {
      ok = write( "Exif.GPSInfo.GPSSpeedRef", "N", "Ascii" );
    }
    break;
    case 'K':
    {
      ok = write( "Exif.GPSInfo.GPSSpeedRef", "K", "Ascii" );
    }
    break;
    default:
    {
      ok = write( "Exif.GPSInfo.GPSSpeedRef", "K", "Ascii" );
    }
    break;
  }
  
  if( !ok )
  {
    cvLastError = QString( "Unable to write file: " + cvImageFileName );
    return false;
  }
    
  double lvValueInt;
  modf( theValue*99999, &lvValueInt );
  return write( "Exif.GPSInfo.GPSDestDistance", QString( QString::number( (int)fabs( lvValueInt ) )+"/99999" ), "Rational" );
}
/*!
 *\param theValue a QString containing the status of the GPS receiver to be written to the exif metadata
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtExifIoEngine::writeStatus( QString theValue )
{
  return write( "Exif.GPSInfo.GPSStatus", theValue, "Ascii" );
}

/*!
 *\param theValue a QString containing the time stamp to be written to the exif metadata. the format is ""hh:mm:ss"
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtExifIoEngine::writeTimeStamp( QString theString )
{
  EgtDebug( "entered writeTimeStamp(QString)" );
  int lvValue1 = 0;
  bool lvValue1Done = false;
  int lvValue2 = 0;
  bool lvValue2Done = false;
  int lvValue3 = 0;
  int lvValue = 0;

  for(int i=0; i<theString.length(); i++)
  {
    if( theString[i].digitValue()!= -1 )
      lvValue=theString[i].digitValue()+lvValue*10;
    
    else if( theString[i] ==':' )
    {
      if( !lvValue1Done && !lvValue2Done )
      {
        lvValue1Done = true;
        lvValue1 = lvValue;
        lvValue = 0;
      }
      else
        if( lvValue1Done && !lvValue2Done )
        {
          lvValue2Done = true;
          lvValue2 = lvValue;
          lvValue = 0;
        }
    }
  }

 lvValue3 =lvValue;
          
 return write( "Exif.GPSInfo.GPSTimeStamp", QString( QString::number( lvValue1 ) +"/1"+ QString::number( lvValue2 ) +"/1"+ QString::number( lvValue3 )) +"/1", "Rational" );
}

/*!
 *\param theValue a float containing the direction of the GPS movement to be written to the exif metadata
 *\param theRefference a char that indicates what refference was used (Geographical norht pole or magnetical north pole)
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtExifIoEngine::writeTrack( float theValue, char theRefference )
{
  EgtDebug( "entered writeTrack(float)" );
  bool ok; 
    
  if( theRefference=='M' )
  {
    ok = write( "Exif.Exif.GPSInfo.GPSTrackRef", "M", "Ascii" );
  }
  else
  {
    ok = write( "Exif.GPSInfo.GPSTrackRef", "T", "Ascii" );
  }
 
  if( !ok )
  {
    cvLastError = QString( "Unable to write file: " + cvImageFileName);
    return false;
  }

  double lvValueInt;
  modf( theValue*99999, &lvValueInt );
  return write( "Exif.GPSInfo.GPSTrack", QString( QString::number( (int)fabs( lvValueInt ) )+"/99999" ), "Rational" );
}

/*!
 *\param theValue a integer containing the version of GPS to be written to the exif metadata
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtExifIoEngine::writeVersionID( int theValue )
{
  return write( "Exif.GPSInfo.GPSVersionID", QString::number( theValue ), "Byte" );
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
