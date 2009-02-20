/*
** File: egtexifengine.h
** Author(s): Roberto Garcia-Yunta, Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2008-09-22
**
** Copyright (c) 2008-2009, American Museum of Natural History. All rights reserved.
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
#include <QStringList>
#include <QVariant>
#include <QString>
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

  /*! \brief A struct to hold a mappings between the Egt key namespace
   * and common names for keys to be displayed in the gui
   */
  struct KeyMap
  {
      QString key;
      QString commonName;
      QStringList units;
  };

  /*! \brief Constructor */
  EgtExifEngine();
  
  /*! \brief Constructor */
  EgtExifEngine( QString );
  
  /*! \brief Constructor */
  EgtExifEngine( const QModelIndex& );

  /*! \brief Gets the dependency for an  Egt key
   *
   * \param theKey The Egt keys for which you want the dependency
   * \returns the dependent Egt key
   */
  QString dependency( QString theKey ) { return cvDependencies.value( theKey ); }

  /*! \brief Check to see if the current file has EXIF data expected by this engine */
  bool hasExpectedExif() { return cvHasExpectedExif; }

  /*! \brief Check if a key exists in the EXIF header */
  bool hasKey( QString );

  /*! \brief Is the image valid? */
  bool isValidImage();

  /*! \brief Convience method that check both hasExpectedExif() and IsValidImage() */
  bool isValidImageWithExpectedExif() { return cvIsValidImage && cvHasExpectedExif; }

  /*! \brief Returns a constant reference to the key map list for this engine */
  const QList< EgtExifEngine::KeyMap > &keys() { return cvKeys; }

  /*! \brief Returns the last error */
  const QString &lastError() { return cvLastError; }

  /*! \brief Generic read statement to be implemented by child classes */
  virtual QVariant read( QString theKey, bool* isValid = 0 ) { if ( 0 != isValid ) { *isValid = false; } return QVariant(); }

  /*! \brief Base read method that returns the raw Exiv value for the provided EXIF key as a QString */
  QString readKeyValueAsString(QString);

  /*! \brief Base read method that returns the raw Exiv value for the provided EXIF key */
  const Exiv2::Value& readTag( QString );

   /*! \brief Sets the image to be managed with this class */
  void setFile( QString theFileName );

  /*! \brief Generic write statement to be implemented by child classes */
  virtual bool write( QString theKey, QString theValue ) { return false; }

  /*! \brief Base write method that actually writes data base to the image header */
  bool writeTag(QString, QString, QString);

protected:
  /*! \brief Dependency list that maps one Egt Key to another Egt Key */
  QMap< QString, QString > cvDependencies;

  /*! \brief Flag indicating if the expected EXIF data was found in the current file */
  bool cvHasExpectedExif;

  /*! \brief Exiv2 image pointer */
  Exiv2::Image::AutoPtr cvImage;
  
  /*! \brief The current file */
  QString  cvImageFileName;
  
  /*! \brief Flag to indicate if the current file is a valid image valid */
  bool cvIsValidImage;

  /*! \brief List of Egt keys accessible by the engine */
  QList< KeyMap > cvKeys;

  /*! \brief Last error */
  QString cvLastError;
  
  /*! \brief Exiv2 data used in case we have to work with invalid data */
  Exiv2::DataValue cvNotValidValue;


  /*! \brief Adds key maps to the egt keys list */
  void addKey( QString, QString, QString theUnits="" );

  /*! \brief Method call from setFile that actually trys to open a file and also check to see if it is a valid image */
  void openFile( QString theFileName );
};
#endif
