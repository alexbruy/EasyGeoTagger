#ifndef EGTGPSEXIFENGINE_UNITTEST_H
#define EGTGPSEXIFENGINE_UNITTEST_H

#include <QTextEdit>

#include "egtgpsexifengine.h"


class EgtGpsExifEngine_UnitTest
{
  //Q_OBJECT

  public:
    EgtGpsExifEngine_UnitTest();
    int runTest( QTextEdit* );
  
  private:
    EgtGpsExifEngine* cvGpsExifEngine;
};

#endif //EGTGPSEXIFENGINE_UNITTEST_H
