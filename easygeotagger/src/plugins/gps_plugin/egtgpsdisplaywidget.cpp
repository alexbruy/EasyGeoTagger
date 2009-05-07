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
  ui->pbtnSendToEditor->setEnabled( false );
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
  
    cvSynchronizeDialog.close();
    theEvent->accept();
 }

void EgtGpsDisplayWidget::setDataTable( EgtGpsDataTableWidget* theDataTable)
{
  EgtDebug( "entered" );

  if( 0 == theDataTable ) { return; }

  cvDataTable = theDataTable;

  QVBoxLayout* lvLayout = new QVBoxLayout;
  ui->widgetTable->setLayout( lvLayout );
  lvLayout->addWidget(cvDataTable);
  lvLayout->setContentsMargins( 1, 1, 1, 1 );

  connect( ui->pbtnDeleteRow, SIGNAL( clicked( ) ), cvDataTable, SLOT( deleteRow( ) ) );
  connect( cvDataTable, SIGNAL(timeStampSelected(bool)), this, SLOT( enableSynchronizationButtons(bool)) );
  connect( cvDataTable, SIGNAL( rowSelected( bool ) ), this, SLOT( enableButtons( bool ) ) );
}


/*
 *
 * SIGNAL and SLOTS
 *
 */


void EgtGpsDisplayWidget::enableButtons( bool isRowSelected)
{
  EgtDebug( "entered" );

  ui->pbtnSendToEditor->setEnabled( isRowSelected );
  ui->pbtnDeleteRow->setEnabled( isRowSelected );

  if( ! cvDataTable->isAnyColumnHeaderSet() )
  {
    ui->pbtnSendToEditor->setEnabled( false );
  }
}

/*!
 * \param theStatus boolean that indicates whether the syncrhonize group box must be enabled or not
 */
void EgtGpsDisplayWidget::enableSynchronizationButtons( bool theStatus )
{
  EgtDebug( "entered" );

  ui->gbTimeOffset->setEnabled(theStatus);
}

void EgtGpsDisplayWidget::on_pbtnSetOffsetWithImage_clicked()
{
  cvSynchronizeDialog.showDialog( true );
  cvSynchronizeDialog.move( this->pos() );
}

void EgtGpsDisplayWidget::on_pbtnSetOffsetManual_clicked()
{
  cvSynchronizeDialog.showDialog( false );
  cvSynchronizeDialog.move( this->pos() );
}

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