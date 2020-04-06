#pragma once
#include "my_structs.h"
#include <vector>
#include <memory>
#include <iostream>
#include <fstream> 


//Timeout de config = 120 x cycle (de base 1 seconde)
#define TIMEOUT_CONFIG 120
//Timeout plus long pour les équipements à réglage manu (Ensacheuse et Formeuse) 30min ->
#define TIMEOUT_CONFIG_MANUEL 1800

//Import PCVue methods
extern IAPIMgr * svmgrAPI;

const enum {
	ETAT_INDETERMINE		= 0,
	ETAT_ARRET				= 1,
	ETAT_PRODUCTION			= 2,
	ETAT_DEF_NON_BLOQUANT	= 3,
	ETAT_DEF_BLOQUANT		= 4,
	ETAT_DEM_PROPRE			= 5,
	ETAT_PROPRETE			= 6,
	ETAT_DEM_CONFIG			= 7,
	ETAT_CONFIGURATION		= 8,
	ETAT_DEM_VIDANGE		= 9,
	ETAT_VIDE				= 10,
};

class Variable
{
protected:
	CString m_name;       // Variable that will be read to after update
	int m_adviseN;		//Token for advise

public:

	Variable();
	Variable(CString name, int adviseN);

	CString GetVarName();
	void Advise();
	void Unadvise();
	int GetAdviseNumber();
	virtual void WriteVar();
//	virtual void SetVar();

	~Variable();
};

class TypeBool : public Variable {

	bool m_value;
public:
	TypeBool();
	TypeBool(CString name, int adviseN, bool value);
	void SetVar(bool value);
	void WriteVar(bool value);
	bool GetVarValue();
};

class TypeAna : public Variable {

	double m_value;
public:
	TypeAna();
	TypeAna(CString name, int adviseN, double value);

	void WriteVar(double value);
	void SetVar(double value);
	double GetVarValue();
};

class TypeString : public Variable {

	CString m_value;
public:
	TypeString();
	TypeString(CString name, int adviseN, CString value);

	void WriteVar(CString value);
	void SetVar(CString value);
	CString GetVarValue();
};

class Equipement
{
protected:
	
	//Liste des varaibles communes
	struct sMemberCommun {
		TypeBool	* demandeDistant;
		TypeBool	* modeDistant;
		TypeBool	* alarmeNonBloquante;
		TypeBool	* alarmeBloquante;
		TypeAna		* numAlarme;
		TypeAna		* etatAnimation;
		TypeAna		* etatAPI;
		TypeAna		* mdvSPV;
		TypeAna		* mdvAPI;
		TypeBool	* timeoutComAPI;
		TypeBool	* timeoutMarcheAPI;
	} m_varCommun;
	int cptTimeoutConfig;
	int cptTimeoutMarche;
	int cptTimeoutCom;
	int memMdVAPI;
	static int ID;
	int IDmachine;
	bool modeDistant_OLD; //Ajout 10/12/19
	bool machineSortie_L1; //Ajout 11/12/19
	bool machineSortie_L2; //Ajout 11/12/19

public:

	Equipement();
	Equipement(sNameCommun names, int start);
	virtual bool GestionProprete(int ligne);
	virtual bool GestionConfig(int ligne);
	virtual bool EnvoiDemVidange(int ligne, bool ValidTare); //Modif 12/12/19
	virtual bool ResetDemandeProprete(int ligne);
	virtual bool ResetDemandeVidange(int ligne);
	virtual bool ResetDemandeConfiguration(int ligne);
	virtual bool SetMotEtat();
	bool GetModeDistant(); //Retourne true si l'équipement est distant
	bool GetModeDistant_OLD(); // Ajout 10/12/19 - Retourne true si l'équipement était distant
	bool SetModeDistant_OLD(bool val); // Ajout 10/12/19 - Affecte l'etat de l'équipement distant/local
	virtual bool EnvoiDemMarche(int ligne);
	virtual bool EnvoiDmeArret(int ligne);
	int GetCodeDef();
	bool MotDeVieAPI();
	bool SetMachineSortie(int ligne, bool val); // Ajout 11/12/19 - Affecte le bit machine sortie
	virtual double GetEtatAPI(int ligne); //Ajout 26/12/19 - Retourne l'état de l'API en fonction de la ligne
	virtual bool SetCampagne(int ligne, bool val); // Ajout 27/03/2020
	~Equipement();
};

class RobotCaisses : public Equipement {

