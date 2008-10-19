/***************************************************************************
      qgsexifprovider.cpp  -  Data provider for imagery with GPS data stored in EXIF headers
                             -------------------
    begin                : 2008-09-17
    copyright            : (C) 2008 by Peter J. Ersts
    email                : ersts at amnh.org
    
    Heavily relied on qgsdelimitedtextprovider.h/cpp - Data provider for delimted text
    originally written by Gary E. Sherman as a guide and template
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsexifprovider.h"

#include <image.hpp>
#include <exif.hpp>
#include <cassert>

#include <QDir>
#include <QSettings>

#include "qgsrect.h"
#include "qgslogger.h"
#include "qgsapplication.h"
#include "qgsdataprovider.h"
#include "qgscoordinatereferencesystem.h"

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
  mSelectionRectangle = QgsRect();
  
  // assume the layer is invalid until proven otherwise
  mValid = false;
  
  //Validate directory
  if( !mSourceDirectory.exists() || !mSourceDirectory.isReadable() )
  {
    QgsDebugMsg( "Directory does not exist or is not readable" );
    return;
  }
  QgsDebugMsg( "Valid and readable directory" );
  
  // set the initial extent
  mExtent = QgsRect();
  
  //Get only the files in the directory
  QStringList myFileList = mSourceDirectory.entryList(QDir::Files);
  
  double myLongitude = 0;
  double myLatitude = 0;
  bool firstPoint = true;
  for( int iterator = 0; iterator < myFileList.size(); iterator++ )
  {
    QgsDebugMsg( "Parsing file: " + myFileList[iterator] );
    
    if( extractCoordinates( mSourceDirectory.absolutePath() + QDir::toNativeSeparators("/") + myFileList[iterator], &myLongitude, &myLatitude ) )
    {
      mValidImageList << myFileList[iterator];
    }
    else
    {
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
      
      // TODO: Maybe make this a map and try to load from all images
      // Load the keys from this image -- may not match all other images
      loadGpsExifKeys( mSourceDirectory.absolutePath() + QDir::toNativeSeparators("/") + myFileList[iterator] );
      firstPoint = false;
    }
  }
  
  if( mValidImageList.size() > 0 )
  {
    mValid = true;
  }

  QgsDebugMsg( "Done checking validity" );
}

QgsExifProvider::~QgsExifProvider()
{

}

bool QgsExifProvider::extractCoordinates(const QString& theFile, double* theLongitude, double* theLatitude)
{
  int myNorthing = 1;
  int myEasting = 1;
  try 
    {
      Exiv2::Image::AutoPtr myImage = Exiv2::ImageFactory::open( qPrintable( theFile ) );
      assert( myImage.get() != 0 );
      myImage->readMetadata();
      
      /*
       * Get the Longitude
       */
      Exiv2::ExifKey myKey( "Exif.GPSInfo.GPSLongitudeRef" );
      Exiv2::ExifData::iterator it = myImage->exifData().findKey( myKey );
      if( it == myImage->exifData().end() )
      {  
        QgsDebugMsg( "Unable to locate the Exif.GPSInfo.GPSLongitudeRef key" );
        return false;
      }
      
      myEasting = 1;
      QString myIteratorValue( it->value().toString().c_str() );
      if( myIteratorValue.compare( "W", Qt::CaseSensitive ) == 0 )
      {
        myEasting = -1;
      }
      
      myKey = Exiv2::ExifKey("Exif.GPSInfo.GPSLongitude");
      it = myImage->exifData().findKey(myKey);
      if( it == myImage->exifData().end() )
      {
        QgsDebugMsg( "Unable to locate the Exif.GPSInfo.GPSLongitude key" );
        return false;
      }
      
      *theLongitude = 0.0;
      for( int runner = it->value().count() - 1; runner >= 0; runner-- )
      {
        *theLongitude = ( double ) *theLongitude + it->value().toFloat( runner );
        if( runner != 0 )
        {
          *theLongitude = *theLongitude / 60.0;
        }
      }
      *theLongitude = *theLongitude * myEasting;
      
      /*
       * Get the Latitude
       */
      myKey = Exiv2::ExifKey( "Exif.GPSInfo.GPSLatitudeRef" );
      it = myImage->exifData().findKey( myKey );
      if( it == myImage->exifData().end() )
      {  
        QgsDebugMsg( "Unable to locate the Exif.GPSInfo.GPSLatitudeRef key" );
        return false;
      }
      
      myNorthing = 1;
      myIteratorValue = it->value().toString().c_str();
      if( myIteratorValue.compare( "S", Qt::CaseSensitive ) == 0 )
      {
        myNorthing = -1;
      }
      
      myKey = Exiv2::ExifKey("Exif.GPSInfo.GPSLatitude");
      it = myImage->exifData().findKey(myKey);
      if( it == myImage->exifData().end() )
      {
        QgsDebugMsg( "Unable to locate the Exif.GPSInfo.GPSLatitude key" );
        return false;
      }
      
      *theLatitude = 0.0;
      for( int runner = it->value().count() - 1; runner >= 0; runner-- )
      {
        *theLatitude = ( double ) *theLatitude + it->value().toFloat( runner );
        if( runner != 0 )
        {
          *theLatitude = *theLatitude / 60.0;
        }
      }
      *theLatitude = *theLatitude * myNorthing;
      
      
    }
    catch (Exiv2::AnyError& e)
    {
      QgsDebugMsg( "Error caught ["+ QString( e.what() ) +"]" );
      return false;
    }
   
   return true;
}

