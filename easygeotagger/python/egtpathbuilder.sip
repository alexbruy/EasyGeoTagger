class EgtPathBuilder
{

%TypeHeaderCode
#include <egtpathbuilder.h>
%End

  public:
    /*! \brief Build an absolute path for the selected item */
    QString buildPath(const QModelIndex& theIndex);

};