	struct sMemberRobotCaisses {
		TypeBool		* configureL1;
		TypeBool		* configureL2;
		TypeAna			* circuitPropreteL1;
		TypeAna			* circuitPropreteL2;
		TypeAna			* paramPropreteL1;
		TypeAna			* paramPropreteL2;
		TypeAna			* codeArticleL1;
		TypeAna			* codeArticleL2;
		TypeAna			* RnumeroBacL1;
		TypeAna			* RnumeroBacL2;
		TypeAna			* RcodeArticleL1;
		TypeAna			* RcodeArticleL2;
		TypeAna			* RqtePiecesL1;
		TypeAna			* RqtePiecesL2;
		TypeBool		* demandePropreteL1;
		TypeBool		* demandePropreteL2;
		TypeBool		* etatPropreteL1;
		TypeBool		* etatPropreteL2;
		TypeBool		* demandeConfigL1;
		TypeBool		* demandeConfigL2;
		TypeBool		* timeoutConfigL1;
		TypeBool		* timeoutConfigL2;
		TypeBool		* demandeVidangeL1;
		TypeBool		* modeVidangeL1;
		TypeBool		* demandeVidangeL2;
		TypeBool		* modeVidangeL2;
		TypeAna			* etatAnimationL1;
		TypeAna			* etatAnimationL2;
		TypeAna			* etatAPIL1;
		TypeAna			* etatAPIL2;
		TypeBool		* topBacVerseL1;
		TypeBool		* ackBacVerseL1;
		TypeBool		* topBacVerseL2;
		TypeBool		* ackBacVerseL2;
		TypeAna			* HUcontIDL1;
		TypeString		* HUbatchIDL1;
		TypeString		* HUIDL1;
		TypeAna			* HUqtyL1;
		TypeAna			* HUcontIDL2;
		TypeString		* HUbatchIDL2;
		TypeString		* HUIDL2;
		TypeAna			* HUqtyL2;
		TypeBool		* demandeMarcheL1;
		TypeBool		* demandeMarcheL2;
		TypeBool		* demandeMarche;
		TypeBool		* CampagneL1; //Ajout 19/03/2020
		TypeBool		* CampagneL2; //Ajout 19/03/2020
		TypeBool		* DemandeArticleL1; //Ajout 24/03/2020
		TypeBool		* DemandeArticleL2; //Ajout 24/03/2020
	} m_varRobotCaisses;
	bool FlagModeDistantRobotBacsBleus1;
	bool FlagModeDistantRobotBacsBleus2;

public:
	RobotCaisses();
	RobotCaisses(sNameRobotCaisses names, int start);
	bool GestionProprete(int ligne);
	bool GestionConfig(int ligne);
	bool EnvoiDemVidange(int ligne, bool ValidTare);
	bool ResetDemandeProprete(int ligne);
	bool ResetDemandeVidange(int ligne);
	bool ResetDemandeConfiguration(int ligne);
	bool SetMotEtat();

	int GetAdvNtopBac(int ligne);
	std::string GetLineHU(int ligne);
	double GetHUcontID(int ligne);
	CString GetbatchID(int ligne);
	CString GetHUID(int ligne);
	double GetHUqty(int ligne);
	bool ResetTopBacVerse(int ligne);
	bool SetAckBacVerse(int ligne);
	bool ResetAckBacVerse(int ligne);
	bool GetAckBacVerse(int ligne);
	bool GetTopBacVerse(int ligne);

	bool EnvoiDemMarche(int ligne);
	bool EnvoiDmeArret(int ligne);

	bool EnvoiMarcheCellule();
	bool GetFlagModeDistant(int ligne); //Ajout 26/12/19
	bool SetFlagModeDistant(int ligne, bool val); //Ajout 26/12/19
	double GetEtatAPI(int ligne); // Ajout 26/12/19
	bool SetCampagne(int ligne, bool val); // Ajout 19/03/2020
	bool GetDemandeArticle(int ligne); // Ajout 24/03/2020
};

class RobotCouvercle : public Equipement {

	struct sMemberRobotCouvercle {
		TypeBool		* configureL1;
		TypeBool		* configureL2;
		TypeAna			* circuitPropreteL1;
		TypeAna			* circuitPropreteL2;
		TypeAna			* paramPropreteL1;
		TypeAna			* paramPropreteL2;
		TypeBool		* magasinCouvercleNivBas;
		TypeBool		* demandePropreteL1;
		TypeBool		* demandePropreteL2;
		TypeBool		* etatPropreteL1;
		TypeBool		* etatPropreteL2;
		TypeBool		* demandeVidangeL1;
		TypeBool		* modeVidangeL1;
		TypeBool		* demandeVidangeL2;
		TypeBool		* modeVidangeL2;
		TypeAna			* etatAnimationL1;
		TypeAna			* etatAnimationL2;
		TypeAna			* etatAPIL1;
		TypeAna			* etatAPIL2;
		TypeBool		* demandeConfigL1;
		TypeBool		* demandeConfigL2;
		TypeBool		* demandeMarcheL1;
		TypeBool		* demandeMarcheL2;
		TypeBool		* magEtiquetteNivBasL1;
		TypeBool		* magEtiquetteNivBasL2;
		TypeBool		* demandeMarche;
		TypeBool		* CampagneL1; //Ajout 19/03/2020
		TypeBool		* CampagneL2; //Ajout 19/03/2020
	} m_varRobotCouvercle;


	

public:
	RobotCouvercle();
	RobotCouvercle(sNameRobotCouvercle names, int start);
	bool GestionProprete(int ligne);
	bool GestionConfig(int ligne);
	bool EnvoiDemVidange(int ligne, bool ValidTare); //Modif 12/12/19
	bool ResetDemandeProprete(int ligne);
	bool ResetDemandeVidange(int ligne);
	bool ResetDemandeConfiguration(int ligne);
	bool SetMotEtat();
	bool EnvoiDemMarche(int ligne);
	bool EnvoiDmeArret(int ligne);
	bool EnvoiMarcheCellule();
	bool SetCampagne(int ligne, bool val); // Ajout 19/03/2020
};

