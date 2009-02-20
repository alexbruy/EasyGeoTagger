
class EgtExifEngine
{

%TypeHeaderCode
#include <egtexifengine.h>
%End

public:
  /*! \brief A struct to hold a mappings between the Egt key namespace
   * and common names for keys to be displayed in the gui
   */
  struct KeyMap
  {
      QString key;
      QString commonName;
      QStringList units;
  };

  /*! \brief Constructor */
  EgtExifEngine();

  /*! \brief Constructor */
  EgtExifEngine( QString );

  /*! \brief Constructor */
  EgtExifEngine( const QModelIndex& );

  /*! \brief Gets the dependency for an  Egt key
   *
   * \param theKey The Egt keys for which you want the dependency
   * \returns the dependent Egt key
   */
  QString dependency( QString theKey );

  /*! \brief Check to see if the current file has EXIF data expected by this engine */
  bool hasExpectedExif();

  /*! \brief Check if a key exists in the EXIF header */
  bool hasKey( QString );

  /*! \brief Is the image valid? */
  bool isValidImage();

  /*! \brief Convience method that check both hasExpectedExif() and IsValidImage() */
  bool isValidImageWithExpectedExif();

  /*! \brief Returns a constant reference to the key map list for this engine */
  const QList< EgtExifEngine::KeyMap > &keys();

  /*! \brief Returns the last error */
  const QString &lastError();

  /*! \brief Generic read statement to be implemented by child classes */
  virtual QVariant read( QString theKey, bool* isValid = 0 );

  /*! \brief Base read method that returns the raw Exiv value for the provided EXIF key as a QString */
  QString readKeyValueAsString(QString);

  /*! \brief Base read method that returns the raw Exiv value for the provided EXIF key */
  //const Exiv2::Value& readTag( QString );

   /*! \brief Sets the image to be managed with this class */
  void setFile( QString theFileName );

  /*! \brief Generic write statement to be implemented by child classes */
  virtual bool write( QString theKey, QString theValue );

  /*! \brief Base write method that actually writes data base to the image header */
  bool writeTag(QString, QString, QString);

};
