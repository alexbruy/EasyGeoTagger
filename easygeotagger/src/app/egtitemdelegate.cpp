/*
** File: egtitemdelegate.cpp
** Author(s): Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2008-09-22
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
#include "egtitemdelegate.h"
#include "egtgpsexifengine.h"

#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <QDir>

EgtItemDelegate::EgtItemDelegate()
{
  cvDisplayGpsExifAvailability = false;
}

/*
 *
 * SIGNAL and SLOTS
 *
 */
void EgtItemDelegate::displayGpsExifAvailability(int theState)
{
  if(theState == 0)
  {
    cvDisplayGpsExifAvailability = false;
  }
  else
  {
    cvDisplayGpsExifAvailability = true;
  }
}

/*
 *
 * PRIVATE FUNCTIONS
 *
 */
void EgtItemDelegate::paint(QPainter* thePainter, const QStyleOptionViewItem& theOption, const QModelIndex& theIndex ) const
{

  QStyleOptionViewItem lvViewOption(theOption);
  //TODO: See if it is posisble to cache this data, tried but found the const to be problematic
  if(cvDisplayGpsExifAvailability && theIndex.column() == 0)
  {
    EgtGPSExifEngine lvExifEngine( theIndex );
    if( lvExifEngine.isValidImage() )
    {
      if( lvExifEngine.hasExpectedExif() )
      {
        QPixmap lvPixmap( lvViewOption.rect.x(),lvViewOption.rect.y() );
        lvPixmap.fill( QColor( 0,255,0,75 ) );
        thePainter->drawPixmap( lvViewOption.rect, lvPixmap );
      }
      else
      {
        QPixmap lvPixmap( lvViewOption.rect.x(),lvViewOption.rect.y() );
        lvPixmap.fill( QColor( 255,0,0,75 ) );
        thePainter->drawPixmap( lvViewOption.rect, lvPixmap );
      }
    }
  }

  QItemDelegate::paint(thePainter, lvViewOption, theIndex);
}

