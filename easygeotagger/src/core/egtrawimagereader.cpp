/*
** File: egtrawimagereader.cpp
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

#include "egtrawimagereader.h"
#include "egtlogger.h"

 EgtRawImageReader::EgtRawImageReader(  QImage* theImage, QString theFileName )
 {
   cvImage = theImage;
   cvIsValid = true;
   cvFileName = theFileName;
 }

 EgtRawImageReader::~EgtRawImageReader()
 {
 }

/*!
 * \returns whether the image is valid or not.
 */
bool EgtRawImageReader::isValid()
{
  return cvIsValid;
}

 void EgtRawImageReader::run()
 {
   if( !preprocessRaw( cvFileName ) ) 
   { 
     cvIsValid = false;
     emit( rawReady( cvIsValid ) ); 
   }

   int lvError;
   libraw_processed_image_t* lvImage = cvRawProcessor.dcraw_make_mem_image( &lvError );
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
       cvIsValid = false;
       emit( rawReady( cvIsValid ) );
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
        emit( progress( 0, lvImage->height, lvImage->height ) );
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
    cvIsValid = false;
    EgtDebug( "Unable to make mem_image: "+  QString( libraw_strerror( lvError ) ) );
  }

  free( lvImage );
  cvRawProcessor.recycle();
  emit( rawReady( cvIsValid ) );
}

/*!
 * \param theImageFilename absolute path and filename of the image to open
 */
bool EgtRawImageReader::preprocessRaw( QString theImageFilename )
{
/*
 * This function is largely modeled after examples provided with LibRaw
 */
  EgtDebug( "entered" );
  
  //Try to open the file
  int lvErrorCode = cvRawProcessor.open_file( theImageFilename.toLocal8Bit() );
  if( LIBRAW_SUCCESS != lvErrorCode )
  {
      EgtDebug( "["+ theImageFilename +"] failed to open-> "+  libraw_strerror( lvErrorCode ) );
      cvRawProcessor.recycle();
      return false;
  }
  
  //Unpack the image data
  lvErrorCode = cvRawProcessor.unpack();
  if( LIBRAW_SUCCESS != lvErrorCode)
  {
    EgtDebug( "["+ theImageFilename +"] failed to unpack-> "+  libraw_strerror( lvErrorCode ) );
    cvRawProcessor.recycle(); 
    return false;
  }
  
  EgtDebug( "Unpacked: "+ theImageFilename );
  EgtDebug( "Camera: "+ QString( cvRawProcessor.imgdata.idata.make ) );
  EgtDebug( "Width: "+ QString::number( cvRawProcessor.imgdata.sizes.width ) +"\tHeight: "+ QString::number( cvRawProcessor.imgdata.sizes.height ) );


  //Process data -- not totally sure this is necessary here
  lvErrorCode = cvRawProcessor.dcraw_process();       
  if(LIBRAW_SUCCESS != lvErrorCode)
  {
    EgtDebug( "Unable to process image: "+  QString( libraw_strerror( lvErrorCode ) ) );
    cvRawProcessor.recycle(); 
    return false; 
  }
  
  return true;
}
