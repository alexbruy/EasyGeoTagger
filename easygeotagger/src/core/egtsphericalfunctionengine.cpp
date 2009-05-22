/*
** File: egtsphericalfunctionengine.cpp
** Author: Peter J. Ersts ( ersts  at amnh.org )
** Creation Date: 2009-05-21
**
** This is a quick partial port of Spherical Function Engine ---> http://biodiversityinformatics.amnh.org/open_source/pdc/download
** File: SphericalFunctionEngine.java
** Author: Peter J. Ersts ( ersts at amnh.org )
** Creation Date: 2004-11-10
** Revision Date: 2005-11-09
**
** version 1.1_RC_1 [2005-11-09] Release candidate packaged for distribution ( P.J.Ersts )
**
** version 1.1_PR [2005-10-19] Modified some parameter names and added additional comments ( P.J.Ersts )
**
** version 1.1_PR [2005-10-13] Fix initialBearing( .... ) so the result is relative to 360 deg ( P.J.Ersts )
**
** version 1.0_PR [2004-11-25] ( P.J.Ersts )
**
** Copyright ( c ) 2004,2005 American Museum of Natural History. All rights reserved.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Library General Public
** License as published by the Free Software Foundation; either
** version 2 of the License, or ( at your option ) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.
**
** You should have received a copy of the GNU Library General Public
** License along with this library; if not, write to the
** Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
** MA 02110-1301, USA.
**
** This software is based upon work paritially supported by NASA under award number
** NAG5-12333. Additionally, this program was prepared by the the above author( s )
** under award No. NA04AR4700191 from the National Oceanic and Atmospheric
** Administration, U.S. Department of Commerce.  The statements, findings,
** conclusions, and recommendations are those of the author( s ) and do not
** necessarily reflect the views of the National Oceanic and Atmospheric
** Administration or the Department of Commerce.
**
**/
#include "egtsphericalfunctionengine.h"


/**
 * Constructor, sets default spherical representation of the earth to WGS84.
 */
SphericalFunctionEngine::SphericalFunctionEngine( )
{
  spheroidRadius = WGS84;
}

/**
 * Constructor
 * @param value	Radius of spherical representation of the earth.  Currently assumed to be in meeters.
 */
SphericalFunctionEngine::SphericalFunctionEngine( double theRadius )
{
  spheroidRadius = theRadius;
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */
/**
 * Calculates the coordinates a point that lies some percetance of the great circle distance between Point1 and Point2 and
 * along the great circle defined by said points
 * @param longitudePoint1	Longitude of the start point, Point1, expressed in in decimal degrees
 * @param latitudePoint1 Latitude of Point1, expressed in in decimal degrees
 * @param longitudePoint2	Longitude of the destination, Point2, expressed in in decimal degrees
 * @param latitudePoint2 Latitude of Point2, expressed in in decimal degrees
 * @param fraction The percentace of the distance from Point1 to Point2 where the desired point is located, Point3.
 * @return An array containing the coordinates of the new location, [0] = longitude, [1] = latitude, expressed in decimal degrees.
 */
double* SphericalFunctionEngine::calculateIntermediateGreatCirclePoints( double longitudePoint1, double latitudePoint1, double longitudePoint2, double latitudePoint2, double fraction ) {
  double distance = greatCircleDistance( longitudePoint1, latitudePoint1, longitudePoint2, latitudePoint2, "rad" );
  double scalarA = sin( ( 1 - fraction ) * distance ) / sin( distance );
  double scalarB = sin( fraction * distance ) / sin( distance );
  double x = ( scalarA * cos( toRadians( latitudePoint1 ) ) * cos( toRadians( longitudePoint1 ) ) ) + ( scalarB*cos( toRadians( latitudePoint2 ) ) * cos( toRadians( longitudePoint2 ) ) );
  double y = ( scalarA * cos( toRadians( latitudePoint1 ) ) * sin( toRadians( longitudePoint1 ) ) ) + ( scalarB*cos( toRadians( latitudePoint2 ) ) * sin( toRadians( longitudePoint2 ) ) );
  double z = ( scalarA * sin( toRadians( latitudePoint1 ) ) ) + ( scalarB*sin( toRadians( latitudePoint2 ) ) );
  double* position = new double[2];
  position[0] = toDegrees( atan2( y, x ) );
  position[1] = toDegrees( atan2( z, sqrt( ( x * x ) + ( y * y ) ) ) );

  //Differs from the Java version -- some rounding errors when points are the same
  if( ( longitudePoint1 - longitudePoint2 ) < 0.0000000001 )
  {
    position[0] = longitudePoint1;
  }
  if( ( latitudePoint1 - latitudePoint2 ) < 0.0000000001 )
  {
    position[1] = latitudePoint1;
  }
  //end

  return position;
}

/**
 * Calculates the great circle distance between one geographic position ( Point1 ) to a second
 * geographic position ( Point2 ).  Longitude and Latitude are expected to be represented
 * as decimal degrees.
 *
 * @param longitudePoint1 Longitude of the start or current position, Point1, expressed in decimal degrees.
 * @param latitudePoint1 Latitude of the start or current position, Point1, expressed in decimal degrees.
 * @param longitudePoint2 Longitude of the destination, Point2, expressed in decimal degrees.
 * @param latitudePoint2 Latitude of the destination, Point2, expressed in decimal degrees.
 * @param units The units that the bearing sould be returned in.  Valid options are "rad" for radians and "deg" for decimal degrees, "nm" for nautical mile, "km" for kilometer and "m" for meters.
 * @return Great circle distance between Point1 to Point2
 */
double SphericalFunctionEngine::greatCircleDistance( double longitudePoint1, double latitudePoint1, double longitudePoint2, double latitudePoint2, QString units ) {
  double distance = acos( ( sin( toRadians( latitudePoint1 ) ) * sin( toRadians( latitudePoint2 ) ) ) + ( cos( toRadians( latitudePoint1 ) ) * cos( toRadians( latitudePoint2 ) ) * cos( toRadians( longitudePoint2 ) - toRadians( longitudePoint1 ) ) ) );
  if( 0 == units.compare( "rad", Qt::CaseInsensitive ) )
    return distance;
  else if( 0 == units.compare( "deg", Qt::CaseInsensitive ) )
    return toDegrees( distance );
  else if( 0 == units.compare( "nm", Qt::CaseInsensitive ) )
    return toDegrees( distance ) * 60;
  else if( 0 == units.compare( "km", Qt::CaseInsensitive ) )
    return toDegrees( distance ) * ( ( 2 * M_PI * spheroidRadius ) /360 ) /1000;
  else if( 0 == units.compare( "m", Qt::CaseInsensitive ) )
    return toDegrees( distance ) * ( ( 2 * M_PI * spheroidRadius ) / 360 );
  return -9999;
}

/*
 *
 * PRIVATE FUNCTIONS
 *
 */
double SphericalFunctionEngine::toDegrees( double theRadians )
{
  return theRadians * ( 180.0 / M_PI );
}

double SphericalFunctionEngine::toRadians( double theDegrees )
{
  return theDegrees * ( M_PI / 180.0 );
}
