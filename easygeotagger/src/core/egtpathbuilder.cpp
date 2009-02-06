/*
** File: egtpathbuilder.cpp
** Author(s): Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2008-09-26
**
** Copyright (c) 2008-2009, American Museum of Natural History. All rights reserved.
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
#include "egtpathbuilder.h"

#include <QDir>

/*
 *
 * PUBLIC FUNCTIONS
 *
 */
/*!
 * \param theIndex the QModelIndex representing the object for which to build an abolute path
 * \returns a the absolute path to the object represented by the QModeIndex
 */
QString EgtPathBuilder::buildPath(const QModelIndex& theIndex)
{
  if(!theIndex.isValid()) { return ""; }
  
  if(!theIndex.parent().isValid())
  {
    #ifdef WIN32
    return theIndex.data().toString();
    #else
    return "";
    #endif
  }
  
  return buildPath( theIndex.parent() ) + QDir::toNativeSeparators( "/" ) + theIndex.data().toString();
}
