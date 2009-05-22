/*
** File: egtsphericalfunctionengine.h
** Author: Peter J. Ersts (ersts  at amnh.org)
** Creation Date: 2009-05-21
**
** This is a quick partial port of Spherical Function Engine ---> http://biodiversityinformatics.amnh.org/open_source/pdc/download
** File: SphericalFunctionEngine.java
** Author: Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2004-11-10
** Revision Date: 2005-11-09
**
** version 1.1_RC_1 [2005-11-09] Release candidate packaged for distribution (P.J.Ersts)
**
** version 1.1_PR [2005-10-19] Modified some parameter names and added additional comments (P.J.Ersts)
**
** version 1.1_PR [2005-10-13] Fix initialBearing(....) so the result is relative to 360 deg (P.J.Ersts)
**
** version 1.0_PR [2004-11-25] (P.J.Ersts)
**
** Copyright (c) 2004,2005 American Museum of Natural History. All rights reserved.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Library General Public
** License as published by the Free Software Foundation; either
** version 2 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.
**
** You should have received a copy of the GNU Library General Public
** License along with this library; if not, write to the
** Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
** MA 02110-1301, USA.
**
** This software is based upon work paritially supported by NASA under award number
** NAG5-12333. Additionally, this program was prepared by the the above author(s)
** under award No. NA04AR4700191 from the National Oceanic and Atmospheric
** Administration, U.S. Department of Commerce.  The statements, findings,
** conclusions, and recommendations are those of the author(s) and do not
** necessarily reflect the views of the National Oceanic and Atmospheric
** Administration or the Department of Commerce.
**
**/

#ifndef EGTSPHERICALFUNCTIONENGINE_H
#define EGTSPHERICALFUNCTIONENGINE_H

#include "math.h"

#include <QString>

class SphericalFunctionEngine
{
  const static int WGS84 = 6378137;

  public:
  SphericalFunctionEngine();

  SphericalFunctionEngine( double );

  double* calculateIntermediateGreatCirclePoints(double longitudePoint1, double latitudePoint1, double longitudePoint2, double latitudePoint2, double fraction);

  double greatCircleDistance(double longitudePoint1, double latitudePoint1, double longitudePoint2, double latitudePoint2, QString units);

  private:
    double toDegrees( double );

    double toRadians( double );



    double spheroidRadius;
};

#endif // EGTSPHERICALFUNCTIONENGINE_H
