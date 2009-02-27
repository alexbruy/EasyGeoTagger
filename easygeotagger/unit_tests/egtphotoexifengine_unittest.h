#ifndef EGTPHOTOEXIFENGINE_UNITTEST_H
#define EGTPHOTOEXIFENGINE_UNITTEST_H

#include <QTextEdit>

#include "egtphotoexifengine.h"


class EgtPhotoExifEngine_UnitTest
{
  //Q_OBJECT

  public:
    EgtPhotoExifEngine_UnitTest();
    int runTest( QTextEdit* );
  
  private:
    EgtPhotoExifEngine* cvPhotoExifEngine;
};

#endif //EGTGPSEXIFENGINE_UNITTEST_H
