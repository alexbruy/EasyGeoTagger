/*
** File: egtrawthread.cpp
** Author(s): Roberto Garcia Yunta, Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2008-10-28
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

#include "egtrawthread.h"
#include "egtlogger.h"

 EgtRawThread::EgtRawThread( LibRaw* theRawProcessor, QImage* theImage )
 {
   cvImage = theImage;
   cvRawProcessor = theRawProcessor;
   cvErrorCode = 0;
 }

 EgtRawThread::~EgtRawThread()
 {
 }

 void EgtRawThread::run()
 {
   libraw_processed_image_t* lvImage = cvRawProcessor->dcraw_make_mem_image( &cvErrorCode );
   if( 0 != lvImage )
   {
     if( 0 != cvImage )
     {
       free( cvImage );
     }

     cvImage = new QImage( lvImage->width, lvImage->height, QImage::Format_RGB32 );
     if( cvImage->isNull() )
     {
       EgtDebug( "Unable to allocate memory for cvOriginalImage" );
       cvErrorCode = 1;
       return;
     }
 
    if( LIBRAW_IMAGE_BITMAP == lvImage->type )
    {
      int lvOffset = 0;

      if( lvImage->colors == 3 )
      {
        //Will this actually work if bits = 1?
        for( int lvY = 0; lvY < lvImage->height; lvY++ )
        {
          emit( progress( 0, lvImage->height, lvY ) );
          for( int lvX = 0; lvX < lvImage->width; lvX++ )
          {
            lvOffset = (lvY * lvImage->width * 3 ) + ( lvX * 3 );
            cvImage->setPixel( lvX, lvY, lvImage->data[ lvOffset ] << 16 | lvImage->data[ lvOffset+1 ]<<8|lvImage->data[ lvOffset+2 ]  );

          }
        }
        //emit( progress( 0, lvImage->height, lvImage->height ) );
      }
    }
    else
    {
      //PROCESS AS JPEG - How exactly will that be different?
      return;
    }
  }
  else
  {
    cvErrorCode = 1;
    EgtDebug( "Unable to make mem_image: "+  QString( libraw_strerror( cvErrorCode ) ) );
  }

  free( lvImage );
  cvRawProcessor->recycle();
}

/*!
 * \returns 0 in case of success. An integer different to 0 in case of error.
 */
 int EgtRawThread::getErrorCode()
 {
   return cvErrorCode;
 }
