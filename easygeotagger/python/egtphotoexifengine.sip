
class EgtPhotoExifEngine : EgtExifEngine
{

%TypeHeaderCode
#include <egtphotoexifengine.h>
%End

  public:
    /*! \brief Constructor */
    EgtPhotoExifEngine( );

    /*! \brief Constructor */
    EgtPhotoExifEngine( QString );

    /*! \brief Constructor */
    EgtPhotoExifEngine( const QModelIndex& );

    /*! \brief Returns the data associated with the Exif.Photo.DateTimeOriginal tag */
    QString dateTimeOriginal( bool* isValid );

    /*! \brief Generic read statement */
    QVariant read( QString theTag, bool* isValid = 0 );

    /*! \brief Set and open a new file */
    void setFile( QString theFileName );

    /*! \brief Generic write statement */
    bool write( QString, QString );

    /*! \brief Writes data to the Exif.Photo.DateTimeOriginal tag */
    bool writeDateTimeOriginal( QString );

};
