/*
** File: egtgpsexifengine.h
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
#ifndef EGTGPSEXIFENGINE_H
#define EGTGPSEXIFENGINE_H

#include "egtexifengine.h"

/*! \brief Read and write EXIF data
 *
 * This class is largely a wrapper for Exiv2 to provide simple access to GPS EXIF header data
 */
class MS_DLL_SPEC EgtGpsExifEngine : public EgtExifEngine
{

public:
  /*! \brief Constructor */
  EgtGpsExifEngine( );
  
  /*! \brief Constructor */
  EgtGpsExifEngine( QString );
  
  /*! \brief Constructor */
  EgtGpsExifEngine( const QModelIndex& );

  /*! \brief Returns the altitude */
  double altitude( bool * isValid = 0 );

  /*! \brief Returns the name of the GPS Area */
  QString areaInformation( bool * isValid = 0 );

  /*! \brief Returns the date */
  QString dateStamp( bool * isValid = 0 );

  /*! \brief Returns the bearing to the destination point */
  double destBearing( bool * isValid = 0 );

  /*! \brief Returns the reference used for the bearing to the destination point( ( M )agnetic or ( T )rue north pole ) */
  QString destBearingRef( bool * isValid = 0 );

  /*! \brief Returns the distance to the destination point */
  double destDistance( bool * isValid = 0 );

  /*! \brief Returns the reference used for the destination distance.( ( k )ilometers, ( M )iles or k( N )ots ) */
  QString destDistanceRef( bool * isValid = 0 );

  /*! \brief Returns the latitude of the destination point*/
  double destLatitude( bool * isValid = 0 );

  /*! \brief Returns the longitude of the destination point */
  double destLongitude( bool * isValid = 0 );

  /*! \brief Returns the differential correction applied to the gps receiver */
  int differential( bool * isValid = 0 );

  /*! \brief Returns the direction of the image when it was captured */
  float direction( bool * isValid = 0 );

  /*! \brief Returns the reference used for the direction to the destination point( ( M )agnetic or ( T )rue north pole ) */
  QString directionRef( bool * isValid = 0 );

  /*! \brief Returns the GPS DOP ( data degree of precision )*/
  double gpsDOP( bool * isValid = 0 );

  /*! \brief is the image valid? */
  bool isValidImage( );

  /*! \brief Returns the last error occured */
  QString lastError( ); 

  /*! \brief Returns the latitude */
  double latitude( bool * isValid = 0 );
  
  /*! \brief Returns the longitude */
  double longitude( bool * isValid = 0 );

  /*! \brief Returns the geodetic survey data used by the GPS receiver */
  QString mapDatum( bool * isValid = 0 );  

  /*! \brief Returns the GPS measurement mode */
  QString measureMode( bool * isValid = 0 );

  /*! \brief Returns the processing method */
  QString processingMethod( bool * isValid = 0 );

  /*! \brief Generic method used to read any tag */
  QVariant read( QString, bool * isValid = 0 );

  /*! \brief Returns the GPS satellites used for measurement */
  QString satellites( bool * isValid = 0 );

  /*! \brief Sets the image to be managed with this class */
  void setFile( QString theFileName );
  
  /*! \brief Returns the longitude */
  double speed( bool * isValid = 0 );

  /*! \brief Returns the Reference used to measure the speed. ( K )ilometer, ( M )iles or k( N )ots */
  QString speedRef( bool * isValid = 0 );

  /*! \brief Returns the status of the GPS receiver when the picture was taken */
  QString status( bool * isValid = 0 );

  /*! \brief Returns the time stamp */
  QString timeStamp( bool * isValid = 0 );

  /*! \brief Returns the direction of the GPS receiver movement*/
  double track( bool * isValid = 0 );
 
  /*! \brief Returns the Reference used to measure the track. ( M )agnetic or ( T )rue north pole */
  QString trackRef( bool * isValid = 0 );

  /*! \brief Returns the version of the GPS */
  int versionID( bool * isValid = 0 );

  /*! \brief Generic function used to write any tag */
  bool write( QString, QString );

  /*! \brief writes the altitude */
  bool writeAltitude( double );

  /*! \brief convenience function used to write the altitude using as parameter a QString */
  bool writeAltitude( QString );

  /*! \brief writes the area information */
  bool writeAreaInformation( QString );

  /*! \brief writes the date stamp */
  bool writeDateStamp( QString );

  /*! \brief writes the bearing of the destination point */
  bool writeDestBearing( float );

  /*! \brief convenience function used to write the destination bearing using as parameter a QString */
  bool writeDestBearing( QString );

  /*! \brief convenience function used to write the destination bearing ref. using as parameter a QString */
  bool writeDestBearingRef( QString );

  /*! \brief writes the distance to the destination point*/
  bool writeDestDistance( double );

  /*! \brief convenience function used to write the destination distance using as parameter a QString */
  bool writeDestDistance( QString );

  /*! \brief convenience function used to write the destination distance ref. using as parameter a QString */
  bool writeDestDistanceRef( QString );

  /*! \brief writes the latitude of the destination point */
  bool writeDestLatitude( double );

  /*! \brief convenience function used to write the destination latitude using as parameter a QString */
  bool writeDestLatitude( QString );  

  /*! \brief writes the longitude of the destination point */
  bool writeDestLongitude( double );

  /*! \brief convenience function used to write the destination longitude using as parameter a QString */
  bool writeDestLongitude( QString );

  /*! \brief writes the differential correction applied to the GPS received */
  bool writeDifferential( int );

  /*! \brief convenience function used to write the differential using as parameter a QString */
  bool writeDifferential( QString );

  /*! \brief writes the direction of the image when it was taken */
  bool writeDirection( float );

  /*! \brief convenience function used to write the direction using as parameter a QString */
  bool writeDirection( QString );

  /*! \brief convenience function used to write the direction ref. using as parameter a QString */
  bool writeDirectionRef( QString );

  /*! \brief writes the latitude from a double number */
  bool writeLatitude( double );
  
  /*! \brief writes the latitude from a QString */
  bool writeLatitude( QString );
  
   /*! \brief writes the longitude from a double */
  bool writeLongitude( double );
  
  /*! \brief writes the longitude from a QString */
  bool writeLongitude( QString );

  /*! \brief writes the Gps DOP */
  bool writeGpsDOP( double );
  
  /*! \brief convenience function used to write the gps dop using as parameter a QString */
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

  /*! \brief convenience function used to write the speed using as parameter a QString */
  bool writeSpeed( QString ); 
 
  /*! \brief convenience function used to write the speed ref. using as parameter a QString */
  bool writeSpeedRef( QString ); 

  /*! \brief writes the status of the GPS receiver in the moment the picture was taken */
  bool writeStatus( QString );

  /*! \brief writes the time stamp */
  bool writeTimeStamp( QString );

  /*! \brief writes the direction of the receiver movement */
  bool writeTrack( float );

  /*! \brief convenience function used to write the track using as parameter a QString */
  bool writeTrack( QString );

  /*! \brief convenience function used to write the track ref. using as parameter a QString */
  bool writeTrackRef( QString );

  /*! \brief writes the version of GPS */
  bool writeVersionID( int );

  /*! \brief convenience function used to write the version ID using as parameter a QString */
  bool writeVersionID( QString );

private:

  /*! \brief Initialize class specific variables */
  void init( );

};
#endif

