/*
** File: egtgpsplugin.cpp
** Author( s ): Roberto Garcia Yunta
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

#include "egtgpsplugin.h"
#include "egtlogger.h"

#include <QtPlugin>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QApplication>

EgtGpsPlugin::EgtGpsPlugin( )
{
  cvCategories = QObject::tr( "Coordinate Extraction" );
  cvDescription = QObject::tr( "Reads data from GPS files" );
  cvName = QObject::tr( "GPS Reader" );

    
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */


/*!
 * \param theButton pointer to a QPushButton that is to be connect to the  run slot
 */
void EgtGpsPlugin::connectRunButton( QPushButton* theButton )
{
  connect( theButton, SIGNAL( clicked( ) ), this, SLOT( run( ) ) );
}

void EgtGpsPlugin::initPlugin( )
{
  //Hook listeners into the application interface
  if( 0 != cvApplicationInterface )
  {
    cvDisplayWidget.setVisible( false );
    cvDisplayWidget.setDataTable( &cvDataTableWidget );
    //connect to application interface
    connect( &cvDataTableWidget, SIGNAL( keyValuePair( QString, QString ) ), cvApplicationInterface, SLOT( acceptKeyValuePair( QString, QString ) ) );

    cvDisplayWidget.setApplicationInterface( cvApplicationInterface );

    connect( cvDisplayWidget.cvpbtnOffsetManual, SIGNAL( clicked( ) ), &cvSynchronizeDialog, SLOT( setOffset( ) ) );
    connect( cvDisplayWidget.cvpbtnOffsetPic, SIGNAL( clicked( ) ), &cvSynchronizeDialog, SLOT( setOffsetPic( ) ) );
    connect( cvDisplayWidget.cvpbtnDeleteRow, SIGNAL( clicked() ),&cvDataTableWidget, SLOT( deleteRow() ) );
    connect( cvDisplayWidget.cvpbtnSendCoordinates, SIGNAL( clicked( ) ), &cvDataTableWidget, SLOT( sendCoordinates( ) ) );
    connect( &cvDataTableWidget, SIGNAL( timeStampSelected( bool ) ), &cvDisplayWidget, SLOT( setSynchronizing(bool) ) );
    connect( cvDisplayWidget.cvpbtnOffsetManual, SIGNAL( clicked( ) ), &cvSynchronizeDialog, SLOT( setOffset( ) ) );
    connect( &cvSynchronizeDialog, SIGNAL( offsetSet( int, QString ) ), &cvDataTableWidget, SLOT( setOffsetAndTimeStamp( int, QString ) ) );
    connect( &cvDataTableWidget, SIGNAL( displayButtonsStatus( bool, bool ) ), &cvDisplayWidget, SLOT( setButtonsStatus( bool, bool ) ) );
  }
}

/*
 *
 * SIGNAL and SLOTS
 *
 */
void EgtGpsPlugin::connectConfigurationButton( QPushButton* theButton )
{
  connect( theButton, SIGNAL( clicked( ) ), this, SLOT( showConfigurationPanel( ) ) );
}

void EgtGpsPlugin::run( )
{
  EgtDebug( "entered" );
  
  //Build or reshow the plugins GUI component
  if( cvDisplayWidget.isVisible( ) )
  {
    EgtDebug( "dock is already open and visible" );
    return;
  }

  EgtDebug( "dock is already open but not visible" );

  cvDisplayWidget.move( cvApplicationInterface->positionOfFirstVisibleWidget( ) );
  cvDisplayWidget.show( );
  EgtDebug( "done" );
}


Q_EXPORT_PLUGIN2( gpsplugin, EgtGpsPlugin );


