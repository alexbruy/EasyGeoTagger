#include "egtexifio.h"

#include "egtlogger.h"

#include <image.hpp>
#include <exif.hpp>
#include <cassert>

#include <QFileInfo>
#include <QDir>

//TODO: Set routines should take in a QMap and where the map key is the EXIF tag/key
//TODO: This class operates far to statically, make it more of a real class constructor takes QString or QModelIndex,
//TODO: Add set file class like QFileInfo


EgtExifIO::EgtExifIO( QString theImageFile )
{
  cvImageFile = theImageFile;
  cvHasGpsExif = hasGpsExif(theImageFile);
  cvIsValidImage = isValidImage(theImageFile);
}

QString EgtExifIO::buildPath(const QModelIndex& theIndex)
{
  //EgtDebug("buildPath()");
  if(!theIndex.isValid()) { return ""; }
  
  if(!theIndex.parent().isValid())
  {
  #ifdef WIN32
  return theIndex.data().toString();
  #else
  return "";
  #endif
  }
  
  return buildPath(theIndex.parent()) + QDir::toNativeSeparators ("/") + theIndex.data().toString();
}

//This is a real hack to prove a concept
float EgtExifIO::getLatitude()
{
  EgtDebug("getLatitude()");
    
  QString lvValue = read("Exif.GPSInfo.GPSLatitudeRef");
      
  if(QString::compare( lvValue, "" ) == 0)
    return 0.0;

  int lvNorthing = 1;
      
  if(QString::compare( lvValue, "S" ) == 0)
    lvNorthing = -1;
      
  lvValue = read("Exif.GPSInfo.GPSLatitude");
      
  if(QString::compare( lvValue, "" ) == 0)
    return 0.0;
      
  return tokenizeCoordinate(lvValue)*lvNorthing;
}

//This is a real hack to prove a concept
float EgtExifIO::getLongitude()
{
  EgtDebug("getLongitude()");
    
      QString lvValue = read("Exif.GPSInfo.GPSLongitudeRef");
      
      if(QString::compare( lvValue, "" ) == 0)
        return 0.0;

      int lvNorthing = 1;
      
      if(QString::compare( lvValue, "W" ) == 0)
        lvNorthing = -1;
      
      lvValue = read("Exif.GPSInfo.GPSLongitude");
      
      if(QString::compare( lvValue, "" ) == 0)
        return 0.0;

      return tokenizeCoordinate(lvValue)*lvNorthing;
}

bool EgtExifIO::hasGpsExif(QString theFile)
{
  EgtDebug("hasGpsExif()");
  try 
  {
    Exiv2::Image::AutoPtr lvImage = Exiv2::ImageFactory::open(qPrintable(theFile));
    assert(lvImage.get() != 0);
    lvImage->readMetadata();
    Exiv2::ExifData::iterator it = lvImage->exifData().findIfdIdIdx(Exiv2::gpsIfdId, 1);
    if(it == lvImage->exifData().end())
      return false;
  }
  catch (Exiv2::AnyError& e)
  {
    return false;
  }
  
  return true;
}

bool EgtExifIO::hasGpsExif(const QModelIndex& theIndex)
{
  return hasGpsExif(buildPath(theIndex));
}

bool EgtExifIO::isDirectory(QString thePath)
{
  EgtDebug("isDirectory()");
  QFileInfo lvFileInfo(thePath);
  return lvFileInfo.isDir();
}

bool EgtExifIO::isDirectory(const QModelIndex& theIndex)
{
  return isDirectory(buildPath(theIndex));
}

bool EgtExifIO::isValidImage(QString theFile)
{
  EgtDebug("isValidImage()");
  try 
  {
    Exiv2::Image::AutoPtr lvImage = Exiv2::ImageFactory::open(qPrintable(theFile));
    assert(lvImage.get() != 0);
  }
  catch (Exiv2::AnyError& e)
  {
    return false;
  }
  
  return true;
}

bool EgtExifIO::isValidImage(const QModelIndex& theIndex)
{
  return isValidImage(buildPath(theIndex));
}

