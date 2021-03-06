/*
** File: egtgpxparser.h
** Author( s ): Peter J. Ersts ( ersts at amnh.org ), Roberto Garcia-Yunta
** Creation Date: 2009-05-08
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
#ifndef EGTGPXPARSER_H
#define EGTGPXPARSER_H

#include <QMap>
#include <QSet>
#include <QList>
#include <QString>
#include <QDomNode>
#include <QStringList>
#include <QDomDocument>

class EgtGpxParser
{
  public:
    enum FeatureTypes
    {
      WayPoints,
      Tracks,
      Routes
    };

    EgtGpxParser();

    QList< QString > headers( );

    QString lastError( ) { return cvLastError; }

    const QList< QMap< QString, QString> > &parse( EgtGpxParser::FeatureTypes );

    const QList< QMap< QString, QString> > &parse( EgtGpxParser::FeatureTypes, QString );

    void setFileName( QString theFileName ) { cvFileName = theFileName; }
 
  
  private:

    void parseRoutes( );
    void parseTracks( );
    void parseWayPoints();
    QMap< QString, QString > parseWayPoint( QDomNode );



    QList< QMap< QString, QString> >  cvData;

    QDomDocument cvGpxDocument;

    QString cvFileName;

    QSet< QString > cvHeaders;

    QString cvLastError;

    QMap<QString, QString> cvGpxWayPointTags;
};
#endif
