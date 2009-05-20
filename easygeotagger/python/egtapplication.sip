
class EgtApplication : QObject
{

%TypeHeaderCode
#include <egtapplication.h>
%End

  public:
    /*! \brief Constructor */
    EgtApplication( bool displayFlash = true );

    /*! \brief Constructor */
    EgtApplication( QString, bool displayFlash = true );

    /*! \brief Return a pointer to the application interface */
    EgtApplicationInterface* applicationInterface( );

    /*! \brief Show the gui, just a wrapper for EgtMainWindow->show( ) */
    void show( );

};
