/*
** File: egtgpsexifengine.cpp
** Author(s): Roberto Garcia-Yunta, Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2008-09-22
**
** Copyright (c) 2008-2009, American Museum of Natural History. All rights reserved.
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
#include "egtgpsexifengine.h"

#include "egtpathbuilder.h"
#include "egtlogger.h"

#include <math.h>
#include <cassert>

#include <QFileInfo>
#include <QObject>
#include <QDir>


EgtGpsExifEngine::EgtGpsExifEngine() : EgtExifEngine()
{
    //Add all of the Egt keys that this engine can read/write
    //Place in the order that they will typically be displayed to the user
    addKey("Egt.GPS.DateStamp", QObject::tr( "Date stamp" ) );
    addKey("Egt.GPS.TimeStamp", QObject::tr( "Time stamp" ) );
    addKey("Egt.GPS.Longitude", QObject::tr( "Longitude" ) );
    addKey("Egt.GPS.Latitude", QObject::tr( "Latitude" ) );
    addKey("Egt.GPS.Altitude", QObject::tr( "Altitude" ) );
    addKey("Egt.GPS.ImageDirection", QObject::tr( "Image direction" ), "|M|T" );
    addKey("Egt.GPS.MapDatum", QObject::tr( "Map datum" ) );
    addKey("Egt.GPS.Speed", QObject::tr( "Speed" ), "|K|M|N" );
    addKey("Egt.GPS.Track", QObject::tr( "Track" ), "|M|T" );
    addKey("Egt.GPS.DestLatitude", QObject::tr( "Destination latitude" ) );
    addKey("Egt.GPS.DestLongitude", QObject::tr( "Destination longitude" ) );
    addKey("Egt.GPS.DestBearing", QObject::tr( "Destination bearing" ), "|M|T" );
    addKey("Egt.GPS.DestDistance", QObject::tr( "Destination distance" ), "|K|M|N" );
    addKey("Egt.GPS.VersionID", QObject::tr( "Version id" ) );
    addKey("Egt.GPS.Satellites", QObject::tr( "Satellites" ) );
    addKey("Egt.GPS.Status", QObject::tr( "Status" ) );
    addKey("Egt.GPS.MeasureMode", QObject::tr( "Measure mode" ) );
    addKey("Egt.GPS.DOP", QObject::tr( "GPS DOP" ) );
    addKey("Egt.GPS.ProcessingMethod", QObject::tr( "Processing method" ) );
    addKey("Egt.GPS.AreaInformation", QObject::tr( "Area information" ) );
    addKey("Egt.GPS.Differential", QObject::tr( "GPS differential" ) );

    //Add the dependencies
    cvDependencies["Egt.GPS.Longitude"] = "Egt.GPS.Latitude";
    cvDependencies["Egt.GPS.Latitude"] = "Egt.GPS.Longitude";
    cvDependencies["Egt.GPS.DestLatitude"] = "Egt.GPS.DestLongitude";
}

EgtGpsExifEngine::EgtGpsExifEngine( QString theImageFilename ) : EgtExifEngine( theImageFilename )
{
    EgtGpsExifEngine();
}

EgtGpsExifEngine::EgtGpsExifEngine( const QModelIndex& theIndex ) : EgtExifEngine( theIndex )
{
    EgtGpsExifEngine();
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
double EgtGpsExifEngine::altitude( bool * isValid )
{
  EgtDebug( "entered altitude(bool)" );
  
  if( 0 != isValid ) { *isValid =false; }
  if( !isValidImageWithExpectedExif() ) { return 0.0; }

  const Exiv2::Value& lvValue = readTag( "Exif.GPSInfo.GPSAltitudeRef" );
      
      
  Exiv2::TypeId lvTypeId = lvValue.typeId ();
  
  if( lvTypeId == Exiv2::invalidTypeId )
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return 0.0;  
  } 

  int lvSeaLevel = 1;

  if( QString::compare( QString( lvValue.toString().c_str() ), "1" ) == 0 )
  {
    lvSeaLevel = -1;
  }      

  const Exiv2::Value & lvValue2 = readTag( "Exif.GPSInfo.GPSAltitude" );
   
  lvTypeId = lvValue2.typeId ();   
  if( lvTypeId == Exiv2::invalidTypeId )
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return 0.0;  
  }  
  
  double lvAltitude = 0.0;

  for( int i=lvValue2.count()-1; i>=0; i-- )
  {
    lvAltitude+= (double)lvValue2.toFloat(i);
  }

  if( isValid ){ *isValid =true; }
      
  
  return lvAltitude*lvSeaLevel;
}

/*!
 * \param isValid if the access to the picture was successful
 * \returns A QString recording the name of the GPS area. The first byte indicates the character code used, and this is 
 * followed by the name of the GPS area.
 */
QString EgtGpsExifEngine::areaInformation( bool * isValid )
{
  EgtDebug( "entered areaInformation(bool)" );

  if( 0 != isValid ) { *isValid =false; }
  if( !isValidImageWithExpectedExif() ) { return ""; }

  const Exiv2::Value & lvValue = readTag( "Exif.GPSInfo.GPSAreaInformation" );
   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return "";  
  }  
  
  QString lvAreaInformation;
  lvAreaInformation= QString(lvValue.toString(0).c_str());
  
  if( isValid ){ *isValid =true; }
  
  return lvAreaInformation;
}

/*!
 * \param isValid if the access to the picture was successful
 * \returns A Qstring recording date and time information relative to UTC (Coordinated Universal Time). The format is
 * "YYYY:MM:DD.".
 */
QString EgtGpsExifEngine::dateStamp( bool * isValid )
{
  EgtDebug( "entered dateStamp(bool)" );

  if( 0 != isValid ) { *isValid =false; }
  if( !isValidImageWithExpectedExif() ) { return ""; }

  const Exiv2::Value & lvValue = readTag( "Exif.GPSInfo.GPSDateStamp" );
   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return "";  
  }  
  
  QString lvDateStamp;
  lvDateStamp= QString(lvValue.toString(0).c_str());

  if( isValid ){ *isValid =true; }
  
  return lvDateStamp;
}

/*!
 * \param isValid if the access to the picture was successful
 * \param theRefference a char pointer containing the units of the measure (Kilometers, miles or knots)
 * \returns The bearing to the destination point. The range of values is from 0.00 to 359.99.
 */
double EgtGpsExifEngine::destBearing( bool * isValid )
{
  EgtDebug( "entered destBearing(bool)" );

  if( 0 != isValid ) { *isValid =false; }
  if( !isValidImageWithExpectedExif() ) { return 0.0; }

  const Exiv2::Value & lvValue = readTag( "Exif.GPSInfo.GPSDestBearing" );
   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return 0.0;  
  }  
  
  double lvBearing = (double)lvValue.toFloat(0);

  if( isValid ){ *isValid =true; }
  
  return lvBearing;
}

