
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
    /*! \brief Accepts a key and data data pair for re broadcasting */
    void acceptKeyValuePair( QString, QString );

    /*! \brief Slot to accept item selections */
    void acceptModelIndexSelections( const QModelIndex& theIndex);

    /*! \brief Slot to accept requests to load a single plugin for a directory of plugins */
    void loadPlugins( QString );

    /*! \brief Slot to accept requests to fresh the file browser */
    void refreshFileBrowser();

  signals:
    /* \brief Signal to broadcast a mouse click event in the file browser */
    void indexSelected( const QModelIndex& );

    /* \brief Signal to relay requests to refresh the file browser */
    void fileBrowserRefreshRequest();

    /*! \brief Rebroadcasts key and tag data pair */
    void keyValuePair( QString, QString );

    /*! \brief Broadcast a load plugin request intended for the plugin manager */
    void loadPluginRequest( QString );

};
