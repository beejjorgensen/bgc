<?xml version="1.0" encoding="ISO-8859-1"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:param name="multipage" select="0"/>

<xsl:output method="html"/>

<xsl:strip-space elements="*"/>

<!-- =============================================================
==== output-redirect
============================================================== -->
<xsl:template name="output-redirect">
	<xsl:param name="file"/>
	<xsl:text>&#x0a;</xsl:text>
	<xsl:text disable-output-escaping="yes"><![CDATA[<!-- BEEJ-GUIDE-OUTPUT-REDIRECT ]]></xsl:text>
	<xsl:value-of select="$file"/>
	<xsl:text disable-output-escaping="yes"><![CDATA[.html -->]]></xsl:text>
	<xsl:text>&#x0a;</xsl:text>
</xsl:template>

<!-- =============================================================
==== output-menu
============================================================== -->
<xsl:template name="output-menu">
	<table width="100%" border="0" cellspacing="0" cellpadding="0">
	<tr>

	<td align="left">
	<xsl:variable name="previd">
		<xsl:value-of select="preceding-sibling::sect1[position()=1]/attribute::id"/>
	</xsl:variable>
	<xsl:choose>
	<xsl:when test="$previd!=''">
		<a>
			<xsl:attribute name="href">
				<xsl:value-of select="$previd"/>
				<xsl:text>.html</xsl:text>
			</xsl:attribute>
			<xsl:text>Prev</xsl:text>
		</a>
	</xsl:when>
	<xsl:otherwise>
		<font color="#7f7f7f">Prev</font>
	</xsl:otherwise>
	</xsl:choose>
	</td>

	<td align="center"><a href="{//@id}.html"><xsl:text>Contents</xsl:text></a></td>

	<td align="right">
	<xsl:variable name="nextid">
		<xsl:value-of select="following-sibling::sect1[position()=1]/attribute::id"/>
	</xsl:variable>
	<xsl:choose>
	<xsl:when test="$nextid!=''">
		<a>
			<xsl:attribute name="href">
				<xsl:value-of select="$nextid"/>
				<xsl:text>.html</xsl:text>
			</xsl:attribute>
			<xsl:text>Next</xsl:text>
		</a>
	</xsl:when>
	<xsl:otherwise>
		<font color="#7f7f7f">Next</font>
	</xsl:otherwise>
	</xsl:choose>
	</td>

	</tr>
	</table>
</xsl:template>

<!-- =============================================================
==== href-page-name
============================================================== -->
<xsl:template name="href-page-name">
	<xsl:param name="ref"/>

	<xsl:for-each select="/descendant::sect1/*">
		<xsl:if test="@id = $ref">
			<!-- we found the id, so grab the name from up the tree -->
			<xsl:value-of select="ancestor::sect1/attribute::id"/>
			<xsl:text>.html#</xsl:text>
			<xsl:value-of select="$ref"/>
		</xsl:if>
	</xsl:for-each>
</xsl:template>

<!-- =============================================================
==== html-header
============================================================== -->
<xsl:template name="html-header">
	<xsl:param name="title"/>
	<xsl:text disable-output-escaping="yes"><![CDATA[<html>]]></xsl:text>
	<head>
	<title><xsl:value-of select="./guideinfo/title"/></title>
	</head>
	<xsl:text disable-output-escaping="yes"><![CDATA[<body bgcolor="#ffffff">]]></xsl:text>
</xsl:template>

<!-- =============================================================
==== html-footer
============================================================== -->
<xsl:template name="html-footer">
	<xsl:text disable-output-escaping="yes"><![CDATA[</body>]]></xsl:text>
	<xsl:text disable-output-escaping="yes"><![CDATA[</html>]]></xsl:text>
</xsl:template>

<!-- =============================================================
==== faq-toc
============================================================== -->
<xsl:template name="faq-toc">
	<xsl:for-each select="faqentry/q">
		<p>
		<a href="#{generate-id()}">
		<xsl:apply-templates select="." mode="faq-toc"/>
		</a>
		</p>
	</xsl:for-each>