QString EgtGpsExifEngine::destBearingRef( bool * isValid )
{
  EgtDebug( "entered destBearingRef(bool)" );

  QString lvBearingReference = "";

  if( 0 != isValid ) { *isValid =false; }
  if( !isValidImageWithExpectedExif() ) { return lvBearingReference; }

  const Exiv2::Value & lvValue = readTag( "Exif.GPSInfo.GPSDestBearingRef" );
   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return lvBearingReference;
  }  

  if( isValid ){ *isValid =true; }
  
  lvBearingReference = QString(lvValue.toString().c_str());

  return lvBearingReference;
}
 


/*!
 * \param isValid if the access to the picture was successful
 * \returns whether differential correction is applied to the GPS receiver.
 */
int EgtGpsExifEngine::differential( bool * isValid )
{
  EgtDebug( "entered differential(bool)" );

  if( 0 != isValid ) { *isValid =false; }
  if( !isValidImageWithExpectedExif() ) { return -1; }

  const Exiv2::Value & lvValue = readTag( "Exif.GPSInfo.GPSDifferential" );
   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return -1;  
  }  
  
  int lvDifferential = int(lvValue.toFloat(0));
  
  if( isValid ){ *isValid =true; }
  
  return lvDifferential;
}

/*!
 * \param isValid if the access to the picture was successful
 * \returns the direction of the image when it was captured. The range of values is from 0.00 to 359.99.
 */
float EgtGpsExifEngine::direction( bool * isValid )
{
  EgtDebug( "entered direction(bool)" );
      
  if( 0 != isValid ) { *isValid =false; }
  if( !isValidImageWithExpectedExif() ) { return 0.0; }

  const Exiv2::Value & lvValue2 = readTag( "Exif.GPSInfo.GPSImgDirection" );
   
  Exiv2::TypeId lvTypeId = lvValue2.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;

    if( isValid ){ *isValid =true; }

    return 0.0;  
  }  
  
  int lvDirection = int(lvValue2.toFloat(0));
  
  if( isValid ){ *isValid =true; }
  
  return lvDirection;
}


QString EgtGpsExifEngine::directionRef( bool * isValid )
{
  EgtDebug( "entered directionRef(bool)" );

  QString lvDirectionReference = "";

  if( 0 != isValid ) { *isValid =false; }
  if( !isValidImageWithExpectedExif() ) { return lvDirectionReference; }

  const Exiv2::Value & lvValue = readTag( "Exif.GPSInfo.GPSImgDirectionRef" );
   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return lvDirectionReference;
  }  

  if( isValid ){ *isValid =true; }
  
  lvDirectionReference = QString(lvValue.toString().c_str());

  return lvDirectionReference;
}

/*!
 * \param isValid if the access to the picture was successful
 * \returns the distance to the destination point
 */
double EgtGpsExifEngine::destDistance( bool * isValid )
{
  EgtDebug( "entered destDistance(char, bool)" );
      
  if( 0 != isValid ) { *isValid =false; }
  if( !isValidImageWithExpectedExif() ) { return 0.0; }

  const Exiv2::Value & lvValue2 = readTag( "Exif.GPSInfo.GPSDestDistance" );
   
  Exiv2::TypeId lvTypeId = lvValue2.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return 0.0;  
  }  
  
  double lvDistance = double(lvValue2.toFloat(0));
  
  if( isValid ){ *isValid =true; }
  
  return lvDistance;
}


QString EgtGpsExifEngine::destDistanceRef( bool * isValid )
{
  EgtDebug( "entered destDistanceRef(bool)" );

  QString lvDistanceReference = "";

  if( 0 != isValid ) { *isValid =false; }
  if( !isValidImageWithExpectedExif() ) { return lvDistanceReference; }

  const Exiv2::Value & lvValue = readTag( "Exif.GPSInfo.GPSDestDistanceRef" );
   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return lvDistanceReference;
  }  

  if( isValid ){ *isValid =true; }
  
  lvDistanceReference = QString(lvValue.toString().c_str());

  return lvDistanceReference;
}

/*!
 * \param isValid if the access to the picture was successful
 * \returns the latitude of the destination point
 */
double EgtGpsExifEngine::destLatitude( bool * isValid )
{
  EgtDebug( "entered destLatitude(bool)" );
 
  if( 0 != isValid ) { *isValid =false; }
  if( !isValidImageWithExpectedExif() ) { return 0.0; }

  const Exiv2::Value& lvValue = readTag( "Exif.GPSInfo.GPSDestLatitudeRef" );
  Exiv2::TypeId lvTypeId = lvValue.typeId ();

  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return 0.0;  
  }

  int lvNorthing = 1;
  if(QString::compare( QString(lvValue.toString().c_str()), "S" ) == 0)
    lvNorthing = -1;
      
  const Exiv2::Value & lvValue2 = readTag( "Exif.GPSInfo.GPSDestLatitude" );
   
  lvTypeId = lvValue2.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return 0.0;  
  }
  
  double lvDestLatitude = 0.0;
  
  for(int i=lvValue2.count()-1; i>=0; i--)
  {
    lvDestLatitude+= (double)lvValue2.toFloat(i);
    if( i> 0){ lvDestLatitude/= 60; }
      
  }

  if( isValid ){ *isValid =true; }
 
  return lvDestLatitude*lvNorthing;
}

/*!
 * \param isValid if the access to the picture was successful
 * \returns the longitude of the destination point
 */
double EgtGpsExifEngine::destLongitude( bool * isValid )
{
  EgtDebug( "entered destLongitude(bool)" );
 
  if( 0 != isValid ) { *isValid =false; }
  if( !isValidImageWithExpectedExif() ) { return 0.0; }

  const Exiv2::Value& lvValue = readTag( "Exif.GPSInfo.GPSDestLongitudeRef" );
  Exiv2::TypeId lvTypeId = lvValue.typeId ();

  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return 0.0;  
  }

  int lvNorthing = 1;
  if(QString::compare( QString(lvValue.toString().c_str()), "W" ) == 0)
    lvNorthing = -1;
      
  const Exiv2::Value & lvValue2 = readTag( "Exif.GPSInfo.GPSDestLongitude" );
   
  lvTypeId = lvValue2.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return 0.0;  
  }
  
  double lvDestLongitude = 0.0;
  
  for(int i=lvValue2.count()-1; i>=0; i--)
  {
    lvDestLongitude+= (double)lvValue2.toFloat(i);
    if( i > 0 ){ lvDestLongitude/= 60; }
      
  }

  if( isValid ){ *isValid =true; }
 
  return lvDestLongitude*lvNorthing;
}

/*!
 * \param isValid if the access to the picture was successful
 * \returns the GPS DOP (data degree of precision). An HDOP value is written during two-dimensional measurement, and PDOP during
 * three-dimensional measurement.
 */
