/*
** File: egtpathbuilder.h
** Author(s): Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2008-09-26
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
#ifndef EGTPATHBUILDER_H
#define EGTPATHBUILDER_H

#include <QModelIndex>

/*! \brief Builds absolute paths from a QModelIndex
 *
 */
class EgtPathBuilder
{

  public:
    /*! \brief Build an absolute path for the selected item */
    QString buildPath(const QModelIndex& theIndex);

};
#endif
