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


EgtImageEngine::EgtImageEngine( QString theFile )
{
  cvRawImage = NULL;
  cvShrinkedRawImage = NULL;
  cvBytesPerPixel = 3; /* or 1 for GRACYSCALE images */
  cvColorSpace = JCS_RGB; /* or JCS_GRAYSCALE for grayscale images */
  setFile( theFile );
  
}

EgtImageEngine::~EgtImageEngine()
{
 free(cvRawImage);
 free(cvShrinkedRawImage);
}

QImage EgtImageEngine::image(int theWidth,int theHeight)
{
  EgtDebug( QString( "Entered in image()." ) );
  if (theWidth > cvOriginalwidth || theHeight > cvOriginalHeight)
  {
    EgtDebug( QString( "The generated image has to be smaller than the original." ) );
    QImage lvNodata;
    return lvNodata;
  }
  if(!(allocateMemory(theWidth,theHeight)))
  {
    QImage lvNodata;
    return lvNodata;
  }
  /*creation of the array that contains the thumbnail in raw format*/
  shrinkData();

  int lvPixelCounter = 0;

  QImage lvImage(cvShrinkedWidth, cvShrinkedHeight, QImage::Format_RGB32);
  for(int i = 0; i< cvShrinkedHeight*cvShrinkedWidth ; i++)
  {
    lvImage.setPixel(i%cvShrinkedWidth,i/cvShrinkedWidth,cvShrinkedRawImage[lvPixelCounter]<<16|cvShrinkedRawImage[lvPixelCounter+1]<<8|cvShrinkedRawImage[lvPixelCounter+2]);
    lvPixelCounter+=3;
  }
  
  return lvImage;
}

void EgtImageEngine::setFile( QString theImageFilename )
{
  EgtDebug( QString( "Entered in setFile()." ) );

  cvImageFile = theImageFilename;
  readJpegFile();
}

bool EgtImageEngine::writeJpegFile( QString theOutputFile )
{
  EgtDebug( QString( "Entered in writeJpegFile()." ) );
  
  struct jpeg_compress_struct lvCInfo;
  struct jpeg_error_mgr lvJerr;
	
  /* this is a pointer to one row of image data */
  JSAMPROW lvRowPointer[1];
  FILE *lvOutFile = fopen( theOutputFile.toStdString().c_str(), "wb" );
	
  if ( lvOutFile  == NULL ) 
  {
    EgtDebug( QString( "Error opening output jpeg file ["+ theOutputFile +"]" ) );
    return false;
  }
  lvCInfo.err = jpeg_std_error( &lvJerr );
  jpeg_create_compress(&lvCInfo);
  jpeg_stdio_dest(&lvCInfo, lvOutFile);

  /* Setting the parameters of the output file here */
  lvCInfo.image_width = cvShrinkedWidth;	
  lvCInfo.image_height = cvShrinkedHeight;
  lvCInfo.input_components = cvBytesPerPixel;
  lvCInfo.in_color_space = cvColorSpace;
  /* default compression parameters, we shouldn't be worried about these */
  jpeg_set_defaults( &lvCInfo );
  /* Now do the compression .. */
  jpeg_start_compress( &lvCInfo, TRUE );
  /* like reading a file, this time write one row at a time */
  while( lvCInfo.next_scanline < lvCInfo.image_height )
  {
    lvRowPointer[0] = &cvShrinkedRawImage[ lvCInfo.next_scanline * lvCInfo.image_width *  lvCInfo.input_components];
    jpeg_write_scanlines( &lvCInfo, lvRowPointer, 1 );
  }
  /* similar to read file, clean up after we're done compressing */
  jpeg_finish_compress( &lvCInfo );
  jpeg_destroy_compress( &lvCInfo );
  fclose( lvOutFile );
  /* success code is 1! */
  return true;
}

bool EgtImageEngine::allocateMemory(int theWidth, int theHeight)
{
  EgtDebug( QString( "Entered in allocateMemory()." ) );
  cvShrinkedHeight = theHeight;
  cvShrinkedWidth = theWidth;
  if(cvShrinkedRawImage!= NULL)
  {
    unsigned char* lvTempShrinkedRawImage;
    lvTempShrinkedRawImage = (unsigned char*)realloc(cvShrinkedRawImage,cvShrinkedHeight*cvShrinkedWidth*sizeof(char)*3);
    if(lvTempShrinkedRawImage == NULL)
    {
      EgtDebug( QString( "Error allocating memory" ) );
      return false;
    }
    cvShrinkedRawImage = lvTempShrinkedRawImage;
  }
  else
  {
    cvShrinkedRawImage = (unsigned char*)malloc(cvShrinkedHeight*cvShrinkedWidth*sizeof(char)*3); 
    if(cvShrinkedRawImage == NULL)
    {
      EgtDebug( QString( "Error allocating memory" ) );
      return false;
    }
  }
  return true;
}

