/*
** File: egtimageengine.h
** Author(s): Roberto Garcia Yunta
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
#include <stdio.h>
#include <jpeglib.h>
#include <qimage.h>

#include <QModelIndex>

#define DWORD_24BIT_WIDTHBYTES_ALIGN( width ) ((width*3) + ( width % 4 == 0 ? 0 : width % 2 == 0 ? 2 : (width+1) % 4 == 0 ? 3 : 1 ));
#define F_DELTA 0.0001f 


class EgtImageEngine 
{ 
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
    
    bool allocateMemory(int, int);
    float* createCoeff( int, int, int );
    bool readJpegFile();
    void shrinkData();

    int cvBytesPerPixel;   
    J_COLOR_SPACE cvColorSpace; 
    QString cvImageFile;
    
    /** \brief Was the last image opened sucessfully */
    bool cvIsValidImage;
    
    /** \brief Local copy of the current resized image */
    QImage* cvCurrentImage;
    
    int cvOriginalHeight;
    int cvOriginalwidth;
    unsigned char *cvRawImage;
    
    //This variables need to be renamed to something else, maybe just cvNewHeight
    int cvShrinkedHeight;
    int cvShrinkedWidth;
    unsigned char *cvShrinkedRawImage;
};
#endif