class Ensacheuse : public Equipement {
	struct sMemberEnsacheuse {
		TypeBool		* configure;
		TypeBool		* finRouleauFilm;
		TypeAna			* hauteurSachet;
		TypeAna			* adjonctionEau;
		TypeAna			* qteEauReelle;
		TypeAna			* circuitProprete;
		TypeAna			* paramProprete;
		TypeBool		* demandeProprete;
		TypeBool		* etatProprete;
		TypeBool		* demandeConfig;
		TypeBool		* timeoutConfig;
		TypeBool		* demandeVidange;
		TypeBool		* modeVidange;
		TypeBool		* demandeMarche;
		TypeAna			* animationIcone;
		TypeBool		* Campagne; //Ajout 19/03/2020
	} m_varEnsacheuse;

public:
	Ensacheuse();
	Ensacheuse(sNameEnsacheuse names, int start);
	bool GestionProprete(int ligne);
	bool GestionConfig(int ligne);
	bool EnvoiDemVidange(int ligne, bool ValidTare); //Modif 12/12/19
	bool ResetDemandeProprete(int ligne);
	bool ResetDemandeVidange(int ligne);
	bool ResetDemandeConfiguration(int ligne);
	bool SetMotEtat();
	int GetQteEauRAdviseN();
	bool EnvoiDemMarche(int ligne);
	bool EnvoiDmeArret(int ligne);
	bool SetCampagne(int ligne, bool val); // Ajout 19/03/2020
};

class Formeuse : public Equipement {
	struct sMemberFormeuse {
		TypeString	* typeContenant;
		TypeString	* typeContenantPre;
		TypeBool	* demandeConfig;
		TypeBool	* timeoutConfig;
		TypeBool	* demandeVidange;
		TypeBool	* demandeProprete;
		TypeAna		* circuitProprete;
		TypeAna		* paramProprete;
		TypeBool	* demandeMarche;
		TypeBool	* magasinCartonNivBas;
		TypeBool	* magasinAdhesifNivBas;
		TypeAna		* animationIcone;
		TypeBool	* CampagneL1; //Ajout 27/03/2020
		TypeBool	* CampagneL2; //Ajout 27/03/2020
	} m_varFormeuse;

public:
	Formeuse();
	Formeuse(sNameFormeuse names, int start);
	bool GestionProprete(int ligne);
	bool GestionConfig(int ligne);
	bool EnvoiDemVidange(int ligne, bool ValidTare); //Modif 12/12/19
	bool ResetDemandeProprete(int ligne);
	bool ResetDemandeVidange(int ligne);
	bool ResetDemandeConfiguration(int ligne);
	bool IsSamePackaging(CString cont);
	bool IsDispo();
	bool SetMotEtat();
	bool EnvoiDemMarche(int ligne);
	bool EnvoiDmeArret(int ligne);
	bool SetCampagne(int ligne, bool val); // Ajout 19/03/2020
};

class Peseuse : public Equipement {
	struct sMemberPeseuse {
		TypeBool		* configure;
		TypeAna			* circuitProprete;
		TypeAna			* paramProprete;
		TypeBool		* demandeProprete;
		TypeBool		* etatProprete;
		TypeBool		* demandeConfig;
		TypeString		* cheminFichier;
		TypeBool		* timeoutConfig;
		TypeBool		* demandeVidange;
		TypeBool		* modeVidange;
		TypeBool		* demandeMarche;
		TypeAna			* animationIcone;
		TypeBool		* coherenceSetup; //Ajout 26/12/19
		TypeBool		* Campagne; //Ajout 19/03/2020		
	} m_varPeseuse;
	int NBRCreationFichier; //Ajout 20/12/19

public:
	Peseuse();
	Peseuse(sNamePeseuse names, int start);
	bool GestionProprete(int ligne);
	bool GestionConfig(int ligne);
	bool EnvoiPoids(double poids, int ligne);
	bool EnvoiDemVidange(int ligne, bool ValidTare); //Modif 12/12/19
	bool ResetDemandeProprete(int ligne);
	bool ResetDemandeVidange(int ligne);
	bool ResetDemandeConfiguration(int ligne);
	bool SetMotEtat();
	bool EnvoiDemMarche(int ligne);
	bool EnvoiDmeArret(int ligne);
	bool ResetCoherenceSetup(); //Ajout 26/12/19
	bool SetCampagne(int ligne, bool val); // Ajout 19/03/2020

};