double EgtGpsExifEngine::gpsDOP( bool * isValid )
{
  EgtDebug( "entered gpsDOP(bool)" );

  const Exiv2::Value & lvValue = readTag( "Exif.GPSInfo.GPSDOP" );
  
  if( 0 != isValid ) { *isValid =false; }
  if( !isValidImageWithExpectedExif() ) { return 0.0; }
 
  Exiv2::TypeId lvTypeId = lvValue.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return 0.0;  
  }  
  
  double lvDop = double(lvValue.toFloat(0));
  
  if( isValid ){ *isValid =true; }
  
  return lvDop;
}

/*!
 * \returns whether the image is valid or not
 */
bool EgtGpsExifEngine::isValidImage()
{
  return cvIsValidImage;
}

/*!
 * \returns A message with the last error occured
 */
QString EgtGpsExifEngine::lastError() 
{ 
  return cvLastError; 
}

/*!
 *\param a boolean that indicates whether the operation was successful or not.
 * \returns the latitude of the current image
 */
double EgtGpsExifEngine::latitude( bool * isValid )
{
  EgtDebug( "entered latitude(bool)" );
  
  if( 0 != isValid ) { *isValid =false; }
  if( !isValidImageWithExpectedExif() ) { return 0.0; }

  const Exiv2::Value& lvValue = readTag( "Exif.GPSInfo.GPSLatitudeRef" );
      
      
  Exiv2::TypeId lvTypeId = lvValue.typeId ();
  
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return 0.0;  
  } 

  int lvNorthing = 1;

  if( QString::compare( QString( lvValue.toString().c_str() ), "S" ) == 0 )
  {
    lvNorthing = -1;
  }
      
  const Exiv2::Value & lvValue2 = readTag( "Exif.GPSInfo.GPSLatitude" );
   
  lvTypeId = lvValue2.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return 0.0;  
  }  
  
  double lvLatitude = 0.0;
  
  for(int i=lvValue2.count()-1; i>=0; i--)
  {
    lvLatitude+= (double)lvValue2.toFloat(i);
    if( i > 0 ){ lvLatitude/= 60; }
  }

  if( isValid ){ *isValid =true; }
  
  return lvLatitude*lvNorthing;
}

/*!
 * \returns the longitude of the current image
 */
double EgtGpsExifEngine::longitude( bool * isValid )
{  
  EgtDebug( "entered longitude(bool)" );

  if( 0 != isValid ) { *isValid =false; }
  if( !isValidImageWithExpectedExif() ) { return 0.0; }
 
  const Exiv2::Value& lvValue = readTag( "Exif.GPSInfo.GPSLongitudeRef" );
  Exiv2::TypeId lvTypeId = lvValue.typeId ();

  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return 0.0;  
  }

  int lvNorthing = 1;
  if(QString::compare( QString(lvValue.toString().c_str()), "W" ) == 0)
    lvNorthing = -1;
      
  const Exiv2::Value & lvValue2 = readTag( "Exif.GPSInfo.GPSLongitude" );
   
  lvTypeId = lvValue2.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return 0.0;  
  }
  
  double lvLongitude = 0.0;
  
  for(int i=lvValue2.count()-1; i>=0; i--)
  {
    lvLongitude+= (double)lvValue2.toFloat(i);
    if(i > 0){ lvLongitude/= 60; }
  }

  if( isValid ){ *isValid = true; }
    
  return lvLongitude*lvNorthing;
}


/*!
 *\param isValid if the access to the picture was successful
 * \returns the geodetic survey data used by the GPS receiver. If the survey data is restricted to Japan, the value of this tag is
 * "TOKYO" or "WGS-84".
 */
QString EgtGpsExifEngine::mapDatum( bool * isValid )
{
  EgtDebug( "entered mapDatum(bool)" );

  if( 0 != isValid ) { *isValid =false; }
  if( !isValidImageWithExpectedExif() ) { return ""; }

  const Exiv2::Value & lvValue = readTag( "Exif.GPSInfo.GPSMapDatum" );
   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return "";  
  }  

  if( isValid ){ *isValid =true; }
  
  return QString(lvValue.toString(0).c_str());
}

/*!
 *\param isValid if the access to the picture was successful
 *\returns the GPS measurement mode. "2" means two-dimensional measurement and "3" means three-dimensional measurement is in
 * progress.
 */
QString EgtGpsExifEngine::measureMode( bool * isValid )
{
  EgtDebug( "entered measureMode(bool)" );

  if( 0 != isValid ) { *isValid =false; }
  if( !isValidImageWithExpectedExif() ) { return ""; }
 
  const Exiv2::Value & lvValue = readTag( "Exif.GPSInfo.GPSMeasureMode" );
   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();   
  if( lvTypeId == Exiv2::invalidTypeId )
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return "";  
  }  
  
  if( isValid ){ *isValid =true; }
      
  return QString(lvValue.toString(0).c_str());
}

/*!
 *\param isValid if the access to the picture was successful
 *\returns A character string recording the name of the method used for location finding. The first byte indicates the character
 * code used, and this is followed by the name of the method.
 */
QString EgtGpsExifEngine::processingMethod( bool * isValid )
{
  EgtDebug( "entered processingMethod(bool)" );

  if( 0 != isValid ) { *isValid =false; }
  if( !isValidImageWithExpectedExif() ) { return ""; }

  const Exiv2::Value & lvValue = readTag( "Exif.GPSInfo.GPSProcessingMethod" );
   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return "";  
  }  
  
  if( isValid ){ *isValid =true; }
  
  return QString(lvValue.toString(0).c_str());
}


