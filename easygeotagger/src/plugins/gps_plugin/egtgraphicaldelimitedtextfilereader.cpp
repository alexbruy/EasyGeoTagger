/*
** File: 
** Author(s): 
** Creation Date: 
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
#include "egtgraphicaldelimitedtextfilereader.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QTextStream>

EgtGraphicalDelimitedTextFileReader::EgtGraphicalDelimitedTextFileReader(  ):EgtDelimitedTextFileReader()
{
  //Setup the Select Delimiter Dialog
  cvSelectDelimiterDialog.setWindowTitle( tr( "Select Delimiter" ) );
  cvSelectDelimiterDialog.setModal( true );
  cvSelectDelimiterDialog.setLayout( new QVBoxLayout() );
  cvSelectDelimiterDialog.layout()->setSpacing( 1 );
  cvSelectDelimiterDialog.layout()->setContentsMargins( 1, 1, 1, 1 );

  QLabel* lvLabel = new QLabel( &cvSelectDelimiterDialog );
  lvLabel->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  lvLabel->setText("Specify a delimiter:");
  lvLabel->setAlignment( Qt::AlignBottom | Qt::AlignRight );
  cvSelectDelimiterDialog.layout()->addWidget( lvLabel );

  cvDelimiterText = new QTextEdit( &cvSelectDelimiterDialog );
  cvSelectDelimiterDialog.layout()->addWidget( cvDelimiterText );

  QPushButton *lvAcceptButton = new QPushButton( "&Ok", &cvSelectDelimiterDialog );
  cvSelectDelimiterDialog.layout()->addWidget( lvAcceptButton );
  connect( lvAcceptButton, SIGNAL( clicked() ), this, SLOT( lvAcceptButton_clicked() ) ); 
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */

void EgtGraphicalDelimitedTextFileReader::selectDelimiter()
{
  cvSelectDelimiterDialog.show();
}

/*!
 * \param theButton pointer to a QPushButton that is to be connect to the  showConfigureationPanel slot
 */
void EgtGraphicalDelimitedTextFileReader::selectFile()
{
  QString lvFileName = QFileDialog::getOpenFileName(0, tr("Open GPS File"), "/home", tr("GPS Files (*.txt *.gps)"));
  setFileName( lvFileName );
}


/*
 *
 * SIGNAL and SLOTS
 *
 */
void EgtGraphicalDelimitedTextFileReader::lvAcceptButton_clicked()
{
  setDelimiter( cvDelimiterText->toPlainText() );
  cvSelectDelimiterDialog.close();
}




