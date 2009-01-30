/*
** File: egtdelimitedtextfilereader.h
** Author(s): Roberto Garcia Yunta
** Creation Date: 2008-12-19
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
#ifndef EGTDELIMITEDTEXFILEREADER_H
#define EGTDELIMITEDTEXFILEREADER_H

#include "egtfilereader.h"

class EgtDelimitedTextFileReader: public EgtFileReader
{
  Q_OBJECT

  public:

    /*! \brief Constuctor */
    EgtDelimitedTextFileReader();
    
    /*! \brief Returns the column headers */
    virtual QStringList columnHeaders();

    /*! \brief Returns whether the file has headers or not */
    virtual bool hasColumnHeaders();

    /*! \brief Function that performs preprocessing actions */
    bool preprocessFile( QString );

    /*! \brief Function used to read a gps file */
    virtual QList<QStringList> read( bool * ok = 0 );

    /*! \brief Function to set the name of the file to be read */
    void setFileName( QString );

    /*! \brief Function to set the text delimiter */
    void setDelimiter( QString );

  protected:
    
    /*! \brief Contains the headers from the file */
    QStringList cvColumnHeaders;

    /*! \brief QString that contains the name of the file to be read */
    QString cvFileName; 

    /*! \brief QString that contains the delimiter */
    QString cvDelimiter;

    /*! \brief Indicates if the file has a header */
    bool cvHasColumnHeaders;

    /*! \brief Contains the last error occurred */
    QString cvLastError;
};
#endif

