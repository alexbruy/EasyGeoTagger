

class EgtGpxParser : public QXmlDefaultHandler
{
public:
  bool startDocument( );
  bool endElement( const QString&, const QString&, const QString &name );

  bool startElement( const QString&, const QString&, const QString&, const QXmlAttributes& );

  QList<QStringList> data( );
  void reset( );

private:
  
  QList<QStringList>  cvFileData;
  bool cvInGpx;

};