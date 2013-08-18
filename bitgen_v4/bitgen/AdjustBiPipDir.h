#ifndef __ADJUST_BIPIP_DIR_H__
#define __ADJUST_BIPIP_DIR_H__


#include "Netlist.h"
#include <string>
#include <vector>

const int MAXX = 100;
const int MAXY = 100;

using std::string;
using std::vector;
// DriveStatus是指端口在pip中的角色
enum DriveStatus { isDriven, isDriver, isUnknown };


//
DriveStatus JudgeDriveStatusInOneTile(
	const string& portName,
	const string& tileName,
	const NetlistNet& netToAdjust
	);

//
DriveStatus JudgeDriveStatus(
	const string& portName,
	const string& tileName,
	const NetlistNet& netToAdjust
	);

struct TilePort {
	string portName;
	string tileName;
};

void GetConnPortsOfEndTile(
	/*string& endTilePort, 
	string& endTileName, */
	vector<TilePort>& connPorts,
	const string& curTilePort,
	const string& curTileName,
	const NetlistNet& netToAdjust 
	);
// 
bool TryAdjustBiPipDir( NetPip& pipToAdjust, NetlistNet& net );

bool isBiPort( const string & portName );
bool isBiPip( const NetPip & pip );

void AdjustNet( NetlistNet& net );
void AdjustNetOneRound( NetlistNet& net);
#endif