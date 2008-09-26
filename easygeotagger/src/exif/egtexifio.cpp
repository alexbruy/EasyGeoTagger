#include "egtexifio.h"

#include "egtlogger.h"
#include <math.h>

#include <cassert>

#include <QFileInfo>
#include <QDir>


EgtExifIO::EgtExifIO( QString theImageFilename )
{
  setFile( theImageFilename );
}

EgtExifIO::EgtExifIO( const QModelIndex& theIndex )
{
  setFile( buildPath( theIndex ) );
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */
//This is a real hack to prove a concept
float EgtExifIO::getLatitude()
{
  EgtDebug( "entered" );
  
  const Exiv2::Value& lvValue = read( "Exif.GPSInfo.GPSLatitudeRef" );
        
      
  Exiv2::TypeId lvTypeId = lvValue.typeId ();
  
  if(lvTypeId == Exiv2::invalidTypeId)
    return 0.0;  

  int lvNorthing = 1;

  if(QString::compare( QString(lvValue.toString().c_str()), "S" ) == 0)
    lvNorthing = -1;
      
  const Exiv2::Value & lvValue2 = read( "Exif.GPSInfo.GPSLatitude" );
   
  lvTypeId = lvValue2.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
    return 0.0;  
  
  float lvLatitude = 0.0;
  
  for(int i=lvValue2.count()-1; i>=0; i--)
  {
  	lvLatitude+= lvValue2.toFloat(i);
  	if(i>0)
  		lvLatitude/= 60;
  }
  
     
  return lvLatitude;
}

//This is a real hack to prove a concept
float EgtExifIO::getLongitude()
{  
  EgtDebug( "entered" );
  
  const Exiv2::Value& lvValue = read( "Exif.GPSInfo.GPSLongitudeRef" );   
  Exiv2::TypeId lvTypeId = lvValue.typeId ();
  
  if(lvTypeId == Exiv2::invalidTypeId)
    return 0.0;  

  int lvNorthing = 1;

  if(QString::compare( QString(lvValue.toString().c_str()), "S" ) == 0)
    lvNorthing = -1;
      
  const Exiv2::Value & lvValue2 = read( "Exif.GPSInfo.GPSLongitude" );
   
  lvTypeId = lvValue2.typeId ();   
  if(lvTypeId == Exiv2::invalidTypeId)
    return 0.0;  
  
  float lvLatitude = 0.0;
  
  for(int i=lvValue2.count()-1; i>=0; i--)
  {
  	lvLatitude+= lvValue2.toFloat(i);
  	if(i>0)
  		lvLatitude/= 60;
  }
  
     
  return lvLatitude;
}

bool EgtExifIO::hasGpsExif()
{
  return cvHasGpsExif;
}

bool EgtExifIO::isValidImage()
{
  return cvIsValidImage;
}

void EgtExifIO::setFile( QString theImageFilename )
{
  EgtDebug( "entered" );
  cvImageFile = theImageFilename;
  cvIsValidImage = false;
  cvHasGpsExif = false;
  try 
  {
    cvImage = Exiv2::ImageFactory::open( theImageFilename.toStdString() );
    assert( cvImage.get() != 0 );
    cvIsValidImage = true;
    try 
    {
      cvImage->readMetadata();
      //TODO: Change this statement, no longer works with Exiv2 v0.18
      
      Exiv2::ExifData::iterator it = cvImage->exifData().findIfdIdIdx( Exiv2::gpsIfdId, 1 );
      if( it != cvImage->exifData().end() )
      {
        cvHasGpsExif = true;
      }
      
      
      
    }
    catch ( Exiv2::AnyError& e )
    {
      EgtDebug( QString( "Error caught ["+ QString( e.what() ) +"]" ).unicode() );
    }
  }
  catch ( Exiv2::AnyError& e )
  {
    EgtDebug( QString( "Error caught ["+ QString( e.what() ) +"]" ).unicode() );
  }
}

bool EgtExifIO::setLongitude( QString theValue )
{
  EgtDebug( "entered" );
	bool ok;
	float lvLongitude=theValue.toFloat( &ok );
	if( !ok )
		return false;
	if( lvLongitude < 0 )
	{
		ok = write( "Exif.GPSInfo.GPSLongitudeRef", "W" );
		if( !ok )
			return false;
	}
	else
	{
		ok = write( "Exif.GPSInfo.GPSLongitudeRef", "E" );
		if( !ok )
			return false;
	}
	
	//TODO:convert from degrees
	return write( "Exif.GPSInfo.GPSLongitude", theValue );
}

bool EgtExifIO::setLatitude( QString theValue )
{
	EgtDebug( "entered" );
  bool ok; 
	float lvLongitude=theValue.toFloat( &ok );
	if( !ok )
		return false;
	if( lvLongitude < 0 )
	{
		ok = write( "Exif.GPSInfo.GPSLatitudeRef", "S" );
		if( !ok )
			return false;
	}
	else
	{
		ok = write( "Exif.GPSInfo.GPSLatitudeRef", "N" );
		if( !ok )
			return false;
	}
	
	//TODO:convert from degrees
  return write( "Exif.GPSInfo.GPSLatitude", theValue );
}

bool EgtExifIO::write( QString theKey, QString theString )
{
  EgtDebug( "entered" );
  
  try 
  {
    Exiv2::ExifData exifData;
    Exiv2::ExifKey key( theKey.toStdString() );
      
    QString lvTypeName = exifData[theKey.toStdString()].typeName(); 
    if( QString::compare( lvTypeName, "Ascii" ) == 0 )
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create( Exiv2::asciiString );
     rv->read( theString.toStdString() );
     exifData.add( key, rv.get() );
    }
    else if( QString::compare( lvTypeName, "Rational" ) == 0 )
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create( Exiv2::unsignedRational );
     rv->read( theString.toStdString() );
     exifData.add( key, rv.get() );
    }
    else if( QString::compare( lvTypeName, "SRational" ) == 0 )
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create( Exiv2::signedRational );
     rv->read( theString.toStdString() );
     exifData.add( key, rv.get() );
    }
    else if( QString::compare( lvTypeName, "Short" ) == 0 )
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create( Exiv2::unsignedShort );
     rv->read( theString.toStdString() );
     exifData.add( key, rv.get() );
    }
    else if( QString::compare( lvTypeName, "SShort" ) == 0 )
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create( Exiv2::signedShort );
     rv->read( theString.toStdString() );
     exifData.add( key, rv.get() );
    }
    else if( QString::compare( lvTypeName, "Byte" ) == 0 )
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create( Exiv2::unsignedByte );
     rv->read( theString.toStdString() );
     exifData.add( key, rv.get() );
    }
    else if( QString::compare( lvTypeName, "SByte" ) == 0 )
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create( Exiv2::signedByte );
     rv->read( theString.toStdString() );
     exifData.add( key, rv.get() );
    }
    else if( QString::compare( lvTypeName, "Long" ) == 0)
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create( Exiv2::unsignedLong );
     rv->read( theString.toStdString() );
     exifData.add( key, rv.get() );
    }
    else if( QString::compare( lvTypeName, "SLong" ) == 0)
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create( Exiv2::signedLong  );
     rv->read( theString.toStdString() );
     exifData.add( key, rv.get() );
    }
    else //Undefined
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create( Exiv2::undefined );
     rv->read( theString.toStdString() );
     exifData.add( key, rv.get() );
    }
      
    // Writing the exif data to the image file
    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open( cvImageFile.toStdString() );
    assert(image.get() != 0);

    image->setExifData( exifData );
    image->writeMetadata();
    return true;      
  }
  catch ( Exiv2::AnyError& e )
  {
    EgtDebug( QString( "Error caught ["+ QString( e.what() ) +"]" ).unicode() );
    return false;
  }
}

