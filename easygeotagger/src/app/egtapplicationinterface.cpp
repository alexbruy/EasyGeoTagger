/*
** File: egtapplicationinterface.cpp
** Author( s ): Peter J. Ersts ( ersts at amnh.org )
** Creation Date: 2008-09-30
**
** Copyright ( c ) 2008-2009, American Museum of Natural History. All rights reserved.
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
#include "egtapplicationinterface.h"
#include "egtlogger.h"

#include <QWidgetList>
#include <QApplication>

EgtApplicationInterface::EgtApplicationInterface( QMainWindow* theMainWindow )
{
    cvGui = theMainWindow;
    if( 0 != cvGui )
    {
      connect( cvGui, SIGNAL( fileBrowserItemSelected( const QModelIndex& ) ), this, SLOT( acceptModelIndexSelections( const QModelIndex& ) ) );
      connect( this, SIGNAL( fileBrowserRefreshRequest( ) ), cvGui, SLOT( refreshFileBrowser( ) ) );
    }
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */
QStringList EgtApplicationInterface::availableProviders()
{
  if( 0!= cvDataProviderManager )
  {
    return cvDataProviderManager->availableProviders();
  }

  return QStringList();
}

EgtDataProvider* EgtApplicationInterface::dataProvider( QString theProviderName )
{
  if( 0!= cvDataProviderManager )
  {
    return cvDataProviderManager->provider( theProviderName );
  }

  return new EgtDataProvider();
}

QPoint EgtApplicationInterface::positionOfFirstVisibleWidget( )
{
  //Find the first non hidden widget and open the config dialog
  QWidgetList lvWidgetList = QApplication::topLevelWidgets( );
  for( int lvIterator = 0; lvIterator < lvWidgetList.size( ); lvIterator++ )
  {
    if( !lvWidgetList[ lvIterator ]->isHidden( ) )
    {
      return lvWidgetList[ lvIterator ]->pos( );
    }
  }
  return QPoint( 0, 0 );
}

/*!
 * \param theWidgetTitle the title of the widget you want to look for. *NOTE: this should already be the translated version of the string
 */
QPoint EgtApplicationInterface::positionOfWidget( QString theWidgetTitle )
{
  QWidgetList lvActiveWindows = QApplication::topLevelWidgets( );

  for( int i = 0; i < lvActiveWindows.size( ); i++ )
  {
    if( theWidgetTitle == lvActiveWindows[i]->windowTitle( ) )
    {
      return lvActiveWindows[i]->pos( );
    }
  }

  return QPoint( 0, 0 );
}

/*
 *
 * PUBLIC SLOTS
 *
 */
void EgtApplicationInterface::acceptModelIndexSelections( const QModelIndex& theIndex )
{
  EgtDebug( "Received model index selection signal" );
  emit( indexSelected( theIndex ) );
}

void EgtApplicationInterface::acceptKeyValuePair( QString theKey, QString theData )
{
  EgtDebug( "Received key: "+ theKey +"\tCargo: "+ theData );
  emit keyValuePair( theKey, theData );
}

void EgtApplicationInterface::loadPlugins( QString thePath )
{
  EgtDebug( "Received load plugin request: "+ thePath );
  emit loadPluginRequest( thePath );
}

void EgtApplicationInterface::refreshFileBrowser( )
{
  EgtDebug( "Received file browser refresh request" );
  emit( fileBrowserRefreshRequest( ) );
}
