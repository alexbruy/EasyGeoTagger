/*
** File: easygeotaggeridtool.cpp
** Author( s ): Peter J. Ersts ( ersts at amnh.org )
** Creation Date: 2008-10-20
**
** Copyright ( c ) 2008, American Museum of Natural History. All rights reserved.
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
#include "qgscursors.h"
#include "qgsmaptopixel.h"
#include "qgsvectorlayer.h"
#include "qgsmaprenderer.h"
#include "qgsvectordataprovider.h"
#include "qgscoordinatetransform.h"

#include "easygeotaggeridtool.h"

#include <QMessageBox>

/*!
* Constructor for the id style tool, this tool inherits the QgsMapTool and requires a pointer to
* to the map canvas.
* @param canvas - Pointer to the QGIS map canvas
*/
EasyGeoTaggerIdTool::EasyGeoTaggerIdTool( QgsMapCanvas* theCanvas )
: QgsMapTool( theCanvas )
{
  //set cursor
  QPixmap myIdentifyQPixmap = QPixmap( ( const char ** ) identify_cursor );
  mCursor = QCursor( myIdentifyQPixmap, 1, 1 );
  
  //set the current tool to this object
  mCanvas->setMapTool( this );
  
  //set the default CRS
  cvDestinationCRS = new QgsCoordinateReferenceSystem( );
  cvDestinationCRS->createFromProj4( "+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs" );
}

/*!
* Mouse release, i.e., select, event
* @param mouseEvent - Pointer to a QMouseEvent
*/
void EasyGeoTaggerIdTool::canvasReleaseEvent( QMouseEvent* mouseEvent )
{
  //Check to see if there is a layer selected
  if( mCanvas->currentLayer( ) && cvDestinationCRS )
  {
    QgsCoordinateTransform lvTransformer;
    lvTransformer.setDestCRS( *cvDestinationCRS );
    
    if( mCanvas->hasCrsTransformEnabled( ) )
    {
      lvTransformer.setSourceCrs( mCanvas->mapRenderer( )->destinationSrs( ) );
    }
    else
    {
      lvTransformer.setSourceCrs( mCanvas->currentLayer( )->srs( ) );
      qDebug( qPrintable( mCanvas->currentLayer()->srs().toProj4()));
      qDebug( qPrintable( cvDestinationCRS->toProj4()) );
    }
    
    QgsPoint lvTransformedLocation = lvTransformer.transform( mCanvas->getCoordinateTransform( )->toMapCoordinates( mouseEvent->x( ), mouseEvent->y( ) ) );
    
    emit keyValuePair( "Egt.GPS.Longitude", QString::number( lvTransformedLocation.x( ) ) );
    emit keyValuePair( "Egt.GPS.Latitude", QString::number( lvTransformedLocation.y( ) ) );
  }
  else
  {
    QMessageBox::warning( mCanvas, tr( "Warning" ), tr( "No active layers found" ) );
  }
}
