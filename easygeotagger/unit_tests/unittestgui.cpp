#include <QtGui> 
#include <QImage>
#include <QMessageBox>

#include "ui_unittestguibase.h"
#include "unittestgui.h"


UnitTestGui::UnitTestGui():  ui( new Ui::UnitTestGuiBase )
{
  ui->setupUi( this );
  cvPhotoExifEngine =new EgtPhotoExifEngine();
}


void UnitTestGui::on_pbtnStartTest_clicked()
{
  bool lvOk;
  int lvNumFailures = 0;

  QImage lvPic = QImage( 128,128,QImage::Format_RGB32 );
  lvPic.fill(125);
  lvPic.save("./test.jpg");
  cvPhotoExifEngine->setFile("./test.jpg");

  lvOk = cvPhotoExifEngine->writeDateTimeOriginal("2005:07:09 10:02:47");
  if ( lvOk )
  {
    ui->teOutput->append("class: [EgtPhotoExifEngine]. Function: writeDateTimeOriginal()--> Ok");
  }
  else
  {
    lvNumFailures++;
    ui->teOutput->append("class: [EgtPhotoExifEngine]. Function: writeDateTimeOriginal()--> Fail");
  }
  
  cvPhotoExifEngine->dateTimeOriginal(&lvOk);
  if( lvOk )
  {
    ui->teOutput->append("class: [EgtPhotoExifEngine]. Function: dateTimeOriginal()--> Ok");
  }
  else
  {
    lvNumFailures++;
    ui->teOutput->append("class: [EgtPhotoExifEngine]. Function: dateTimeOriginal()--> Fail");
  }

  QMessageBox::information(this, tr("Unit Test"),
                                tr(("Test finished with "+QString::number(lvNumFailures)+" errors").toStdString().c_str()),
                                QMessageBox::Ok,
                                QMessageBox::Ok);
}