class Transitique : public Equipement {
	struct sMemberTrans {
		TypeBool		* configureL1;
		TypeBool		* configureL2;
		TypeString		* typeContenantL1;
		TypeString		* typeContenantL2;
		TypeAna			* circuitPropreteL1;
		TypeAna			* circuitPropreteL2;
		TypeAna			* paramPropreteL1;
		TypeAna			* paramPropreteL2;
		TypeBool		* demandePropreteL1;
		TypeBool		* demandePropreteL2;
		TypeBool		* etatPropreteL1;
		TypeBool		* etatPropreteL2;
		TypeBool		* demandeConfigL1;
		TypeBool		* demandeConfigL2;
		TypeBool		* timeoutConfigL1;
		TypeBool		* timeoutConfigL2;
		TypeBool		* demandeVidangeL1;
		TypeBool		* modeVidangeL1;
		TypeBool		* demandeVidangeL2;
		TypeBool		* modeVidangeL2;
		TypeAna			* etatAnimationL1;
		TypeAna			* etatAnimationL2;
		TypeAna			* etatAPIL1;
		TypeAna			* etatAPIL2;
		TypeAna			* ligneF1;
		TypeAna			* ligneF2;
		TypeAna			* numOFL1;
		TypeAna			* numOFL2;
		TypeBool		* demandeMarcheL1;
		TypeBool		* demandeMarcheL2;
		//Ajout 30/10/2019
		TypeAna			* vracSacL1;
		TypeAna			* vracSacL2;
		TypeAna			* qteParColisL1;
		TypeAna			* qteParColisL2;
		TypeString		* designationL1;
		TypeString		* designationL2;
		TypeBool		* demandeMarche;
		//Ajout 31/10/2019
		TypeString		* numLotL1;
		TypeString		* numLotL2;
		// Pas besoin de mettre le 0 devant
		TypeAna			* codeArticleL1;
		TypeAna			* codeArticleL2;
		//Ajout 03/12/2019
		TypeAna			* Provenance_Piece_L1;
		//Ajout 06/02/2020
		TypeBool			* derogationIntroductionManuelL1;
		TypeBool			* derogationIntroductionManuelL2;
		TypeBool		* CampagneL1; //Ajout 19/03/2020
		TypeBool		* CampagneL2; //Ajout 19/03/2020
	} m_varTrans;

public:
	Transitique();
	Transitique(sNameTrans names, int start);
	bool GestionProprete(int ligne);
	bool GestionConfig(int ligne);
	bool EnvoiDemVidange(int ligne, bool ValidTare); //Modif 12/12/19
	bool ResetDemandeProprete(int ligne);
	bool ResetDemandeVidange(int ligne);
	bool ResetDemandeConfiguration(int ligne);
	bool SetLigneFormeuse(int formeuse, int ligne);
	bool SetMotEtat();
	bool EnvoiDemMarche(int ligne);
	bool EnvoiDmeArret(int ligne);

	int GetAffFormAPI(int formeuse);
	bool UpdateAffFormAPI(int ligne, bool valF1, bool valF2, bool gefbox);
	bool EnvoiMarcheCellule();

	void SetProvenance_L1(bool val); //Ajout 03/12/2019

	//Ajout 06/02/2020
	bool GetDerogationIntroManu(int ligne);
	void SetDerogationIntroManu(int ligne, bool val);
	int GetLigneFormeuse(int ligne);
	bool SetCampagne(int ligne, bool val); // Ajout 19/03/2020
};