float* EgtImageEngine::createCoeff(int theLength, int theNewLength, int theboolShrink)
{
  EgtDebug( QString( "Entered in createCoeff()." ) );

  int lvSum = 0, lvSum2;
  float *lvRes = (float *)malloc(2 * theLength * sizeof(float));
  if(lvRes == NULL)
  {
    EgtDebug( QString( "Error allocating memory" ) );
    return lvRes;
  }
  float *lvCoeff = lvRes;
  float lvNorm = (theboolShrink)? (float)theNewLength / theLength : 1;
  int lvDenom = (theboolShrink)? theLength : theNewLength;

  memset (lvRes,0.0,2 * theLength * sizeof(float));
  int i;
  for(i = 0; i < theLength; i++, lvCoeff += 2)
  {
    lvSum2 = lvSum + theNewLength;
    if(lvSum2 > theLength)
    {
      *lvCoeff = (float)(theLength - lvSum) / lvDenom;
      lvCoeff[1] = (float)(lvSum2 - theLength) / lvDenom;
      lvSum2 -= theLength;
    }
    else
    {
      *lvCoeff = lvNorm;
      if(lvSum2 == theLength)
      {
        lvCoeff[1] = -1;
        lvSum2 = 0;
      }
    }
    lvSum = lvSum2;
  }

  return lvRes;
}

bool EgtImageEngine::readJpegFile()
{
  EgtDebug( QString( "Entered in readJpegFile()." ) );

  /* these are standard libjpeg structures for reading(decompression) */
  struct jpeg_decompress_struct lvCInfo;
  struct jpeg_error_mgr lvJerr;
  /* libjpeg data structure for storing one row, that is, scanline of an image */
  JSAMPROW lvRowPointer[1];
	
  FILE *lvInfile = fopen( cvImageFile.toStdString().c_str(), "rb" );
  unsigned long lvLocation = 0;
  int i = 0;
	
  if ( lvInfile == NULL )
  {
    EgtDebug( QString( "Error opening jpeg file ["+ cvImageFile +"]" ) );
    return false;
  }
  /* here we set up the standard libjpeg error handler */
  lvCInfo.err = jpeg_std_error( &lvJerr );
  /* setup decompression process and source, then read JPEG header */
  jpeg_create_decompress( &lvCInfo );
  /* this makes the library read from lvInfile */
  jpeg_stdio_src( &lvCInfo, lvInfile );
  /* reading the image header which contains image information */
  jpeg_read_header( &lvCInfo, TRUE );
	
  /*
  EgtDebug( QString( "JPEG File Information:" ) );
  EgtDebug( QString( "Image width and height: "+QString(lvCInfo.image_width)+" pixels and "+QString(lvCInfo.image_height)+" pixels."));
  EgtDebug( QString( "Color components per pixel: "+QString(lvCInfo.num_components) ) );
  EgtDebug( QString( "Color space: "+QString(lvCInfo.jpeg_color_space) ) );
  */
  /*Set up of the dimensions of the image*/
  cvOriginalHeight = lvCInfo.image_height;
  cvOriginalwidth = lvCInfo.image_width;	
  /* Start decompression jpeg here */
  jpeg_start_decompress( &lvCInfo );

  if(cvRawImage!= NULL)
  {
    unsigned char* lvTempRawImage;
    lvTempRawImage = (unsigned char*)realloc(cvRawImage, lvCInfo.output_width*lvCInfo.output_height*lvCInfo.num_components);
    if(lvTempRawImage == NULL)
    {
      EgtDebug( QString( "Error allocating memory" ) );
      return false;
    }
    cvRawImage = lvTempRawImage;
  }
  else
  /* allocate memory to hold the uncompressed image */
  cvRawImage = (unsigned char*)malloc( lvCInfo.output_width*lvCInfo.output_height*lvCInfo.num_components );
  if(cvRawImage == NULL)
  {
    EgtDebug( QString( "Error allocating memory" ) );
    return false;
  }
  /* now actually read the jpeg into the raw buffer */
  lvRowPointer[0] = (unsigned char *)malloc( lvCInfo.output_width*lvCInfo.num_components );
  if(lvRowPointer[0] == NULL)
  {
    EgtDebug( QString( "Error allocating memory" ) );
    return false;
  }
  /* read one scan line at a time */
  while( lvCInfo.output_scanline < lvCInfo.image_height )
  {
    jpeg_read_scanlines( &lvCInfo, lvRowPointer, 1 );
    for( i=0; i<lvCInfo.image_width*lvCInfo.num_components;i++) 
      cvRawImage[lvLocation++] = lvRowPointer[0][i];
  }
  /* wrap up decompression, destroy objects, free pointers and close open files */
  jpeg_finish_decompress( &lvCInfo );
  jpeg_destroy_decompress( &lvCInfo );
  free( lvRowPointer[0] );
  fclose( lvInfile );
  /* yup, we succeeded! */
  return true;
}

