/*
** File: egtexifioengine.h
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
#ifndef EGTEXIFIOENGINE_H
#define EGTEXIFIOENGINE_H

#include <QModelIndex>

#include <exiv2/image.hpp>
#include <exiv2/exif.hpp>

/*! \brief Read and write EXIF data
 *
 * This class is largely a wrapper for Exiv2 to provide simple access to EXIF header data
 */
class MS_DLL_SPEC EgtExifIoEngine
{

public:
  /*! \brief Constructor */
  EgtExifIoEngine();
  
  /*! \brief Constructor */
  EgtExifIoEngine( QString );
  
  /*! \brief Constructor */
  EgtExifIoEngine( const QModelIndex& );

  /*! \brief Returns the altitude */
  double altitude( bool * isValid = 0 );

  /*! \brief Returns the name of the GPS Area */
  QString areaInformation( bool * isValid = 0 );

  /*! \brief Returns the date */
  QString dateStamp( bool * isValid = 0 );

  /*! \brief Returns the bearing to the destination point */
  double destBearing( char*, bool * isValid = 0 );

  /*! \brief Returns the differential correction applied to the gps receiver */
  int differential( bool * isValid = 0 );

  /*! \brief Returns the direction of the image when it was captured */
  float direction( char *, bool * isValid = 0 );

  /*! \brief Returns the distance to the destination point */
  double destDistance( char *, bool * isValid = 0 );

  /*! \brief Returns the latitude of the destination point*/
  double destLatitude( bool * isValid = 0 );

  /*! \brief Returns the longitude of the destination point */
  double destLongitude( bool * isValid = 0 );

  /*! \brief Returns the GPS DOP (data degree of precision)*/
  double gpsDOP( bool * isValid = 0 );

  /*! \brief has the image gps exif metadata? */
  bool hasGpsExif();
  
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

  /*! \brief Returns the GPS satellites used for measurement */
  QString satellites( bool * isValid = 0 );

  /*! \brief Sets the image to be managed with this class */
  void setFile( QString theImageFilename );
  
  /*! \brief Returns the longitude */
  double speed( char *, bool * isValid = 0 );

  /*! \brief Returns the status of the GPS receiver when the picture was taken */
  QString status( bool * isValid = 0 );

  /*! \brief Returns the time stamp */
  QString timeStamp( bool * isValid = 0 );

  /*! \brief Returns the direction of the GPS receiver movement*/
  double track( char *, bool * isValid = 0 );
 
  /*! \brief Returns the version of the GPS */
  int versionID( bool * isValid = 0 );

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

  /*! \brief writes the area information */
  bool writeAreaInformation( QString );

  /*! \brief writes the date stamp */
  bool writeDateStamp( QString);

  /*! \brief writes the bearing of the destination point */
  bool writeDestBearing( float, char theRefference = 'T' );

  /*! \brief writes the differential correction applied to the GPS received */
  bool writeDifferential( int );

  /*! \brief writes the direction of the image when it was taken */
  bool writeDirection( float, char theRefference = 'T' );

  /*! \brief writes the distance to the destination point*/
  bool writeDestDistance( double, char theUnit = 'K' );

  /*! \brief writes the latitude of the destination point */
  bool writeDestLatitude( double, char theRefference = 'N' );

  /*! \brief writes the longitude of the destination point */
  bool writeDestLongitude( double, char theRefference = 'N' );
  
  /*! \brief writes the Gps DOP */
  bool writeGpsDOP( double );
  
  /*! \brief writes the Map Datum */
  bool writeMapDatum( QString );
 
  /*! \brief writes the Measure mode */
  bool writeMeasureMode( QString );

  /*! \brief writes the Processing method */
  bool writeProcessingMethod( QString );

  /*! \brief writes information about the satellites used to the measurement */
  bool writeSatellites( QString );

  /*! \brief writes the speed of the GPS receiver movement */
  bool writeSpeed( double, char theUnit = 'K' );

  /*! \brief writes the status of the GPS receiver in the moment the picture was taken */
  bool writeStatus( QString );

  /*! \brief writes the time stamp */
  bool writeTimeStamp( QString );

  /*! \brief writes the direction of the receiver movement */
  bool writeTrack( float, char theRefference = 'T' );

  /*! \brief writes the version of GPS */
  bool writeVersionID( int );

private:
  /*! \brief Converts from decimal degrees to Rational notation */
  QString convertToRational(QString);
  
  /*! \brief reads the value specified as parameter */
  const Exiv2::Value& read(QString);
  
  /*! \brief reads the value specified as parameter and returns it as a QString */
  QString readKeyValueAsString(QString);
  
  /*! \brief writes exif metadata */
  bool write(QString, QString, QString);
  
  /*! \brief has the image GPS exif metadata? */
  bool cvHasGpsExif;

  /*! \brief Exiv2 data to handle the image */
  Exiv2::Image::AutoPtr cvImage;
  
  /*! \brief Name of the file picture */
  QString  cvImageFileName;
  
  /*! \brief is the image valid? */
  bool cvIsValidImage;
  
  /*! \brief Last error occured */
  QString cvLastError;
  
  /*! \brief Exiv2 data used in case we have to work with invalid data */
  Exiv2::DataValue cvNotValidValue;
};
#endif
