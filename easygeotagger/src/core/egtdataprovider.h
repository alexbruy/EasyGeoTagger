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

#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>

/*! \brief Base Data Provider
 *
 * This class is the base data provider and plugin interface
 *
 * \todo Update next( ) and previous( ) to return QMap< fieldName, QString > or QMap< fieldName, QVariant >
 * \todo Update internal strage of data to reflect the above changes
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

    /*! \brief Does the data file have column headers */
    bool hasColumnHeaders( ) { return cvHasColumnHeaders; }

    /*! \brief Entry point for derived classes so they can graphically display their options */
    virtual void configure( ) { emit dataProviderReady( ); }

    /*! \returns the last error message */
    QString lastError( ) { return cvLastError; }

    /*! \returns the proivders name */
    QString name( ) { return cvName; }

    /*! \brief Get the next row of data */
    QStringList next( );

    /*! \returns the number of fields */
    int numberOfFields( ) { return cvNumberOfFields; }

    /*! \returns the number of records the provider currently has loaded */
    int numberOfRecords( ) { return cvData.size( ); }

    /*! \brief the the revious row of data */
    QStringList previous( );

    /*! \brief Reset the proivder back to the first record */
    void reset( ) { cvCurrentRecord = -1; }

  signals:
    void dataProviderReady( );

  protected:
    /*! \brief Read function to be implemented by the derrived classes */
    virtual EgtDataProvider::ErrorType read( ) { }




    QStringList cvColumnHeaders;

    int cvCurrentRecord;

    QList< QStringList > cvData;

    bool cvHasColumnHeaders;

    QString cvLastError;

    QString cvName;

    int cvNumberOfFields;   
};

Q_DECLARE_INTERFACE( EgtDataProvider, "org.amnh.bif.Plugin.EgtDataProvider/1.0" );

#endif
