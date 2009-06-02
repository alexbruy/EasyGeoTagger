/*
** File: egtdelimitedtextprovider.cpp
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
#include "egtdelimitedtextprovider.h"
#include "egtlogger.h"

#include <QFile>
#include <QtPlugin>
#include <QStringList>
#include <QTextStream>
#include <QFileDialog>

EgtDelimitedTextProvider::EgtDelimitedTextProvider( ) : EgtDataProvider( )
{
  cvDelimiter = ",";
  cvFileName = ""; 
  cvLastError = "";
  cvHasColumnHeaders = false;
  cvName = "Delimited Text";
  cvConfigurationDialog = new EgtDelimitedTextConfigurationDialog( this );
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */
void EgtDelimitedTextProvider::configure( QPoint theScreenLocation )
{
  EgtDebug( "entered" );

  QStringList lvFileNames;
  QFileDialog lvFileDialog( 0, tr( "Open Text File" ), "/home", tr( "Text Files ( *.txt *.csv )" ) );
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

      cvConfigurationDialog->move( theScreenLocation );
      cvConfigurationDialog->show( );
    }
  }
  else
  {
    cvLastError = tr( "User canceled initialization" );
    initialized( false );
  }
}

/*!
 * \param theDelimiter a QString that contains a delimiter
 */
void EgtDelimitedTextProvider::setDelimiter( QString theDelimiter )
{
  EgtDebug( "entered" );

  cvDelimiter = theDelimiter;
}

/*!
 * \param theFileName a QString that contains the name of the file to be read
 */
EgtDataProvider::ErrorType EgtDelimitedTextProvider::setFileName( QString theFileName )
{
  EgtDebug( "entered" );

  cvFileName = theFileName;
  return read();
}

void EgtDelimitedTextProvider::setHasColumnHeaders( bool theValue )
{ 
  EgtDebug( "entered" );

  cvHasColumnHeaders = theValue;
}

/*
 *
 * PROTECTED FUNCTIONS
 *
 */
EgtDataProvider::ErrorType EgtDelimitedTextProvider::read( )
{
  EgtDebug( "entered" );

  cvValid = false;
  cvData.clear();
  cvColumnHeaders.clear();
  cvLastError = "";

  QFile lvFile( cvFileName );
  QStringList lvStringList;
  bool lvError = false;

  if ( !lvFile.open( QFile::ReadOnly | QFile::Text ) )
  {
    cvLastError = QObject::tr( "Can't read the file" ) + ": " + cvFileName;
    return EgtDataProvider::Fatal;
  }

  QTextStream stream( &lvFile );
  QString lvNewData;

  lvNewData = stream.readLine( );
  //If the user indicates there are headers, split first line right into cvColumnHeaders
  if( cvHasColumnHeaders )
  {
    cvColumnHeaders = lvNewData.split( cvDelimiter );
    cvNumberOfFields = cvColumnHeaders.size( );
  }
  else
  {
    //If not make up some headers, and load the first line
    QStringList lvFirstRow = lvNewData.split( cvDelimiter );
    QMap< QString, QString > lvData;
    cvNumberOfFields = lvFirstRow.size( );
    for( int lvRunner = 0; lvRunner < cvNumberOfFields; lvRunner++ )
    {
      cvColumnHeaders << QString::number( lvRunner + 1 );
      lvData[ QString::number( lvRunner ) ] = lvFirstRow[ lvRunner ];
    }

    cvData << lvData;
  }

  while( ! stream.atEnd( ) )
  {
    lvNewData = stream.readLine( );
    QStringList lvIndividualFields = lvNewData.split( cvDelimiter );

    if( lvIndividualFields.size( ) == cvNumberOfFields )
    {
      //Load each row into the map, assuming the same order as the headers
      QMap< QString, QString > lvData;
      for( int lvRunner = 0; lvRunner < cvNumberOfFields; lvRunner++ )
      {
        lvData[ cvColumnHeaders[ lvRunner ] ] = lvIndividualFields[ lvRunner ];
      }

      cvData << lvData;
    }
    else
    {
      lvError = true;
      cvLastError = QObject::tr( "One or more rows have been discarded" ) + ": " + cvFileName;
    }
  }

  cvValid = true;
  if( lvError ) { return EgtDataProvider::Warning; }
qDebug() << "here";
  return EgtDataProvider::None;
}

Q_EXPORT_PLUGIN2( delimitedtextprovider, EgtDelimitedTextProvider );