</xsl:template>

<!-- =============================================================
==== toc
============================================================== -->
<xsl:template name="toc">
	<h2>Contents</h2>
	<xsl:for-each select="/guide/sect1">
		<dl>
		<dt>
		<xsl:number level="multiple" count="sect1|sect2|manpage" format="1.1: "/>
		<xsl:if test="$multipage = 0">
			<a href="#{@id}"><xsl:apply-templates select="./title" mode="toc"/></a>
		</xsl:if>
		<xsl:if test="$multipage = 1">
			<a href="{@id}.html"><xsl:apply-templates select="./title" mode="toc"/></a>
		</xsl:if>
		</dt>
		<xsl:for-each select="sect2|manpage">
			<dd>
			<xsl:number level="multiple" count="sect1|sect2|manpage" format="1.1: "/>
			<xsl:if test="$multipage = 0">
				<a href="#{@id}"><xsl:apply-templates select="./title" mode="toc"/></a>
			</xsl:if>
			<xsl:if test="$multipage = 1">
				<xsl:variable name="isman"><xsl:value-of select="self::manpage"/></xsl:variable>
				<xsl:choose>
					<xsl:when test="$isman=''">  <!-- we're a sect2 -->
						<a>
							<xsl:attribute name="href">
								<xsl:value-of select="ancestor-or-self::sect1/attribute::id"/>
								<xsl:text>.html#</xsl:text>
								<xsl:value-of select="@id"/>
							</xsl:attribute>
							<xsl:apply-templates select="./title" mode="toc"/>
						</a>
					</xsl:when>
					<xsl:otherwise>  <!-- we're a man page -->
						<a>
							<xsl:attribute name="href">
								<xsl:value-of select="./@id"/>
								<xsl:text>.html</xsl:text>
							</xsl:attribute>
							<xsl:apply-templates select="./title" mode="toc"/>
						</a>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:if>
			</dd>
		</xsl:for-each>
		</dl>
	</xsl:for-each>
</xsl:template>

<!-- =============================================================
==== normal matched templates start here
============================================================== -->

<xsl:template match="/guide">
	<xsl:if test="$multipage = 1">
		<xsl:call-template name="output-redirect">
			<xsl:with-param name="file" select="@id"/>
		</xsl:call-template>
	</xsl:if>

	<xsl:call-template name="html-header">
		<xsl:with-param name="title" select="./guideinfo/title"/>
	</xsl:call-template>

	<xsl:apply-templates/>

	<xsl:if test="$multipage = 0">  <!-- if multi, we do this after toc -->
		<xsl:call-template name="html-footer"/>
	</xsl:if>
</xsl:template>

<xsl:template match="guideinfo">
	<xsl:apply-templates/>
	<hr/>
	<xsl:call-template name="toc"/>

	<xsl:if test="$multipage = 1">  <!-- if not multi, we do this after guide -->
		<hr/>
		<xsl:call-template name="html-footer"/>
	</xsl:if>
</xsl:template>

<xsl:template match="guideinfo/title">
	<p><center><h1><xsl:value-of select="."/></h1></center></p>
</xsl:template>

<xsl:template match="title" mode="toc">
	<xsl:apply-templates/>
</xsl:template>

<xsl:template match="sect1">
	<xsl:if test="$multipage = 1">
		<xsl:call-template name="output-redirect">
			<xsl:with-param name="file" select="@id"/>
		</xsl:call-template>

		<xsl:call-template name="html-header">
			<xsl:with-param name="title" select="./title"/>
		</xsl:call-template>
	</xsl:if>

	<xsl:if test="$multipage = 1">
		<xsl:call-template name="output-menu"/>
		<hr/>
	</xsl:if>

	<xsl:apply-templates/>

	<xsl:if test="$multipage = 1">
		<hr/>
		<xsl:call-template name="output-menu"/>
		<xsl:call-template name="html-footer"/>
	</xsl:if>
</xsl:template>

