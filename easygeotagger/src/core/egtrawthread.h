/*
** File: egtrawthread.h
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

#ifndef EGTRAWTHREAD_H
#define EGTRAWTHREAD_H

#include "libraw/libraw.h"

#include <QThread>
#include <QImage>
#include <QModelIndex>


class EgtRawThread : public QThread
{ 
  Q_OBJECT

  public:
    /*! \brief Constructor */
    EgtRawThread( QImage*, QString );
    /*! \brief Destructor */
    ~EgtRawThread();
    /*! \brief Returns an error code */
    bool isValid();
    /*! \brief runs the thread */
    void run();

  private:
    /*! \brief Performs previous operation on the raw file before reading it */
    bool preprocessRaw( QString );

    QString cvFileName;
    /*! \brief Transformed QImage from the Raw picture */
    QImage* cvImage;
    /*! \brief Integer containing an error code */
    bool cvIsValid;
    /*! \brief LibRaw object to handle the raw data */
    LibRaw cvRawProcessor;
    
  signals:
    void progress( int, int, int );
    void rawReady(bool);

};


#endif