QVariant EgtGpsExifEngine::read( QString theTag, bool * isValid )
{
  EgtDebug( "entered" );

  if( 0 != isValid ) { *isValid = false; }
  if( !isValidImageWithExpectedExif() ) { return QVariant( "" ); }

  if( QString::compare( theTag, "Egt.GPS.Altitude",Qt::CaseInsensitive ) == 0)
  {
    return QVariant( altitude( isValid ) );
  }
  else if( QString::compare( theTag, "Egt.GPS.Latitude",Qt::CaseInsensitive ) == 0 )
  {
    return QVariant( latitude( isValid ) );
  }
  else if( QString::compare( theTag, "Egt.GPS.Longitude",Qt::CaseInsensitive ) == 0)
  {
    return QVariant( longitude( isValid ) );
  }
  else if( QString::compare( theTag, "Egt.GPS.TimeStamp",Qt::CaseInsensitive ) == 0)
  {
    return QVariant( timeStamp( isValid ) );
  }
  else if( QString::compare( theTag, "Egt.GPS.DateStamp",Qt::CaseInsensitive ) == 0)
  {
    return QVariant( dateStamp( isValid ) );
  }
  else if( QString::compare( theTag, "Egt.GPS.Speed",Qt::CaseInsensitive ) == 0)
  {
    return QVariant( speed(  isValid ) );
  }
  else if( QString::compare( theTag, "Egt.GPS.SpeedRef",Qt::CaseInsensitive ) == 0)
  {
    return QVariant( speedRef(  isValid ) );
  }
  else if( QString::compare( theTag, "Egt.GPS.Track",Qt::CaseInsensitive ) == 0)
  {
    return QVariant( track(  isValid ) );
  }
  else if( QString::compare( theTag, "Egt.GPS.TrackRef",Qt::CaseInsensitive ) == 0)
  {
    return QVariant( trackRef(  isValid ) );
  }
  else if( QString::compare( theTag, "Egt.GPS.ImageDirection",Qt::CaseInsensitive ) == 0)
  {
    return QVariant( direction(  isValid ) );
  }
  else if( QString::compare( theTag, "Egt.GPS.ImageDirectionRef",Qt::CaseInsensitive ) == 0)
  {
    return QVariant( directionRef(  isValid ) );
  }
  else if( QString::compare( theTag, "Egt.GPS.DestLatitude",Qt::CaseInsensitive ) == 0)
  {
    return QVariant( destLatitude( isValid ) );
  }
  else if( QString::compare( theTag, "Egt.GPS.DestLongitude",Qt::CaseInsensitive ) == 0)
  {
    return QVariant( destLongitude( isValid ) );
  }
  else if( QString::compare( theTag, "Egt.GPS.DestBearing",Qt::CaseInsensitive ) == 0)
  {
    return QVariant( destBearing(  isValid ) );
  }
   else if( QString::compare( theTag, "Egt.GPS.DestBearingRef",Qt::CaseInsensitive ) == 0)
  {
    return QVariant( destBearingRef(  isValid ) );
  }
  else if( QString::compare( theTag, "Egt.GPS.DestDistance",Qt::CaseInsensitive ) == 0)
  {
    return QVariant( destDistance( isValid ) );
  }
  else if( QString::compare( theTag, "Egt.GPS.DestDistanceRef",Qt::CaseInsensitive ) == 0)
  {
    return QVariant( destDistanceRef( isValid ) );
  }
  else if( QString::compare( theTag, "Egt.GPS.VersionID",Qt::CaseInsensitive ) == 0)
  {
    return QVariant( versionID( isValid ) );
  }
  else if( QString::compare( theTag, "Egt.GPS.Satellites",Qt::CaseInsensitive ) == 0)
  {
    return QVariant( satellites( isValid ) );
  }
  else if( QString::compare( theTag, "Egt.GPS.Status",Qt::CaseInsensitive ) == 0)
  {
    return QVariant( status( isValid ) );
  }
  else if( QString::compare( theTag, "Egt.GPS.MeasureMode",Qt::CaseInsensitive ) == 0)
  {
    return QVariant( measureMode( isValid ) );
  }
  else if( QString::compare( theTag, "Egt.GPS.DOP",Qt::CaseInsensitive ) == 0)
  {
    return QVariant( gpsDOP( isValid ) );
  }
  else if( QString::compare( theTag, "Egt.GPS.ProcessingMethod",Qt::CaseInsensitive ) == 0)
  {
    return QVariant( processingMethod( isValid ) );
  }
  else if( QString::compare( theTag, "Egt.GPS.AreaInformation",Qt::CaseInsensitive ) == 0)
  {
    return QVariant( areaInformation( isValid ) );
  }
  else if( QString::compare( theTag, "Egt.GPS.MapDatum",Qt::CaseInsensitive ) == 0)
  {
    return QVariant( mapDatum( isValid ) );
  }
  else if( QString::compare( theTag, "Egt.GPS.Differential",Qt::CaseInsensitive ) == 0)
  {
    return QVariant( differential( isValid ) );
  }
  else
    return QVariant( "" );
}


/*!
 *\param isValid if the access to the picture was successful
 *\returns the GPS satellites used for measurements. This tag can be used to describe the number of satellites, their ID number,
 * angle of elevation, azimuth, SNR and other information in ASCII notation. The format is not specified. If the GPS receiver is
 * incapable of taking measurements, value of the tag is set to NULL.
 */
QString EgtGpsExifEngine::satellites( bool * isValid )
{
  EgtDebug( "entered satellites(bool)" );

  if( 0 != isValid ) { *isValid =false; }
  if( !isValidImageWithExpectedExif() ) { return ""; }

  const Exiv2::Value & lvValue = readTag( "Exif.GPSInfo.GPSSatellites" );
   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return "";  
  }  
  
  if( isValid ){ *isValid =true; }
  
  return QString(lvValue.toString(0).c_str());
}

/*!
 * \param theImageFilename Absolute path to the image file that is going to be read/written
 */
void EgtGpsExifEngine::setFile( QString theFileName )
{
  openFile( theFileName );
  cvHasExpectedExif = hasKey( "Exif.GPSInfo" );
}

/*!
 *\param isValid if the access to the picture was successful
 *\param theUnit A char pointer that indicates the unit of the measurement(Kilometers, Miles or Knots)
 *\returns the GPS satellites used for measurements. This tag can be used to describe the number of satellites, their ID number,
 * angle of elevation, azimuth, SNR and other information in ASCII notation. The format is not specified. If the GPS receiver is
 * incapable of taking measurements, value of the tag is set to NULL.
 */
double EgtGpsExifEngine::speed( bool * isValid )
{
  EgtDebug( "entered speed(char, bool)" );
      
  if( 0 != isValid ) { *isValid =false; }
  if( !isValidImageWithExpectedExif() ) { return 0.0; }

  const Exiv2::Value & lvValue2 = readTag( "Exif.GPSInfo.GPSSpeed" );
   
  Exiv2::TypeId lvTypeId = lvValue2.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return 0.0;  
  }  

  if( isValid ){ *isValid =true; }
  
  return double(lvValue2.toFloat(0));
}


QString EgtGpsExifEngine::speedRef( bool * isValid )
{
  EgtDebug( "entered speedRef(bool)" );

  if( 0 != isValid ) { *isValid =false; }
  if( !isValidImageWithExpectedExif() ) { return QString("")+"|"+"K"+"|"+"M"+"|"+"N"; }

  QString lvSpeedReference = "";

  const Exiv2::Value & lvValue = readTag( "Exif.GPSInfo.GPSSpeedRef" );
   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return lvSpeedReference;
  }  

  if( isValid ){ *isValid =true; }
      
  lvSpeedReference = QString(lvValue.toString().c_str());

  return lvSpeedReference;
}

/*!
 *\param isValid if the access to the picture was successful
 *\returns the status of the GPS receiver when the image is recorded. "A" means measurement is in progress, and "V" means the
 * measurement is Interoperability.
 */
