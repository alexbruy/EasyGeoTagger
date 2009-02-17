/*
** File: egtgpsplugin.cpp
** Author(s): Roberto Garcia Yunta
** Creation Date: 2008-12-19
**
** Copyright (c) 2008, American Museum of Natural History. All rights reserved.
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
#include "egtapplicationinterface.h"
#include "egtgpsplugin.h"
#include "egtlogger.h"

#include <QtPlugin>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QApplication>

EgtGpsPlugin::EgtGpsPlugin()
{
  cvCategories = QObject::tr( "Coordinate Extraction" );
  cvDescription = QObject::tr( "Reads data from GPS files" );
  cvName = QObject::tr( "GPS Reader" );

  
  cvDataTable = new EgtGpsDataTableWidget();  
  cvMainWidget.setWindowTitle( cvName );
  cvMainWidget.setMinimumSize( 250,150 );

  QWidget* lvPanel = new QWidget();
  QWidget* lvPanelButtons = new QWidget();
  lvPanelButtons->setLayout( new QHBoxLayout() );
  lvPanel->setLayout( new QVBoxLayout() );
  lvPanel->layout()->setContentsMargins( 1, 1, 1, 1 );
  cvTagButton.setText( tr( "Tag picture" ) );
  lvPanel->layout()->addWidget( cvDataTable );
  lvPanelButtons->layout()->addWidget( &cvTagButton );
  cvOpenFileButton.setText( tr( "Open file" ) );
  lvPanelButtons->layout()->addWidget( &cvOpenFileButton );
  lvPanel->layout()->addWidget( lvPanelButtons );

  connect( &cvTagButton, SIGNAL( clicked() ), this, SLOT( cvTagButton_clicked() ) );
  connect( &cvOpenFileButton, SIGNAL( clicked() ), this, SLOT( cvOpenFile_clicked() ) );

  cvMainWidget.setLayout( new QVBoxLayout() );
  cvMainWidget.layout()->addWidget( lvPanel );
  connect(&cvReaderFactory, SIGNAL(fileReaderCreated( EgtFileReader* )),this, SLOT( fileReader_set( EgtFileReader* ) ));
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */

/*!
 * \param theButton pointer to a QPushButton that is to be connect to the  showConfigureationPanel slot
 */
void EgtGpsPlugin::connectConfigurationButton( QPushButton* theButton )
{
  connect( theButton, SIGNAL( clicked() ), this, SLOT( showConfigurationPanel() ) );
}

/*!
 * \param theButton pointer to a QPushButton that is to be connect to the  run slot
 */
void EgtGpsPlugin::connectRunButton( QPushButton* theButton )
{
  connect( theButton, SIGNAL( clicked() ), this, SLOT( run() ) );
}

void EgtGpsPlugin::initPlugin()
{
  //Hook listeners into the application interface
  if( 0 != cvApplicationInterface )
  {
    cvMainWidget.setVisible( false );

    //connect to application interface
    connect( this, SIGNAL( keyValuePair( QString, QString ) ), cvApplicationInterface, SLOT( acceptKeyValuePair( QString, QString ) ) );
  }
}

/*
 *
 * SIGNAL and SLOTS
 *
 */
void EgtGpsPlugin::cvOpenFile_clicked()
{
  cvReaderFactory.show();
}

void EgtGpsPlugin::cvTagButton_clicked() 
{
  if( cvDataTable->isThereAnyColumnSet() )
  {
    QMap<QString,QString>* lvMap = cvDataTable->getRowItems();

    QMapIterator<QString, QString> lvMapIterator(*lvMap);
    while (lvMapIterator.hasNext())
    {
      lvMapIterator.next();
      emit(keyValuePair("Egt.GPS."+lvMapIterator.key(),lvMapIterator.value()));
    }
  }
  else
  {
    QMessageBox::critical( cvDataTable, tr("Error"),tr("At least one header must be set"),QMessageBox::Ok );
  }

}


void EgtGpsPlugin::fileReader_set( EgtFileReader* theFileReader )
{
  cvDataTable->setFileReader( theFileReader );
}

void EgtGpsPlugin::run()
{
  EgtDebug( "entered" );
  
  //Build or reshow the plugins GUI component
  if( cvMainWidget.isVisible() )
  {
    EgtDebug( "dock is already open and visible" );
    return;
  }

  EgtDebug( "dock is already open but not visible" );

  cvMainWidget.move( cvApplicationInterface->positionOfFirstVisibleWidget() );
  cvMainWidget.show();
  EgtDebug( "done" );
}



/*
 *
 * PRIVATE FUNCTIONS
 *
 */


 Q_EXPORT_PLUGIN2( gpsplugin, EgtGpsPlugin );