void EgtImageEngine::shrinkData()
{
  EgtDebug( QString( "Entered in shrinkData()." ) );

  unsigned char *pLine = cvRawImage, *pPix;
  unsigned char *pOutLine = cvShrinkedRawImage;
  
  unsigned long dwInLn = DWORD_24BIT_WIDTHBYTES_ALIGN(cvOriginalwidth);
  unsigned long dwOutLn = DWORD_24BIT_WIDTHBYTES_ALIGN(cvShrinkedWidth);
  int x, y, i, ii;
  int bCrossRow, bCrossCol;
  float *pRowCoeff = createCoeff(cvOriginalwidth, cvShrinkedWidth, 1); //true
  float *pColCoeff = createCoeff(cvOriginalHeight, cvShrinkedHeight, 1);
  float fTmp, *pXCoeff, *pYCoeff = pColCoeff;
  unsigned long dwBuffLn = 3 * cvShrinkedWidth * sizeof(float);
  float *fBuff = (float *)malloc(6 * cvShrinkedWidth*sizeof(float));
  if(fBuff == NULL)
  {
    EgtDebug( QString( "Error allocating memory" ) );
    return;
  }
  float *fCurrLn = fBuff,
  *fCurrPix,
  *fNextLn = fBuff + 3 * cvShrinkedWidth,
  *fNextPix;
  memset (fBuff,0,2 * dwBuffLn);

  y = 0;
  while(y < cvShrinkedHeight)
  {
    pPix = pLine;
    pLine += dwInLn;

    fCurrPix = fCurrLn;
    fNextPix = fNextLn;

    x = 0;
    pXCoeff = pRowCoeff;
    bCrossRow = pYCoeff[1] > F_DELTA;
    while(x < cvShrinkedWidth)
    {
      fTmp = *pXCoeff * *pYCoeff;
      for(i = 0; i < 3; i++)
        fCurrPix[i] += fTmp * pPix[i];
      bCrossCol = pXCoeff[1] > F_DELTA;
      if(bCrossCol)
      {
        fTmp = pXCoeff[1] * *pYCoeff;
        for(i = 0, ii = 3; i < 3; i++, ii++)
          fCurrPix[ii] += fTmp * pPix[i];
      }

    if(bCrossRow)
    {
      fTmp = *pXCoeff * pYCoeff[1];
      for(i = 0; i < 3; i++)
        fNextPix[i] += fTmp * pPix[i];
      if(bCrossCol)
      {
        fTmp = pXCoeff[1] * pYCoeff[1];
        for(i = 0, ii = 3; i < 3; i++, ii++)
          fNextPix[ii] += fTmp * pPix[i];
      }
    }

    if(fabs(pXCoeff[1]) > F_DELTA)
    {
      x++;
      fCurrPix += 3;
      fNextPix += 3;
    }

    pXCoeff += 2;
    pPix += 3;
  }

  if(fabs(pYCoeff[1]) > F_DELTA)
  {
    // set result line
    fCurrPix = fCurrLn;
    pPix = pOutLine;
    for(i = 3 * cvShrinkedWidth; i > 0; i--, fCurrPix++, pPix++)
      *pPix = (unsigned char)*fCurrPix;

    // prepare line buffers
    fCurrPix = fNextLn;
    fNextLn = fCurrLn;
    fCurrLn = fCurrPix;
    memset (fNextLn,0,dwBuffLn);
    y++;
    pOutLine += dwOutLn;
  }
  pYCoeff += 2;
}

free (pRowCoeff);
free (pColCoeff);
free (fBuff);
}


