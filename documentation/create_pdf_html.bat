java  -Djavax.xml.parsers.DocumentBuilderFactory=org.apache.xerces.jaxp.DocumentBuilderFactoryImpl -Djavax.xml.parsers.SAXParserFactory=org.apache.xerces.jaxp.SAXParserFactoryImpl -Dxslthl.config="file:///C:\DocBook\docbook-xsl-ns-1.73.2\highlighting\xslthl-config.xml" com.icl.saxon.StyleSheet -o easygt_user_guide_v0.4.html EasyGT_UserGuide_v4.xml html_custom2.xsl

java  -Djavax.xml.parsers.DocumentBuilderFactory=org.apache.xerces.jaxp.DocumentBuilderFactoryImpl -Djavax.xml.parsers.SAXParserFactory=org.apache.xerces.jaxp.SAXParserFactoryImpl -Dxslthl.config="file:///C:\DocBook\docbook-xsl-ns-1.73.2\highlighting\xslthl-config.xml" com.icl.saxon.StyleSheet -o easygt_user_guide.fo EasyGT_UserGuide_v4.xml fo_custom2.xsl

fop-0.94/fop -fo easygt_user_guide.fo -pdf easygt_user_guide_v0.4.pdf