QString EgtGpsExifEngine::status( bool * isValid )
{
  EgtDebug( "entered status(bool)" );

  if( 0 != isValid ) { *isValid =false; }
  if( !isValidImageWithExpectedExif() ) { return ""; }

  const Exiv2::Value & lvValue = readTag( "Exif.GPSInfo.GPSStatus" );
   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return "";  
  }  
  
  if( isValid ){ *isValid =true; }
      
  return QString(lvValue.toString(0).c_str());
}

/*!
 *\param isValid if the access to the picture was successful
 *\returns the timeStamp
 */
QString EgtGpsExifEngine::timeStamp( bool * isValid )
{
  EgtDebug( "entered timeStamp(bool)" );
 
  if( 0 != isValid ) { *isValid =false; }
  if( !isValidImageWithExpectedExif() ) { return ""; }

  const Exiv2::Value& lvValue = readTag( "Exif.GPSInfo.GPSTimeStamp" );
  Exiv2::TypeId lvTypeId = lvValue.typeId ();

  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return "";  
  }

  QString lvTimeStamp = QString(QString::number(int(lvValue.toFloat(0))) +":"+ QString::number(int(lvValue.toFloat(1))) +":"+ QString::number(int(lvValue.toFloat(2))) );

  if( isValid ){ *isValid = true; }
 
  return lvTimeStamp;
}

/*!
 *\param isValid if the access to the picture was successful
 *\returns the direction of GPS receiver movement. The range of values is from 0.00 to 359.99.
 */
double EgtGpsExifEngine::track( bool * isValid )
{
  EgtDebug( "entered track(bool)" );

  const Exiv2::Value & lvValue2 = readTag( "Exif.GPSInfo.GPSTrack" );
  
  if( 0 != isValid ) { *isValid =false; }
  if( !isValidImageWithExpectedExif() ) { return 0.0; }

  Exiv2::TypeId lvTypeId = lvValue2.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return 0.0;  
  }  
  
  double lvDirection = double(lvValue2.toFloat(0));
  
  if( isValid ){ *isValid =true; }
      
  
  return lvDirection;
}


QString EgtGpsExifEngine::trackRef( bool * isValid )
{
  EgtDebug( "entered trackRef(bool)" );

  QString lvTrackReference = "";

  if( 0 != isValid ) { *isValid =false; }
  if( !isValidImageWithExpectedExif() ) { return lvTrackReference; }

  const Exiv2::Value & lvValue = readTag( "Exif.GPSInfo.GPSTrackRef" );
   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return lvTrackReference;
  }  

  if( isValid ){ *isValid =true; }
      
  
  lvTrackReference = QString(lvValue.toString().c_str());
  
  return lvTrackReference;
}
/*!
 *\param isValid if the access to the picture was successful
 *\returns Indicates the version of <GPSInfoIFD>. The version is given as 2.0.0.0. This tag is mandatory when <GPSInfo> tag is
 * present. (Note: The <GPSVersionID> tag is given in bytes, unlike the <ExifVersion> tag. When the version is 2.0.0.0, the tag
 * value is 02000000.H).
 */
int EgtGpsExifEngine::versionID( bool * isValid )
{
  EgtDebug( "entered versionID(bool)" );

  if( 0 != isValid ) { *isValid =false; }
  if( !isValidImageWithExpectedExif() ) { return -1; }

  const Exiv2::Value & lvValue = readTag( "Exif.GPSInfo.GPSVersionID" );
   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
  {
    cvLastError = QObject::tr( "Unable to read exif data from file") + ": " + cvImageFileName;
    return -1;  
  }  
  
  int lvDifferential = int(lvValue.toFloat(0));
  
  if( isValid ){ *isValid =true; }
  
  return lvDifferential;
}

bool EgtGpsExifEngine::write( QString theTag, QString theValue )
{
  EgtDebug( "entered" ); 

  if( !isValidImage() ) { return false; }

  if(QString::compare( theTag, "Egt.GPS.Latitude" ,Qt::CaseInsensitive ) == 0)
  {
    return writeLatitude( theValue );
  }
  else if(QString::compare( theTag, "Egt.GPS.Longitude" ,Qt::CaseInsensitive  ) == 0)
  {
    return writeLongitude( theValue );
  }
  else if(QString::compare( theTag, "Egt.GPS.Altitude" ,Qt::CaseInsensitive  ) == 0)
  {
    return writeAltitude( theValue );
  }
  else if(QString::compare( theTag, "Egt.GPS.TimeStamp" ,Qt::CaseInsensitive  ) == 0)
  {
    return writeTimeStamp( theValue );
  }
  else if(QString::compare( theTag, "Egt.GPS.DateStamp" ,Qt::CaseInsensitive  ) == 0)
  {
    return writeDateStamp( theValue );
  }
  else if(QString::compare( theTag, "Egt.GPS.Speed" ,Qt::CaseInsensitive  ) == 0)
  {
    return writeSpeed( theValue );
  }
  else if(QString::compare( theTag, "Egt.GPS.SpeedRef" ,Qt::CaseInsensitive  ) == 0)
  {
    return writeSpeedRef( theValue );
  }
  else if(QString::compare( theTag, "Egt.GPS.Track" ,Qt::CaseInsensitive  ) == 0)
  {
    return writeTrack( theValue );
  }
  else if(QString::compare( theTag, "Egt.GPS.TrackRef" ,Qt::CaseInsensitive  ) == 0)
  {
    return writeTrackRef( theValue );
  }
  else if(QString::compare( theTag, "Egt.GPS.ImageDirection" ,Qt::CaseInsensitive  ) == 0)
  {
    return writeDirection( theValue );
  }
  else if(QString::compare( theTag, "Egt.GPS.ImageDirectionRef" ,Qt::CaseInsensitive  ) == 0)
  {
    return writeDirectionRef( theValue );
  }
  else if(QString::compare( theTag, "Egt.GPS.DestLatitude" ,Qt::CaseInsensitive  ) == 0)
  {
    return writeDestLatitude( theValue );
  }
  else if(QString::compare( theTag, "Egt.GPS.DestLongitude" ,Qt::CaseInsensitive  ) == 0)
  {
    return writeDestLongitude( theValue );
  }
  else if(QString::compare( theTag, "Egt.GPS.DestBearing" ,Qt::CaseInsensitive  ) == 0)
  {
    return writeDestBearing( theValue );
  }
  else if(QString::compare( theTag, "Egt.GPS.DestBearingRef" ,Qt::CaseInsensitive  ) == 0)
  {
    return writeDestBearingRef( theValue );
  }
  else if(QString::compare( theTag, "Egt.GPS.DestDistance" ,Qt::CaseInsensitive  ) == 0)
  {
    return writeDestDistance( theValue );
  }
  else if(QString::compare( theTag, "Egt.GPS.DestDistanceRef" ,Qt::CaseInsensitive  ) == 0)
  {
    return writeDestDistanceRef( theValue );
  }
  else if(QString::compare( theTag, "Egt.GPS.VersionID" ,Qt::CaseInsensitive  ) == 0)
  {
    return writeVersionID( theValue );
  }
  else if(QString::compare( theTag, "Egt.GPS.Satellites" ,Qt::CaseInsensitive  ) == 0)
  {
    return writeSatellites( theValue );
  }
  else if(QString::compare( theTag, "Egt.GPS.Status" ,Qt::CaseInsensitive  ) == 0)
  {
    return writeStatus( theValue );
  }
  else if(QString::compare( theTag, "Egt.GPS.MeasureMode" ,Qt::CaseInsensitive  ) == 0)
  {
    return writeMeasureMode( theValue );
  }
  else if(QString::compare( theTag, "Egt.GPS.DOP" ,Qt::CaseInsensitive  ) == 0)
  {
    return writeGpsDOP( theValue );
  }
  else if(QString::compare( theTag, "Egt.GPS.ProcessingMethod" ,Qt::CaseInsensitive  ) == 0)
  {
    return writeProcessingMethod( theValue );
  }
  else if(QString::compare( theTag, "Egt.GPS.AreaInformation" ,Qt::CaseInsensitive  ) == 0)
  {
    return writeAreaInformation( theValue );
  }
  else if(QString::compare( theTag, "Egt.GPS.MapDatum" ,Qt::CaseInsensitive  ) == 0)
  {
    return writeMapDatum( theValue );
  }
  else if(QString::compare( theTag, "Egt.GPS.Differential" ,Qt::CaseInsensitive  ) == 0)
  {
    return writeDifferential( theValue );
  }
  else{ return false; }
    

}

