
class EgtImageFactory : QObject
{

%TypeHeaderCode
#include <egtimagefactory.h>
%End

  public:
    /*! \brief Constructor */
    EgtImageFactory( );

    /*! \brief Constructor */
    EgtImageFactory( QString );

    /*! \brief Constructor */
    EgtImageFactory( QModelIndex );

    /*! \brief Destructor */
    ~EgtImageFactory( );

    /*! \brief Is the image factory currently working with a valid image */
    bool isValid( );

    /*! \brief Accessor for the name of the current image file, may or may not be a valid image */
    QString fileName( );

    /*! \brief Returns the a copy of the current resize image */
    QImage scaledImage( bool* isValid = 0 ) const;

    /*! \brief Returns a copy of the image resized to the specified dimensions */
    QImage scaleImage( int, int, bool* isValid = 0 );

    /*! \brief Set the image file name */
    void setFile( QString );

    /*! \brief Set the image file name */
    void setFile( QModelIndex );

    /*! \brief Save the original image as a jpeg */
    bool saveOriginalImageAsJpeg( QString );

    /*! \brief Save the current resized image as a jpeg */
    bool saveScaledImageAsJpeg( QString );

  signals:
    void progress( int, int, int );
    void imageLoaded( bool );

};
