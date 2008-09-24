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
float EgtExifIO::getLatitude(QString theFile)
{
  EgtDebug("getLatitude()");
  if(hasGpsExif(theFile))
  {
    try 
    {
      Exiv2::Image::AutoPtr lvImage = Exiv2::ImageFactory::open(qPrintable(theFile));
      assert(lvImage.get() != 0);
      lvImage->readMetadata();
      
      Exiv2::ExifKey lvKey("Exif.GPSInfo.GPSLatitudeRef");
      Exiv2::ExifData::iterator it = lvImage->exifData().findKey(lvKey);
      if(it == lvImage->exifData().end())
        return 0.0;
      int lvNorthing = 1;
      QString lvIteratorValue(it->value().toString().c_str());
      if(lvIteratorValue.compare("S", Qt::CaseSensitive) == 0)
      {
        lvNorthing = -1;
      }
      
      lvKey = Exiv2::ExifKey("Exif.GPSInfo.GPSLatitude");
      it = lvImage->exifData().findKey(lvKey);
      if(it == lvImage->exifData().end())
        return 0.0;
      EgtDebug(it->value().toString().c_str());
      
      float lvLatitude = 0.0;
      for(int runner = it->value().count() - 1; runner >= 0; runner--)
      {
        lvLatitude = lvLatitude + it->value().toFloat(runner);
        if(runner != 0)
        {
          lvLatitude = lvLatitude / 60.0;
        }
      }
      
      return lvLatitude * lvNorthing;

    }
    catch (Exiv2::AnyError& e)
    {
      return 0.0;
    }
  }
  
  return 0.0;
}

//This is a real hack to prove a concept
float EgtExifIO::getLongitude(QString theFile)
{
  EgtDebug("getLongitude()");
  if(hasGpsExif(theFile))
  {
    try 
    {
      Exiv2::Image::AutoPtr lvImage = Exiv2::ImageFactory::open(qPrintable(theFile));
      assert(lvImage.get() != 0);
      lvImage->readMetadata();
      
      Exiv2::ExifKey lvKey("Exif.GPSInfo.GPSLongitudeRef");
      Exiv2::ExifData::iterator it = lvImage->exifData().findKey(lvKey);
      if(it == lvImage->exifData().end())
        return 0.0;
      int lvEasting = 1;
      QString lvIteratorValue(it->value().toString().c_str());
      if(lvIteratorValue.compare("W", Qt::CaseSensitive) == 0)
      {
        lvEasting = -1;
      }
      
      lvKey = Exiv2::ExifKey("Exif.GPSInfo.GPSLongitude");
      it = lvImage->exifData().findKey(lvKey);
      if(it == lvImage->exifData().end())
        return 0.0;
      EgtDebug(it->value().toString().c_str());
      
      float lvLongitude = 0.0;
      for(int runner = it->value().count() - 1; runner >= 0; runner--)
      {
        lvLongitude = lvLongitude + it->value().toFloat(runner);
        if(runner != 0)
        {
          lvLongitude = lvLongitude / 60.0;
        }
      }
      
      return lvLongitude * lvEasting;

    }
    catch (Exiv2::AnyError& e)
    {
      return 0.0;
    }
  }
  
  return 0.0;
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


