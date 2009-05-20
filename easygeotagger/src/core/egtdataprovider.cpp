/*
** File: egtdataprovider.cpp
** Author( s ): Roberto Garcia Yunta, Peter J. Ersts ( ersts at amnh.org )
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
#include "egtdataprovider.h"

EgtDataProvider::EgtDataProvider( )
{
  cvCurrentRecord = -1;
  cvHasColumnHeaders = false;
  cvNumberOfFields = 0;
  cvLastError = "";
  cvName = "UNKNOWN";
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */
/*!
 * \returns a QStringList with the next row of data
 */
QStringList EgtDataProvider::next( )
{
  if( cvData.size( ) == 0 || cvCurrentRecord == cvData.size( ) - 1 )
  {
    return QStringList( );
  }

  cvCurrentRecord++;
  return cvData[ cvCurrentRecord ];
}

/*!
 * \returns a QStringList with the prevvious row of data
 */
QStringList EgtDataProvider::previous( )
{
  if( cvData.size( ) == 0 || cvCurrentRecord <= 0 )
  {
    return QStringList( );
  }

  cvCurrentRecord--;
  return cvData[ cvCurrentRecord ];
}

