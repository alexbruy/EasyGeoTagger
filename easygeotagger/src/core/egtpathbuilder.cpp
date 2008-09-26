#include "egtpathbuilder.h"

#include <QDir>

/*
 *
 * PUBLIC FUNCTIONS
 *
 */
QString EgtPathBuilder::buildPath(const QModelIndex& theIndex)
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
  
  return buildPath( theIndex.parent() ) + QDir::toNativeSeparators( "/" ) + theIndex.data().toString();
}
