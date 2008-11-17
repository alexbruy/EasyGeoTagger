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

EgtRawImageReader::EgtRawImageReader( )
{
  cvAbort = false;
  cvFileName = "";
  cvImageLoaded = false;
  cvIsInitialized = false;
}

EgtRawImageReader::~EgtRawImageReader()
{
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */
 
void EgtRawImageReader::abort()
{ 
  EgtDebug( "entered" );
  cvAbort = true; 
}

QImage EgtRawImageReader::image() const
{
  if( !cvImageLoaded ) { return QImage(); }

  return cvImage;
}

bool EgtRawImageReader::recycle()
{
  if( isRunning() ) { return false; }

  cvImage = QImage();
  return true;
}

void EgtRawImageReader::run()
{
  EgtDebug( "entered" );

  cvImageLoaded = false;
  if( !cvIsInitialized )
  {
    EgtDebug( "thread has not been initialized" );
    emit( progress( 0, 1, 0 ) );
    emit( finished( false ) );
  }
  else
  {
    if( processRaw( cvFileName ) )
    {
      EgtDebug( "processing complete" );
      cvImageLoaded = true;
      emit( progress( 0, 1, 0 ) );
      emit( finished( true ) );
    }
    else
    { 
      EgtDebug( "an error has occurred or the process was aborted" );
      emit( progress( 0, 1, 0 ) );
      emit( finished( false ) );
    }
  }
  
  EgtDebug( "cleaning up" );
  //Reset the parameters at the end of the thread's run
  cvIsInitialized = false;
  cvRawProcessor.recycle();
}

/*!
 * \param theImage pointer back to the EgtImageEngine's QImage to be filled with data
 * \param theFileName absolute path and filename of the image to open
 */
void EgtRawImageReader::setFile( QString theFileName )
{
  EgtDebug( "entered" );

  //Make sure we are not actually running
  if( isRunning() )
  {
    cvAbort = true;
  }

  cvFileName = theFileName;
  if( cvFileName != "" )
  {
    cvIsInitialized = true;
    cvAbort = false; //Should we test again to see if the thread is still running?
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
bool EgtRawImageReader::processRaw( QString theImageFilename )
{
/*
 * This function is largely modeled after examples provided with LibRaw
 */
  EgtDebug( "entered" );
  
  //Check to see if abort flag has been set
  if( cvAbort ) { return false; }
  
  //Try to open the file
  int lvErrorCode = cvRawProcessor.open_file( theImageFilename.toLocal8Bit() );
  if( LIBRAW_SUCCESS != lvErrorCode )
  {
      EgtDebug( "["+ theImageFilename +"] failed to open-> "+  libraw_strerror( lvErrorCode ) );
      cvRawProcessor.recycle();
      return false;
  }
  //Check to see if abort flag has been set
  if( cvAbort ) { return false; }
  
  //Unpack the image data
  lvErrorCode = cvRawProcessor.unpack();
  if( LIBRAW_SUCCESS != lvErrorCode)
  {
    EgtDebug( "["+ theImageFilename +"] failed to unpack-> "+  libraw_strerror( lvErrorCode ) );
    cvRawProcessor.recycle(); 
    return false;
  }
  //Check to see if abort flag has been set
  if( cvAbort ) { return false; }
  
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
  //Check to see if abort flag has been set
  if( cvAbort ) { return false; }
  
  libraw_processed_image_t* lvImage = cvRawProcessor.dcraw_make_mem_image( &lvErrorCode );
  if( 0 != lvImage )
  {

    cvImage = QImage( lvImage->width, lvImage->height, QImage::Format_RGB32 );
    if( cvImage.isNull() )
    {
      EgtDebug( "Unable to allocate memory for cvOriginalImage" );
      free( lvImage );
      return false;
    }

    if( LIBRAW_IMAGE_BITMAP == lvImage->type )
    {
      //Check to see if abort flag has been set
      if( cvAbort ) 
      { 
        free( lvImage ); 
        return false; 
      }
      
      int lvOffset = 0;
      
      if( lvImage->colors == 3 )
      {
        //Will this actually work if bits = 1?
        for( int lvY = 0; lvY < lvImage->height; lvY++ )
        {
          //Check to see if abort flag has been set
          if( cvAbort ) 
          { 
            free( lvImage ); 
            return false; 
          }
          
          emit( progress( 0, lvImage->height, lvY ) );
          for( int lvX = 0; lvX < lvImage->width; lvX++ )
          {
            lvOffset = (lvY * lvImage->width * 3 ) + ( lvX * 3 );
            cvImage.setPixel( lvX, lvY, lvImage->data[ lvOffset ] << 16 | lvImage->data[ lvOffset+1 ]<<8|lvImage->data[ lvOffset+2 ]  );
  
          }
        }
        emit( progress( 0, lvImage->height, lvImage->height ) );
      }
    }
    else
    {
      //PROCESS AS JPEG - How exactly will that be different?
      free( lvImage );
      return false;
    }
  }
  else
  {
    EgtDebug( "Unable to make mem_image: "+  QString( libraw_strerror( lvErrorCode ) ) );
    free( lvImage );
    return false;
  }
  
  free( lvImage );
  return true;
}
