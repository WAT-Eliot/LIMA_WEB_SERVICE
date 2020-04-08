using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace usrmgrDotNetProject
{
    
    sNameEquipements sVarEqp
    {
        NULL,
	    NULL,
	    NULL,
    };


    //Ici mettre tous les noms de variables PCVue

    //Navette Collecte
    //string tempPrefixe = "Mx.NavetteCollecte.";
    sNameNavetteCollecte sVarEqpCollecte
    {
        "Mx.NavetteCollecte.Test",
        /*
        tempPrefixe + "MissionNumber",
	    tempPrefixe + "MissionStatus",
	    tempPrefixe + "DefautCode",
	    tempPrefixe + "DefautLabel",
	    tempPrefixe + "EquipementCode",
	    tempPrefixe + "RunningMode",
	    tempPrefixe + "RunningModeDateTime",
	    tempPrefixe + "SorterInputConveyorNumber",
	    tempPrefixe + "TapingOutputConveyorNumber",
        */

    };

    sNameNavetteCollecte sVarEqpCollecte;

    //Navette Evacuation
    /*
    public string tempPrefixe2 = "Mx.NavetteEvacuation.";
    sNameNavetteEvacuation sVarEqpEvac
    {
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
    */

    struct sNameEquipements
    {
        NavetteCollecte APInavetteCollecte;
        //NavetteEvacuation* APInavetteEvacuation;
        //Sequence* g7;
    };
extern sNameEquipements sVarEqp;


//Structor pour variables Equipement tronc commun
struct sNameCommun
    {
        string MissionNumber;
        string MissionStatus;
        string DefautCode;
        string DefautLabel;
        string EquipementCode;
        string RunningMode;
        string RunningModeDateTime;
    };


    struct sNameNavetteCollecte
    {
        string MissionNumber;
        string MissionStatus;
        string DefautCode;
        string DefautLabel;
        string EquipementCode;
        string RunningMode;
        string RunningModeDateTime;
        string SorterInputConveyorNumber;
        string TapingOutputConveyorNumber;

    };

extern sNameNavetteCollecte sVarEqpCollecte;

struct sNameNavetteEvacuation
    {
        string MissionNumber;
        string MissionStatus;
        string DefautCode;
        string DefautLabel;
        string EquipementCode;
        string RunningMode;
        string RunningModeDateTime;
        string LayerDestination;
        string SorterOutputConveyorNumber;

    };

extern sNameNavetteEvacuation sVarEqpEvac;

    class Equipement
    {
        
        //Liste des varaibles communes
        protected struct sMemberCommun
        {
            public TypeBool* Test = new &TypeBool();
        };

        protected sMemberCommun m_varCommun;

        public void  Main(sNameCommun names, int start)
        {
            m_varCommun.Test = new TypeBool(names.Test, start + 1, 0);
        }
    }

    class NavetteCollecte : Equipement
    {

        public struct sMemberNavetteCollecte
        {
            //TypeAna* SorterInputConveyorNumber;
            //TypeAna* TapingOutputConveyorNumber;

        };

        public sMemberNavetteCollecte m_varNavetteCollecte;


        public void Main(sNameNavetteCollecte names, int start)
        {

        }
    }
}
