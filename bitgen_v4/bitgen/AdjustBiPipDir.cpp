#include "AdjustBiPipDir.h"
#include <cassert>
#include <vector>
#include <exception>
#include <sstream>
#include <boost/foreach.hpp>
#include <cstdio>
#define foreach BOOST_FOREACH

using std::vector;
using std::ostringstream;

DriveStatus JudgeDriveStatusInOneTile( const string& portName, const string& tileName, const NetlistNet& netToAdjust )
{
	assert (portName=="LV0" || portName =="LV24" || portName =="LH0" || portName =="LH24");
	//if ( !isBiport )
	//{
	//	NetPip relatedPip;
	//	foreach( const NetPip& pip, netToAdjust.pips )
	//	{
	//		if( tileName == pip.tileName )
	//		{
	//			if( pip.snkNet == portName ) return isDriven;
	//		}
	//	}
	//	return isDriver;
	//}
	//else
	//{

	foreach( const NetPip& pip, netToAdjust.pips )
	{
		if( tileName == pip.tileName )
		{
			if( !isBiPip(pip) && portName == pip.snkNet ) {
				return isDriver;
			}
		}
	}
	foreach( const NetPip& pip, netToAdjust.pips )
	{
		if( tileName == pip.tileName )
		{
			if( isBiPip(pip) && (portName == pip.snkNet || portName == pip.srcNet) ) {
				string otherPortName = (portName == pip.snkNet) ? pip.srcNet : pip.snkNet;
				foreach( const NetPip& pip, netToAdjust.pips )
				{
					if( tileName == pip.tileName )
					{
						if( !isBiPip(pip) && otherPortName == pip.snkNet ) {
							return isDriven;
						}
					}
				}
				//return isUnknown;
			}
		}
	}

	return isUnknown;
	//}
}

//DriveStatus JudgeDriveStatus( const string& portName, const string& tileName, const NetlistNet& netToAdjust )
//{
//	//string startTileName = pipToAdjust.tileName;
//	DriveStatus driveStatusInCurTile = JudgeDriveStatusInOneTile( portName, tileName, netToAdjust );
//	if ( driveStatusInCurTile == isUnknown )
//	{
//		string endTilePort;
//		string endTileName;
//		GetConnPortOfEndTile( endTilePort, endTileName, portName, tileName, netToAdjust );
//		//if ( endTilePort == "" ) return isDriven;
//		if ( endTilePort != "")
//		DriveStatus driveStatusInEndTile = JudgeDriveStatusInOneTile( endTilePort, endTileName, netToAdjust );
//		switch( JudgeDriveStatusGlobal(endTilePort, endTileName, netToAdjust) ) {
//			case isDriver : return isDriven; break;
//			case isDriven : return isDriver; break;
//			case isUnknown: return isUnknown; break;
//			default: break;
//
//		}
//	}
//	return driveStatusInCurTile;
//}

DriveStatus JudgeDriveStatus( const string& portName, const string& tileName, const NetlistNet& netToAdjust )
{
	DriveStatus driveStatusInStartTile = JudgeDriveStatusInOneTile(portName, tileName, netToAdjust );
	if( driveStatusInStartTile == isUnknown ) {
		/*string endTilePort;
		string endTileName;*/
		vector<TilePort> connPorts;
		GetConnPortsOfEndTile( connPorts, portName, tileName, netToAdjust );
		if ( connPorts.empty() ) return isUnknown;
		//bool hasTerminalPort = false;
		TilePort terminalPort;
		foreach( const TilePort& port, connPorts )
		{
			if ( port.portName == "LH0" || port.portName == "LH24" || port.portName == "LV0" || port.portName == "LV24" )
				terminalPort = port;
		}
		if ( terminalPort.portName != "" ) {			
			DriveStatus driveStatusInEndTile = JudgeDriveStatusInOneTile( terminalPort.portName, terminalPort.tileName, netToAdjust );
			/*switch( driveStatusInEndTile ) {
				case isDriver : return isDriven; break;
				case isDriven : return isDriver; break;
				case isUnknown: return isUnknown; break;
				default: break;
			}*/
			if( driveStatusInEndTile == isDriver ) {
				return isDriver;
			} else {
				return isUnknown;
			}
		}
		else {
			return isDriven;
		}
	}
	return driveStatusInStartTile;
}



