/*
** File: egtrawimagereader.h
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

#ifndef EGTRAWIMAGEREADER_H
#define EGTRAWIMAGEREADER_H

#include "libraw/libraw.h"

#include <QThread>
#include <QImage>
#include <QModelIndex>


class EgtRawImageReader : public QThread
{ 
  Q_OBJECT

  public:
    /*! \brief Constructor */
    EgtRawImageReader();
    
    /*! \brief Destructor */
    ~EgtRawImageReader();
    
    /*! \brief Set the abort flag to true and terminate the thread */
    void abort();
    
    /*! \brief Set the pointer to EgtImageEngine's QImage and the absolute path and filename of the raw image to open */
    void init( QImage*, QString );
    
    /*! \brief Required thread function */
    void run();

  private:
    /*! \brief Performs previous operation on the raw file before reading it */
    bool processRaw( QString );
    
    
    
    

    /*! \brief A flag used to break out of the main loops and terminate the thread */
    bool cvAbort;
    
    /*! \brief Absolute path and filename of the image to open */
    QString cvFileName;
    
    /*! \brief Pointer to the EgtImageEngine's QImage that will be loaded with data from the raw image*/
    QImage* cvImage;
    
    /*! \brief LibRaw object to handle the raw data */
    LibRaw cvRawProcessor;
    
    /*! \brief Flag that indicates if the thread has been initialized with valid parameters */
    bool cvIsInitialized;
    
  signals:
    void progress( int, int, int );
    void rawImageProcessed(bool);

};


#endif
