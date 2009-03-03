/***************************************************************************
      qgsexifprovider.cpp  -  Data provider for imagery with GPS data stored in EXIF headers
                             -------------------
    begin                : 2008-09-17
    copyright            : ( C ) 2008 by Peter J. Ersts
    email                : ersts at amnh.org
    
    Heavily relied on qgsdelimitedtextprovider.h/cpp - Data provider for delimted text
    originally written by Gary E. Sherman as a guide and template
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *( at your option ) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsexifprovider.h"

#include "qgsrectangle.h"
#include "qgslogger.h"
#include "qgsapplication.h"
#include "qgsdataprovider.h"
#include "qgscoordinatereferencesystem.h"

#include <QDir>
#include <QSettings>

static const QString TEXT_PROVIDER_KEY = "exif";
static const QString TEXT_PROVIDER_DESCRIPTION = "EXIF data provider";

QgsExifProvider::QgsExifProvider( QString theDir )
    : QgsVectorDataProvider( theDir )
{
  QgsDebugMsg( "Checking source directory: " + theDir );
  mCurrentFeatureIndex = -1; 
  
  //Set the source directory
  mSourceDirectory.setPath( theDir );
  
  // Set the selection rectangle to null
  mSelectionRectangle = QgsRectangle( );
  
  // assume the layer is invalid until proven otherwise
  mValid = false;
  
  //Validate directory
  if( !mSourceDirectory.exists( ) || !mSourceDirectory.isReadable( ) )
  {
    QgsDebugMsg( "Directory does not exist or is not readable" );
    return;
  }
  QgsDebugMsg( "Valid and readable directory" );
  
  // set the initial extent
  mExtent = QgsRectangle( );
  
  //Get only the files in the directory
  QStringList myFileList = mSourceDirectory.entryList( QDir::Files );
  
  double myLongitude = 0;
  double myLatitude = 0;
  bool firstPoint = true;
  for( int iterator = 0; iterator < myFileList.size( ); iterator++ )
  {
    QgsDebugMsg( "Parsing file: " + myFileList[iterator] );
    mExifEngine.setFile( mSourceDirectory.absolutePath( ) + QDir::toNativeSeparators( "/" ) + myFileList[iterator] );
    if( mExifEngine.isValidImage( ) && mExifEngine.hasExpectedExif( ) )
    {
      myLongitude = mExifEngine.longitude( );
      myLatitude = mExifEngine.latitude( );
      mValidImageList << myFileList[iterator];
    }
    else
    {
      QgsDebugMsg( "Error caught: "+  mExifEngine.lastError( ) );
      continue;
    }
    
    if ( !firstPoint )
    {
      mExtent.combineExtentWith( myLongitude, myLatitude );
    }
    else
    { 
      // Extent for the first point is just the first point
      mExtent.set( myLongitude, myLatitude, myLongitude, myLatitude );
      firstPoint = false;
      QgsDebugMsg( "Loading keys..." );

      //Hard coded field
      mAttributeFields[0] = QgsField( "Filename" ,QVariant::String, "Text" );

      QList< EgtExifEngine::KeyMap > myKeys = mExifEngine.keys( );
      int myKeyListLength = myKeys.size( );
      for( int myKeyRunner = 0; myKeyRunner < myKeyListLength; myKeyRunner++ )
      {
        //TODO: would like to display the common name not the key in the attribute table
        //TODO: add the fields based on their type rather than just as text
        mAttributeFields[ myKeyRunner + 1 ] = QgsField( myKeys[ myKeyRunner ].key , QVariant::String, "Text" );
      }
    }
  }
  
  if( mValidImageList.size( ) > 0 )
  {
    mValid = true;
  }

  QgsDebugMsg( "Done checking validity" );
}

QgsExifProvider::~QgsExifProvider( )
{

}

QString QgsExifProvider::storageType( ) const
{
  return "EXIF header";
}


bool QgsExifProvider::nextFeature( QgsFeature& feature )
{
  // before we do anything else, assume that there's something wrong with
  // the feature
  feature.setValid( false );

  while( mValidImageList.size( ) >= 1 && mCurrentFeatureIndex < mValidImageList.size( ) )
  { 
    double xCoordinate = 0.0;
    double yCoordinate = 0.0;

    //This is unlikely to be false, but concurency issues could exists - i.e., edit exif after layer created
    mExifEngine.setFile( mSourceDirectory.absolutePath( ) + QDir::toNativeSeparators( "/" ) + mValidImageList[mCurrentFeatureIndex] );
    if( !mExifEngine.isValidImage( ) || !mExifEngine.hasExpectedExif( ) )
    {
      mCurrentFeatureIndex++;
      continue;
    }

    xCoordinate = mExifEngine.longitude( );
    yCoordinate = mExifEngine.latitude( );

    // skip the feature if it's out of current bounds
    if ( ! boundsCheck( xCoordinate, yCoordinate ) )
    {
      mCurrentFeatureIndex++;
      continue;
    }
      
    // at this point, one way or another, the current feature values
    // are valid
    feature.setValid( true );

    feature.setFeatureId( mCurrentFeatureIndex );

    //TODO: Tighten up this idea a little --Pulled straight from delimitedtextprovider
    //Seems like there is a more elegant way to do this
    QByteArray  buffer;
    QDataStream s( &buffer, static_cast<QIODevice::OpenMode>( QIODevice::WriteOnly ) ); // open on buffers's data

    switch ( QgsApplication::endian( ) )
    {
      case QgsApplication::NDR :
        // we're on a little-endian platform, so tell the data
        // stream to use that
        s.setByteOrder( QDataStream::LittleEndian );
        s << ( quint8 )1; // 1 is for little-endian
        break;
      case QgsApplication::XDR :
        // don't change byte order since QDataStream is big endian by default
        s << ( quint8 )0; // 0 is for big-endian
        break;
      default :
        qDebug( "%s:%d unknown endian", __FILE__, __LINE__ );
        //delete [] geometry;
        return false;
    }

    s << ( quint32 )QGis::WKBPoint;
    s << xCoordinate;
    s << yCoordinate;

    unsigned char* geometry = new unsigned char[buffer.size( )];
    memcpy( geometry, buffer.data( ), buffer.size( ) );

    feature.setGeometryAndOwnership( geometry, sizeof( wkbPoint ) );
    //End of section to clean up
    mExifEngine.setFile( mSourceDirectory.absolutePath( ) + QDir::toNativeSeparators( "/" ) + mValidImageList[mCurrentFeatureIndex] );
    for ( QgsAttributeList::const_iterator it = mAttributesToFetch.begin( );
          it != mAttributesToFetch.end( );
          ++it )
    {
      if ( it != mAttributesToFetch.begin( ) )
      {
        feature.addAttribute( *it, mExifEngine.read( mAttributeFields[*it].name( ) ) );
      }
      else
      {
        feature.addAttribute( *it, QVariant( mSourceDirectory.absolutePath( ) + QDir::toNativeSeparators( "/" ) + mValidImageList[mCurrentFeatureIndex] ) );
      }

    }

    mCurrentFeatureIndex++;
    return true;
  }

  return false;

} // getNextFeature


void QgsExifProvider::select( QgsAttributeList fetchAttributes,
                                       QgsRectangle rect,
                                       bool fetchGeometry,
                                       bool useIntersect )
{
  QgsDebugMsg( "entered" );
  mSelectionRectangle = rect;
  mAttributesToFetch = fetchAttributes;
  mFetchGeom = fetchGeometry;
  if ( rect.isEmpty( ) )
  {
    QgsDebugMsg( "rect was empty" );
    mSelectionRectangle = mExtent;
  }
  else
  {
    mSelectionRectangle = rect;
  }
  
  rewind( );
}




// Return the extent of the layer
QgsRectangle QgsExifProvider::extent( )
{
  return mExtent;
}

/**
 * Return the feature type
 */
