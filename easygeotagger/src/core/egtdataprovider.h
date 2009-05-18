/*
** File: egtdataprovider.h
** Author( s ): Roberto Garcia Yunta, Peter J. Ersts ( ersts@amnh.org )
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
#ifndef EGTDATAPROVIDER_H
#define EGTDATAPROVIDER_H

#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>

class MS_DLL_SPEC EgtDataProvider : public QObject
{
  Q_OBJECT

  public:
    enum ErrorType
    {
      None,
      Warning,
      Fatal
    };

    EgtDataProvider();

    virtual ~EgtDataProvider() { }

    /*! \brief Function that returns the column headers*/
    QStringList columnHeaders( );

    /*! \brief Returns whether the file has column headers or not */
    bool hasColumnHeaders( );

    virtual void configure() { emit dataProviderReady(); }

    QString name() { return cvName; }

    QStringList next();

    int numberOfFields( );

    int numberOfRecords();

    QStringList previous();

    void reset();

  signals:

    void dataProviderReady();

  protected:
    virtual EgtDataProvider::ErrorType read( ) { }


    /*! \brief Contains the headers from the file */
    QStringList cvColumnHeaders;

    int cvCurrentRecord;

    QList< QStringList > cvData;

    /*! \brief Indicates if the file has a header */
    bool cvHasColumnHeaders;

    QString cvName;

    int cvNumberOfFields;   
};

Q_DECLARE_INTERFACE( EgtDataProvider, "org.amnh.bif.Plugin.EgtDataProvider/1.0" );

#endif
