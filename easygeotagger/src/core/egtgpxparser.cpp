/*
** File: egtgpxparser.cpp
** Author( s ): Peter J. Ersts ( ersts at amnh.org ), Roberto Garcia-Yunta
** Creation Date: 2009-05-08
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
#include "egtgpxparser.h"
#include "egtlogger.h"

#include <QFile>
#include <QObject>
#include <QDomNode>
#include <QDomNodeList>

EgtGpxParser::EgtGpxParser()
{
  cvGpxWayPointTags["ele"] = QObject::tr( "Altitude" );
  cvGpxWayPointTags["time"] = QObject::tr( "Date Time Stamp" );
  cvGpxWayPointTags["magvar"] = QObject::tr( "Magnetic Variation" );
  cvGpxWayPointTags["geoidheight"] = QObject::tr( "Geoid Height" );
  cvGpxWayPointTags["name"] = QObject::tr( "Name" );
  cvGpxWayPointTags["cmt"] = QObject::tr( "Comment" );
  cvGpxWayPointTags["desc"] = QObject::tr( "Description" );
  cvGpxWayPointTags["src"] = QObject::tr( "Source" );
  cvGpxWayPointTags["link"] = QObject::tr( "Link" );
  cvGpxWayPointTags["sym"] = QObject::tr( "Symbology" );
  cvGpxWayPointTags["type"] = QObject::tr( "Type" );
  cvGpxWayPointTags["fix"] = QObject::tr( "Fix" );
  cvGpxWayPointTags["sat"] = QObject::tr( "Satellites" );
  cvGpxWayPointTags["hdop"] = QObject::tr( "HDOP" );
  cvGpxWayPointTags["vdop"] = QObject::tr( "VDOP" );
  cvGpxWayPointTags["pdop"] = QObject::tr( "PDOP" );

/*
<ageofdgpsdata> xsd:decimal </ageofdgpsdata>
<dgpsid> dgpsStationType </dgpsid>
<extensions> extensionsType </extensions>
*/


  cvFileName = "";
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */
QList< QString > EgtGpxParser::headers()
{
  QList< QString > lvHeaders = cvHeaders.toList();
  qSort( lvHeaders );

  return lvHeaders;
}

const QList< QMap< QString, QString> > &EgtGpxParser::parse( EgtGpxParser::FeatureTypes theFeatureType )
{
  EgtDebug( "entered" );

  cvData.clear(); //TODO: check to make sure each map does not need to be cleared
  cvHeaders.clear();

  switch( theFeatureType )
  {
    case EgtGpxParser::WayPoints :
    {
      parseWayPoints();
      break;
    }
    case EgtGpxParser::Routes :
    {
      parseRoutes();
      break;
    }
    case EgtGpxParser::Tracks :
    {
      parseTracks();
      break;
    }
    default:
    {
      cvLastError = QObject::tr( "Request received to parse an unknown feature type" );
    }
  }

  return cvData;
}

const QList< QMap< QString, QString> > &EgtGpxParser::parse( EgtGpxParser::FeatureTypes theFeatureType, QString theFileName )
{
  EgtDebug( "entered" );

  cvData.clear(); //TODO: check to make sure each map does not need to be cleared
  cvHeaders.clear();

  cvFileName = theFileName;
  QFile lvFile( cvFileName );
  if ( !lvFile.open( QIODevice::ReadOnly ) )
  {
    cvLastError = QObject::tr( "Could not open file for reading" ) + ": " + cvFileName;
    return cvData;
  }

  if ( !cvGpxDocument.setContent( &lvFile ) )
  {
    cvLastError = QObject::tr( "An error occured while trying to set the content of the XML document" );
    lvFile.close();
    return cvData;
  }
  lvFile.close();

  return parse( theFeatureType );
}

/*
 *
 * PRIVATE FUNCTIONS
 *
 */