QString QgsExifProvider::getValueAsString( QString theImageFileName, QString theKey )
{
  QgsDebugMsg( "Retrieving value for key: " + theKey);
  
  if( 0 == theKey.compare("Filename") )
  {
    return theImageFileName;
  }
  
  Exiv2::Image::AutoPtr myImage = Exiv2::ImageFactory::open( qPrintable( theImageFileName ) );
  assert( myImage.get() != 0 );
  myImage->readMetadata();
  Exiv2::ExifData myData = myImage->exifData();
   
  Exiv2::ExifKey myKey( qPrintable( theKey ) );
  Exiv2::ExifData::iterator it = myImage->exifData().findKey( myKey );
  if( it == myImage->exifData().end() )
  {
    QgsDebugMsg( "Unable to locate the " + theKey + " key" );
    return QString( "" );
  }
  QgsDebugMsg( "Retrieved: "+ QString( it->value().toString().c_str() ) );
  
  return QString( it->value().toString().c_str() );
}

void QgsExifProvider::loadGpsExifKeys(QString theImageFileName)
{
  QgsDebugMsg( "Loading keys...");
  //Hard coded field
  mAttributeFields[0] = QgsField( "Filename" ,QVariant::String, "Text"  );
  
  
  Exiv2::Image::AutoPtr myImage = Exiv2::ImageFactory::open( qPrintable( theImageFileName ) );
  assert( myImage.get() != 0 );
  myImage->readMetadata();
  Exiv2::ExifData myData = myImage->exifData();
   
  Exiv2::ExifData::iterator it = myData.findIfdIdIdx(Exiv2::gpsIfdId, 1);
  if(it != myData.end())
  {
    int myKeyCount = 1;
    while(it != myData.end() && it->ifdId() == Exiv2::gpsIfdId)
    {
      mAttributeFields[myKeyCount] = QgsField( QString( it->key().c_str() ),QVariant::String, "Text"  );
      QgsDebugMsg( "Found key: "+ QString( it->key().c_str() ));
      myKeyCount++;
      it++;
    }
  }
  QgsDebugMsg( "Done.");
}

QString QgsExifProvider::storageType() const
{
  return "EXIF header";
}


