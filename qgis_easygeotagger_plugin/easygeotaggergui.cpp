/***************************************************************************
 *   Copyright (C) 2003 by Tim Sutton                                      *
 *   tim@linfiniti.com                                                     *
 *                                                                         *
 *   This is a plugin generated from the QGIS plugin template              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "easygeotaggergui.h"
#include "qgscontexthelp.h"

//qt includes

//standard includes

EasyGeoTaggerGui::EasyGeoTaggerGui( QWidget* parent, Qt::WFlags fl )
    : QDialog( parent, fl )
{
  setupUi( this );
}

EasyGeoTaggerGui::~EasyGeoTaggerGui()
{
}

void EasyGeoTaggerGui::on_buttonBox_accepted()
{
  //close the dialog
  accept();
}

void EasyGeoTaggerGui::on_buttonBox_rejected()
{
  reject();
}

void EasyGeoTaggerGui::on_buttonBox_helpRequested()
{
  QgsContextHelp::run( context_id );
}
