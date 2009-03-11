<?xml version="1.0" ?> 
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
   xmlns:d="http://docbook.org/ns/docbook" 
   xmlns:exsl="http://exslt.org/common" 
   xmlns:fo="http://www.w3.org/1999/XSL/Format" 
   xmlns:ng="http://docbook.org/docbook-ng" 
   xmlns:db="http://docbook.org/ns/docbook" 
   xmlns:t="http://nwalsh.com/docbook/xsl/template/1.0"
   xmlns:param="http://nwalsh.com/docbook/xsl/template/1.0/param"
   xmlns:xslthl="http://xslthl.sf.net"
   exclude-result-prefixes="db ng exsl d xslthl" 
   version="1.0">

   <xsl:import href="docbook-xsl-ns-1.73.2/fo/docbook.xsl"/>
   <xsl:import href="docbook-xsl-ns-1.73.2/fo/easygttitlepage.xsl"/>

   <xsl:param name="fop1.extensions">1</xsl:param>
   <xsl:param name="section.autolabel">1</xsl:param>
   <xsl:param name="use.role.for.mediaobject">1</xsl:param>
   <xsl:param name="highlight.source">1</xsl:param>
   
<xsl:template match='xslthl:tag'>
   <fo:inline font-weight="bold" color="blue"><xsl:apply-templates/></fo:inline>
</xsl:template>

<xsl:attribute-set name="toc.margin.properties">
   <xsl:attribute name="break-before">page</xsl:attribute>
</xsl:attribute-set>

<xsl:attribute-set name="toc.margin.properties">
   <xsl:attribute name="break-after">page</xsl:attribute>
</xsl:attribute-set>

<xsl:template match="d:guimenuitem">
   <fo:inline font-style="italic">
      <xsl:call-template name="inline.boldseq" /> 
   </fo:inline>
</xsl:template>

<xsl:template match="d:guisubmenu">
   <fo:inline font-style="italic">
      <xsl:call-template name="inline.boldseq" /> 
   </fo:inline>
</xsl:template>

<xsl:template match="d:guimenu">
   <fo:inline font-style="italic">
      <xsl:call-template name="inline.boldseq" /> 
   </fo:inline>
</xsl:template>

<xsl:template match="d:guilabel">
   <xsl:call-template name="inline.boldseq" /> 
</xsl:template>

<xsl:template match="d:guibutton">
   <xsl:call-template name="inline.boldseq" /> 
</xsl:template>

<xsl:template match="d:link">
   <fo:inline color="blue">
      <xsl:call-template name="inline.boldseq" /> 
   </fo:inline> 
</xsl:template>




<xsl:param name="shade.verbatim" select="1"/>

<xsl:attribute-set name="shade.verbatim.style">
   <xsl:attribute name="background-color">#E0E0E0</xsl:attribute>
   <xsl:attribute name="border-width">0.5pt</xsl:attribute>
   <xsl:attribute name="border-style">solid</xsl:attribute>
   <xsl:attribute name="border-color">#575757</xsl:attribute>
   <xsl:attribute name="padding">3pt</xsl:attribute>
</xsl:attribute-set>


<xsl:attribute-set name="monospace.verbatim.properties" use-attribute-sets="verbatim.properties">
   <xsl:attribute name="font-size">8pt</xsl:attribute>
   <xsl:attribute name="keep-together.within-column">always</xsl:attribute>
</xsl:attribute-set>

</xsl:stylesheet>