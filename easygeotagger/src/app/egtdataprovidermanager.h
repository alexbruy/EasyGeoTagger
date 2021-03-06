/*
** File: egtdataprovidermanager.h
** Author( s ): Peter J. Ersts ( ersts at amnh.org ), Roberto Garcia-Yunta
** Creation Date: 2009-05-15
**
** Copyright ( c ) 2008-2009, American Museum of Natural History. All rights reserved.
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
#ifndef EGTDATAPROVIDERMANAGER_H
#define EGTDATAPROVIDERMANAGER_H

#include "egtdataprovider.h"

#include <QMap>
#include <QStringList>

/*! \brief EasyGeoTagger data provider manager
 *
 * The provider manager is responsible for loading data provider plugins and acts as a provider factory
 */
class MS_DLL_SPEC EgtDataProviderManager : public QObject
{
  Q_OBJECT 
  
  public:

    /*! \brief Constructor */
    EgtDataProviderManager( );

    /*! \brief Destructor */
    ~EgtDataProviderManager( );

    /*! \brief Get the names of the data providers currently registered with the provider manager */
    QStringList availableProviders( );

    /*! \brief Create a new instance of a data provider */
    EgtDataProvider* provider( QString );

  public slots:
    /*! \brief Load a providers or all providers in a directory */
    void loadProviders( QString );

  signals:
    /*! \brief Signal to indicate which plugin was just loaded */
    void providerLoaded( QString );


  private:
    /*! \brief Load all providers in a directory */
    void loadAllProviders( QString );

    /*! \brief Load a single provider */
    bool loadSingleProvider( QString );



    QString cvDefaultProviderPath;
    QMap < QString, EgtDataProvider* > cvProviders;

};
#endif