bool QgsExifProvider::nextFeature( QgsFeature& feature )
{
  // before we do anything else, assume that there's something wrong with
  // the feature
  feature.setValid( false );

  while( mValidImageList.size() >= 1 && mCurrentFeatureIndex < mValidImageList.size() )
  { 
    double xCoordinate = 0.0;
    double yCoordinate = 0.0;

    //This is unlikely to be false, but concurency issues could exists - i.e., edit exif after layer created
    if( !extractCoordinates( mSourceDirectory.absolutePath() + QDir::toNativeSeparators("/") + mValidImageList[mCurrentFeatureIndex], &xCoordinate, &yCoordinate ) )
    {
      mCurrentFeatureIndex++;
      continue;
    }
      
    // skip the feature if it's out of current bounds
    if ( ! boundsCheck( xCoordinate, yCoordinate ) )
    {
      QgsDebugMsg( "Feature out of bound Id=:"+ QString::number( mCurrentFeatureIndex ) );
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

    switch ( QgsApplication::endian() )
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

    unsigned char* geometry = new unsigned char[buffer.size()];
    memcpy( geometry, buffer.data(), buffer.size() );

    feature.setGeometryAndOwnership( geometry, sizeof( wkbPoint ) );
    //End of section to clean up
    
    for ( QgsAttributeList::const_iterator it = mAttributesToFetch.begin();
          it != mAttributesToFetch.end();
          ++it )
    {
      QVariant val(getValueAsString( mSourceDirectory.absolutePath() + QDir::toNativeSeparators("/") + mValidImageList[mCurrentFeatureIndex], mAttributeFields[*it].name() ));
      feature.addAttribute( *it, val );
    }

    mCurrentFeatureIndex++;
    return true;
  }

  return false;

} // getNextFeature


void QgsExifProvider::select( QgsAttributeList fetchAttributes,
                                       QgsRect rect,
                                       bool fetchGeometry,
                                       bool useIntersect )
{
  QgsDebugMsg( "entered" );
  mSelectionRectangle = rect;
  mAttributesToFetch = fetchAttributes;
  mFetchGeom = fetchGeometry;
  if ( rect.isEmpty() )
  {
    QgsDebugMsg( "rect was empty" );
    mSelectionRectangle = mExtent;
  }
  else
  {
    mSelectionRectangle = rect;
  }
  
  begin();
}




// Return the extent of the layer
QgsRect QgsExifProvider::extent()
{
  return mExtent;
}

/**
 * Return the feature type
 */
QGis::WkbType QgsExifProvider::geometryType() const
{
  return QGis::WKBPoint;
}

/**
 * Return the number of features
 */
long QgsExifProvider::featureCount() const
{
  return ( long ) mValidImageList.size();
}

/**
 * Return the number of fields
 */
uint QgsExifProvider::fieldCount() const
{
  return mAttributeFields.size();
}


const QgsFieldMap & QgsExifProvider::fields() const
{
  return mAttributeFields;
}

bool QgsExifProvider::isValid()
{
  return mValid;
}

/**
 * Check to see if the point is within the selection rectangle
 */
bool QgsExifProvider::boundsCheck( double x, double y )
{
  // no selection rectangle => always in the bounds
  if ( mSelectionRectangle.isEmpty() )
    return true;

  return ( x <= mSelectionRectangle.xMax() ) && ( x >= mSelectionRectangle.xMin() ) &&
         ( y <= mSelectionRectangle.yMax() ) && ( y >= mSelectionRectangle.yMin() );
}

int QgsExifProvider::capabilities() const
{
  return 0;
}


QgsCoordinateReferenceSystem QgsExifProvider::getCRS()
{
  // TODO: make provider projection-aware
  return QgsCoordinateReferenceSystem(); // return default CRS
}

QString  QgsExifProvider::name() const
{
  return TEXT_PROVIDER_KEY;
} // ::name()

QString  QgsExifProvider::description() const
{
  return TEXT_PROVIDER_DESCRIPTION;
} //  QgsExifProvider::name()


/**
 * Class factory to return a pointer to a newly created
 * QgsExifProvider object
 */
QGISEXTERN QgsExifProvider *classFactory( const QString *theDir )
{
  return new QgsExifProvider( *theDir );
}

/** Required key function (used to map the plugin to a data store type)
*/
QGISEXTERN QString providerKey()
{
  return TEXT_PROVIDER_KEY;
}

/**
 * Required description function
 */
QGISEXTERN QString description()
{
  return TEXT_PROVIDER_DESCRIPTION;
}

/**
 * Required isProvider function. Used to determine if this shared library
 * is a data provider plugin
 */
QGISEXTERN bool isProvider()
{
  return true;
}