QGis::WkbType QgsExifProvider::geometryType( ) const
{
  return QGis::WKBPoint;
}

/**
 * Return the number of features
 */
long QgsExifProvider::featureCount( ) const
{
  return ( long ) mValidImageList.size( );
}

/**
 * Return the number of fields
 */
uint QgsExifProvider::fieldCount( ) const
{
  return mAttributeFields.size( );
}


const QgsFieldMap & QgsExifProvider::fields( ) const
{
  return mAttributeFields;
}

bool QgsExifProvider::isValid( )
{
  return mValid;
}

/**
 * Check to see if the point is within the selection rectangle
 */
bool QgsExifProvider::boundsCheck( double x, double y )
{
  // no selection rectangle = > always in the bounds
  if ( mSelectionRectangle.isEmpty( ) )
    return true;

  return ( x <= mSelectionRectangle.xMaximum( ) ) && ( x >= mSelectionRectangle.xMinimum( ) ) &&
      ( y <= mSelectionRectangle.yMaximum( ) ) && ( y >= mSelectionRectangle.yMinimum( ) );
}

int QgsExifProvider::capabilities( ) const
{
  return 0;
}


QgsCoordinateReferenceSystem QgsExifProvider::crs( )
{
  // TODO: make provider projection-aware
  return QgsCoordinateReferenceSystem( ); // return default CRS
}

QString  QgsExifProvider::name( ) const
{
  return TEXT_PROVIDER_KEY;
} // ::name( )

QString  QgsExifProvider::description( ) const
{
  return TEXT_PROVIDER_DESCRIPTION;
} //  QgsExifProvider::name( )


/**
 * Class factory to return a pointer to a newly created
 * QgsExifProvider object
 */
QGISEXTERN QgsExifProvider *classFactory( const QString *theDir )
{
  return new QgsExifProvider( *theDir );
}

/** Required key function ( used to map the plugin to a data store type )
*/
QGISEXTERN QString providerKey( )
{
  return TEXT_PROVIDER_KEY;
}

/**
 * Required description function
 */
QGISEXTERN QString description( )
{
  return TEXT_PROVIDER_DESCRIPTION;
}

/**
 * Required isProvider function. Used to determine if this shared library
 * is a data provider plugin
 */
QGISEXTERN bool isProvider( )
{
  return true;
}
