/*
** File: egtdelimitedtextprovider.h
** Author( s ): Roberto Garcia Yunta
** Creation Date: 2008-12-19
**
** Copyright ( c ) 2008, American Museum of Natural History. All rights reserved.
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
#ifndef EGTDELIMITEDTEXPROVIDER_H
#define EGTDELIMITEDTEXPROVIDER_H

#include "egtdataprovider.h"
#include "egtdelimitedtextconfigurationdialog.h"

class EgtDelimitedTextConfigurationDialog;

class EgtDelimitedTextProvider: public EgtDataProvider
{
  Q_OBJECT
  Q_INTERFACES( EgtDataProvider )

  public:

    /*! \brief Constuctor */
    EgtDelimitedTextProvider( );

    void configure( );

    void notifyInitializationComplete( bool );

    EgtDataProvider::ErrorType reload( ) { return read(); }

    /*! \brief Function to set the text delimiter */
    void setDelimiter( QString );

    /*! \brief Function to set the name of the file to be read */
    EgtDataProvider::ErrorType setFileName( QString );

    void setHasColumnHeaders( bool );

    /*! \brief Selects the character that will be considered as delimiter when reading a text file */
    void showConfigurationDialog( );

  protected:

    EgtDelimitedTextConfigurationDialog* cvConfigurationDialog;

    /*! \brief QString that contains the delimiter */
    QString cvDelimiter;

    /*! \brief QString that contains the name of the file to be read */
    QString cvFileName; 

    /*! \brief Generic read function to load the data */
    EgtDataProvider::ErrorType read( );
 
};
#endif

