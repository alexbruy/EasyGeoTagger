
class EgtDataProvider : QObject
{

%TypeHeaderCode
#include <egtapplicationinterface.h>
%End

  public:
    enum ErrorType
    {
      None,
      Warning,
      Fatal
    };

    /*! \brief Constructor */
    EgtDataProvider( );

    virtual ~EgtDataProvider( );

    /*! \brief Returns the column headers*/
    QStringList columnHeaders( );

    /*! \brief Entry point for derived classes so they can graphically display their options */
    virtual void configure( QPoint thePoint );

    /*! \brief Create a new uninitialized instance of this provider */
    virtual EgtDataProvider* instance( );

    /*! \brief Is the provider valid */
    bool isValid();

    /*! \returns the last error message */
    QString lastError( );

    /*! \returns the proivders name */
    QString name( );

    /*! \brief Get the next row of data */
    QMap< QString, QString > next( );

    /*! \returns the number of fields */
    int numberOfFields( );

    /*! \returns the number of records the provider currently has loaded */
    int numberOfRecords( );

    /*! \brief the the revious row of data */
    QMap< QString, QString > previous( );

    /*! \brief Reset the proivder back to the first record */
    void reset( );

  signals:
    void dataProviderReady( );

  protected:
    /*! \brief Read function to be implemented by the derrived classes */
    virtual EgtDataProvider::ErrorType read( );

    void initialized( bool );

};
