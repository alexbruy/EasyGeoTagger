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
#include "egtrawthread.h"

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
  cvHasThumbnail = false;
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

/*!
 * \param isValid optional parameter to return if the scaled image is valid
 * \returns A copy of the resized image or an invalid image if the original image has not been resize yet
 */
QImage EgtImageEngine::scaledImage( bool* isValid )
{
  EgtDebug( "entered" );
  if( !cvIsValidImage || !cvHasBeenResized )
  {
    EgtDebug( "A valid image has not been loaded yet or the original has not been resized yet" );
    if( 0 != isValid ) { *isValid = false; }
    return QImage();
  }
  
  if( 0 != isValid ) { *isValid = true; }
  return cvResizedImage;
} 

/*!
 * \param theWidth the width of the new image
 * \param theHeight the height of the new image
 * \param isValid optional parameter to return if the scaled image is valid
 * \returns A new image resized to the specific dimensions or a blank image if a valid image as not been opened
 */
QImage EgtImageEngine::scaleImage(int theWidth,int theHeight, bool* isValid )
{
  EgtDebug( "entered" );
  
  //Check to see if the last read image was sucessful
  //TODO: Send back generic image showing error message
  if( !cvIsValidImage )
  {
    EgtDebug( "A valid image has not been loaded yet" );
    if( 0 != isValid ) { *isValid = false; }
    return QImage();
  }
  
  cvResizedImage = cvOriginalImage->scaled(theWidth, theHeight, Qt::KeepAspectRatio );
  cvHasBeenResized = !cvResizedImage.isNull();
  
  if( 0 != isValid ) { *isValid = cvHasBeenResized; }
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
    readTiff( theImageFilename );
  }
  else if( theImageFilename.endsWith("jpg", Qt::CaseInsensitive) || theImageFilename.endsWith( "jpeg", Qt::CaseInsensitive ) )
  {
    readJpeg( theImageFilename );
  }
  else
  {
    readRaw( theImageFilename );
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
bool EgtImageEngine::preprocessRaw( QString theImageFilename )
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


//This does not actually work -- first the "thumbnail" is the full size image
/*
  //Unpack the thumbnail
  lvErrorCode = cvRawProcessor.unpack_thumb();
  if( LIBRAW_SUCCESS != lvErrorCode )
  {
    cvHasThumbnail = false;
    cvThumbnailImage = QImage();
    EgtDebug( "Unable to unpack thumbnail: "+  QString( libraw_strerror( lvErrorCode ) ) );
    if( LIBRAW_FATAL_ERROR( lvErrorCode ) ) { return false; }
  }
  else 
  {
    cvHasThumbnail = true;
    cvThumbnailImage = QImage( ( int )cvRawProcessor.imgdata.thumbnail.twidth, ( int )cvRawProcessor.imgdata.thumbnail.theight, QImage::Format_RGB32 );
    
    int lvOffset = 0;
    int lvHeight = ( int )cvRawProcessor.imgdata.thumbnail.theight;
    int lvWidth = ( int )cvRawProcessor.imgdata.thumbnail.twidth;
    char* lvImage = cvRawProcessor.imgdata.thumbnail.thumb;
    for( int lvY = 0; lvY < lvHeight; lvY++ )
    {
      for( int lvX = 0; lvX < lvWidth; lvX++ )
      {
        lvOffset = (lvY * lvWidth * 3 ) + ( lvX * 3 );
        cvThumbnailImage.setPixel( lvX, lvY, lvImage[ lvOffset ] << 16 | lvImage[ lvOffset+1 ] << 8 | lvImage[ lvOffset+2 ]  );
      }
    }
    cvThumbnailImage.save("/home/pete/test.jpg");
  }
*/

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
 
/*!
 * \param theImageFilename absolute path and filename of the image to open
 */
void EgtImageEngine::readJpeg( QString theImageFilename )
{
  EgtDebug( "entered" );
  cvIsValidImage = false;
  cvIsProcessing = true;

 
  emit( progress( 0, 1, 0) );
  cvOriginalImage = new QImage( theImageFilename, "JPG" );
  emit( progress( 0, 1, 1) );

  cvIsProcessing = false;
  cvIsValidImage = !cvOriginalImage->isNull();
 
  emit( imageLoaded( cvIsValidImage ) );
}

/*!
 * \param theImageFilename absolute path and filename of the image to open
 */
void EgtImageEngine::readRaw( QString theImageFilename )
{
  EgtDebug( "entered" );
  cvIsValidImage = false;
  cvIsProcessing = true;

  if( !preprocessRaw( theImageFilename ) ) { emit( imageLoaded( cvIsValidImage ) ); }
//slot thread complete
//connect thread complete to finish signal from thread
//emit image loaded
  //Read the image data then place it into a QImage
  emit( progress( 0, 0, 0) );
  cvOriginalImage = new QImage();
  EgtRawThread lvRawThread( &cvRawProcessor,cvOriginalImage );


  connect( &lvRawThread, SIGNAL( progressThread( int, int, int ) ),this , SLOT( reEmitProgress( int, int, int ) ) );
  connect( &lvRawThread, SIGNAL( rawReady( bool ) ),this , SLOT( threadComplete( bool ) ) );

  lvRawThread.start();
  lvRawThread.wait(); //This statement should not exist

}

/*!
 * \param theImageFilename absolute path and filename of the image to open
 */
void EgtImageEngine::readTiff( QString theImageFilename )
{
  EgtDebug( "entered" );
  cvIsValidImage = false;
  cvIsProcessing = true;  
  emit( progress( 0, 1, 0) );
  cvOriginalImage = new QImage( theImageFilename, "TIFF" );
  emit( progress( 0, 1, 1) );
 

  cvIsProcessing = false;
  cvIsValidImage = !cvOriginalImage->isNull();
 
  emit( imageLoaded( cvIsValidImage ) );
}

/*!
 * \param theMinimum the minumum value for the progress bar
 * \param theMaxumum the maximum value for the progress bar
 * \param theProgress the current progress
 */
void EgtImageEngine::reEmitProgress(int theMinimum, int theMaximum, int theProgress )
{
  emit( progress( theMinimum, theMaximum, theProgress) );
}

void EgtImageEngine::threadComplete( bool theError )
{
  cvIsProcessing = false;
  cvIsValidImage = theError;
  emit( imageLoaded( theError ) );
}
