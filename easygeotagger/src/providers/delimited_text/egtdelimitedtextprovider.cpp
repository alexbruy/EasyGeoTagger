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

#include <QFile>
#include <QtPlugin>
#include <QTextStream>
#include <QApplication>
#include <QFileDialog>

EgtDelimitedTextProvider::EgtDelimitedTextProvider( ) : EgtDataProvider( )
{
  cvDelimiter = ",";
  cvFileName = ""; 
  cvLastError = "";
  cvConfigurationDialog = 0;
  cvName = "Delimited Text";
  cvConfigurationDialog = new EgtDelimitedTextConfigurationDialog( this );
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */
void EgtDelimitedTextProvider::configure( ) //init function
{

  QWidgetList lvActiveWindows = QApplication::topLevelWidgets( );
  QWidget* lvParent = 0;

  for( int i = 0; i < lvActiveWindows.size( ); i++ )
  {
    if( tr( "Select a file type" ) == lvActiveWindows[i]->windowTitle( ) )
    {
      lvParent = lvActiveWindows[i];
    }
  }

  QString lvFileName = QFileDialog::getOpenFileName( lvParent, tr( "Open GPS File" ), "/home", tr( "GPS Files ( *.txt *.gps )" ) );
  if ( "" != lvFileName )
  {
    setFileName( lvFileName );
    
    showConfigurationDialog( );
  }
} 

void EgtDelimitedTextProvider::notifyInitializationComplete( bool isComplete )
{ 
  if( !isComplete )
  {
    cvData.clear();
  }
  emit dataProviderReady();
}

/*!
 * \param theDelimiter a QString that contains a delimiter
 */
void EgtDelimitedTextProvider::setDelimiter( QString theDelimiter )
{
  cvDelimiter = theDelimiter;
}

/*!
 * \param theFileName a QString that contains the name of the file to be read
 */
EgtDataProvider::ErrorType EgtDelimitedTextProvider::setFileName( QString theFileName )
{
  cvFileName = theFileName;
  return read();
}

void EgtDelimitedTextProvider::setHasColumnHeaders( bool theValue )
{ 
  cvHasColumnHeaders = theValue;
}

void EgtDelimitedTextProvider::showConfigurationDialog( )
{
  QWidgetList lvActiveWindows = QApplication::topLevelWidgets( );
  QWidget* lvParent = 0;

  for( int i = 0; i < lvActiveWindows.size( ); i++ )
  {
    if( tr( "GPS Reader" ) == lvActiveWindows[i]->windowTitle( ) )
    {
      lvParent = lvActiveWindows[i];
    }
  }

  QPoint lvPosition = lvParent->pos( );
  //center the window over the parent
  cvConfigurationDialog->move( lvPosition.x( ) + lvParent->width( )/2 - cvConfigurationDialog->width( )/2, lvPosition.y( ) + lvParent->height( )/2 - cvConfigurationDialog->height( )/2 );
  cvConfigurationDialog->show( );
}

/*
 *
 * PROTECTED FUNCTIONS
 *
 */
EgtDataProvider::ErrorType EgtDelimitedTextProvider::read( )
{
  cvData.clear();
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
  QString lvLine;

  if( hasColumnHeaders( ) )
  {
    lvLine = stream.readLine( );
    cvColumnHeaders = lvLine.split( cvDelimiter );
    cvNumberOfFields = lvLine.split( cvDelimiter ).size( );
  }
  else
  {
    lvLine = stream.readLine( );
    cvData << lvLine.split( cvDelimiter );
    cvNumberOfFields = lvLine.split( cvDelimiter ).size( );
  }

  while( ! stream.atEnd( ) )
  {
    lvLine = stream.readLine( );

    if( lvLine.split( cvDelimiter ).size( ) == cvNumberOfFields )
    {
      cvData << lvLine.split( cvDelimiter );
    }
    else
    {
      lvError = true;
      cvLastError = QObject::tr( "One or more rows have been discarded" ) + ": " + cvFileName;
    }
  }

  if( lvError ) { return EgtDataProvider::Warning; }

  return EgtDataProvider::None;
}

Q_EXPORT_PLUGIN2( delimitedtextprovider, EgtDelimitedTextProvider );
