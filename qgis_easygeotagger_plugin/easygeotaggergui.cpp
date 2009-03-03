/***************************************************************************
 *   Copyright ( C ) 2003 by Tim Sutton                                      *
 *   tim@linfiniti.com                                                     *
 *                                                                         *
 *   This is a plugin generated from the QGIS plugin template              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *( at your option ) any later version.                                   *
 ***************************************************************************/
#include "easygeotaggergui.h"
#include "qgscontexthelp.h"

#include <QDesktopWidget>
#include <QRect>

class QgisApp;

//standard includes

EasyGeoTaggerGui::EasyGeoTaggerGui( QgisInterface* theQgisApplication, EgtApplication* theEasyGTApplication, QWidget* parent, Qt::WFlags fl )
    : QDialog( parent, fl )
{
  setupUi( this );
  cvEasyGT = theEasyGTApplication;
  cvQgisInterface = theQgisApplication;
}

EasyGeoTaggerGui::~EasyGeoTaggerGui( )
{
}

void EasyGeoTaggerGui::on_buttonBox_accepted( )
{
  //close the dialog
  accept( );
}

void EasyGeoTaggerGui::on_buttonBox_rejected( )
{
  reject( );
}

void EasyGeoTaggerGui::on_buttonBox_helpRequested( )
{
  QgsContextHelp::run( context_id );
}

void EasyGeoTaggerGui::on_pbtnAutoArrange_clicked( )
{
  if( 0 == cvQgisInterface ) { return; }
  if( 0 == cvEasyGT ) { return; }

  //read the desktop geometry
  QRect screenGeometry = QApplication::desktop( )->availableGeometry( );
  int lvScreenWidth = screenGeometry.width( );
  int lvScreenHeight = screenGeometry.height( );
  
  int newEasyGTHeight = qMax( int( lvScreenHeight * 0.80 ), cvEasyGT->applicationInterface( )->gui( )->minimumHeight( ) );
  int newEasyGTWidth = qMax( int( lvScreenWidth * 0.60 ), cvEasyGT->applicationInterface( )->gui( )->minimumHeight( ) );
  int newPluginDialogHeight = qMax( int( lvScreenHeight * 0.1 ), minimumHeight( ) );
  int newPluginDialogWidth = qMax( int( lvScreenWidth * 0.35 ), minimumWidth( ) );
  int newQgisHeight = qMax( int( lvScreenHeight * 0.55 ), cvQgisInterface->mainWindow( )->minimumHeight( ) );
  int newQgisWidth = qMax( int( lvScreenWidth * 0.35 ), cvQgisInterface->mainWindow( )->minimumWidth( ) );
  
  //place EasyGT
  cvEasyGT->applicationInterface( )->gui( )->resize( newEasyGTWidth, newEasyGTHeight );
  cvEasyGT->applicationInterface( )->gui( )->resize( newEasyGTWidth - ( cvEasyGT->applicationInterface( )->gui( )->width( ) - newEasyGTWidth ), newEasyGTHeight - ( cvEasyGT->applicationInterface( )->gui( )->height( ) - newEasyGTHeight ) );
  cvEasyGT->applicationInterface( )->gui( )->move( int( lvScreenHeight * 0.02 ), int( lvScreenHeight * 0.1 ) );
  
  //place this dialog
  resize( newPluginDialogWidth, newPluginDialogHeight );
  resize( newPluginDialogWidth - ( width( ) - newPluginDialogWidth ), newPluginDialogHeight - ( height( ) - newPluginDialogHeight ) );
  move( int( lvScreenWidth * 0.63 ), int( lvScreenHeight * 0.1 ) );
  
  //place QGIS
  cvQgisInterface->mainWindow( )->setEnabled( false );
  cvQgisInterface->mainWindow( )->resize( newQgisWidth, newQgisHeight - height( ) );
  cvQgisInterface->mainWindow( )->resize( newQgisWidth - ( cvQgisInterface->mainWindow( )->width( ) - newQgisWidth ), newQgisHeight - ( cvQgisInterface->mainWindow( )->height( ) - newQgisHeight ) );
  cvQgisInterface->mainWindow( )->move( int( lvScreenWidth * 0.63 ), int( lvScreenHeight * 0.25 ) );
  cvQgisInterface->mainWindow( )->setEnabled( true );
}

void EasyGeoTaggerGui::on_pbtnActivate_clicked( )
{
  emit setMapTool( );
}