class Balance : public Equipement {
	struct sMemberBal {
		TypeBool	* validationTarreL1;
		TypeBool	* validationTarreL2;
		TypeAna		* poidsLuTarreL1;
		TypeAna		* poidsLuTarreL2;
		TypeAna		* poidsLuRompu;
		TypeBool	* bacRompuValidL1;
		TypeBool	* bacRompuValidL2;
		TypeBool	* voyantBalanceTarreL1;
		TypeBool	* voyantBalanceTarreL2;
		TypeBool	* derogationTarreL1; //Ajout 26/12/19
		TypeBool	* derogationTarreL2; //Ajout 26/12/19
		TypeBool	* validationColisL1; //Ajout 26/12/19
		TypeBool	* validationColisL2; //Ajout 26/12/19
		TypeBool	* derogationColisL1; //Ajout 26/12/19
		TypeBool	* derogationColisL2; //Ajout 26/12/19
		TypeBool	* DLLVoyantTarreL1; //Ajout 24/03/2020
		TypeBool	* DLLVoyantTarreL2; //Ajout 24/03/2020
	} m_varBal;
public:
	Balance();
	Balance(sNameBal names, int start);
	double GetPoidsTarre(int ligne);
	bool ResetPoidsTarre(int ligne);
	bool GetValidTarre(int ligne);
	void ResetValidTarre(int ligne);
	bool GetFinRompus(int ligne);
	void ResetFinRompus(int ligne);
	bool ResetDemandeProprete(int ligne);
	bool ResetDemandeVidange(int ligne);
	bool ResetDemandeConfiguration(int ligne);
	bool SetMotEtat();
	bool EnvoiDemMarche(int ligne);
	bool EnvoiDmeArret(int ligne);
	bool SetVoyantBalanceTarre(int ligne, bool val);
	bool ResetDerogationTarre(int ligne); //Ajout 26/12/19
	bool ResetValidColis(int ligne); //Ajout 26/12/19
	bool ResetDerogationColis(int ligne); //Ajout 26/12/19
	bool GetDLLVoyantTarre(int ligne); //Ajout 24/03/2020
	bool SetDLLVoyantTarre(int ligne, bool val); //Ajout 24/03/2020
};

class RobotPalettisation : public Equipement {
	struct sMemberPal {
		TypeBool		* configureL1;
		TypeBool		* configureL2;
		TypeAna			* circuitPropreteL1;
		TypeAna			* circuitPropreteL2;
		TypeAna			* paramPropreteL1;
		TypeAna			* paramPropreteL2;
		TypeBool		* demandePropreteL1;
		TypeBool		* demandePropreteL2;
		TypeBool		* etatPropreteL1;
		TypeBool		* etatPropreteL2;
		TypeBool		* demandeVidangeL1;
		TypeBool		* modeVidangeL1;
		TypeBool		* demandeVidangeL2;
		TypeBool		* modeVidangeL2;
		TypeAna			* etatAnimationL1;
		TypeAna			* etatAnimationL2;
		TypeAna			* etatAPIL1;
		TypeAna			* etatAPIL2;
		TypeAna			* etatAnimationT1;
		TypeAna			* etatAnimationT2;
		TypeAna			* etatAPIT1;
		TypeAna			* etatAPIT2;
		TypeAna			* circuitPropreteT1;
		TypeAna			* circuitPropreteT2;
		TypeAna			* paramPropreteT1;
		TypeAna			* paramPropreteT2;
		TypeBool		* demandePropreteT1;
		TypeBool		* demandePropreteT2;
		TypeBool		* etatPropreteT1;
		TypeBool		* etatPropreteT2;
		TypeBool		* demandeVidangeT1;
		TypeBool		* modeVidangeT1;
		TypeBool		* demandeVidangeT2;
		TypeBool		* modeVidangeT2;
		TypeBool		* demandeConfigL1;
		TypeBool		* demandeConfigL2;
		TypeAna			* nbPPL1;
		TypeAna			* nbPPL2;
		TypeBool		* demandeMarcheL1;
		TypeBool		* demandeMarcheL2;
		TypeBool		* magPalettesVidesNivBas;
		TypeBool		* demandeMarche;
		TypeBool		* CampagneL1; //Ajout 19/03/2020
		TypeBool		* CampagneL2; //Ajout 19/03/2020
	} m_varPal;
public:
	RobotPalettisation();
	RobotPalettisation(sNamePal names, int start);
	bool GestionProprete(int ligne);
	bool GestionConfig(int ligne);
	bool EnvoiDemVidange(int ligne, bool ValidTare); //Modif 12/12/19
	bool ResetDemandeProprete(int ligne);
	bool ResetDemandeVidange(int ligne);
	bool ResetDemandeConfiguration(int ligne);
	bool SetMotEtat();
	int GetAdvNPP(int ligne);
	bool EnvoiDemMarche(int ligne);
	bool EnvoiDmeArret(int ligne);
	bool EnvoiMarcheCellule();
	double GetNbrPiecePalettisee(int ligne); // Ajout 18/03/2020
	bool SetCampagne(int ligne, bool val); // Ajout 19/03/2020
};

