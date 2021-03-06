/*
** File: egtdataprovider.h
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
#ifndef EGTDATAPROVIDER_H
#define EGTDATAPROVIDER_H

#include <QMap>
#include <QList>
#include <QPoint>
#include <QObject>
#include <QString>
#include <QStringList>

/*! \brief Base Data Provider
 *
 * This class is the base data provider and plugin interface
 *
 */
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

    /*! \brief Constructor */
    EgtDataProvider( );

    virtual ~EgtDataProvider( ) { }

    /*! \brief Returns the column headers*/
    QStringList columnHeaders( ) { return cvColumnHeaders; }

    /*! \brief Entry point for derived classes so they can graphically display their options */
    virtual void configure( QPoint thePoint ) { thePoint.x();/* SIP muffler */ emit dataProviderReady( ); }

    /*! \brief Create a new uninitialized instance of this provider */
    virtual EgtDataProvider* instance( ) { return new EgtDataProvider(); }

    /*! \brief Is the provider valid */
    bool isValid() { return cvValid; }

    /*! \returns the last error message */
    QString lastError( ) { return cvLastError; }

    /*! \returns the proivders name */
    QString name( ) { return cvName; }

    /*! \brief Get the next row of data */
    QMap< QString, QString > next( );

    /*! \returns the number of fields */
    int numberOfFields( ) { return cvNumberOfFields; }

    /*! \returns the number of records the provider currently has loaded */
    int numberOfRecords( ) { return cvData.size( ); }

    /*! \brief the the revious row of data */
    QMap< QString, QString > previous( );

    /*! \brief Reset the proivder back to the first record */
    void reset( ) { cvCurrentRecord = -1; }

  signals:
    void dataProviderReady( );

  protected:
    /*! \brief Read function to be implemented by the derrived classes */
    virtual EgtDataProvider::ErrorType read( ) { return EgtDataProvider::None; }

    void initialized( bool );


    QStringList cvColumnHeaders;

    int cvCurrentRecord;

    QList< QMap< QString, QString > > cvData;

    QString cvLastError;

    QString cvName;

    int cvNumberOfFields;

    bool cvValid;
};

Q_DECLARE_INTERFACE( EgtDataProvider, "org.amnh.bif.Plugin.EgtDataProvider/1.0" );

#endif
