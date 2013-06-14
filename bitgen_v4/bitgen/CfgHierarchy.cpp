#include "CfgHierarchy.h"
#include <sstream>

using namespace bitgen;
using namespace std;

CfgHierarchy::CfgHierarchy()			
	: _curChip(0),_curArch(0),_curPackage(0),
	_curTileInst(0),_curTile(0),

	_curClusterInst(0),_curCluster(0),
	_curSiteInst(0),_curSite(0),
	_curDist(0),
	_curAttr(0),_curOption(0),

	_curBram(0),_curCell(0),

	_curGrmInst(0),_curGrm(0),
	_curRoute(0),_curStep(0),

	_curElementInst(0),_curElement(0),_curPath(0),

	_curSram(0)
{
}

void CfgHierarchy::clear() 
{
	_curChip=0;_curArch=0;_curPackage=0;
	_curTileInst=0;_curTile=0;

	_curClusterInst=0;_curCluster=0;
	_curSiteInst=0;_curSite=0;
	_curDist=0;
	_curAttr=0;_curOption=0;

	_curBram=0;_curCell=0;

	_curGrmInst=0;_curGrm=0;
	_curRoute=0;_curStep=0;

	_curElementInst=0;_curElement=0;_curPath=0;

	_curSram=0;
}

std::string CfgHierarchy::getInstHir() const
{
	stringstream sstr("");
	if ( !_curTileInst )
		sstr << "Tile:" << _curTileInst->get_name() << "("
		<< _curTileInst->get_ref() << ") ";

	if ( !_curClusterInst )
		sstr << "Cluster:" << _curClusterInst->get_name() << "("
		<< _curClusterInst->get_ref() << ") ";

	if ( !_curSiteInst )
		sstr << "Site:" << _curSiteInst->get_name() << "("
		<< _curSiteInst->get_ref() << ") ";

	if ( !_curAttr )
		sstr << "Attribute:" << _curAttr->get_name() << " ";

	if ( !_curOption )
		sstr << "Option:" << _curOption->get_name();

	return sstr.str();

}

std::string CfgHierarchy::getBramHir() const
{
	stringstream sstr("");
	if ( !_curTileInst )
		sstr << "Tile:" << _curTileInst->get_name() << "("
		<< _curTileInst->get_ref() << ") ";

	if ( !_curBram )
		sstr << "Bram Type:" << _curBram->get_type() << " ";

	if ( !_curCell )
		sstr << "Bram Cell:" << _curCell->get_address() << " ";


	return sstr.str();
}

std::string CfgHierarchy::getNetHir() const
{
	stringstream sstr("");
	if ( !_curTileInst )
		sstr << "Tile:" << _curTileInst->get_name() << "("
		<< _curTileInst->get_ref() << ") ";

	if ( !_curGrmInst )
		sstr << "Grm:" << _curGrmInst->get_name() << "("
		<< _curGrmInst->get_ref() << ") ";

	if ( !_curRoute )
		sstr << "Route:" << _curRoute->get_src() << "->"
		<< _curRoute->get_snk() << " ";
	
	if ( !_curElementInst )
		sstr << "Element:" << _curElementInst->get_name() << "("
		<< _curElementInst->get_ref() << ") ";

	if ( !_curStep )
		sstr << "Path:" << _curStep->get_from() << "->"
		<< _curStep->get_to();


	return sstr.str();
}