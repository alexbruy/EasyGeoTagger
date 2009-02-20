
class EgtExifTagGroup : QWidget
{

%TypeHeaderCode
#include <egtexiftaggroup.h>
%End

  public:
    /*! \brief Constructor */
    EgtExifTagGroup( QString );

    /*! \brief Add a Egt key to the collection
     * \param theKey the Egt key to add
     */
    void addKey( QString theKey );

  signals:
    void tagGroupActivated( QStringList );
};
