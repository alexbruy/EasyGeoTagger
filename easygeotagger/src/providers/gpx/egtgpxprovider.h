/*
** File: egtgpxprovider.h
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
#ifndef EGTGPXPROVIDER_H
#define EGTGPXPROVIDER_H



#include "egtgpsprovider.h"



class EgtGpxProvider: public EgtGpsProvider
{
  Q_OBJECT
  Q_INTERFACES( EgtDataProvider )

  public:

    /*! \brief Constuctor */
    EgtGpxProvider( );

    void configure( );

    EgtDataProvider* instance( ) { return new EgtGpxProvider(); }

    /*! \brief Selects the character that will be considered as delimiter when reading a text file */
    void showConfigurationDialog( );

  protected:

    //EgtKmlConfigurationDialog* cvConfigurationDialog;

 
};
#endif