QString EgtExifIO::read(QString theKey)
{
  EgtDebug("read()");
  if(cvHasGpsExif)
  {
    try 
    {
    	std::cout << "reading..." << "\n";
      Exiv2::Image::AutoPtr lvImage = Exiv2::ImageFactory::open( cvImageFile.toStdString() );
      
      assert(lvImage.get() != 0);
      lvImage->readMetadata();
      
      Exiv2::ExifKey lvKey( theKey.toStdString() );
      Exiv2::ExifData::iterator it = lvImage->exifData().findKey(lvKey);
      if(it == lvImage->exifData().end())
        return "";

      QString lvIteratorValue( it->value().toString().c_str() );
      return lvIteratorValue;
    }
    catch (Exiv2::AnyError& e)
    {
      return "";
    }
  }
  
  return "";

}

bool EgtExifIO::setLongitude(QString theValue)
{
	EgtDebug("setLongitude()");
	bool ok;
	float lvLongitude=theValue.toFloat(&ok);
	if(!ok)
		return false;
	if(lvLongitude<0)
	{
		ok = write("Exif.GPSInfo.GPSLongitudeRef", "W");
		if(!ok)
			return false;
	}
	else
	{
		ok = write("Exif.GPSInfo.GPSLongitudeRef", "E");
		if(!ok)
			return false;
	}
	
	//TODO:convert from degrees
	return write("Exif.GPSInfo.GPSLongitude", theValue);
}

bool EgtExifIO::setLatitude(QString theValue)
{
	EgtDebug("setLatitude()");
	float lvLongitude=theValue.toFloat(&ok);
	if(!ok)
		return false;
	if(lvLongitude<0)
	{
		ok = write("Exif.GPSInfo.GPSLatitudeRef", "S");
		if(!ok)
			return false;
	}
	else
	{
		ok = write("Exif.GPSInfo.GPSLatitudeRef", "N");
		if(!ok)
			return false;
	}
	
	//TODO:convert from degrees
  return write("Exif.GPSInfo.GPSLatitude", theValue);
}

float EgtExifIO::tokenizeCoordinate(QString theString)
{
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

bool EgtExifIO::write(QString theKey, QString theString)
{
  EgtDebug("write()");
  
  try 
  {
    Exiv2::ExifData exifData;
    Exiv2::ExifKey key(theKey.toStdString());
      
    QString lvTypeName = exifData[theKey.toStdString()].typeName(); 
    if(QString::compare( lvTypeName, "Ascii" ) == 0)
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create(Exiv2::asciiString);
     rv->read( theString.toStdString() );
     exifData.add(key, rv.get());
    }
    else if(QString::compare( lvTypeName, "Rational" ) == 0)
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create(Exiv2::unsignedRational);
     rv->read( theString.toStdString() );
     exifData.add(key, rv.get());
    }
    else if(QString::compare( lvTypeName, "SRational" ) == 0)
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create(Exiv2::signedRational);
     rv->read( theString.toStdString() );
     exifData.add(key, rv.get());
    }
    else if(QString::compare( lvTypeName, "Short" ) == 0)
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create(Exiv2::unsignedShort);
     rv->read( theString.toStdString() );
     exifData.add(key, rv.get());
    }
    else if(QString::compare( lvTypeName, "SShort" ) == 0)
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create(Exiv2::signedShort  );
     rv->read( theString.toStdString() );
     exifData.add(key, rv.get());
    }
    else if(QString::compare( lvTypeName, "Byte" ) == 0)
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create(Exiv2::unsignedByte );
     rv->read( theString.toStdString() );
     exifData.add(key, rv.get());
    }
    else if(QString::compare( lvTypeName, "SByte" ) == 0)
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create(Exiv2::signedByte );
     rv->read( theString.toStdString() );
     exifData.add(key, rv.get());
    }
    else if(QString::compare( lvTypeName, "Long" ) == 0)
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create(Exiv2::unsignedLong );
     rv->read( theString.toStdString() );
     exifData.add(key, rv.get());
    }
    else if(QString::compare( lvTypeName, "SLong" ) == 0)
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create(Exiv2::signedLong  );
     rv->read( theString.toStdString() );
     exifData.add(key, rv.get());
    }
    else //Undefined
    {
     Exiv2::Value::AutoPtr rv = Exiv2::Value::create(Exiv2::undefined );
     rv->read( theString.toStdString() );
     exifData.add(key, rv.get());
    }
      
    // Writing the exif data to the image file
    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(cvImageFile.toStdString());
    assert(image.get() != 0);

    image->setExifData(exifData);
    image->writeMetadata();
    return true;
      
      
  }
  catch (Exiv2::AnyError& e)
  {
    std::cout << "Caught Exiv2 exception '" << e << "'\n";
    return false;
  }
}
 


