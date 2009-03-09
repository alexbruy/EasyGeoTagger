/*
** File: egtmainwindow.cpp
** Author( s ): Peter J. Ersts ( ersts at amnh.org )
** Creation Date: 2008-09-22
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
#include "egtmainwindow.h"

#include "egtlogger.h"
#include "egtversion.h"
#include "egtitemdelegate.h"

#include <exiv2/image.hpp>
#include <exiv2/exif.hpp>
#include <cassert>

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDirModel>
#include <QDateTime>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include <QCoreApplication>

EgtMainWindow::EgtMainWindow( QWidget* theParent )
    : QMainWindow( theParent ), cvUi( new Ui::EgtMainWindowGui )
{
  EgtDebug( "entered" );
  
  cvUi->setupUi( this );
  connect( cvUi->tvFileBrowser, SIGNAL( clicked( const QModelIndex& ) ), this, SLOT( clicked( const QModelIndex& ) ) );
  setWindowTitle( tr( "EasyGeoTagger" ) +" v."+ EGT_VERSION_MAJOR +"."+ EGT_VERSION_MINOR +"."+ EGT_VERSION_PATCH );

  
  //Set up the file browser window
  QDirModel* lvModel = new QDirModel( QStringList( ), QDir::AllDirs|QDir::Files|QDir::NoDotAndDotDot, QDir::DirsFirst );
  cvUi->tvFileBrowser->setModel( lvModel );
  cvUi->tvFileBrowser->setColumnWidth( 0, 400 ) ;
  cvUi->tvFileBrowser->setCurrentIndex( lvModel->index( QDir::currentPath( ) ) );
  cvUi->tvFileBrowser->scrollTo( lvModel->index( QDir::currentPath( ) ) );
  
  //Add an item delegate to colorize the entries in the file browser
  //TODO: See if this is possible in Qt4.4.0 to accomplish with styles
  EgtItemDelegate* lvItemDelegate = new EgtItemDelegate( );
  connect( cvUi->chkbColorCodeFilenames, SIGNAL( stateChanged( int ) ), lvItemDelegate, SLOT( displayGpsExifAvailability( int ) ) );
  cvUi->tvFileBrowser->setItemDelegate( lvItemDelegate );
  cvUi->tvFileBrowser->setStyleSheet( "QTreeView { selection-background-color: qlineargradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #e7effd, stop: 1 #cbdaf1 ); }" );
  
  //Create a new plugin dock
  cvPluginDock = new QDockWidget( "Plugins", this );
  cvPluginDock->setFeatures( QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable );
  cvPluginDock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
  cvPluginDock->setMinimumSize( 150,150 );
  addDockWidget( Qt::LeftDockWidgetArea, cvPluginDock );
  
  //Set up the progress bar
  cvUi->pbarProgressBar->setMinimum( 0 );
  cvUi->pbarProgressBar->setMaximum( 1 );
  cvUi->pbarProgressBar->setValue( 0 );
  
  connect( &cvImageFactory, SIGNAL( progress( int, int, int ) ), this, SLOT( updateProgress( int, int, int ) ) );
  connect( &cvImageFactory, SIGNAL( imageLoaded( bool ) ), this, SLOT( loadPreview( bool ) ) );
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */
void EgtMainWindow::setPluginToolBox( QToolBox* theToolBox )
{
  //TODO: Need to free all of the panels and the tool box if it exists?
  
  cvPluginDock->setWidget( theToolBox );
}

/*
 *
 * SIGNAL and SLOTS
 *
 */
/*!
 * \param theIndex the current selection in the file browser */
void EgtMainWindow::clicked( const QModelIndex& theIndex )
{
  EgtDebug( "entered" );
  QString lvFileName = cvPathBuilder.buildPath( theIndex );

  cvPhotoExifEngine.setFile( lvFileName );
  cvUi->labelDateTimeOriginal->setText( cvPhotoExifEngine.read( "Egt.Photo.DateTimeOriginal" ).toString( ) );

  cvImageFactory.setFile( lvFileName );

  emit( fileBrowserItemSelected( theIndex ) );
}

/*!
 * \param isValid Did the requested image load correctly
 */