void GetConnPortsOfEndTile(vector<TilePort>& connPorts, const string& startTilePort, const string& startTileName, const NetlistNet& netToAdjust )
{
	int x, y;
	char dir = startTilePort[1];
	
	sscanf_s( startTileName.c_str(), "INT_X%dY%d", &x, &y );
	for( int i = 0 ; i < 4; ++i ) {

		if ( startTilePort == "LH0") {
			x = x - 6;
		}
		if ( startTilePort == "LH24") {
			x = x + 6;
		}
		if ( startTilePort == "LV0" ) {
			y = y - 6;
		}
		if ( startTilePort == "LV24") {
			y = y + 6;
		}
		//if (x < 0 || y < 0) break;
		ostringstream oss;
		oss << "INT_X" << x << "Y" << y;
		string curTileName = oss.str();
		foreach ( const NetPip& pip, netToAdjust.pips ) {
			if ( pip.tileName == curTileName ) {
				ostringstream oss;
				int d = (startTilePort=="LH0"||startTilePort=="LV0") ? (6*i+6): (18-6*i);
				oss << 'L' << dir << d;
				string longLinePort = oss.str();
				if ( pip.srcNet == longLinePort || pip.snkNet == longLinePort ) {
					TilePort aPort;
					aPort.portName = longLinePort;
					aPort.tileName = curTileName;
					connPorts.push_back(aPort);
				}
			}
		}
	}

}


bool TryAdjustBiPipDir( NetPip& pipToAdjust, NetlistNet& netToAdjust )
{
	//if( pipToAdjust.dirType == "->" ) return true;
	assert( pipToAdjust.dirType == "=-");
	string leftPort = pipToAdjust.srcNet;
	string rightPort = pipToAdjust.snkNet;

	DriveStatus leftPortDriveStatus = JudgeDriveStatus( leftPort, pipToAdjust.tileName, netToAdjust );
	DriveStatus rightPortDriveStatus = JudgeDriveStatus( rightPort, pipToAdjust.tileName, netToAdjust);
	if ( leftPortDriveStatus == isDriver || rightPortDriveStatus == isDriven ) {
		pipToAdjust.dirType = "->";
		return true;
	}
	else if ( rightPortDriveStatus == isDriver || leftPortDriveStatus == isDriven ) {
		pipToAdjust.dirType = "->";
		string temp;
		temp = pipToAdjust.srcNet;
		pipToAdjust.srcNet = pipToAdjust.snkNet;
		pipToAdjust.snkNet = temp;
		return true;
	}
	else {
		return false;
	}

}

bool isBiPort( const string & portName )
{ 
		if( portName == "LH24" ||
		    portName == "LH0"  ||
		    portName == "LV24" ||
		    portName == "LV0")
			return true;
		else
			return false;
}

bool isBiPip( const NetPip& pip )
{
	return pip.dirType == "=-";
}


void AdjustNet( NetlistNet& net )
{
	//先扫描一遍看有多少双向线网
	int biPipAmount = 0;
	foreach ( const NetPip & pip, net.pips )
	{
		if ( pip.dirType == "=-" )
		{
			biPipAmount++;
		}
	}
	int i = 0;
	while( i < biPipAmount )
	{
		AdjustNetOneRound(net);
		i++;
	}

	biPipAmount = 0;
	foreach ( const NetPip & pip, net.pips )
	{
		if ( pip.dirType == "=-" )
		{
			biPipAmount++;
		}
	}
	if ( biPipAmount != 0 ) throw std::exception("Cannot judge the direction of the net");
}

void AdjustNetOneRound( NetlistNet& net )
{
	foreach( NetPip& pip, net.pips )
	{
		if ( pip.dirType == "=-" ) {
			TryAdjustBiPipDir(pip, net);
		}
	}
}