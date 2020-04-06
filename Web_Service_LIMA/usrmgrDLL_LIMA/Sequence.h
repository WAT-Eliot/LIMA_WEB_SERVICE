#pragma once
#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <sstream>
#include "Equipement.h"

//Import PCVue methods
extern IAPIMgr * svmgrAPI;
//Import variables: 
//extern MES * mesRayPro;


class Sequence
{
	//Consts
	const enum {
		//Production
		PROD_ATTENTE_SETUP = 0,
		PROD_ATTENTE_PROVENANCE = 2,
		PROD_PROPRETE_ROBOT = 5,
		PROD_CONFIG_GENERALE = 10,
		PROD_ATTENTE_VALIDATION = 15,
		PROD_ENVOI_DEPART_CYCLE = 20,
		PROD_SUIVI_DE_PROD = 25,
		PROD_MODE_VIDANGE_EQUIPEMENT = 30,
		PROD_PHASE_ROMPUS = 35,
		PROD_MODE_VIDANGE_ROBOTBACSBLEUS = 37,
		PROD_ARRET_MACHINES = 40,
		PROD_CAMPAGNE_TERMINEE = 45,
		//Communication lors du setup
		COM_SETUP_ETAT_INITIAL,
		COM_SETUP_LECTURE_SETUP,
		COM_SETUP_ATTENTE_RAZ_MES,
		COM_SETUP_RAZ_SPV,
		//Communication lors de fin campagne
		COM_FIN_CAMPAGNE_ETAT_INITIAL,
		COM_FIN_CAMPAGNE_DME_VIDANGE,
		COM_FIN_CAMPAGNE_RAZ_SEQUENCE,
		//Communication alarmes
		COM_ALARME_ETAT_INITIAL,
		COM_ALARME_ECRITURE,
		COM_ALARME_RAZ_SEQUENCE,
		//Communication HU Inputs
		COM_HU_ETAT_INITIAL,
		COM_HU_ECRITURE,
		COM_HU_RAZ_SEQUENCE,
	};

	//Members

	std::vector<TypeAna *>	vNumEtape;
	std::vector<int>		vNumSequComSetup;
	std::vector<int>		vNumSequComFinCampagne;
	std::vector<int>		vNumSequComAlarme;
	std::vector<int>		vNumSequComHU;
	std::vector<int>		vNumSequComDummySetup;
	std::vector<int>		vMemEtapeDegrade;
	std::vector<int>		vCptComSetup;
	int m_mode;
	int m_delay;
	int m_countSec;
	std::vector<int> m_etatComMES;
	std::vector<bool> m_setupDegrade;
	int m_memAlmEnCoursL1;
	int m_memAlmEnCoursL2;
	std::vector<double>		m_rayproCounterMem;

	std::vector<Equipement*> vConfigCampagne[2];
	std::vector<Equipement*> vConfigCampagne_Temp[2]; // Ajout 06/02/2020
	std::vector<Equipement*> vPropreteCampagne[2];
	std::vector<Equipement*> vPropreteCampagne_Temp[2]; // Ajout 06/02/2020
	std::vector<Equipement*> vModeDistant[2]; // Ajout 04/12/19
	std::vector<Equipement*> vModeDistant_Temp[2]; // Ajout 10/12/19
	std::vector<Equipement*> vScrutation[2]; // Ajout 11/12/19


	boost::thread m_tG7Main;
	boost::thread m_tG7ComAlarme1;
	boost::thread m_tG7ComAlarme2;
	boost::thread m_tG7ComHUInputL1;
	boost::thread m_tG7ComHUInputL2;
	boost::thread m_tG7ComDummySetup1;
	boost::thread m_tG7ComDummySetup2;

	boost::mutex mutHUL1;
	boost::mutex mutHUL2;
	boost::mutex mutAlarmeL1;
	boost::mutex mutAlarmeL2;
	boost::mutex mutComDummySetup1;
	boost::mutex mutComDummySetup2;

	//vars Tampon PCVue
	bool spvAttenteLot;
	bool spvEnAttenteLancementProd;
	bool mesConfirmeLancementProd;
	bool mesDemandeFinProd;
	//int spvSyncSetup;
	//int mesSyncSetup;
	int cptComSetupL1;
	int cptComSetupL2;


	//HU Input
	std::vector<huInput> vHUInputL1;
	std::vector<huInput> vHUInputL2;

	std::vector<std::string>	vConditionnement_Ref;
	std::vector<std::string>	vConditionnement_Type;
	std::vector<std::string>	vConditionnement_Coiffe;


public:
	//Constructor
	Sequence(void);

	//Start sequence
	void Start();

	//Start sequence alarme
	void StartAlarme(int id, double num, bool val);

	//Stop sequence
	void Stop();

	//G7 de production 
	void G7Main();

	//Check datas 
	int GetComSetup(int ligne);

