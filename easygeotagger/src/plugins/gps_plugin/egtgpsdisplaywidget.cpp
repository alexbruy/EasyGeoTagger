/*
** File: egtgpsdisplaywidget.cpp
** Author( s ): Roberto Garcia Yunta
** Creation Date: 2009-03-20
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
#include "egtgpsdisplaywidget.h"

#include "egtlogger.h"
#include "ui_egtgpsdisplaywidgetbase.h"

#include <QMessageBox>
#include <QApplication>
#include <QModelIndex>
#include <QFileDialog>


EgtGpsDisplayWidget::EgtGpsDisplayWidget( QWidget* theParent )
    : QWidget( theParent ), ui( new Ui::EgtGpsDisplayWidgetBase )
{
  ui->setupUi( this );
  setWindowIcon( QIcon( ":/icons/internet-web-browser.svg" ) );

  ui->gbTimeOffset->setEnabled( false );
  ui->pbtnSendCoordinates->setEnabled( false );
  ui->pbtnDeleteRow->setEnabled( false );

  connect( ui->pbtnOpenFile, SIGNAL( clicked( ) ), this, SLOT( openFile( ) ) );
  connect( &cvProviderFactory, SIGNAL( dataProviderCreated( EgtDataProvider* ) ),this, SLOT( setDataProvider( EgtDataProvider* ) ) );

}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */

/*!
 * \param theEvent event received when the user closes the window
 */
void EgtGpsDisplayWidget::closeEvent(QCloseEvent *theEvent)
 {
    EgtDebug( "entered" );
  
    emit closingWindow();
    //cvSynchronizeDialog->close();
    theEvent->accept();
 }


QPushButton* EgtGpsDisplayWidget::getPbtnOffsetManual()
{
  EgtDebug( "entered" );

  return ui->pbtnOffsetManual;
}

QPushButton* EgtGpsDisplayWidget::getPbtnOffsetPic()
{
  EgtDebug( "entered" );

  return ui->pbtnOffsetPic;
}

QPushButton* EgtGpsDisplayWidget::getPbtnDeleteRow()
{
  EgtDebug( "entered" );

  return ui->pbtnDeleteRow;
}
 
QPushButton* EgtGpsDisplayWidget::getPbtnSendCoordinates()
{
  EgtDebug( "entered" );

  return ui-> pbtnSendCoordinates;
}

/*!
 * \param theAppInterface contains the actual application interface used to comunicate with the main app.
 */
void EgtGpsDisplayWidget::setApplicationInterface( EgtApplicationInterface* theAppInterface )
{
  EgtDebug( "entered" );

  cvApplicationInterface = theAppInterface;
  //connect( cvApplicationInterface, SIGNAL( indexSelected( const QModelIndex& ) ), this, SLOT( clicked( const QModelIndex& ) ) );
}

void EgtGpsDisplayWidget::setDataTable( EgtGpsDataTableWidget* theDataTable)
{
  EgtDebug( "entered" );

  cvDataTable = theDataTable;

  QVBoxLayout* lvLayout = new QVBoxLayout;
  ui->widgetTable->setLayout( lvLayout );
  lvLayout->addWidget(cvDataTable);
  lvLayout->setContentsMargins( 1, 1, 1, 1 );
}


/*
 *
 * SIGNAL and SLOTS
 *
 */
void EgtGpsDisplayWidget::openFile( )
{
  EgtDebug( "entered" );

  cvProviderFactory.show( );
}

/*!
 * \param theProvider Pointer to the actual data provider
 */
void EgtGpsDisplayWidget::setDataProvider( EgtDataProvider* theProvider )
{
  EgtDebug( "entered" );

  cvDataTable->setProvider( theProvider );
}

/*!
 * \param theStatus boolean that indicates whether the syncrhonize group box must be enabled or not
 */
void EgtGpsDisplayWidget::setSynchronizing( bool theStatus )
{
  EgtDebug( "entered" );

  ui->gbTimeOffset->setEnabled(theStatus);
}

void EgtGpsDisplayWidget::setButtonsStatus( bool theStatusOfCoordinates, bool theStatusOfDelete )
{
  EgtDebug( "entered" );

  ui->pbtnSendCoordinates->setEnabled( theStatusOfCoordinates );
  ui->pbtnDeleteRow->setEnabled( theStatusOfDelete );
}
