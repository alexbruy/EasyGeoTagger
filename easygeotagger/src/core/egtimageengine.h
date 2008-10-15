/*
** File: egtimageengine.h
** Author(s): Roberto Garcia Yunta, Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2008-10-07
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
#ifndef EGTIMAGEENGINE_H
#define EGTIMAGEENGINE_H

#include "libraw/libraw.h"

#include <QImage>
#include <QModelIndex>

class EgtImageEngine : public QObject
{ 
  Q_OBJECT 
  
  public:
    EgtImageEngine( );
    EgtImageEngine( QString );
    ~EgtImageEngine();
    
    QImage image();
    QImage image( int, int );
  
    void setFile( QString );
    bool saveAsJpeg( QString );
  
  private:
    void init();
    
    bool readJpeg( QString );
    bool readRaw( QString );
    bool readTiff( QString );
    
    /** \brief Has the original image been resized */
    bool cvHasBeenResized;
    
    /** \brief Was the last image opened sucessfully */
    bool cvIsValidImage;    
    
    /** \brief Raw image processor */
    LibRaw cvRawProcessor;
    
    /** \brief Local copy of the current resized image */
    QImage cvResizedImage;
    
    /** \brief QImage containing the original image data */
    QImage* cvOriginalImage;
  
  signals:
    void progress( int, int, int );
};
#endif