/*
 *
 * PRIVATE FUNCTIONS
 *
 */
QString EgtExifIO::buildPath(const QModelIndex& theIndex)
{
  if(!theIndex.isValid()) { return ""; }
  
  if(!theIndex.parent().isValid())
  {
    #ifdef WIN32
    return theIndex.data().toString();
    #else
    return "";
    #endif
  }
  
  return buildPath( theIndex.parent() ) + QDir::toNativeSeparators( "/" ) + theIndex.data().toString();
}


const Exiv2::Value& EgtExifIO::read(QString theKey)
{
  EgtDebug( "entered" );
  
  Exiv2::DataValue lvNotValidValue(Exiv2::invalidTypeId );
  Exiv2::Value& lvNotValid = lvNotValidValue;
  
  if( cvIsValidImage )
  {
    try 
    {
      EgtDebug( "Valid image, reading..." );
      cvImage->readMetadata();
      Exiv2::ExifKey lvKey( theKey.toStdString() );
      Exiv2::ExifData::iterator it = cvImage->exifData().findKey( lvKey );
      if( it == cvImage->exifData().end() )
      {
        EgtDebug( QString( "key ["+ theKey + "] no found" ).unicode() );
        return lvNotValid;
      }

      EgtDebug( QString( "key ["+ theKey + "] found" ).unicode() );
      return it->value();
    }
    catch (Exiv2::AnyError& e)
    {
      EgtDebug( QString( "Error caught ["+ QString( e.what() ) +"]" ).unicode() );
      return lvNotValid;
    }
  }
  
  EgtDebug( "Image is not valid" );
  return lvNotValid; 

}


