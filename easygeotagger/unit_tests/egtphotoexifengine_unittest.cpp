#include "egtphotoexifengine_unittest.h"


EgtPhotoExifEngine_UnitTest::EgtPhotoExifEngine_UnitTest()
{
  cvPhotoExifEngine =new EgtPhotoExifEngine();
}

int EgtPhotoExifEngine_UnitTest::runTest(QTextEdit* theOutputWindow)
{
  bool lvOk;
  int lvNumFailures = 0;

  cvPhotoExifEngine->setFile("./test.jpg");

  lvOk = cvPhotoExifEngine->writeDateTimeOriginal("2005:07:09 10:02:47");
  if ( lvOk )
  {
    theOutputWindow->append("class: [EgtPhotoExifEngine]. Function: writeDateTimeOriginal()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtPhotoExifEngine]. Function: writeDateTimeOriginal()--> Fail");
  }
  
  cvPhotoExifEngine->dateTimeOriginal(&lvOk);
  if( lvOk )
  {
    theOutputWindow->append("class: [EgtPhotoExifEngine]. Function: dateTimeOriginal()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtPhotoExifEngine]. Function: dateTimeOriginal()--> Fail");
  }
  
  return lvNumFailures;
}