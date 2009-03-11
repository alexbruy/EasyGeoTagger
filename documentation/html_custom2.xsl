<?xml version="1.0" ?> 
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
   xmlns:d="http://docbook.org/ns/docbook" 
   xmlns:exsl="http://exslt.org/common" 
   xmlns:fo="http://www.w3.org/1999/XSL/Format" 
   xmlns:ng="http://docbook.org/docbook-ng" 
   xmlns:db="http://docbook.org/ns/docbook" 
   xmlns:xslthl="http://xslthl.sf.net"
   exclude-result-prefixes="db ng exsl d xslthl" 
   version="1.0">

   <xsl:import href="docbook-xsl-ns-1.73.2/html/docbook.xsl"/>
   <xsl:import href="docbook-xsl-ns-1.73.2/html/easygttitlepage.xsl"/>

   <xsl:param name="use.extensions">0</xsl:param>
   <xsl:param name="highlight.source">1</xsl:param>

<xsl:template match="xslthl:tag">
   <b class="hl-tag" style="color: green">
   <xsl:apply-templates /> 
   </b>
</xsl:template>

<xsl:template match="d:guimenuitem">
   <em>
      <xsl:call-template name="inline.boldseq" />
   </em>
</xsl:template>

<xsl:template match="d:guisubmenu">
   <em>
      <xsl:call-template name="inline.boldseq" />
   </em>
</xsl:template>

<xsl:template match="d:guimenu">
   <em>
      <xsl:call-template name="inline.boldseq" />
   </em>
</xsl:template>

<xsl:template match="d:guilabel">
   <xsl:call-template name="inline.boldseq" /> 
</xsl:template>

<xsl:template match="d:guibutton">
   <xsl:call-template name="inline.boldseq" /> 
</xsl:template>

</xsl:stylesheet>