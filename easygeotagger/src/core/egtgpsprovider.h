/*
** File: egtgpsprovider.h
** Author( s ): Roberto Garcia Yunta
** Creation Date: 2009-05-22
**
** Copyright ( c ) 2009, American Museum of Natural History. All rights reserved.
** 
** This library/program is free software; you can redistribute it 
** and/or modify it under the terms of the GNU Library General Public
** License as published by the Free Software Foundation; either
** version 2 of the License, or ( at your option ) any later version.
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
#ifndef EGTGPSPROVIDER_H
#define EGTGPSPROVIDER_H


#include "egtdataprovider.h"

class EgtGpsProvider: public EgtDataProvider
{

  public:

    /*! \brief Constuctor */
    EgtGpsProvider( );

    void notifyInitializationComplete( bool );

    EgtDataProvider::ErrorType reload( ) { return read(); }

    /*! \brief Function to set the name of the file to be read */
    EgtDataProvider::ErrorType setFileName( QString );
  protected:

    /*! \brief QString that contains the name of the file to be read */
    QString cvFileName; 

    QString cvFileType;

    EgtDataProvider::ErrorType convert( QString, QString, QString& );

    EgtDataProvider::ErrorType readGpx( QString );

    /*! \brief Generic read function to load the data */
    EgtDataProvider::ErrorType read( );

typedef int (*ConvertFunction)(const char*, const char*, const char*, const char*, const char*, const char*);
ConvertFunction cvConvert;
};
#endif
