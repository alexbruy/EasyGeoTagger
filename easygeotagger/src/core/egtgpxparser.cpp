
bool EgtGpxParser::startDocument()
{
  cvInGpx = false;
  return true;
}

bool EgtGpxParser::endElement( const QString&, const QString&, const QString &name )
{
  if( name == "gpx" )
  cvInGpx = false;

  return true;
}

bool EgtGpxParser::startElement( const QString&, const QString&, const QString &name, const QXmlAttributes &attrs )
{
  if( cvInGpx && name == "wpt" )
  {
    QString elevation, time, longitude, latitude;

    for( int i=0; i<attrs.count(); i++ )
    {
      QStringList lvRowData;
      if( attrs.localName( i ) == "ele" )
        elevation = attrs.value( i );
      else if( attrs.localName( i ) == "time" )
        time = attrs.value( i );
      else if( attrs.localName( i ) == "lon" )
        longitude = attrs.value( i );
      else if( attrs.localName( i ) == "lat" )
        latitude = attrs.value( i );
    }
    lvRowData<<longitude<<latitude<<elevation<<time;
    cvFileData<<lvRowData;
  }
  else if( name == "gpx" )
    cvInGpx = true;

  return true;
}

QList<QStringList> EgtGpxParser::data()
{
  return cvFileData;
}

void EgtGpxParser::reset()
{
  cvFileData.clear();
}