void  EgtGpxParser::parseRoutes()
{
  EgtDebug( "entered" );

  //Get all of the route nodes
  QDomNodeList lvRouteNodes = cvGpxDocument.elementsByTagName( "rte" );
  for( int lvNodeRunner = 0; lvNodeRunner < lvRouteNodes.count(); lvNodeRunner++ )
  {
    //For each route node
    QDomNode lvRouteNode = lvRouteNodes.item( lvNodeRunner );
    if( lvRouteNode.isNull() )
    {
      EgtDebug( "Node is empty" );
      continue;
    }

    //Extract the name of the route and then each point
    QString lvName = lvRouteNode.firstChildElement( "name" ).text();
    QDomNodeList lvRoutePoints = lvRouteNode.toElement().elementsByTagName( "rtept" );
    for( int lvPointRunner = 0; lvPointRunner < lvRoutePoints.count(); lvPointRunner++ )
    {
      //For each track point, extract its data
      QMap< QString, QString > lvPoint = parseWayPoint( lvRoutePoints.item( lvPointRunner ) );
      cvHeaders << "Route Name";
      lvPoint[ "Route Name" ] = lvName;
      cvData << lvPoint;
    }
  }
}

void  EgtGpxParser::parseTracks()
{
  EgtDebug( "entered" );

  //Get all of the track nodes
  QDomNodeList lvTrackNodes = cvGpxDocument.elementsByTagName( "trk" );
  for( int lvNodeRunner = 0; lvNodeRunner < lvTrackNodes.count(); lvNodeRunner++ )
  {
    //For each track node
    QDomNode lvTrackNode = lvTrackNodes.item( lvNodeRunner );
    if( lvTrackNode.isNull() )
    {
      EgtDebug( "Node is empty" );
      continue;
    }

    //Extract the name of the track and then each track point
    QString lvName = lvTrackNode.firstChildElement( "name" ).text();
    QDomNodeList lvTrackSegments = lvTrackNode.toElement().elementsByTagName( "trkseg" );
    for( int lvSegmentRunner = 0; lvSegmentRunner < lvTrackSegments.count(); lvSegmentRunner++ )
    {
      //For each track point, extract its data
      QDomNodeList lvTrackPoints = lvTrackSegments.item( lvSegmentRunner ).childNodes();
      for( int lvPointRunner = 0; lvPointRunner < lvTrackPoints.count(); lvPointRunner++ )
      {
        QMap< QString, QString > lvPoint = parseWayPoint( lvTrackPoints.item( lvPointRunner ) );
        cvHeaders << "Track Name";
        lvPoint[ "Track Name" ] = lvName;
        cvData << lvPoint;
      }
    }
  }
}

void EgtGpxParser::parseWayPoints()
{
  EgtDebug( "entered" );

  //Get all waypoints nodes
  QDomNodeList lvPoints = cvGpxDocument.elementsByTagName( "wpt" );
  for( int lvPointRunner = 0; lvPointRunner < lvPoints.count(); lvPointRunner++ )
  {
    //For each track point, extract its data
    QMap< QString, QString > lvPoint = parseWayPoint( lvPoints.item( lvPointRunner ) );
    cvData << lvPoint;
  }
}

QMap< QString, QString > EgtGpxParser::parseWayPoint( QDomNode theWayPoint )
{

  QMap< QString, QString > lvWayPointData;

  //loop through each way point tag, add to the map if we get valid data
  QMapIterator< QString, QString > lvTags( cvGpxWayPointTags );
  lvWayPointData[ "Latitude" ] = theWayPoint.toElement().attribute( "lat" );
  cvHeaders << "Latitude";
  lvWayPointData[ "Longitude" ] = theWayPoint.toElement().attribute( "lon" );
  cvHeaders << "Longitude";

  while( lvTags.hasNext() )
  {
    lvTags.next();
    QDomElement lvElement = theWayPoint.firstChildElement( lvTags.key() );
    if( !lvElement.isNull() )
    {
      cvHeaders << lvTags.value();
      lvWayPointData[ lvTags.value() ] = lvElement.text();
      //Sanatize the time stamps some gpx are in format yyyy-mm-ddThh:mm:ssZ
      if( 0 == lvTags.value().compare( QObject::tr( "Date Time Stamp" ), Qt::CaseInsensitive ) )
      {
        QString lvTemp = lvWayPointData[ lvTags.value() ];
        lvTemp[4] = ':';
        lvTemp[7] = ':';
        lvTemp[10] = ' ';
        if( 19 == lvTemp.length() )
        {
          lvTemp.chop( 1 );
        }
        lvWayPointData[ lvTags.value() ] = lvTemp;
      }
    }
  }

  return lvWayPointData;
}

