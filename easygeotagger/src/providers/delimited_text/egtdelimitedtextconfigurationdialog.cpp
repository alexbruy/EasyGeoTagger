/*
** File: egtdelimitedtextconfigurationdialog.cpp
** Author( s ): Roberto Garcia Yunta
** Creation Date: 2009-05-13
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
#include "egtdelimitedtextconfigurationdialog.h"
#include "ui_egtdelimitedtextconfigurationdialog.h"


#include <QMessageBox>

EgtDelimitedTextConfigurationDialog::EgtDelimitedTextConfigurationDialog( EgtDelimitedTextProvider* theProvider, QWidget* theParent ): QDialog( theParent ), ui( new Ui::EgtDelimitedTextConfigurationDialogBase )
{
  ui->setupUi( this );
  setWindowIcon( QIcon( ":/icons/EasyGT.svg" ) );

  cvProvider = theProvider;

  connect( ui->buttonBox, SIGNAL( accepted( ) ), this, SLOT( accepted( ) ) );
  connect( ui->buttonBox, SIGNAL( rejected( ) ), this, SLOT( rejected( ) ) ); 
}

void EgtDelimitedTextConfigurationDialog::accepted( )
{
  if( 0 == cvProvider->numberOfRecords() ) 
  {
    QMessageBox::critical( this, tr( "Error" ),tr( "You must have a valid file" ),QMessageBox::Ok );
  }
  else
  {
    cvProvider->notifyInitializationComplete( true ); 
  }
  
}

void EgtDelimitedTextConfigurationDialog::on_cbHeader_stateChanged( )
{
  cvProvider->setHasColumnHeaders( ui->cbHeader->isChecked( ) ); 
  delimiterChanged( );
}

void EgtDelimitedTextConfigurationDialog::on_leCustom_textEdited( )
{
  if( ui->rbtnCustom->isChecked( ) )
  {
    cvProvider->setDelimiter( ui->leCustom->text( ) );
    delimiterChanged( );
  }
}

void EgtDelimitedTextConfigurationDialog::on_rbtnBlank_toggled( bool theValue )
{
  if( theValue )
  {
    cvProvider->setDelimiter( " " );
    delimiterChanged( );
  }
}

void EgtDelimitedTextConfigurationDialog::on_rbtnComma_toggled( bool theValue )
{
  if( theValue )
  {
    cvProvider->setDelimiter( "," );
    delimiterChanged( );
  }
}

void EgtDelimitedTextConfigurationDialog::on_rbtnCustom_toggled( bool theValue )
{
  if( theValue )
  {
    cvProvider->setDelimiter( ui->leCustom->text( ) );
    delimiterChanged( );
  }
}

void EgtDelimitedTextConfigurationDialog::on_rbtnPipe_toggled( bool theValue )
{
  if( theValue )
  {
    cvProvider->setDelimiter( "|" );
    delimiterChanged( );
  }
}

void EgtDelimitedTextConfigurationDialog::rejected( )
{
  cvProvider->notifyInitializationComplete( false );
}

/*
 *
 * PRIVATE FUNCTIONS
 *
 */
void EgtDelimitedTextConfigurationDialog::delimiterChanged( )
{

  cvProvider->reload();

  if( ! cvProvider->lastError().isEmpty( ) )
  {
    QMessageBox::warning( this, tr( "Error" ),cvProvider->lastError() ,QMessageBox::Ok ); 
  }

  QString lvHTML = "<table border = \"1\" cellspacing = \"0\" cellpadding = \"0\" ";
  lvHTML += "width = \"100%\">";

  int lvNumRows = ( cvProvider->numberOfRecords() > 3 )? 3: cvProvider->numberOfRecords(); // We just show up to 3 rows 
   
  if( cvProvider->hasColumnHeaders( ) )
  {
    QStringList lvHeader = cvProvider->columnHeaders(); 
    lvHTML += "<tr bgcolor = \"#4AB02A\">";
    for( int i = 0; i < lvHeader.size( ); i++ )
    {
      lvHTML = lvHTML+"<td>"+lvHeader.at( i )+"</td>";
    }
    lvHTML += "</tr>";
  }

  for( int i = 0; i < lvNumRows; i++ )
  {
    QStringList lvCurrentRecord = cvProvider->next(); 
      
    lvHTML += "<tr>";
    for( int j = 0; j < cvProvider->numberOfFields(); j++ )
    {
      lvHTML = lvHTML+"<td>"+lvCurrentRecord.at( j )+"</td>"; 
    }
    lvHTML += "</tr>";
  }
  lvHTML += "</table>";

  cvProvider->reset();

  ui->tePreview->setHtml( lvHTML );
}