/*!
 *\param theValue a double containing the altitude to be written to the exif metadata
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtGpsExifEngine::writeAltitude( double theValue )
{
  EgtDebug( "entered writeAltitude(double)" );
 
  if( !isValidImage() ) { return false; }
 
  bool ok; 
    
  if( theValue < 0 )
  {
    ok = writeTag( "Exif.GPSInfo.GPSAltitudeRef", "1", "Byte" );
  }
  else
  {
    ok = writeTag( "Exif.GPSInfo.GPSAltitudeRef", "0", "Byte" );
  }
  
  if( !ok )
  {
    cvLastError = QObject::tr( "Unable to write file") + ": " + cvImageFileName;
    return false;
  }  

  double lvValueInt;
  modf( theValue*99999, &lvValueInt );
  return writeTag( "Exif.GPSInfo.GPSAltitude", QString( QString::number( (int)fabs( lvValueInt ) )+"/99999" ), "Rational" );
}

bool EgtGpsExifEngine::writeAltitude( QString theValue )
{
  EgtDebug( "entered writeAltitude(QString)" );

  if( !isValidImage() ) { return false; }

  bool ok; 
  double lvAltitude=theValue.toDouble( &ok );
  if( ok )
  {
    return writeAltitude( lvAltitude );
  }
  else 
  {
    return false;
  }
}

/*!
 *\param theValue a double containing the area information to be written to the exif metadata
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtGpsExifEngine::writeAreaInformation( QString theValue )
{
  EgtDebug( "entered writeAreaInformation(QString)" );
  return writeTag( "Exif.GPSInfo.GPSAreaInformation", theValue, "Undefined" );
}

/*!
 *\param theValue a double containing the date stamp to be written to the exif metadata.The format is "YYYY:MM:DD"
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtGpsExifEngine::writeDateStamp( QString theValue )
{
  EgtDebug( "entered writeDateStamp(QString)" );
  return writeTag( "Exif.GPSInfo.GPSDateStamp", theValue, "Ascii" );
}

/*!
 *\param theValue a float containing the bearing of the destination point to be written to the exif metadata
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtGpsExifEngine::writeDestBearing( float theValue )
{
  EgtDebug( "entered writeDestBearing(float)" );

  if( !isValidImage() ) { return false; }

  double lvValueInt;
  modf( theValue*99999, &lvValueInt );
  return writeTag( "Exif.GPSInfo.GPSDestBearing", QString( QString::number( (int)fabs( lvValueInt ) )+"/99999" ), "Rational" );
}

bool EgtGpsExifEngine::writeDestBearing( QString theValue )
{
  EgtDebug( "entered writeDestBearing(QString)" );

  if( !isValidImage() ) { return false; }

  bool ok; 
  float lvDestBearing=theValue.toFloat( &ok );
  if( ok )
  {
   return writeDestBearing( lvDestBearing ); 
  } 
  else { return false; }
    
}


bool EgtGpsExifEngine::writeDestBearingRef( QString theValue )
{
  EgtDebug( "entered writeDestBearingRef(QString)" );

  if( !isValidImage() ) { return false; }

  return writeTag( "Exif.GPSInfo.GPSDestBearingRef", theValue, "Ascii" );
}

/*!
 *\param theValue an integer containing the differential correction to be written to the exif metadata
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtGpsExifEngine::writeDifferential( int theValue )
{
  return writeTag( "Exif.GPSInfo.GPSDifferential", QString::number( theValue ), "Short" );
}

bool EgtGpsExifEngine::writeDifferential( QString theValue )
{
  EgtDebug( "entered writeDifferential(QString)" );

  if( !isValidImage() ) { return false; }

  bool ok; 
  int lvDifferential=theValue.toInt( &ok );
  if( ok )
  {
    return writeDifferential( lvDifferential);
  }
  else{ return false; }
}

/*!
 *\param theValue a float containing the direction of the gps receiver to be written to the exif metadata
 *\param theRefference a char that indicates the refference(geographic north pole(T) or magnetic noth pole(M))
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtGpsExifEngine::writeDirection( float theValue )
{
  EgtDebug( "entered writeDirection(float)" );

  if( !isValidImage() ) { return false; }

  double lvValueInt;
  modf( theValue*99999, &lvValueInt );
  return writeTag( "Exif.GPSInfo.GPSImgDirection", QString( QString::number( (int)fabs( lvValueInt ) )+"/99999" ), "Rational" );
}

bool EgtGpsExifEngine::writeDirection( QString theValue )
{
  EgtDebug( "entered writeDirection(QString)" );

  if( !isValidImage() ) { return false; }

  bool ok; 
  float lvDirection=theValue.toFloat( &ok );
  if( ok )
  {
    return writeDirection( lvDirection );
  }
  else 
  {
    return false;
  }
}


bool EgtGpsExifEngine::writeDirectionRef( QString theValue )
{
  EgtDebug( "entered writeDirectionRef(QString)" );

  if( !isValidImage() ) { return false; }

  return writeTag( "Exif.GPSInfo.GPSImgDirectionRef", theValue, "Ascii" );
}

/*!
 *\param theValue a double containing the distance to the destination point to be written to the exif metadata
 *\param theUnit a char that indicates the unit used(Kilometers, Miles or Knots)
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtGpsExifEngine::writeDestDistance( double theValue )
{
  EgtDebug( "entered :writeDestDistance(double)" );

  if( !isValidImage() ) { return false; }

  double lvValueInt;
  modf( theValue*99999, &lvValueInt );
  return writeTag( "Exif.GPSInfo.GPSDestDistance", QString( QString::number( (int)fabs( lvValueInt ) )+"/99999" ), "Rational" );
}

bool EgtGpsExifEngine::writeDestDistance( QString theValue )
{
  EgtDebug( "entered writeDestDistance(QString)" );

  if( !isValidImage() ) { return false; }

  bool ok; 
  double lvDestDistance=theValue.toDouble( &ok );
  if( ok )
  {
    return writeDestDistance( lvDestDistance );
  }
  else
  { 
    return false;
  }
}

bool EgtGpsExifEngine::writeDestDistanceRef( QString theValue )
{
  EgtDebug( "entered writeDestDistanceRef(QString)" );

  if( !isValidImage() ) { return false; }

  return writeTag( "Exif.GPSInfo.GPSDestDistanceRef", theValue, "Ascii" );
}

/*!
 *\param theValue a double containing the latitude of the destination point to be written to the exif metadata
 *\param theRefference a char that indicates the refference(geographic north pole(T) or magnetic noth pole(M))
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtGpsExifEngine::writeDestLatitude( double theValue )
{
  EgtDebug( "entered writeDestLatitude(double)" );

  if( !isValidImage() ) { return false; }

  bool ok; 
    
  if( theValue < 0 )
  {
    ok = writeTag( "Exif.GPSInfo.GPSDestLatitudeRef", "S", "Ascii" );
    if( !ok )
    {
      cvLastError = QString( "Unable to write file: " + cvImageFileName);
      return false;
    }
  }
  else
  {
    ok = writeTag( "Exif.GPSInfo.GPSDestLatitudeRef", "N", "Ascii" );
    if( !ok )
    {
      cvLastError = QObject::tr( "Unable to write file") + ": " + cvImageFileName;
      return false;
    }
  }
  
  return writeTag( "Exif.GPSInfo.GPSDestLatitude", convertToRational( QString::number( theValue, 'f', 7 ) ), "Rational" );
}

bool EgtGpsExifEngine::writeDestLatitude( QString theValue )
{
  EgtDebug( "entered writeDestLatitude(QString)" );

  if( !isValidImage() ) { return false; }

  bool ok; 
  double lvDestLatitude=theValue.toDouble( &ok );
  if( ok )
  {
    return writeDestLatitude( lvDestLatitude );
  }
  else 
  {
    return false;
  }
}

/*!
 *\param theValue a double containing the longitude of the destination point to be written to the exif metadata
 *\param theRefference a char that indicates the refference(geographic north pole(T) or magnetic noth pole(M))
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtGpsExifEngine::writeDestLongitude( double theValue )
{
  EgtDebug( "entered writeDestLongitude(double)" );

  if( !isValidImage() ) { return false; }

  bool ok; 
    
  if( theValue < 0 )
  {
    ok = writeTag( "Exif.GPSInfo.GPSDestLongitudeRef", "S", "Ascii" );
    if( !ok )
    {
      cvLastError = QObject::tr( "Unable to write file") + ": " + cvImageFileName;
      return false;
    }
  }
  else
  {
    ok = writeTag( "Exif.GPSInfo.GPSDestLongitudeRef", "N", "Ascii" );
    if( !ok )
    {
      cvLastError = QObject::tr( "Unable to write file") + ": " + cvImageFileName;
      return false;
    }
  }
  
  return writeTag( "Exif.GPSInfo.GPSDestLongitude", convertToRational( QString::number( theValue, 'f', 7 ) ), "Rational" );
}

bool EgtGpsExifEngine::writeDestLongitude( QString theValue )
{
  EgtDebug( "entered writeDestLongitude(QString)" );

  if( !isValidImage() ) { return false; }

  bool ok; 
  double lvDestLongitude=theValue.toDouble( &ok );
  if( ok )
  {
    return writeDestLongitude( lvDestLongitude );
  }
  else 
  {
    return false;
  }
}

/*!
 *\param theValue a double containing the GPS DOP to be written to the exif metadata
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtGpsExifEngine::writeGpsDOP( double theValue )
{

  if( !isValidImage() ) { return false; }

  double lvValueInt;
  modf( theValue*99999, &lvValueInt );
  return writeTag( "Exif.GPSInfo.GPSDOP", QString( QString::number( (int)fabs( lvValueInt ) )+"/99999" ), "Rational" );
}

bool EgtGpsExifEngine::writeGpsDOP( QString theValue )
{
  EgtDebug( "entered writeGpsDOP(QString)" );

  if( !isValidImage() ) { return false; }

  bool ok; 
  double lvGpsDOP=theValue.toDouble( &ok );
  if( ok )
  {
    return writeGpsDOP( lvGpsDOP );
  }
  else 
  {
    return false;
  }
}

/*!
 * \param theValue Latitude represented as a double value, to be written in the exif metadata
 * \returns Whether the operation was successful or not
 */
