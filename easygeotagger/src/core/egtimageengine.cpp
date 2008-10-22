/*
** File: egtimageengine.cpp
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

#include "egtimageengine.h"
#include "egtlogger.h"

#include <QFileInfo>
#include <QDir>

EgtImageEngine::EgtImageEngine( )
{
  init();
}

EgtImageEngine::EgtImageEngine( QString theFile )
{
  init();
  setFile( theFile );
}

EgtImageEngine::~EgtImageEngine()
{
}

void EgtImageEngine::init()
{
  cvOriginalImage = 0;
  cvHasBeenResized = false;
  cvIsValidImage = false;
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */
/** 
* Return a copy of the most recently resized version of the image.
* 
*/
//TODO: rename this function
/*!
 * \returns A copy of the resized image or an invalid image if the original image has not been resize yet
 */
QImage EgtImageEngine::image()
{
  EgtDebug( "entered" );
  if( !cvIsValidImage || !cvHasBeenResized )
  {
    EgtDebug( "A valid image has not been loaded yet or the original has not been resized yet" );
    return QImage();
  }
  
  return cvResizedImage;
} 

//TODO: rename this function
/*!
 * \param theWidth the width of the new image
 * \param theHeight the height of the new image
 * \returns A new image resized to the specific dimensions or a blank image if a valid image as not been opened
 */
QImage EgtImageEngine::image(int theWidth,int theHeight)
{
  EgtDebug( "entered" );
  
  //Check to see if the last read image was sucessful
  //TODO: Send back generic image showing error message
  if( !cvIsValidImage )
  {
    EgtDebug( "A valid image has not been loaded yet" );
    return QImage(theWidth, theHeight, QImage::Format_RGB32);
  }
  
  cvResizedImage = cvOriginalImage->scaled(theWidth, theHeight, Qt::KeepAspectRatio );
  cvHasBeenResized = !cvResizedImage.isNull();
  return cvResizedImage;
}

/*!
 * \param theOutputFile absolute path and filename in which to save the resized image
 * \returns true on success, false on failure
 */
bool EgtImageEngine::saveAsJpeg( QString theOutputFile )
{
  if( !cvIsValidImage || !cvHasBeenResized )
  {
    EgtDebug( "A valid image has not been loaded or the original has not been resized yet" );
    return false;
  }
  
  return cvResizedImage.save( theOutputFile, "JPG", 100 );
}

/*!
 * \param theImageFilename absolute path and filename of the image to open
 */
void EgtImageEngine::setFile( QString theImageFilename )
{
  EgtDebug( "entered" );
  
  //Reset resized flag
  cvHasBeenResized = false;
  
  //If we were given a dir just bail
  QFileInfo lvFileInfo( theImageFilename );
  if( lvFileInfo.isDir() )
  {
    EgtDebug( "received directory not image...bailing" );
    return;
  }
  
  
  //TODO: Eventually there will have to be a switch here to figure out which read() to call
  //Maybe try reading the magic number?
  
  if( theImageFilename.endsWith("tif", Qt::CaseInsensitive) || theImageFilename.endsWith( "tiff", Qt::CaseInsensitive ) )
  {
    cvIsValidImage = readTiff( theImageFilename );
  }
  else if( theImageFilename.endsWith("jpg", Qt::CaseInsensitive) || theImageFilename.endsWith( "jpeg", Qt::CaseInsensitive ) )
  {
    cvIsValidImage = readJpeg( theImageFilename );
  }
  else
  {
    cvIsValidImage = readRaw( theImageFilename );
  }
  
}

/*
 *
 * PRIVATE FUNCTIONS
 *
 */
/*!
 * \param theImageFilename absolute path and filename of the image to open
 */
bool EgtImageEngine::readJpeg( QString theImageFilename )
{
  EgtDebug( "entered" );
  
  emit( progress( 0, 1, 0) );
  cvOriginalImage = new QImage( theImageFilename, "JPG" );
  emit( progress( 0, 1, 1) );
  
  return !cvOriginalImage->isNull();
}

/*!
 * \param theImageFilename absolute path and filename of the image to open
 */
bool EgtImageEngine::readRaw( QString theImageFilename )
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
  
  //Read the image data then place it into a QImage
  emit( progress( 0, 0, 0) );
  libraw_processed_image_t* lvImage = cvRawProcessor.dcraw_make_mem_image( &lvErrorCode );
  if( 0 != lvImage )
  {
    if( 0 != cvOriginalImage )
    {
      free( cvOriginalImage );
    }
    cvOriginalImage = new QImage(lvImage->width, lvImage->height, QImage::Format_RGB32 );
    if( cvOriginalImage->isNull() )
    {
      EgtDebug( "Unable to allocate memory for cvOriginalImage" );
      return false;
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
            cvOriginalImage->setPixel( lvX, lvY, lvImage->data[ lvOffset ] << 16 | lvImage->data[ lvOffset+1 ] << 8 | lvImage->data[ lvOffset+2 ]  );
          }
        }
        emit( progress( 0, lvImage->height, lvImage->height ) );
      }
    }
    else
    {
      //PROCESS AS JPEG - How exactly will that be different?
      return false;
    }
  }
  else
  {
      EgtDebug( "Unable to make mem_image: "+  QString( libraw_strerror( lvErrorCode ) ) );
  }

  cvRawProcessor.recycle();
  return true;
}

/*!
 * \param theImageFilename absolute path and filename of the image to open
 */
bool EgtImageEngine::readTiff( QString theImageFilename )
{
  EgtDebug( "entered" );
  
  emit( progress( 0, 1, 0) );
  cvOriginalImage = new QImage( theImageFilename, "TIFF" );
  emit( progress( 0, 1, 1) );
  
  return !cvOriginalImage->isNull();
}