class Etiqueteuse : public Equipement {
	struct sMemberEtq {
		TypeBool		* configureL1;
		TypeBool		* configureL2;
		TypeBool		* rouleauEtiquetteNivBas;
		TypeBool		* demandeConfigL1;
		TypeBool		* demandeConfigL2;
		TypeBool		* timeoutConfigL1;
		TypeBool		* timeoutConfigL2;
		TypeBool		* demandePropreteL1;
		TypeBool		* demandePropreteL2;
		TypeAna			* etatAPIL1;
		TypeAna			* etatAPIL2;
		TypeAna			* etatAnimationL1;
		TypeAna			* etatAnimationL2;
		TypeBool		* demandeVidangeL1;
		TypeBool		* demandeVidangeL2;
		TypeBool		* demandeMarcheL1;
		TypeBool		* demandeMarcheL2;
		TypeBool		* demandeMarche;
		TypeBool		* CampagneL1; //Ajout 19/03/2020
		TypeBool		* CampagneL2; //Ajout 19/03/2020
	} m_varEtq;
public:
	Etiqueteuse();
	Etiqueteuse(sNameEtq names, int start);
	bool GestionProprete(int ligne);
	bool GestionConfig(int ligne);
	bool ResetDemandeProprete(int ligne);
	bool ResetDemandeVidange(int ligne);
	bool ResetDemandeConfiguration(int ligne);
	bool EnvoiDemVidange(int ligne, bool ValidTare); //Modif 12/12/19
	bool SetMotEtat();
	bool EnvoiDemMarche(int ligne);
	bool EnvoiDmeArret(int ligne);
	bool EnvoiMarcheCellule();
	bool SetCampagne(int ligne, bool val); // Ajout 19/03/2020
};

class Imprimante : public Equipement {
	struct sMemberImp {
		TypeBool		* configureL1;
		TypeBool		* configureL2;
		TypeBool		* encreNivBas;
		TypeBool		* demandeConfigL1;
		TypeBool		* demandeConfigL2;
		TypeBool		* timeoutConfigL1;
		TypeBool		* timeoutConfigL2;
		TypeBool		* demandePropreteL1;
		TypeBool		* demandePropreteL2;
		TypeAna			* etatAPIL1;
		TypeAna			* etatAPIL2;
		TypeAna			* etatAnimationL1;
		TypeAna			* etatAnimationL2;
		TypeBool		* demandeVidangeL1;
		TypeBool		* demandeVidangeL2;
		TypeBool		* demandeMarcheL1;
		TypeBool		* demandeMarcheL2;
		TypeBool		* demandeMarche;
		TypeBool		* CampagneL1; //Ajout 19/03/2020
		TypeBool		* CampagneL2; //Ajout 19/03/2020
	} m_varImp;
public:
	Imprimante();
	Imprimante(sNameImp names, int start);
	bool GestionProprete(int ligne);
	bool GestionConfig(int ligne);
	bool ResetDemandeProprete(int ligne);
	bool ResetDemandeVidange(int ligne);
	bool ResetDemandeConfiguration(int ligne);
	bool EnvoiDemVidange(int ligne, bool ValidTare); //Modif 12/12/19
	bool SetMotEtat();
	bool EnvoiDemMarche(int ligne);
	bool EnvoiDmeArret(int ligne);
	bool EnvoiMarcheCellule();
	bool SetCampagne(int ligne, bool val); // Ajout 19/03/2020
};

class Fermeuse : public Equipement {
	struct sMemberFerm {
		TypeBool		* configureL1;
		TypeBool		* configureL2;
		TypeBool		* rouleauAdhesifNivBas;
		TypeBool		* demandePropreteL1;
		TypeBool		* demandePropreteL2;
		TypeBool		* demandeConfigL1;
		TypeBool		* demandeConfigL2;
		TypeAna			* etatAPIL1;
		TypeAna			* etatAPIL2;
		TypeAna			* etatAnimationL1;
		TypeAna			* etatAnimationL2;
		TypeBool		* demandeVidangeL1;
		TypeBool		* demandeVidangeL2;
		TypeBool		* demandeMarcheL1;
		TypeBool		* demandeMarcheL2;
		TypeBool		* demandeMarche;
		TypeBool		* CampagneL1; //Ajout 19/03/2020
		TypeBool		* CampagneL2; //Ajout 19/03/2020
	} m_varFerm;
public:
	Fermeuse();
	Fermeuse(sNameFerm names, int start);
	bool GestionProprete(int ligne);
	bool GestionConfig(int ligne);
	bool ResetDemandeProprete(int ligne);
	bool ResetDemandeVidange(int ligne);
	bool ResetDemandeConfiguration(int ligne);
	bool EnvoiDemVidange(int ligne, bool ValidTare); //Modif 12/12/19
	bool SetMotEtat();
	bool EnvoiDemMarche(int ligne);
	bool EnvoiDmeArret(int ligne);
	bool EnvoiMarcheCellule();
	bool SetCampagne(int ligne, bool val); // Ajout 19/03/2020
};

class MES{
	//Liste des varaibles communes
	double m_rayproCounterMem;