QString EgtExifIO::readKeyValueAsString(QString theKey)
{
  EgtDebug( "entered" );
  if( cvIsValidImage )
  {
    try 
    {
      EgtDebug( "Valid image, reading..." );
      cvImage->readMetadata();
      Exiv2::ExifKey lvKey( theKey.toStdString() );
      Exiv2::ExifData::iterator it = cvImage->exifData().findKey( lvKey );
      if( it == cvImage->exifData().end() )
      {
        EgtDebug( QString( "key ["+ theKey + "] no found" ).unicode() );
        return "";
      }

      EgtDebug( QString( "key ["+ theKey + "] found" ).unicode() );
      QString lvIteratorValue( it->value().toString().c_str() );
      return lvIteratorValue;
    }
    catch (Exiv2::AnyError& e)
    {
      EgtDebug( QString( "Error caught ["+ QString( e.what() ) +"]" ).unicode() );
      return "";
    }
  }
  
  EgtDebug( "Image is not valid" );
  return "";
}

float EgtExifIO::tokenizeCoordinate(QString theString)
{
  EgtDebug( "entered" );
  int lvValue1 = 0;
  bool lvValue1Done = false;
  int lvValue2 = 0;
  bool lvNumberFound = false;
  float lvCoordinate = 0.0;
  bool lvFirstRationalDone = false;

  for(int i=0; i<theString.length(); i++)
  {
    if( theString[i].digitValue()!= -1 )
    {
      if(!lvValue1Done)
      {
        lvNumberFound = true;
        lvValue1=theString[i].digitValue()+lvValue1*10;
      }
      else
      {
        lvValue2=theString[i].digitValue()+lvValue2*10;
      }   
    }
    
    else if( theString[i] =='/' )
    {
      lvValue1Done = true;
    }
    
    else if( theString[i] == ' ' && lvNumberFound )
    {
      lvCoordinate = lvCoordinate + lvValue1/lvValue2;
      if(lvFirstRationalDone)
      {
        lvCoordinate = lvCoordinate / 60.0;
      }
      lvFirstRationalDone = true;
      lvValue1 = 0;
      lvValue2 = 0;
      lvValue1Done = false;
    }
  }

  lvCoordinate = lvCoordinate + lvValue1/lvValue2;
  lvCoordinate = lvCoordinate / 60.0;
          
  return lvCoordinate;
}
