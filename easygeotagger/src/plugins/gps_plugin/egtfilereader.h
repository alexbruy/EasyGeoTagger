/*
** File: egtfilereader.h
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
#ifndef EGTFILEREADER_H
#define EGTFILEREADER_H

#include <QStringList>

class EgtFileReader: public QObject
{
  Q_OBJECT
  public:

    /*! \brief Constructor */
    EgtFileReader(){};

    /*! \brief Function that returns the column headers*/
    virtual QStringList columnHeaders() = 0;

    /*! \brief Returns whether the file has column headers or not */
    virtual bool hasColumnHeaders() = 0;

    /*! \brief Function used to read a gps file */
    virtual QList<QStringList> read( bool* = 0 ) = 0;
 
};
#endif
