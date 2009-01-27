/*
** File: egtgpsexifengine.h
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
#ifndef EGTGPSEXIFENGINE_H
#define EGTGPSEXIFENGINE_H

#include "egtexifengine.h"

/*! \brief Read and write EXIF data
 *
 * This class is largely a wrapper for Exiv2 to provide simple access to GPS EXIF header data
 */
class MS_DLL_SPEC EgtGPSExifEngine : public EgtExifEngine
{

public:
  /*! \brief Constructor */
  EgtGPSExifEngine();
  
  /*! \brief Constructor */
  EgtGPSExifEngine( QString );
  
  /*! \brief Constructor */
  EgtGPSExifEngine( const QModelIndex& );

  /*! \brief Returns the altitude */
  double altitude( bool * isValid = 0 );

  /*! \brief Returns the name of the GPS Area */
  QString areaInformation( bool * isValid = 0 );

  /*! \brief Returns the date */
  QString dateStamp( bool * isValid = 0 );

  /*! \brief Returns the bearing to the destination point */
  double destBearing( bool * isValid = 0 );

  QString destBearingRef( bool * isValid = 0 );

  /*! \brief Returns the differential correction applied to the gps receiver */
  int differential( bool * isValid = 0 );

  /*! \brief Returns the direction of the image when it was captured */
  float direction(  bool * isValid = 0 );

  QString directionRef(  bool * isValid = 0 );

  /*! \brief Returns the distance to the destination point */
  double destDistance( bool * isValid = 0 );

  QString destDistanceRef( bool * isValid = 0 );

  /*! \brief Returns the latitude of the destination point*/
  double destLatitude( bool * isValid = 0 );

  /*! \brief Returns the longitude of the destination point */
  double destLongitude( bool * isValid = 0 );

  /*! \brief Returns the GPS DOP (data degree of precision)*/
  double gpsDOP( bool * isValid = 0 );

  /*! \brief is the image valid? */
  bool isValidImage();

  /*! \brief Returns the last error occured */
  QString lastError(); 

  /*! \brief Returns the latitude */
  double latitude( bool * isValid = 0 );
  
  /*! \brief Returns the longitude */
  double longitude( bool * isValid = 0 );

  /*! \brief Returns the geodetic survey data used by the GPS receiver */
  QString mapDatum( bool * isValid = 0 );  

  /*! \brief Returns the GPS measurement mode */
  QString measureMode( bool * isValid = 0 );

  /*! \brief Returns the processing method */
  QString processingMethod( bool * isValid=0 );

  QVariant read( QString, bool * isValid = 0 );

  /*! \brief Returns the GPS satellites used for measurement */
  QString satellites( bool * isValid = 0 );

  /*! \brief Sets the image to be managed with this class */
  void setFile( QString theFileName );
  
  /*! \brief Returns the longitude */
  double speed(  bool * isValid = 0 );

  QString speedRef( bool * isValid = 0 );

  /*! \brief Returns the status of the GPS receiver when the picture was taken */
  QString status( bool * isValid = 0 );

  /*! \brief Returns the time stamp */
  QString timeStamp( bool * isValid = 0 );

  /*! \brief Returns the direction of the GPS receiver movement*/
  double track( bool * isValid = 0 );
 
  QString trackRef( bool * isValid = 0 );

  /*! \brief Returns the version of the GPS */
  int versionID( bool * isValid = 0 );

  bool write(QString, QString);

  /*! \brief writes the latitude from a double number */
  bool writeLatitude(double);
  
  /*! \brief writes the latitude from a QString */
  bool writeLatitude( QString );
  
   /*! \brief writes the longitude from a double */
  bool writeLongitude( double );
  
  /*! \brief writes the longitude from a QString */
  bool writeLongitude( QString );

  /*! \brief writes the altitude */
  bool writeAltitude( double );

  bool writeAltitude( QString );

  /*! \brief writes the area information */
  bool writeAreaInformation( QString );

  /*! \brief writes the date stamp */
  bool writeDateStamp( QString);

  /*! \brief writes the bearing of the destination point */
  bool writeDestBearing( float );

  bool writeDestBearing( QString );

  bool writeDestBearingRef( QString );

  /*! \brief writes the differential correction applied to the GPS received */
  bool writeDifferential( int );

  bool writeDifferential( QString );

  /*! \brief writes the direction of the image when it was taken */
  bool writeDirection( float );

  bool writeDirection( QString );

  bool writeDirectionRef( QString );

  /*! \brief writes the distance to the destination point*/
  bool writeDestDistance( double );

  bool writeDestDistance( QString );

  bool writeDestDistanceRef( QString );

  /*! \brief writes the latitude of the destination point */
  bool writeDestLatitude( double );

  bool writeDestLatitude( QString );  

  /*! \brief writes the longitude of the destination point */
  bool writeDestLongitude( double );

  bool writeDestLongitude( QString );

  /*! \brief writes the Gps DOP */
  bool writeGpsDOP( double );
  
  bool writeGpsDOP( QString );

  /*! \brief writes the Map Datum */
  bool writeMapDatum( QString );

  /*! \brief writes the Measure mode */
  bool writeMeasureMode( QString );

  /*! \brief writes the Processing method */
  bool writeProcessingMethod( QString );

  /*! \brief writes information about the satellites used to the measurement */
  bool writeSatellites( QString );

  /*! \brief writes the speed of the GPS receiver movement */
  bool writeSpeed( double );

  bool writeSpeed( QString); 
 
  bool writeSpeedRef( QString); 

  /*! \brief writes the status of the GPS receiver in the moment the picture was taken */
  bool writeStatus( QString );

  /*! \brief writes the time stamp */
  bool writeTimeStamp( QString );

  /*! \brief writes the direction of the receiver movement */
  bool writeTrack( float );

  bool writeTrack( QString );

  bool writeTrackRef( QString );

  /*! \brief writes the version of GPS */
  bool writeVersionID( int );

  bool writeVersionID( QString );

private:

  /*! \brief Converts from decimal degrees to Rational notation */
  QString convertToRational(QString);

};
#endif