<xsl:template match="sect1/title">
	<p>
	<xsl:if test="$multipage = 0">
		<br/>
	</xsl:if>
	<h2>
	<xsl:number level="multiple" count="sect1|sect2|manpage" format="1.1: "/>
	<a name="{../@id}">
	<xsl:apply-templates/>
	</a>
	</h2></p>
</xsl:template>

<xsl:template match="sect2/title">
	<p><br/><h3>
	<xsl:number level="multiple" count="sect1|sect2|manpage" format="1.1: "/>
	<a name="{../@id}">
	<xsl:apply-templates/>
	</a>
	</h3></p>
</xsl:template>

<xsl:template match="sect3/title">
	<p><h4>
	<xsl:number level="multiple" count="sect1|sect2|manpage|sect3" format="1.1.1: "/>
	<a name="{../@id}">
	<xsl:apply-templates/>
	</a>
	</h4></p>
</xsl:template>

<xsl:template match="manpage">
	<xsl:if test="$multipage = 1">
		<xsl:call-template name="output-redirect">
			<xsl:with-param name="file" select="@id"/>
		</xsl:call-template>

		<xsl:call-template name="html-header">
			<xsl:with-param name="title" select="./title"/>
		</xsl:call-template>
	</xsl:if>

	<xsl:if test="$multipage = 1">
		<xsl:call-template name="output-menu"/>
		<hr/>
	</xsl:if>

	<xsl:apply-templates/>

	<xsl:if test="$multipage = 1">
		<hr/>
		<xsl:call-template name="output-menu"/>
		<xsl:call-template name="html-footer"/>
	</xsl:if>
</xsl:template>

<xsl:template match="manpage/title">
	<p><h2>
	<a name="{../@id}">
	<xsl:apply-templates/>
	</a>
	</h2></p>
</xsl:template>

<xsl:template match="short">
	<xsl:apply-templates/>
</xsl:template>

<xsl:template match="prototypes">
	<dl><dt><b>Prototypes</b></dt>
	<dd><xsl:apply-templates/></dd></dl>
</xsl:template>

<xsl:template match="description">
	<dl><dt><b>Description</b></dt>
	<dd><xsl:apply-templates/></dd></dl>
</xsl:template>

<xsl:template match="rvalue">
	<dl><dt><b>Return Value</b></dt>
	<dd><xsl:apply-templates/></dd></dl>
</xsl:template>

<xsl:template match="errors">
	<dl><dt><b>Error Codes</b></dt>
	<dd><xsl:apply-templates/></dd></dl>
</xsl:template>

<xsl:template match="example">
	<dl><dt><b>Example</b></dt>
	<dd><xsl:apply-templates/></dd></dl>
</xsl:template>

<xsl:template match="seealso">
	<dl><dt><b>See Also</b></dt>
	<dd><xsl:apply-templates/></dd></dl>
</xsl:template>



<xsl:template match="date">
	<p><center><xsl:value-of select="."/></center></p>
</xsl:template>

<xsl:template match="version">
	<p><center><xsl:value-of select="."/></center></p>
</xsl:template>

<xsl:template match="author">
	<xsl:apply-templates/>
</xsl:template>

<xsl:template match="author/name">
	<p><center><i><b><xsl:value-of select="."/></b></i></center></p>
</xsl:template>

<xsl:template match="author/email">
	<p><center><i>
	<a href="mailto:{.}"><xsl:apply-templates/></a>
	</i></center></p>
</xsl:template>

<xsl:template match="email">
	<tt><a href="mailto:{.}"><xsl:apply-templates/></a></tt>
</xsl:template>

<xsl:template match="copyright">
	<p><center><small><xsl:apply-templates/></small></center></p>
</xsl:template>

<xsl:template match="p">
	<p><xsl:apply-templates/></p>
</xsl:template>

<xsl:template match="func|type|tt|statement">
	<tt><xsl:apply-templates/></tt>
</xsl:template>

<xsl:template match="operator">
	<b><xsl:apply-templates/></b>
