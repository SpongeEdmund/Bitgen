
#ifndef __CIL_VISITOR_H__
#define __CIL_VISITOR_H__


class ElementLib; class GrmLib; class SiteLib;
class BramLib; class TileLib; class ArchLib;

class CilVisitor
{
public:
	virtual ~CilVisitor() {}

	virtual void visitElementLib( ElementLib* elementLib ) = 0;
	virtual void visitGrmLib( GrmLib* grmLib ) = 0;
	virtual void visitSiteLib( SiteLib* siteLib ) = 0;
	virtual void visitBramLib( BramLib* bramLib ) = 0;
	virtual void visitTileLib( TileLib* tileLib ) = 0;
	virtual void visitArchLib( ArchLib* archLib ) = 0;
};

#endif