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
#include "exifprovidertestgui.h"
#include "qgscontexthelp.h"

//qt includes

//standard includes

ExifProviderTestGui::ExifProviderTestGui( QWidget* parent, Qt::WFlags fl )
    : QDialog( parent, fl )
{
  setupUi( this );
}

ExifProviderTestGui::~ExifProviderTestGui()
{
}

void ExifProviderTestGui::on_buttonBox_accepted()
{
  //close the dialog
  accept();
}

void ExifProviderTestGui::on_buttonBox_clicked(QAbstractButton* theButton)
{
  if(buttonBox->standardButton ( theButton) == QDialogButtonBox::Ok)
  {
    emit draw(leDirectory->text());
    qDebug("Button pushed");
  }
}

void ExifProviderTestGui::on_buttonBox_rejected()
{
  reject();
}

void ExifProviderTestGui::on_buttonBox_helpRequested()
{
  QgsContextHelp::run( context_id );
}