	struct sMemberMES {
		sMemberSetup setupL1;
		sMemberSetup setupL2;
		sMemberSync syncL1;
		sMemberSync syncL2;
		TypeAna		* rayproCounterL1;
		TypeAna		* rayproCounterL2;
		TypeAna		* plcCounterL1;
		TypeAna		* plcCounterL2;
		TypeBool	* commEnsL1;
		TypeBool	* commEnsL2;
		TypeBool	* commEtq;
		TypeBool	* commFerm;
		TypeBool	* commForm1;
		TypeBool	* commForm2;
		TypeBool	* commImp;
		TypeBool	* commPesL1;
		TypeBool	* commPesL2;
		TypeBool	* commRbb;
		TypeBool	* commRe;
		TypeBool	* commRp;
		TypeBool	* commTr;
	} m_varMES;



public:
	MES();
	MES(sNameMES names, int start);
	double GetRayProCounter(int ligne);
	void IncrementBitDeViePLC(int ligne);
	bool GetValidCampagne(int ligne);
	bool SetValidFinCampagne(int ligne);
	bool ResetValidFinCampagne(int ligne);
	double GetMesSyncSetup(int ligne);
	double GetspvSyncSetup(int ligne);
	double GetMesSyncStoppage(int ligne);
	bool SetSyncTime(int ligne);
	bool SetValidCampagne(int ligne);
	double GetspvSyncStoppage(int ligne);
	bool ResetValidCampagne(int ligne);
	bool SetPlcBatchReqStatus(int ligne, double val);
	double SetspvSyncSetup(int ligne, double val);
	double SetspvSyncStoppage(int ligne, double val);
	sFlatSetup GetMesSetup(int ligne);
	bool SetStoppageDatas(int id, double num, bool val);
	double GetMesSyncHU(int ligne);
	double GetspvSyncHU(int ligne);
	double SetspvSyncHU(int ligne, double val);
	bool SetHUDatas(int ligne, huInput hu);
	bool SetGoodPartQty(int ligne, double nbCont);
	bool setProvenance(int ligne, bool val);
	bool GetProvenance(int ligne);
	bool GetMesBatchReqStatus(int ligne); // Ajout 10/02/2020



	~MES();
};

class SPV {
	bool m_g7_formeuse1;
	bool m_g7_formeuse2;

	struct sMemberSPV {
		TypeBool		* modeDegradeL1;
		TypeBool		* modeDegradeL2;
		TypeBool		* validSetupL1;
		TypeBool		* validSetupL2;
		TypeBool		* confirmLancProdL1;
		TypeBool		* confirmLancProdL2;
		TypeBool		* plcDemFinProdL1;
		TypeBool		* plcDemFinProdL2;
		TypeBool		* plcConfirmFinProdL1;
		TypeBool		* plcConfirmFinProdL2;
		TypeBool		* annulerProdL1;
		TypeBool		* annulerProdL2;
		TypeAna			* cptBacVersesL1;
		TypeAna			* cptBacVersesL2;
		TypeBool		* fatBtnManu;
		//Affectation formeuses
		TypeAna			* affF1;
		TypeAna			* affF2;
		TypeBool		* validAffForm1;
		TypeBool		* validAffForm2;
		TypeBool		* passageDegradeL1;
		TypeBool		* passageDegradeL2;
		TypeBool		* annulerCampReglageL1;
		TypeBool		* annulerCampReglageL2;
		TypeBool		* campagneDegradeL1;
		TypeBool		* campagneDegradeL2;
		//Popup provenance OF
		TypeBool		* bitPopupProvenanceL1;
		TypeBool		* bitPopupProvenanceL2;
		TypeBool		* choixProvenanceL1;
		TypeBool		* choixProvenanceL2;
		//Ajout variables pour HU Humidification
		TypeAna			* HUcontIDL1;
		TypeString		* HUbatchIDL1;
		TypeString		* HUIDL1;
		TypeAna			* HUqtyL1;
		TypeBool		* validHUHumidification;
		// Ajout passage en mode distant
		TypeBool		* passageDistantL1;
		TypeBool		* passageDistantL2;
		sMemberSetup setupL1;
		sMemberSetup setupL2;
		//Popup Choix Formeuse - Ajout 03/02/20
		TypeBool		* bitPopupChoixFormeuseL1;
		TypeBool		* bitPopupChoixFormeuseL2;
		TypeAna			* ChoixFormeuseL1;
		TypeAna			* ChoixFormeuseL2;

		TypeBool		* provenancePieceL1; //Ajout 11/02/2020
		TypeBool		* provenancePieceL2; //Ajout 11/02/2020
		TypeBool		* rompuEnCours; //Ajout 11/02/2020

		//Prise en charge du nouveau setup
		TypeBool		* spvNouveauSetupL1; // Ajout 17/03/2020
		TypeBool		* spvNouveauSetupL2; // Ajout 17/03/2020

		TypeString		* etatCampagneL1; // Ajout 18/03/2020
		TypeString		* etatCampagneL2; // Ajout 18/03/2020
		TypeBool		* bitPopupNbrPieceValideL1; // Ajout 18/03/2020
		TypeBool		* bitPopupNbrPieceValideL2; // Ajout 18/03/2020
		TypeBool		* nbrPieceValideL1; // Ajout 18/03/2020
		TypeBool		* nbrPieceValideL2; // Ajout 18/03/2020


		TypeBool		* actualisationRefConditionnement; // Ajout 19/03/2020

