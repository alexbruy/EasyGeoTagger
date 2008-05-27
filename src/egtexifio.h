#ifndef EGTEXIFIO_H
#define EGTEXIFIO_H

#include <QModelIndex>


class EgtExifIO
{

public:
  EgtExifIO() {}
  
  float getLatitude(QString);
  float getLongitude(QString);
  
  bool hasGpsExif(QString);
  bool hasGpsExif(const QModelIndex&);
  
  bool isDirectory(QString);
  bool isDirectory(const QModelIndex&);
  bool isValidImage(QString);
  bool isValidImage(const QModelIndex&);
  
  
private:
  QString buildPath(const QModelIndex&); 

};
#endif