void EgtMainWindow::loadPreview( bool isValid )
{
  EgtDebug( "entered" );
  
  if( isValid )
  {
    cvUi->labelPreview->setPixmap( QPixmap::fromImage( cvImageFactory.scaleImage( cvUi->labelPreview->width( ), cvUi->labelPreview->height( ) ) ) );
  }
}

void EgtMainWindow::on_actionAbout_activated( )
{
  QString lvBody = tr( "<html>"\
                       "<body>"\
                       "<center><h1>EasyGeoTagger</h1></center>"\
                       "<strong>Developers:</strong> Peter J. Ersts ( lead developer ), Roberto Garcia-Yunta, Ned Horning<br><br>"\
                       "This is an early beta release and we expect there to be some issues. Please provide us with feedback,"\
                       "positive or negative. The more we hear from the you, the users, the better we can make our products. "\
                       "<br><br>As with any beta, you are incouraged to use copies of your data and never the originals as data "\
                       "corruption can occur if a bug is encountered. EasyGeoTagger is licensed under GPL v2 and is distributed "\
                       "in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of "\
                       "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public License for more details.<br><br>"\
                       "Questions and Comments<br>"\
                       "<a href = \"http://biodiversityinformatics.amnh.org/open_source/easygeotagger/comments.php\">"\
                       "http://biodiversityinformatics.amnh.org/open_source/easygeotagger/comments.php</a><br><br>"\
                       "Bugs!<br>"\
                       "<a href = \"https://trac.biodiversityinformatics.amnh.org/projects/easygeotagger/newticket\">"\
                       "https://trac.biodiversityinformatics.amnh.org/projects/easygeotagger/newticket</a>"\
                       "<br><br><br><span style=\"font-size: 10px;color: #555555\">This work was made possible through a grant by the the John D. and Catherine T. MacArthur Foundation"\
                       " with additional support from the Center For Biodiversity and Conservation ( American Museum of "\
                       "Natural History ), and The Spanish Ministry of Science and Innovation's INTEGRANTS program."\
                       "</span>"\
                       "</body>"\
                       "</html>"
                     );
  cvWebBrowser.setHtml( lvBody );
  cvWebBrowser.setWindowIcon( QIcon( ":/icons/internet-web-browser.svg" ) );
  cvWebBrowser.setWindowTitle( tr( "About - EasyGeoTagger" ) );
  cvWebBrowser.show( );
}

void EgtMainWindow::on_actionExit_activated( )
{
  QCoreApplication::exit( );
}

void EgtMainWindow::on_actionLoadAll_activated( )
{
  QSettings lvQSettings;
  QString lvLastPath = lvQSettings.value( "lastPluginDirectory", "" ).toString( );
  QString lvDirectory = QFileDialog::getExistingDirectory( this, tr( "Select a directory of plugins" ), lvLastPath, QFileDialog::ShowDirsOnly );

  if( lvDirectory != "" )
  {
    lvQSettings.setValue( "lastPluginDirectory", lvDirectory );

    emit loadPlugins( lvDirectory );
  }
}

void EgtMainWindow::on_actionLoadSingle_activated( )
{
  QSettings lvQSettings;
  QString lvLastPath = lvQSettings.value( "lastPluginDirectory", "" ).toString( );
  QString lvFile = QFileDialog::getOpenFileName( this, tr( "Select a plugin" ), lvLastPath, tr( "Shared libraries ( *.so *.dll *.dylib )" ) );

  if( lvFile != "" )
  {
    QFileInfo lvFileInfo( lvFile );
    lvQSettings.setValue( "lastPluginDirectory", lvFileInfo.absoluteDir( ).absolutePath( ) );

    emit loadPlugins( lvFile );
  }
}

void EgtMainWindow::refreshFileBrowser( )
{
 ( ( QDirModel* )cvUi->tvFileBrowser->model( ) )->refresh( );
}

/*!
 * \param theMinimum the minumum value for the progress bar
 * \param theMaximum the maximum value for the progress bar
 * \param theProgress the current progress
 */
void EgtMainWindow::updateProgress( int theMinimum, int theMaximum, int theProgress )
{
  //TODO: consider if it is better to set these individually
  cvUi->pbarProgressBar->setMinimum( theMinimum );
  cvUi->pbarProgressBar->setMaximum( theMaximum );
  cvUi->pbarProgressBar->setValue( theProgress );
}
