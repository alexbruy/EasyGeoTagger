#ifndef EGTEXIFIO_H
#define EGTEXIFIO_H

#include <QModelIndex>


class EgtExifIO
{

public:
  EgtExifIO( QString theImageFile );
  
  float getLatitude();
  float getLongitude();
  
  bool hasGpsExif(QString);
  bool hasGpsExif(const QModelIndex&);
  
  bool isDirectory(QString);
  bool isDirectory(const QModelIndex&);
  bool isValidImage(QString);
  bool isValidImage(const QModelIndex&);
  
  bool setLatitude(QString);
  bool setLongitude(QString);

  
  
  
private:
  QString buildPath(const QModelIndex&); 
  QString read(QString);
  float tokenizeCoordinate(QString);
  QString cvImageFile;
  
  bool cvIsValidImage;
  bool cvHasGpsExif;
  bool write(QString, QString);
  
};
#endif
