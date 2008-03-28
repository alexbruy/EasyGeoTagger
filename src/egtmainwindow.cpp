#include "egtexifio.h"
#include "egtmainwindow.h"
#include "egtitemdelegate.h"

#include <image.hpp>
#include <exif.hpp>
#include <cassert>

#include <QDir>
#include <QDirModel>

EgtMainWindow::EgtMainWindow()
{
  setupUi(this);
  connect(tvFileBrowser, SIGNAL(clicked(const QModelIndex&)), this, SLOT(clicked(const QModelIndex&)));  
  connect(tvFileBrowser, SIGNAL(expanded(const QModelIndex&)), this, SLOT(expanded(const QModelIndex&)));
  
  QDirModel* lvModel = new QDirModel(QStringList(), QDir::AllDirs|QDir::Files|QDir::NoDotAndDotDot, QDir::DirsFirst);
  
  tvFileBrowser->setModel(lvModel);
  tvFileBrowser->setColumnWidth(0, 400);
  tvFileBrowser->setCurrentIndex(lvModel->index(QDir::currentPath()));
  tvFileBrowser->scrollTo(lvModel->index(QDir::currentPath()));
  
  tvFileBrowser->setItemDelegate(new EgtItemDelegate());
  tvFileBrowser->setStyleSheet("QTreeView { selection-background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #e7effd, stop: 1 #cbdaf1); }");
    
  twExif->setColumnCount(2);
  twExif->setColumnWidth(0, 180);
  twExif->setColumnWidth(1, 180);
  twExif->setHorizontalHeaderLabels(QStringList() << "Key" << "Value");
}

QString EgtMainWindow::buildPath(const QModelIndex& theIndex)
{
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
  int lvChildCount = 0;
  while(theIndex.child(lvChildCount, 0).isValid())
  {
    lvChildCount++;
  }
}








