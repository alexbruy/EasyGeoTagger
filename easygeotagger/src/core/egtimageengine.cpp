/*
** File: egtimageengine.cpp
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

#include "egtimageengine.h"
#include "egtlogger.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cassert>

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

bool EgtImageEngine::saveAsJpeg( QString theOutputFile )
{
  if( !cvIsValidImage || !cvHasBeenResized )
  {
    EgtDebug( "A valid image has not been loaded yet or the original has not been resized yet" );
    return false;
  }
  
  return cvResizedImage.save( theOutputFile, "JPG", 100 );
}

void EgtImageEngine::setFile( QString theImageFilename )
{
  EgtDebug( "entered" );
  
  //Reset resized flag
  cvHasBeenResized = false;
  
  //TODO: Eventually there will have to be a switch here to figure out which read() to call
  //Maybe try reading the magic number
  
  //TODO: verify that this is a valid JPG image
  cvIsValidImage = readJpeg( theImageFilename );
}

/*
 *
 * PRIVATE FUNCTIONS
 *
 */

bool EgtImageEngine::readJpeg( QString theImageFilename )
{
  //TODO: function looks like it was taken from jpeg_sample.c written by Janaed Satter, if so credit is needed http://www.cim.mcgill.ca/~junaed/libjpeg.php
  EgtDebug( "entered" );

  /* these are standard libjpeg structures for reading(decompression) */
  struct jpeg_decompress_struct lvCInfo; //TODO: change this variable to something meaningful
  struct jpeg_error_mgr lvJerr; //TODO: change this variable to something meaningful
	
  FILE *lvInfile = fopen( theImageFilename.toStdString().c_str(), "rb" );
	
  if ( 0 == lvInfile )
  {
    EgtDebug( "Error opening jpeg file ["+ theImageFilename +"]" );
    return false;
  }
  
  /* here we set up the standard libjpeg error handler */
  lvCInfo.err = jpeg_std_error( &lvJerr ); //TODO: Use this or remove it
  /* setup decompression process and source, then read JPEG header */
  jpeg_create_decompress( &lvCInfo );
  /* this makes the library read from lvInfile */
  jpeg_stdio_src( &lvCInfo, lvInfile );
  /* reading the image header which contains image information */
  jpeg_read_header( &lvCInfo, TRUE );
	
  EgtDebug( "JPEG File Information:" );
  EgtDebug( "Image width and height: "+ QString::number( lvCInfo.image_width ) +" by "+ QString::number( lvCInfo.image_height ) );
  EgtDebug( "Color components per pixel: "+ QString::number( lvCInfo.num_components ) );
  EgtDebug( "Color space: "+ QString::number( lvCInfo.jpeg_color_space ) );
  
  /* Start decompression jpeg here */
  jpeg_start_decompress( &lvCInfo );
  
  //Allocate the scan line buffer that will be release on exit from the function
  JSAMPARRAY lvScanlineBuffer;
  lvScanlineBuffer = (*lvCInfo.mem->alloc_sarray)((j_common_ptr) &lvCInfo, JPOOL_IMAGE, lvCInfo.output_width * lvCInfo.output_components, 1);
  if( 0 == lvScanlineBuffer )
  {
    EgtDebug( "Error allocating memory for scan line buffer" );
    return false;
  }
  
  //Free the current image if one exists
  if( 0 != cvOriginalImage )
  {
    free( cvOriginalImage );
  }
  //Request new image
  cvOriginalImage = new QImage( lvCInfo.image_width, lvCInfo.image_height, QImage::Format_RGB32 );
  if( 0 == cvOriginalImage )
  {
    EgtDebug( "Error allocating memory for original image" );
    return false;
  }
  
  /* Read one scan line at a time and place into QImage */
  int lvRow = 0;
  while( lvCInfo.output_scanline < lvCInfo.image_height )
  {
    jpeg_read_scanlines( &lvCInfo, lvScanlineBuffer, 1 );
    
    //Load the buffer into the QImage based on the number of components
    if( 3 == lvCInfo.num_components )
    {
      for( int lvIterator=0; lvIterator < lvCInfo.image_width * 3; lvIterator += 3 ) 
      {
        cvOriginalImage->setPixel( lvIterator / 3, lvRow, lvScanlineBuffer[0][lvIterator]<<16|lvScanlineBuffer[0][lvIterator+1]<<8|lvScanlineBuffer[0][lvIterator+2] );
      }
    }
    //Possible?
    else if( 2 == lvCInfo.num_components )
    {
    }
    //Possible?
    else if( 1 == lvCInfo.num_components )
    {
    }
    
    lvRow++;
  }

  /* wrap up decompression, destroy objects, free pointers and close open files */
  jpeg_finish_decompress( &lvCInfo );
  jpeg_destroy_decompress( &lvCInfo );
  fclose( lvInfile );

  return true;
}
