#include "egtgpxparser.h"

EgtGpxParser::EgtGpxParser()
{
  cvMapGpxElements["ele"] = "Altitude";
  cvMapGpxElements["lon"] = "Longitude";
  cvMapGpxElements["lat"] = "Latitude";
  cvMapGpxElements["time"] = "DateTimeStamp";

  cvMapDataTypes[ EgtGpxParser::WayPoints ] = "wpt";
  cvMapDataTypes[ EgtGpxParser::Tracks ] = "trkpt";
  cvMapDataTypes[ EgtGpxParser::Routes ] = "rtept";

  cvElementName = "";
  cvInActualData =false;
  cvDataType = EgtGpxParser::Tracks;
}

bool EgtGpxParser::characters ( const QString & ch )
{
  if( cvInActualData && cvElementName!= "" )
  {
    if( 0 == cvElementName.compare( "DateTimeStamp", Qt::CaseInsensitive ) ) 
    {
      QString lvContent = ch;
      lvContent.replace("T"," ");
      lvContent.replace("Z"," ");
      lvContent.replace("-",":");
      cvRowData.insert( cvHeader.indexOf( cvElementName ), lvContent.trimmed() );
    }
    else
    {
      cvRowData.insert( cvHeader.indexOf( cvElementName ), ch );
    }
  }

  return true;
}

QStringList EgtGpxParser::headers()
{
  return cvHeader;
}

QList<QStringList> EgtGpxParser::data()
{
  return cvFileData;
}

bool EgtGpxParser::endElement( const QString&, const QString &localname, const QString &name )
{
  if( 0 == name.compare( cvMapDataTypes[ cvDataType ], Qt::CaseInsensitive ) )
  {
    cvFileData<<cvRowData; 

    for(int i =0; i < cvRowData.size(); i++){cvRowData[i] = ""; } //reset cvRowData
    cvInActualData = false;
  }
  cvElementName ="";
  return true;
}

int EgtGpxParser::numberOfFields()
{
  return cvHeader.size();
}

void EgtGpxParser::reset()
{
  cvElementName = "";
  cvInActualData = false;
  cvFileData.clear();
  cvHeader.clear();
}

void EgtGpxParser::setDataType( EgtGpxParser::DataType theDataType )
{
  cvDataType = theDataType;
}

bool EgtGpxParser::startDocument()
{
  return true;
}

bool EgtGpxParser::startElement( const QString&, const QString&, const QString &name, const QXmlAttributes &attrs )
{
  if( 0 == name.compare( cvMapDataTypes[ cvDataType ], Qt::CaseInsensitive ) )
  {
    for( int i=0; i<attrs.count(); i++ )
    {
      QString lvAttributeName = cvMapGpxElements[ attrs.localName( i ) ];

      if( lvAttributeName!= "" && !cvHeader.contains( lvAttributeName ) ) 
      { 
        cvHeader.append( lvAttributeName ); 
        cvRowData.append("");
      }
      cvRowData.replace( cvHeader.indexOf( lvAttributeName ), attrs.value( i ) );  
    }
    cvInActualData = true; 
  }
  else
  { 
    if(cvInActualData)
    { 
      cvElementName = cvMapGpxElements[ name ]; 
      if( cvElementName!= "" && !cvHeader.contains( cvElementName ) ) { cvHeader.append( cvElementName ); }
    } 
  }

  return true;
}