		//Besoin des formeuse sur une ligne - Ajout 07/02/2020
		bool rompuEnCoursL1 = false; //Ajout 11/02/2020
		bool rompuEnCoursL2 = false; //Ajout 11/02/2020

	} m_varSPV;
	
public:
	SPV();
	SPV(sNameSPV names, int start);
	sFlatSetup GetSetupLocal(int ligne);
	bool ResetSetupLocal(int ligne);
	bool GetValidSetupLocal(int ligne);
	bool ResetValidSetupLocal(int ligne);
	bool GetEtatDegrade(int ligne);
	bool GetValidCampagne(int ligne);
	bool ResetValidCampagne(int ligne);
	bool GetDmeFinCampagne(int ligne);
	bool ResetDmeFinCampagne(int ligne);
	void SetEtatDegrade(int ligne, bool val);
	bool GetValidFinCampagne(int ligne);
	bool ResetValidFinCampagne(int ligne);
	bool GetAnnulerCampagne(int ligne);
	bool ResetAnnulerCampagne(int ligne);
	double CalculMoyenneBac(int ligne);
	double UpdateCalculMoyenneBac();
	bool GetFatBtnManu();
	bool SetFatBtnManu(bool val);

	void ResetValidAffForm(int formeuse);

	//bool GetDmeDegrade(int ligne);

	// Monte le bit interne changement de formeuse à 1 pour par la
	//suite lancer la conf
	bool ChangeAffFormeuse(int formeuse);
	// permet de récupérer ce bit
	bool Getm_g7_formeuse(int formeuse);
	// réaffectation des formeuses coté transitique et
	// RAZ bit demande spv
	bool Setm_g7_formeuse(int formeuse);
	int GetValidAttrFormAdvNum(int formeuse);

	bool GetPassageDegrade(int ligne);
	bool ResetPassageDegrade(int ligne);
	void SetCampagneDegrade(int ligne, bool val);

	bool GetBitPopupProvenance(int ligne);
	bool SetBitPopupProvenance(int ligne, bool val);

	bool GetChoixProvenance(int ligne);
	bool SetChoixProvenance(int ligne, bool val);

	void SetSetup(int ligne, sFlatSetup setupDistant);

	// On peut repasser en mode distant uniquement
	//si on est en attente de Setup. Au cas ou l'opérateur
	//se trompe en déclenchant le mode dégradé
	bool GetPassageDistant(int ligne);
	bool ResetPassageDistant(int ligne);

	bool GetBitPopupChoixFormeuse(int ligne); //Ajout 03/02/20
	bool SetBitPopupChoixFormeuse(int ligne, bool val); //Ajout 03/02/20

	double GetChoixFormeuse(int ligne); //Ajout 07/02/20
	bool SetChoixFormeuse(int ligne, int val); //Ajout 07/02/20

	bool SetProvenancePiece(int ligne, int val); //Ajout 11/02/2020
	bool SetRompuEnCoursResumer(bool val); //Ajout 11/02/2020
	bool SetRompuEnCours(int ligne, bool val); //Ajout 11/02/2020
	bool GetRompuEnCours(int ligne); //Ajout 11/02/2020

	//Ajout HU Humidification
	double	GetHUcontID();
	CString GetbatchID();
	CString GetHUID();
	double	GetHUqty();
	int		GetValidHUHumidAdvNum();
	void	ResetValidHUHumid();

	void setProvenance(int ligne, bool val);

	bool GetspvNouveauSetup(int ligne); //Ajout 17/03/2020
	bool SetspvNouveauSetup(int ligne, bool val); //Ajout 17/03/2020
	bool SetEtatCampagne(int ligne, CString val); //Ajout 18/03/2020
	bool GetBitNbrPieceValide(int ligne); //Ajout 18/03/2020
	bool SetBitNbrPieceValide(int ligne, bool val); //Ajout 18/03/2020
	bool GetNbrPieceValide(int ligne); //Ajout 18/03/2020
	bool SetNbrPieceValide(int ligne, bool val); //Ajout 18/03/2020
	bool GetActualisationRefConditionnement(); //Ajout 19/03/2020
	bool SetActualisationRefConditionnement(bool val); //Ajout 19/03/2020

};

class Tremie : public Equipement {
	struct sMemberTremie {
		TypeBool	* timeoutMarcheAPI;
		TypeBool	* demandeMarche;
	} m_varTremie;

public:
	Tremie();
	Tremie(sNameTremie names, int start);
	bool EnvoiDemMarche(int ligne);
	bool EnvoiDmeArret(int ligne);
};

double GetMotEtat(bool alarmeBloquante,
	bool alarmeNonBloquante,
	bool demandeProprete,
	bool demandeConfig,
	bool demandeVidange,
	bool machineSortie,
	bool timeoutConfig,
	bool timeoutCom,
	bool timeoutMarche,
	double etat);