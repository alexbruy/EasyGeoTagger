/*
** File: egtgoogledisplaywidget.cpp
** Author(s): Peter J. Ersts (ersts at amnh.org)
** Creation Date: 2009-02-10
**
** Copyright (c) 2009, American Museum of Natural History. All rights reserved.
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
#include "egtgoogledisplaywidget.h"

#include "egtlogger.h"
#include "ui_egtgoogledisplaywidgetbase.h"

#include <QWebPage>
#include <QWebFrame>

EgtGoogleDisplayWidget::EgtGoogleDisplayWidget( QWidget* theParent )
    : QWidget( theParent ), ui( new Ui::EgtGoogleDisplayWidgetBase )
{
  ui->setupUi( this );
  setWindowIcon( QIcon( ":/24/icons/24/google.png" ) );
  ui->wvBrowser->load( QUrl( "http://biodiversityinformatics.amnh.org/open_source/easygeotagger/googlemap/" ) );
  connect ( ui->pbtnSaveCoordinates, SIGNAL( clicked() ), this, SLOT( saveCoordinates() ) );
}

void EgtGoogleDisplayWidget::saveCoordinates()
{
  QWebFrame* lvFrame = ui->wvBrowser->page()->mainFrame();
  QString lvLatitude = lvFrame->evaluateJavaScript("document.getElementById('latitude').value").toString();
  QString lvLongitude = lvFrame->evaluateJavaScript("document.getElementById('longitude').value").toString();
  emit keyValuePair( "Egt.GPS.Latitude", lvLatitude );
  emit keyValuePair( "Egt.GPS.Longitude", lvLongitude );
}
