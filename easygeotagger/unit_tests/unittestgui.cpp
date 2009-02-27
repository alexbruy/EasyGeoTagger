#include <QtGui> 
#include <QImage>
#include <QMessageBox>

#include "ui_unittestguibase.h"
#include "unittestgui.h"

#include "egtgpsexifengine_unittest.h"
#include "egtphotoexifengine_unittest.h"

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

  EgtPhotoExifEngine_UnitTest lvPhotoTest;
  EgtGpsExifEngine_UnitTest lvGpsTest;
    
  lvNumFailures+= lvPhotoTest.runTest(ui->teOutput);
  lvNumFailures+= lvGpsTest.runTest(ui->teOutput);
  
  

  QMessageBox::information(this, tr("Unit Test"),
                                tr(("Test finished with "+QString::number(lvNumFailures)+" errors").toStdString().c_str()),
                                QMessageBox::Ok,
                                QMessageBox::Ok);
}

QTextEdit* UnitTestGui::outputWindow()
{
  return ui->teOutput;
}