</xsl:template>

<xsl:template match="var">
	<b><tt><xsl:apply-templates/></tt></b>
</xsl:template>

<xsl:template match="com|const">
	<b><tt><xsl:apply-templates/></tt></b>
</xsl:template>

<xsl:template match="param|file">
	<i><tt><xsl:apply-templates/></tt></i>
</xsl:template>

<xsl:template match="emp">
	<i><xsl:apply-templates/></i>
</xsl:template>

<xsl:template match="b">
	<b><xsl:apply-templates/></b>
</xsl:template>

<xsl:template match="smiley">
	<tt>:-)</tt>
</xsl:template>

<xsl:template match="super">
	<super><small><xsl:apply-templates/></small></super>
</xsl:template>

<xsl:template match="winky">
	<tt>;-)</tt>
</xsl:template>

<xsl:template match="evilwinky">
	<tt>};-)</tt>
</xsl:template>

<xsl:template match="br">
	<br/>
</xsl:template>

<xsl:template match="quote">
	<blockquote><xsl:apply-templates/></blockquote>
</xsl:template>

<xsl:template match="code|screen">
	<p>
	<table width="100%" border="0" cellpadding="10" cellspacing="0">
	<tr><td bgcolor="#cfcfcf">
	<pre><xsl:apply-templates/></pre>
	</td></tr>
	</table>
	</p>
</xsl:template>

<xsl:template match="offsettext">
	<dl><dt></dt><dd><xsl:apply-templates/></dd></dl>
</xsl:template>

<xsl:template match="numlist">
	<ol><xsl:apply-templates/></ol>
</xsl:template>

<xsl:template match="list">
	<ul><xsl:apply-templates/></ul>
</xsl:template>

<xsl:template match="li">
	<li><xsl:apply-templates/></li>
</xsl:template>

<xsl:template match="deflist">
	<dl compact="t"><xsl:apply-templates/></dl>
</xsl:template>

<xsl:template match="def">
	<xsl:apply-templates/>
</xsl:template>

<xsl:template match="deftitle">
	<dt><b><xsl:apply-templates/></b></dt>
</xsl:template>

<xsl:template match="deftext">
	<dd><xsl:apply-templates/></dd>
</xsl:template>

<xsl:template match="figure">
	<p>
	<center>
	<img src="{image/@file}.gif" alt="{image}"></img>
	<br></br>
	<xsl:apply-templates select="title"/>
	</center>
	</p>
</xsl:template>

<xsl:template match="figure/title">
	<b><xsl:apply-templates/></b>
</xsl:template>

<xsl:template match="image">
	<img src="{@file}.gif" alt="{.}"></img>
</xsl:template>

<xsl:template match="faq">
	<xsl:call-template name="faq-toc"/>
	<p/><br/>
	<dl>
	<xsl:apply-templates/>
	</dl>
</xsl:template>

<xsl:template match="faqentry">
	<xsl:apply-templates/>
</xsl:template>

<xsl:template match="faqentry/q">
	<dt><b><a name="{generate-id()}"><xsl:apply-templates/></a></b></dt>
</xsl:template>

<xsl:template match="faqentry/q" mode="faq-toc">
	<b><xsl:apply-templates/></b>
</xsl:template>

<xsl:template match="faqentry/a">
	<dd><xsl:apply-templates/></dd>
</xsl:template>

<xsl:template match="link">
	<xsl:if test="$multipage = 0">
		<a href="#{@dest}"><xsl:apply-templates/></a>
	</xsl:if>
	<xsl:if test="$multipage = 1">
		<a>
			<xsl:attribute name="href">
				<xsl:call-template name="href-page-name">
					<xsl:with-param name="ref" select="@dest"/>
				</xsl:call-template>
			</xsl:attribute>
			<xsl:apply-templates/>
		</a>
	</xsl:if>
</xsl:template>

<xsl:template match="ulink">
	<a href="{@url}"><xsl:apply-templates/></a>
</xsl:template>

</xsl:stylesheet>
