/*
** File: egtitemdelegate.h
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
#ifndef EGTITEMDELEGATE_H
#define EGTITEMDELEGATE_H

#include <QMap>
#include <QString>
#include <QItemDelegate>

/*! \brief An item deletegate to shade the entries in the file browser.
 *
 * When active, this item delegate will color the back ground red for entries that are valid
 * images but have no gps exif data. A green background will be shown for entries that are
 * valid images and have gps exif data.
 */
class MS_DLL_SPEC EgtItemDelegate : public QItemDelegate
{
  Q_OBJECT

public:
  /*! \brief Constructor */
  EgtItemDelegate();
  
  /*! \brief Painter */
  void paint(QPainter*, const QStyleOptionViewItem&, const QModelIndex&) const;

public slots:
  /*! \brief Slot to set flag indicating the desire to display availability */
  void displayGpsExifAvailability(int);

private:

  bool cvDisplayGpsExifAvailability;
};
#endif
