/*
** File: egtgpxprovider.cpp
** Author( s ): Roberto Garcia Yunta, Peter J. Ersts ( ersts at amnh.org )
** Creation Date: 2009-05-22
**
** Copyright ( c ) 2009, American Museum of Natural History. All rights reserved.
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
#include "egtgpxprovider.h"

#include "egtgpxparser.h"
#include "egtlogger.h"

#include <QtPlugin>
#include <QFileDialog>

EgtGpxProvider::EgtGpxProvider( ) : EgtGpsProvider( )
{
  cvFileName = ""; 
  cvLastError = "";
  cvName = "GPX";
  cvFileType = "gpx";
  ui = new Ui::ConfigurationDialog();
  ui->setupUi( &cvConfigurationDialog );
  connect( ui->buttonBox, SIGNAL( rejected() ), this, SLOT( cancel() ) );
  connect( ui->buttonBox, SIGNAL( accepted() ), this, SLOT( finishConfiguration() ) );
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */
void EgtGpxProvider::configure( QPoint theScreenLocation )
{
  EgtDebug( "entered" );


  QStringList lvFileNames;
  QFileDialog lvFileDialog( 0, tr( "Open GPX File" ), "/home", tr( "GPX Files ( *.gpx  )" ) );
  lvFileDialog.setFileMode( QFileDialog::ExistingFile ); // A single existing file
  lvFileDialog.move( theScreenLocation );
  if ( lvFileDialog.exec() )
  {
    lvFileNames = lvFileDialog.selectedFiles();
  }

  if( ! lvFileNames.isEmpty() )
  {
    QString lvFileName = lvFileNames[0];

    if ( "" != lvFileName )
    {
      setFileName( lvFileName );
    
      cvConfigurationDialog.move( theScreenLocation );
      cvConfigurationDialog.show( );
    }
  }
  else
  {
    cvLastError = tr( "User canceled initialization" );
    initialized( false );
  }
} 

/*
 *
 * PRIVATE FUNCTIONS
 *
 */
void EgtGpxProvider::cancel()
{
  cvLastError = tr( "User canceled initialization" );
  initialized( false );
}

void EgtGpxProvider::finishConfiguration()
{
  if( ui->rbtnRoute->isChecked() )
  {
    cvFeatureType = EgtGpxParser::Routes;
  }
  else if( ui->rbtnTrack->isChecked() )
  {
    cvFeatureType = EgtGpxParser::Tracks;
  }
  else
  {
    cvFeatureType = EgtGpxParser::WayPoints;
  }

  initialized( EgtDataProvider::None ==  read() );
}

Q_EXPORT_PLUGIN2( gpxprovider, EgtGpxProvider );