bool EgtGpsExifEngine::writeLatitude( double theValue )
{
  if( !isValidImage() ) { return false; }

  bool ok; 
    
  if( theValue < 0 )
  {
    ok = writeTag( "Exif.GPSInfo.GPSLatitudeRef", "S", "Ascii" );
    if( !ok )
    {
      cvLastError = QObject::tr( "Unable to write file") + ": " + cvImageFileName;
      return false;
    }
  }
  else
  {
    ok = writeTag( "Exif.GPSInfo.GPSLatitudeRef", "N", "Ascii" );
    if( !ok )
    {
      cvLastError = QObject::tr( "Unable to write file") + ": " + cvImageFileName;
      return false;
    }
  }
  
  return writeTag( "Exif.GPSInfo.GPSLatitude", convertToRational( QString::number( theValue, 'f', 7 ) ), "Rational" );
}

/*!
 * \param theValue QString representing the Latitude to be written in the exif metadata
 * \returns Whether the operation was successful or not
 */
bool EgtGpsExifEngine::writeLatitude( QString theValue )
{
  EgtDebug( "entered writeLatitude(QString)" );

  if( !isValidImage() ) { return false; }

  bool ok; 
  double lvLatitude=theValue.toDouble( &ok );
  if( ok )
  {
    return writeLatitude( lvLatitude );
  }
  else 
  {
    return false;
  }
}

/*!
 * \param theValue Longitude represented as a double value, to be written in the exif metadata
 * \returns Whether the operation was successful or not
 */
