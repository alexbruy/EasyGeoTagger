
class EgtExifEditor : QObject
{

%TypeHeaderCode
#include <egtexifeditor.h>
%End

  public:
    /*! \brief Constructor */
    EgtExifEditor( QString, EgtExifEngine* );

    /*! \brief Add a tag group to the editor */
    void addTagGroup( EgtExifTagGroup* );

    /*! \brief Get a pointer to the editing widget */
    QWidget* editorWidget();

    /*! \brief Load all exif tags */
    void loadExifData( bool );

    /*! \brief Show the configuration dialog */
    void showConfigurationDialog( );

    /*! \brief Show the configuration dialog */
    void showConfigurationDialog( QPoint );

  public slots:
    /*! \brief Slot to receive key value pairs from the application interface */
    void setTagData( QString theKey, QString theValue, bool setCachedValue = true );

    /*! \brief Slot to receive key value pairs from the application interface */
    void setTagData( QString theKey, QVariant theValue, bool setCachedValue = true );
};
