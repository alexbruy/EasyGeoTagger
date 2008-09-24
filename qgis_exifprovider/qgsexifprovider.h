/***************************************************************************
      qgsexifprovider.h  -  Data provider for imagery with GPS data stored in EXIF headers
                             -------------------
    begin                : 2008-09-17
    copyright            : (C) 2008 by Peter J. Ersts
    email                : ersts at amnh.org
    
    Heavily relied on qgsdelimitedtextprovider.h/cpp - Data provider for delimted text
    originally written by Gary E. Sherman as a guide and template
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsvectordataprovider.h"
#include "qgsfield.h"
#include "qgsfeature.h"

#include <QDir>
#include <QStringList>
#include <QTextStream>

/**
\class QgsExifProvider
\brief Data provider for imagery with GPS data stored in EXIF headers.
*
* The provider only needs the directory which contains
* the image collection
*
*/
class QgsExifProvider : public QgsVectorDataProvider
{
    Q_OBJECT

  public:

    QgsExifProvider( QString theDir = QString() );

    virtual ~QgsExifProvider();

    /* Implementation of functions from QgsVectorDataProvider */

    /**
     * Returns the permanent storage type for this layer as a friendly name.
     */
    virtual QString storageType() const;

    /** Select features based on a bounding rectangle. Features can be retrieved with calls to getNextFeature.
     *  @param fetchAttributes list of attributes which should be fetched
     *  @param rect spatial filter
     *  @param fetchGeometry true if the feature geometry should be fetched
     *  @param useIntersect true if an accurate intersection test should be used,
     *                     false if a test based on bounding box is sufficient
     */
    virtual void select( QgsAttributeList fetchAttributes = QgsAttributeList(),
                         QgsRect rect = QgsRect(),
                         bool fetchGeometry = true,
                         bool useIntersect = false );

    /**
     * Get the next feature resulting from a select operation.
     * @param feature feature which will receive data from the provider
     * @return true when there was a feature to fetch, false when end was hit
     *
     * mFile should be open with the file pointer at the record of the next
     * feature, or EOF.  The feature found on the current line is parsed.
     */
    virtual bool getNextFeature( QgsFeature& feature );

    /**
     * Get feature type.
     * @return int representing the feature type
     */
    virtual QGis::WKBTYPE geometryType() const;

    /**
     * Number of features in the layer
     * @return long containing number of features
     */
    virtual long featureCount() const;

    /**
     * Number of attribute fields for a feature in the layer
     */
    virtual uint fieldCount() const;

    /**
     * Return a map of indexes with field names for this layer
     * @return map of fields
     */
    virtual const QgsFieldMap & fields() const;

    /** Restart reading features from previous select operation */
    virtual void reset();

    /** Returns a bitmask containing the supported capabilities
        Note, some capabilities may change depending on whether
        a spatial filter is active on this provider, so it may
        be prudent to check this value per intended operation.
     */
    virtual int capabilities() const;


    /* Implementation of functions from QgsDataProvider */

    /** return a provider name

        Essentially just returns the provider key.  Should be used to build file
        dialogs so that providers can be shown with their supported types. Thus
        if more than one provider supports a given format, the user is able to
        select a specific provider to open that file.

        @note

        Instead of being pure virtual, might be better to generalize this
        behavior and presume that none of the sub-classes are going to do
        anything strange with regards to their name or description?
     */
    QString name() const;

    /** return description

        Return a terse string describing what the provider is.

        @note

        Instead of being pure virtual, might be better to generalize this
        behavior and presume that none of the sub-classes are going to do
        anything strange with regards to their name or description?
     */
    QString description() const;

    /**
     * Return the extent for this data layer
     */
    virtual QgsRect extent();

    /**
     * Returns true if this is a valid delimited file
     */
    bool isValid();

    virtual QgsCoordinateReferenceSystem getCRS();

    /* new functions */

    /**
     * Check to see if the point is withn the selection
     * rectangle
     * @param x X value of point
     * @param y Y value of point
     * @return True if point is within the rectangle
    */
    bool boundsCheck( double x, double y );





  private:

    //! Fields
    QgsFieldMap mAttributeFields;

    QgsAttributeList mAttributesToFetch;

    QDir mSourceDirectory;
    
    //TODO: Make this into QList<> of a class that already has the EXIF header open
    QStringList mValidImageList;
    
    int mCurrentFeatureIndex;

    //! Layer extent
    QgsRect mExtent;

    //! Current selection rectangle

    QgsRect mSelectionRectangle;

    bool mValid;

    int mGeomType;

    struct wkbPoint
    {
      unsigned char byteOrder;
      quint32 wkbType;
      double x;
      double y;
    };
    wkbPoint mWKBpt;

    bool extractCoordinates(const QString& theFile, double* theLongitude, double* theLatitude);
    QString getValueAsString( QString theImageFileName, QString theKey );
    void loadGpsExifKeys(QString);

};
