
class EgtRawImageReader : QThread
{

%TypeHeaderCode
#include <egtrawimagereader.h>
%End

  public:
    /*! \brief Constructor */
    EgtRawImageReader();

    /*! \brief Destructor */
    ~EgtRawImageReader();

    /*! \brief Set the abort flag to true and terminate the thread */
    void abort();

    /*! \brief Return the raw image */
    QImage image() const;

     /*! \brief Releases the memory used for the raw image */
    bool recycle();

    /*! \brief Required thread function */
    void run();

    /*! \brief Set the absolute path and filename of the raw image to open */
    void setFile( QString );
};
