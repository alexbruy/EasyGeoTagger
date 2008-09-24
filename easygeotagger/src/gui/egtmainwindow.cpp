
#include "egtmainwindow.h"

#include "egtlogger.h"
#include "egtexifio.h"
#include "egtitemdelegate.h"

#include <image.hpp>
#include <exif.hpp>
#include <cassert>

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDirModel>
#include <QDateTime>
#include <QTextStream>
#include <QMessageBox>

EgtMainWindow::EgtMainWindow()
{
  EgtDebug("EgtMainWindow()");
  setupUi(this);
  connect(tvFileBrowser, SIGNAL(clicked(const QModelIndex&)), this, SLOT(clicked(const QModelIndex&)));  
  connect(tvFileBrowser, SIGNAL(expanded(const QModelIndex&)), this, SLOT(expanded(const QModelIndex&)));
  
  QDirModel* lvModel = new QDirModel(QStringList(), QDir::AllDirs|QDir::Files|QDir::NoDotAndDotDot, QDir::DirsFirst);
  
  tvFileBrowser->setModel(lvModel);
  tvFileBrowser->setColumnWidth(0, 400);
  tvFileBrowser->setCurrentIndex(lvModel->index(QDir::currentPath()));
  tvFileBrowser->scrollTo(lvModel->index(QDir::currentPath()));
  
  EgtItemDelegate* lvItemDelegate =  new EgtItemDelegate();
  connect(chkbColorCodeFilenames, SIGNAL(stateChanged(int)), lvItemDelegate, SLOT(displayGpsExifAvailability(int)));
  tvFileBrowser->setItemDelegate(lvItemDelegate);
  tvFileBrowser->setStyleSheet("QTreeView { selection-background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #e7effd, stop: 1 #cbdaf1); }");
    
  twExif->setColumnCount(2);
  twExif->setColumnWidth(0, 180);
  twExif->setColumnWidth(1, 180);
  twExif->setHorizontalHeaderLabels(QStringList() << "Key" << "Value");
}

QString EgtMainWindow::buildPath(const QModelIndex& theIndex)
{
  //EgtDebug("buildPath()");
  if(!theIndex.isValid()) { return ""; }
  
  if(!theIndex.parent().isValid())
  {
  #ifdef WIN32
  return theIndex.data().toString();
  #else
  return "";
  #endif
  }
  
  return buildPath(theIndex.parent()) + QDir::toNativeSeparators ("/") + theIndex.data().toString();
}

/*
 *
 * SIGNAL and SLOTS
 *
 */

void EgtMainWindow::clicked(const QModelIndex& theIndex)
{
  EgtDebug("clicked()");
  QString lvCurrentFile = buildPath(theIndex);
  cvFileInfo.setFile(lvCurrentFile);
  if(cvFileInfo.isDir())
    return;
  
  try 
  {
    Exiv2::Image::AutoPtr lvImage = Exiv2::ImageFactory::open(qPrintable(lvCurrentFile));
    assert(lvImage.get() != 0);
    lvImage->readMetadata();
    Exiv2::ExifData lvData = lvImage->exifData();
    Exiv2::Exifdatum lvDatum = lvData["Exif.Image.DateTime"];
    labelTimestamp->setText(QString(lvDatum.value().toString().c_str()));
    
    twExif->clear();
    twExif->setRowCount(0);
    twExif->setHorizontalHeaderLabels(QStringList() << "Key" << "Value");
    
    Exiv2::ExifData::iterator it = lvData.findIfdIdIdx(Exiv2::gpsIfdId, 1);
    if(it != lvData.end())
    {
      int lvRowCount = 0;
      while(it->ifdId() == Exiv2::gpsIfdId)
      {
        twExif->insertRow(lvRowCount);
        twExif->setItem(lvRowCount, 0, new QTableWidgetItem(QString(it->tagLabel().c_str())));
        twExif->setItem(lvRowCount, 1, new QTableWidgetItem(QString(it->value().toString().c_str())));
        it++;
        lvRowCount++;
      }
    }
    
  }
  catch (Exiv2::AnyError& e)
  { 
    twExif->clear();
    twExif->setRowCount(0);
    twExif->setHorizontalHeaderLabels(QStringList() << "Key" << "Value");
    labelTimestamp->setText("");
  }
}

void EgtMainWindow::expanded(const QModelIndex& theIndex)
{
  EgtDebug("expanded()");
  int lvChildCount = 0;
  while(theIndex.child(lvChildCount, 0).isValid())
  {
    lvChildCount++;
  }
}

void EgtMainWindow::on_pbtnCreateLayerFromDir_clicked()
{
  EgtDebug("on_pbtnCreateLayerFromDir_clicked()");
  QFileInfo lvFileInfo;
  QString lvCurrentFile = buildPath(tvFileBrowser->currentIndex());
  lvFileInfo.setFile(lvCurrentFile);
  if(!lvFileInfo.isDir())
  {
    tvFileBrowser->setCurrentIndex(tvFileBrowser->currentIndex().parent());
  }
  
  lvCurrentFile = buildPath(tvFileBrowser->currentIndex());
  lvFileInfo.setFile(lvCurrentFile);
  if(lvFileInfo.isWritable())
  {
    QDateTime lvTimestamp = QDateTime::currentDateTime();
    QFile lvOutputFile(QDir::toNativeSeparators (lvCurrentFile + "/" + tvFileBrowser->currentIndex().data().toString() + "Export_" + lvTimestamp.toString("yyyyMMdd_hhmmss") + ".csv"));
    if(lvOutputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      QTextStream lvOutputWriter(&lvOutputFile);
      lvOutputWriter.setRealNumberPrecision(10);
      lvOutputWriter << "Longitude,Latitude,File\n";
      
      int lvExportedImages = 0;
      int lvChildCount = 0;
/*
      EgtExifIO lvEEIO;
      QModelIndex lvCurrentIndex = tvFileBrowser->currentIndex();
      while(lvCurrentIndex.child(lvChildCount, 0).isValid())
      {
        QString lvImageFile = buildPath(lvCurrentIndex.child(lvChildCount, 0));
        EgtDebug(qPrintable(lvImageFile));
        if(lvEEIO.hasGpsExif(lvImageFile))
        {
          lvOutputWriter << lvEEIO.getLongitude(lvImageFile) << "," << lvEEIO.getLatitude(lvImageFile) << "," << lvImageFile << "\n";
          lvExportedImages++;
        }
        lvChildCount++;
      }
    
      lvOutputFile.close();
      QMessageBox::warning(this, tr("Export Complete"), tr("%n images exported.", "", lvExportedImages) );
*/
    }
    else
    {
      QMessageBox::critical(this, tr("Write Error"), tr("Could create a new file for output") );
    }
  }
  else
  {
    QMessageBox::critical(this, tr("Write Error"), tr("The current directory is not writeable") );
  }

  if(tvFileBrowser->model())
  {
    ((QDirModel*)tvFileBrowser->model())->refresh();
  }
}







