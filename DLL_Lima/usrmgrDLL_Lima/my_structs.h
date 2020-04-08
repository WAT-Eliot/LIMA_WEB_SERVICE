#pragma once
#include "stdafx.h"
#include <vector>

//Forward declaration des classes
class NavetteCollecte;
class NavetteEvacuation;

class TypeBool;
class TypeAna;
class TypeString;
class Sequence;

//Structure de machines

struct sNameEquipements {
	NavetteCollecte		* APInavetteCollecte;
	NavetteEvacuation	* APInavetteEvacuation;
	Sequence			* g7;
};
extern sNameEquipements sVarEqp;


//Structor pour variables Equipement tronc commun
struct sNameCommun {
	CString		MissionNumber;
	CString		MissionStatus;
	CString		DefautCode;
	CString		DefautLabel;
	CString		EquipementCode;
	CString		RunningMode;
	CString		RunningModeDateTime;
};


struct sNameNavetteCollecte {
	CString		MissionNumber;
	CString		MissionStatus;
	CString		DefautCode;
	CString		DefautLabel;
	CString		EquipementCode;
	CString		RunningMode;
	CString		RunningModeDateTime;
	CString		SorterInputConveyorNumber;
	CString		TapingOutputConveyorNumber;

};

extern sNameNavetteCollecte sVarEqpCollecte;

struct sNameNavetteEvacuation {
	CString		MissionNumber;
	CString		MissionStatus;
	CString		DefautCode;
	CString		DefautLabel;
	CString		EquipementCode;
	CString		RunningMode;
	CString		RunningModeDateTime;
	CString		LayerDestination;
	CString		SorterOutputConveyorNumber;

};

extern sNameNavetteEvacuation sVarEqpEvac;