
class EgtApplicationInterface : QObject
{

%TypeHeaderCode
#include <egtapplicationinterface.h>
%End

  public:
    /*! \brief Constructor */
    EgtApplicationInterface( QMainWindow* );

    QMainWindow* gui();

  public slots:
    /*! \brief Slot accept coordinates from other plugins or external applications */
    void acceptCoordinates( double, double );

    /*! \brief Slot to accept item selections */
    void acceptModelIndexSelections( const QModelIndex& theIndex);

    /*! \brief Slot to accept requests to fresh the file browser */
    void refreshFileBrowser();

  signals:
    /*! \brief Relays coordinates received by acceptCoordinates( double, double ) */
    void coordinatesReceived( double, double );

    /* \brief Signal to broad cast a mouse click event in the file browser */
    void indexSelected( const QModelIndex& );

    /* \brief Signal to relay requests to refresh the file browser */
    void fileBrowserRefreshRequest();

};
