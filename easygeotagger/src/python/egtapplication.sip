
class EgtApplication : QObject
{

%TypeHeaderCode
#include <egtapplication.h>
%End

  public:
    /*! \brief Constructor */
    EgtApplication();

    /*! \brief Constructor */
    EgtApplication( QString );

    /*! \brief Return a pointer to the application interface */
    EgtApplicationInterface* applicationInterface();

    /*! \brief Show the gui, just a wrapper for EgtMainWindow->show() */
    void show();

};
