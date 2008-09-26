#ifndef EGTEXIFIO_H
#define EGTEXIFIO_H

#include <QModelIndex>

#include <image.hpp>
#include <exif.hpp>

class EgtExifIO
{

public:
  EgtExifIO( const QModelIndex& );
  EgtExifIO( QString );
  
  float getLatitude();
  float getLongitude();
  
  bool hasGpsExif();
  bool isValidImage();
  
  void setFile( QString theImageFilename );
  bool setLatitude(QString);
  bool setLongitude(QString);

  
  
  
private:
  QString buildPath(const QModelIndex& theIndex);
  const Exiv2::Value& read(QString);
  QString readKeyValueAsString(QString);
  float tokenizeCoordinate(QString);
  
  QString cvImageFile;
  Exiv2::Image::AutoPtr cvImage;
  bool cvIsValidImage;
  bool cvHasGpsExif;
  bool write(QString, QString);
  
};
#endif
