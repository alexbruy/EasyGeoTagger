#ifndef EGTPATHBUILDER_H
#define EGTPATHBUILDER_H

#include <QModelIndex>

class EgtPathBuilder
{

  public:
    QString buildPath(const QModelIndex& theIndex);

};
#endif
