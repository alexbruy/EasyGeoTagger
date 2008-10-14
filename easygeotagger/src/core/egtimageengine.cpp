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
    EgtDebug( "A valid image has not been loaded or the original has not been resized yet" );
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

bool EgtImageEngine::readJpeg( QString theImageFilename )
{
  EgtDebug( "entered" );
  cvOriginalImage = new QImage( theImageFilename, "JPG" );
  return !cvOriginalImage->isNull();
}

bool EgtImageEngine::readRaw( QString theImageFilename )
{
  return false;
}

bool EgtImageEngine::readTiff( QString theImageFilename )
{
  EgtDebug( "entered" );
  cvOriginalImage = new QImage( theImageFilename, "TIFF" );
  return !cvOriginalImage->isNull();
}

