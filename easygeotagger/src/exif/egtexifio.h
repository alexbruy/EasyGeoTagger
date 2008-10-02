/*
** File: egtexifio.h
** Author(s): Roberto Garcia-Yunta, Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2008-09-22
**
** Copyright (c) 2008, American Museum of Natural History. All rights reserved.
** 
** This library/program is free software; you can redistribute it 
** and/or modify it under the terms of the GNU Library General Public
** License as published by the Free Software Foundation; either
** version 2 of the License, or (at your option) any later version.
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
  QString convertToRational(QString);
  const Exiv2::Value& read(QString);
  QString readKeyValueAsString(QString);
  float tokenizeCoordinate(QString);
  
  QString cvImageFile;
  Exiv2::Image::AutoPtr cvImage;
  bool cvIsValidImage;
  bool cvHasGpsExif;
  bool write(QString, QString, QString);
  
};
#endif
