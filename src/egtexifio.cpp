#include "egtexifio.h"

#include <image.hpp>
#include <exif.hpp>
#include <cassert>

#include <QFileInfo>
#include <QDir>

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
  
  return buildPath(theIndex.parent()) + QDir::toNativeSeparators ("/") + theIndex.data().toString();
}

bool EgtExifIO::hasGpsExif(QString theFile)
{
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
  QFileInfo lvFileInfo(thePath);
  return lvFileInfo.isDir();
}

bool EgtExifIO::isDirectory(const QModelIndex& theIndex)
{
  return isDirectory(buildPath(theIndex));
}

bool EgtExifIO::isValidImage(QString theFile)
{
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


