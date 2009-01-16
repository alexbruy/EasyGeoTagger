/*
** File: egtexifengine.h
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
#ifndef EGTEXIFENGINE_H
#define EGTEXIFENGINE_H

#include <QModelIndex>
#include <QVariant>
#include <QList>
#include <QMap>

#include <exiv2/image.hpp>
#include <exiv2/exif.hpp>

/*! \brief Read and write EXIF data
 *
 * This class is largely a wrapper for Exiv2 to provide simple access to EXIF header data
 */
class MS_DLL_SPEC EgtExifEngine
{

public:

  struct KeyMap
  {
      QString key;
      QString commonName;
  };

  /*! \brief Constructor */
  EgtExifEngine();
  
  /*! \brief Constructor */
  EgtExifEngine( QString );
  
  /*! \brief Constructor */
  EgtExifEngine( const QModelIndex& );

  QString dependency( QString theKey ) { return cvDependencies.value( theKey ); }

  bool hasExpectedExif() { return cvHasExpectedExif; }

  bool hasKey( QString );

  /*! \brief is the image valid? */
  bool isValidImage();

  bool isValidImageWithExpectedExif() { return cvIsValidImage && cvHasExpectedExif; }

  const QList< KeyMap > &keys() { return cvKeys; }

  const QString &lastError() { return cvLastError; }

  virtual QVariant read( QString theKey, bool* isValid = 0 ) { if ( 0 != isValid ) { *isValid = false; } return QVariant(); }

  /*! \brief reads the value specified as parameter */
  const Exiv2::Value& readTag(QString);

  /*! \brief reads the value specified as parameter and returns it as a QString */
  QString readKeyValueAsString(QString);

   /*! \brief Sets the image to be managed with this class */
  void setFile( QString theFileName );

  virtual bool write( QString theKey, QString theValue ) { return false; }

  /*! \brief writes exif metadata */
  bool writeTag(QString, QString, QString);

protected:
  QMap< QString, QString > cvDependencies;

  bool cvHasExpectedExif;

  /*! \brief Exiv2 data to handle the image */
  Exiv2::Image::AutoPtr cvImage;
  
  /*! \brief Name of the file picture */
  QString  cvImageFileName;
  
  /*! \brief is the image valid? */
  bool cvIsValidImage;

  QList< KeyMap > cvKeys;

  /*! \brief Last error occured */
  QString cvLastError;
  
  /*! \brief Exiv2 data used in case we have to work with invalid data */
  Exiv2::DataValue cvNotValidValue;



  void addKey( QString, QString );
  void openFile( QString theFileName );
};
#endif