bool EgtGpsExifEngine::writeLongitude( double theValue )
{
  if( !isValidImage() ) { return false; }

  bool ok; 

  if( theValue < 0 )
  {
    ok = writeTag( "Exif.GPSInfo.GPSLongitudeRef", "W", "Ascii" );
    if( !ok )
    {
      cvLastError = QObject::tr( "Unable to write file") + ": " + cvImageFileName;
      return false;
    }
  }
  else
  {
    ok = writeTag( "Exif.GPSInfo.GPSLongitudeRef", "E", "Ascii" );
    if( !ok )
    {
      cvLastError = QObject::tr( "Unable to write file") + ": " + cvImageFileName;
      return false;
    }
  }
  
  return writeTag( "Exif.GPSInfo.GPSLongitude", convertToRational( QString::number( theValue, 'f', 7 ) ), "Rational" );
}


/*!
 * \param theValue QString representing the Longitude to be written in the exif metadata
 * \returns Whether the operation was successful or not
 */
bool EgtGpsExifEngine::writeLongitude( QString theValue )
{
  EgtDebug( "entered writeLongitude(QString)" );

  if( !isValidImage() ) { return false; }

  bool ok; 
  double lvLongitude=theValue.toDouble( &ok );
  if( ok )
  {
    return writeLongitude( lvLongitude );
  }
  else
  { 
    return false;
  }
}

/*!
 *\param theValue a QString containing the geodetic survey data used by the GPS receiver to be written to the exif metadata
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtGpsExifEngine::writeMapDatum( QString theValue )
{
  EgtDebug( "entered writeMapDatum(QString)" );
  return writeTag( "Exif.GPSInfo.GPSMapDatum", theValue, "Ascii" );
}

/*!
 *\param theValue a QString containing the measure mode to be written to the exif metadata
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtGpsExifEngine::writeMeasureMode( QString theValue)
{
  EgtDebug( "entered writeMeasureMode(QString)" );
  return writeTag( "Exif.GPSInfo.GPSMeasureMode", theValue, "Ascii" );
}

/*!
 *\param theValue a QString containing the processing method to be written to the exif metadata
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtGpsExifEngine::writeProcessingMethod( QString theValue)
{
  EgtDebug( "entered writeProcessingMethod(QString)" );
  return writeTag( "Exif.GPSInfo.GPSProcessingMethod", theValue, "Undefined" );
}

/*!
 *\param theValue a QString containing the satellites used for measurements to be written to the exif metadata
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtGpsExifEngine::writeSatellites( QString theValue )
{
  EgtDebug( "entered writeSatellites(QString)" );
  return writeTag( "Exif.GPSInfo.GPSSatellites", theValue, "Ascii" );
}

/*!
 *\param theValue a QString containing the speed of the gps receiver to be written to the exif metadata
  *\param theUnit a char that indicates the units of the measurement(Kilometers, Miles or Knots)
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtGpsExifEngine::writeSpeed( double theValue )
{
  EgtDebug( "entered writeSpeed(double)" );

  if( !isValidImage() ) { return false; }

  double lvValueInt;
  modf( theValue*99999, &lvValueInt );
  return writeTag( "Exif.GPSInfo.GPSSpeed", QString( QString::number( (int)fabs( lvValueInt ) )+"/99999" ), "Rational" );
}

bool EgtGpsExifEngine::writeSpeed( QString theValue )
{
  EgtDebug( "entered writeSpeed(QString)" );

  if( !isValidImage() ) { return false; }

  bool ok; 
  double lvSpeed=theValue.toDouble( &ok );
  if( ok )
  {
    return writeSpeed( lvSpeed );
  }
  else 
  {
    return false;
  }
}


bool EgtGpsExifEngine::writeSpeedRef( QString theValue )
{
  EgtDebug( "entered writeSpeedRef(QString)" );

  if( !isValidImage() ) { return false; }

  return writeTag( "Exif.GPSInfo.GPSSpeedRef", theValue, "Ascii" );
}
/*!
 *\param theValue a QString containing the status of the GPS receiver to be written to the exif metadata
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtGpsExifEngine::writeStatus( QString theValue )
{
  EgtDebug( "entered writeStatus(QString)" );
  return writeTag( "Exif.GPSInfo.GPSStatus", theValue, "Ascii" );
}

/*!
 *\param theValue a QString containing the time stamp to be written to the exif metadata. the format is ""hh:mm:ss"
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtGpsExifEngine::writeTimeStamp( QString theString )
{
  EgtDebug( "entered writeTimeStamp(QString)" );

  if( !isValidImage() ) { return false; }

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
 return writeTag( "Exif.GPSInfo.GPSTimeStamp", QString( QString::number( lvValue1 ) +"/1 "+ QString::number( lvValue2 ) +"/1 "+ QString::number( lvValue3 )) +"/1", "Rational" );
}

/*!
 *\param theValue a float containing the direction of the GPS movement to be written to the exif metadata
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtGpsExifEngine::writeTrack( float theValue )
{
  EgtDebug( "entered writeTrack(float, char)" );

  if( !isValidImage() ) { return false; }

  double lvValueInt;
  modf( theValue*99999, &lvValueInt );
  return writeTag( "Exif.GPSInfo.GPSTrack", QString( QString::number( (int)fabs( lvValueInt ) )+"/99999" ), "Rational" );
}

bool EgtGpsExifEngine::writeTrack( QString theValue )
{
  EgtDebug( "entered writeTrack(QString)" );

  if( !isValidImage() ) { return false; }

  bool ok; 
  float lvTrack=theValue.toFloat( &ok );
  if( ok )
  {
    return writeTrack( lvTrack );
  }
  else 
  {
    return false;
  }
}


bool EgtGpsExifEngine::writeTrackRef( QString theValue )
{
  EgtDebug( "entered writeTrackRef(QString)" );

  if( !isValidImage() ) { return false; }

  return writeTag( "Exif.GPSInfo.GPSTrackRef", theValue, "Ascii" );
}
/*!
 *\param theValue a integer containing the version of GPS to be written to the exif metadata
 *\returns a boolean that indicates whether the operation was succcessful or not
 */
bool EgtGpsExifEngine::writeVersionID( int theValue )
{
  EgtDebug( "entered writeVersionID(QString)" );
  return writeTag( "Exif.GPSInfo.GPSVersionID", QString::number( theValue ), "Byte" );
}

bool EgtGpsExifEngine::writeVersionID( QString theValue )
{
  EgtDebug( "entered writeVersionID(QString)" );

  if( !isValidImage() ) { return false; }

  bool ok; 
  int lvVersionID=theValue.toInt( &ok );
  if( ok )
  {
    return writeVersionID( lvVersionID );
  }
  else 
  {
    return false;
  }
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
QString EgtGpsExifEngine::convertToRational(QString theDegrees)
{
  bool ok;

  double lvTheDegrees =fabs(theDegrees.toDouble(&ok));
  if(!ok){ return ""; }
    
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

