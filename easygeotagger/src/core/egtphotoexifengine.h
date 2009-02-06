/*
** File: egtphotoexifengine.h
** Author(s): Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2009-01-13
**
** Copyright (c) 2009, American Museum of Natural History. All rights reserved.
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
#ifndef EGTPHOTOEXIFENGINE_H
#define EGTPHOTOEXIFENGINE_H

#include "egtexifengine.h"

/*! \brief Read and write EXIF data
 *
 * This class is largely a wrapper for Exiv2 to provide simple access to Photo EXIF header data
 */
class MS_DLL_SPEC EgtPhotoExifEngine : public EgtExifEngine
{

public:
  /*! \brief Constructor */
  EgtPhotoExifEngine();

  /*! \brief Constructor */
  EgtPhotoExifEngine( QString );

  /*! \brief Constructor */
  EgtPhotoExifEngine( const QModelIndex& );

  /*! \brief Returns the data associated with the Exif.Photo.DateTimeOriginal tag */
  QString dateTimeOriginal( bool* isValid );

  /*! \brief Generic read statement */
  QVariant read( QString theTag, bool* isValid=0 );

  /*! \brief Set and open a new file */
  void setFile( QString theFileName );

  /*! \brief Generic write statement */
  bool write( QString, QString );

  /*! \brief Writes data to the Exif.Photo.DateTimeOriginal tag */
  bool writeDateTimeOriginal( QString );

};
#endif
