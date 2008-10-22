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

#include <exiv2/image.hpp>
#include <exiv2/exif.hpp>

class EgtExifIO
{

public:
  EgtExifIO();
  EgtExifIO( QString );
  EgtExifIO( const QModelIndex& );
  
  double latitude();
  double longitude();
  
  bool hasGpsExif();
  bool isValidImage();

  QString lastError(); 

  void setFile( QString theImageFilename );
  
  bool writeLatitude(double);
  bool writeLatitude( QString );
  
  bool writeLongitude(double );
  bool writeLongitude( QString );
  
  
private:
  QString convertToRational(QString);
  const Exiv2::Value& read(QString);
  QString readKeyValueAsString(QString);
  float tokenizeCoordinate(QString);
  bool write(QString, QString, QString);
  
  Exiv2::Image::AutoPtr cvImage;
  QString cvImageFile;
  
  bool cvIsValidImage;
  bool cvHasGpsExif;
  QString cvLastError;
  Exiv2::DataValue cvNotValidValue;
};
#endif
