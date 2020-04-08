#include "StdAfx.h"
#include "my_structs.h"

//Structure de machines

 sNameEquipements sVarEqp{
	 NULL,
	 NULL,
	 NULL,
};


//Ici mettre tous les noms de variables PCVue

//Robot Bacs Bleus
CString tempPrefixe = "Mx.NavetteCollecte.";
sNameNavetteCollecte sVarEqpCollecte{
	tempPrefixe + "MissionNumber",
	tempPrefixe + "MissionStatus",
	tempPrefixe + "DefautCode",
	tempPrefixe + "DefautLabel",
	tempPrefixe + "EquipementCode",
	tempPrefixe + "RunningMode",
	tempPrefixe + "RunningModeDateTime",
	tempPrefixe + "SorterInputConveyorNumber",
	tempPrefixe + "TapingOutputConveyorNumber",

};

//Robot Couvercle
CString tempPrefixe2 = "Mx.NavetteEvacuation.";
sNameNavetteEvacuation sVarEqpEvac{
	tempPrefixe2 + "MissionNumber",
	tempPrefixe2 + "MissionStatus",
	tempPrefixe2 + "DefautCode",
	tempPrefixe2 + "DefautLabel",
	tempPrefixe2 + "EquipementCode",
	tempPrefixe2 + "RunningMode",
	tempPrefixe2 + "RunningModeDateTime",
	tempPrefixe2 + "LayerDestination",
	tempPrefixe2 + "SorterOutputConveyorNumber",
	
};