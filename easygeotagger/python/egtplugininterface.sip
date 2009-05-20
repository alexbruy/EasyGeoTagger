
class EgtPluginInterface : QObject
{

%TypeHeaderCode
#include <egtplugininterface.h>
%End

  public:
    /*! \brief Constuctor */
    EgtPluginInterface( );

    /*! \brief Virtual Destuctor */
    virtual ~EgtPluginInterface( );

    /*! \brief Pure virtual function to return the list of categories that the plugin will show up in */
    QStringList categories( );

    /*! \brief Pure virtual function to connect the provided button to the showConfigurationPanel function */
    virtual void connectConfigurationButton( QPushButton* ) = 0;

    /*! \brief Pure virtual function to connect the provided button to the run function */
    virtual void connectRunButton( QPushButton* ) = 0;

    /*! \brief Pure virtual function to return the description for this plugin */
    QString description( );

    /*! \brief Pure virtual function to return the ability of this plugin to be configured */
    virtual bool isConfigurable( ) = 0;

    /*! \brief Pure virtual function to return the name of this plugin */
    QString name( );

    /*! \brief Virtual function to run initialization steps if needed */
    virtual void initPlugin( );

    /*! \brief Set the pointer to the EasyGeoTagger application interface */
    void setApplicationInterface( EgtApplicationInterface* theInterface );

  public slots:
    /*! \brief Slot called to activate or launch the plugin */
    virtual void run( );

    /*! \brief Slot to display the condifuration panel */
    virtual void showConfigurationPanel( );

};