	//Check datas 
	int GetComFinCampagne(int ligne); // NON UTILISEE

	//Gestion bit de vie : incrémente le membre m_etatComMES pour chaque echec de comm
	//Puis le RAZ a chaque succès
	int BitDeVie(int ligne, int cptTries);

	//phase d'envoi des demandes de configuration
	bool phaseConfig(int ligne);

	//Phase d'envoi de la demande de configuration du robot caisse
	bool phaseConfigRobot(sFlatSetup oldSetup, sFlatSetup setup, int ligne);

	//Phase d'envoi des demandes de marche
	bool phaseEnvoiMarche(int ligne);

	//Phase d'envoi des demandes vidange
	bool phaseEnvoiVidange(int ligne);

	//Phase d'envoi des demandes d'arret
	bool phaseArret(int ligne);

	//Récupérer le setup
	bool GetMesSetup(int ligne);
	
	//Check datas 
	void GetComAlarme(int id, double num, bool val, int ligne);

	//Com HU
	void GetComHU(int ligne);

	//Repasser en Condition Initialles
	
	bool CI_Vector(int ligne); // Ajout 19/03/2020
	bool CI_Arret(int ligne); // Ajout 19/03/2020
	bool CI_Proprete(int ligne); // Ajout 23/03/2020
	bool CI_CommMes(int ligne);
	bool CI_Production(int ligne);
	bool CI_SPV(int ligne);
	bool CI_Scrutation(int ligne); // Ajout 23/03/2020
	bool CI_Vidange(int ligne); // Ajout 23/03/2020
	bool CI_Configuration(int ligne); // Ajout 23/03/2020
	bool CI_Campagne(int ligne); // Ajout 27/03/2020

	//Jump
	bool JumpVidange(int ligne);
	bool JumpArretMachine(int ligne);

	//Destructor
	~Sequence(void);

	int GetAlrmEtape(int ligne);

	int GetMemAlmEnCours(int ligne);
	void SetMemAlmEnCours(int ligne, int val);

	void AddHUTable(int ligne, double contID, CString batchID, CString huID, double qte);
	//maj du fichier en fonction du vecteur
	void MajFichier(int ligne, std::vector<huInput> vec);
	//import du fichier au lancement de la sup
	void ImportFichierHuInput(int ligne);

	//Savoir si la campagne courante sur ligne 1 est un OF d'humidification
	bool GetOFHumidification();

	void CreateFichierEtape(int ligne, int etape);

	void CreateFichierSetup(int ligne, sFlatSetup setup);

	void CreateFichierVProprete(int ligne, std::vector<Equipement*> vec);

	void CreateFichierVConfig(int ligne, std::vector<Equipement*> vec);

	int ImportFichierEtape(int ligne);

	void ImportFichierSetup(int ligne);

	void ImportFichierVProprete(int ligne);

	void ImportFichierVConfig(int ligne);

	bool GetG7FormAsync(int formeuse);

	bool isSetupDuplicate(int ligne);

	void ComDummySetup(int ligne);

	bool GetVectorModeDistant(int ligne); //Ajout 04/12/19

	void CreateFichierVModeDistant(int ligne, std::vector<Equipement*> vec); //Ajout 04/12/19

	void ImportFichierVModeDistant(int ligne); //Ajout 04/12/19

	bool CI_ModeDistant(int ligne); //Ajout 04/12/19

	bool ScrutationModeDistant(int ligne); //Ajout 10/12/19

	bool VerificationModeDistant(int ligne); //Ajout 10/12/19

	bool GetVectorScrutation(int ligne); //Ajout 11/12/19

	void CreateFichierVScrutation(int ligne, std::vector<Equipement*> vec); //Ajout 11/12/19

	void ImportFichierVScrutation(int ligne); //Ajout 11/12/19

	//Initialisation des demandes pour la ligne corespondante
	bool InitDemande(int ligne); //Ajout 26/12/19

	//Sortir de la formeuse quand passage en intoduction manuelle
	bool SortieFormeuseProprete(int ligne); //Ajout 06/02/2020

	//Sortir de la formeuse quand passage en intoduction manuelle
	bool SortieFormeuseConfiguration(int ligne); //Ajout 06/02/2020

	//Sortir de la formeuse quand passage en intoduction manuelle
	bool SortieFormeuseVidange(int ligne); //Ajout 06/02/2020

	bool GetBesoinFormeuse(int ligne, int iPosition); // Ajout 07/02/2020

	void ImportFichierConditionnement(); // Ajout 19/03/2020

	//renvoi de la liste des équipements qui doivent tourner
	bool GetVectorConfig(int ligne, int Position); // Ajout 19/03/2020

	//renvoi de la liste des équipements qui doivent tourner
	bool GetVectorProprete(int ligne, int Position); // Ajout 19/03/2020

	int DefPositionRefConditionnement(int ligne); // Ajout 19/03/2020

};


template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << a_value;
	return out.str();
}