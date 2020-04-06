#pragma once
#include "stdafx.h"
#include <vector>

//Forward declaration des classes
class RobotCaisses;
class RobotCouvercle;
class Ensacheuse;
class Peseuse;
class Transitique;
class Balance;
class RobotPalettisation;
class Etiqueteuse;
class Imprimante;
class Fermeuse;
class Formeuse;
class Equipement;
class MES;
class SPV;

class TypeBool;
class TypeAna;
class TypeString;
class Sequence;



//Structure de machines

struct sNameEquipements {
	RobotCaisses		* APIrobotCaisses ;
	RobotCouvercle		* APIrobotCouvercle;
	Ensacheuse			* APIensacheuseL1;
	Ensacheuse			* APIensacheuseL2;
	Peseuse				* APIpeseuseL1;
	Peseuse				* APIpeseuseL2;
	Transitique			* APItransitique;
	Balance				* APIbalance;
	RobotPalettisation 	* APIpalettiseur;
	Etiqueteuse 		* APIetiqueteuse;
	Imprimante 			* APIimprimante;
	Fermeuse 			* APIfermeuse;
	Formeuse 			* APIformeuse1;
	Formeuse 			* APIformeuse2;
	MES 				* mesRayPro;
	SPV					* spvWAT;
	Equipement			* Tremie;
	Sequence			* g7;

};
extern sNameEquipements sVarEqp;

//Structure d'un Setup
struct sMemberSetup {
	TypeAna		* refPieces;
	TypeString	* descProduit;
	TypeAna		* idOperateur;
	TypeString	* numLot;
	TypeAna		* numOF;
	TypeAna		* nbPieces;
	TypeAna		* nbPiecesPPalette;
	TypeAna		* nbPiecesPContenant;
	TypeString	* typeConditionnement;
	TypeAna		* nbPiecesPSachet;
	TypeAna		* hauteurSachet;
	TypeAna		* poidsPiece;
	TypeAna		* adjonctionEau;
	TypeBool	* provenance;
	//UnitSizeHandling
	TypeString	* handlingUnit0;
	TypeString	* handlingUnit1;
	TypeString	* handlingUnit2;
	TypeAna		* handlingUnitSize0;
	TypeAna		* handlingUnitSize1;
	TypeAna		* handlingUnitSize2;
};

//Structure d'un Setup Plat
//Structure d'un Setup
struct sFlatSetup {
	double		refPieces;
	CString		descProduit;
	double		idOperateur;
	CString		numLot;
	double		numOF;
	double		nbPieces;
	double		nbPiecesPPalette;
	double		nbPiecesPContenant;
	CString		typeConditionnement;
	double		nbPiecesPSachet;
	double		hauteurSachet;
	double		poidsPiece;
	double		adjonctionEau;
	bool		provenance;
};

//Setup en cours
extern sFlatSetup setupEnCoursL1;
extern sFlatSetup setupEnCoursL2;
//Mémoire du Setup précédent
extern sFlatSetup memSetupL1;
extern sFlatSetup memSetupL2;

extern std::vector<sFlatSetup> vFlatSetup;

//Structure de synchro MES
struct sMemberSync {
	TypeBool	* spvAttLot;
	TypeBool	* spvAttConfirmLancProd;
	TypeBool	* mesConfirmLancProd;
	TypeBool	* mesDemFinProd;
	TypeAna		* plcSyncSetup;
	TypeAna		* mesSyncSetup;
	TypeBool	* mesStartReq;
	TypeBool	* spvStartReq;
	TypeAna		* mestoppageSync;
	TypeAna		* spvtoppageSync;
	TypeAna		* tsStoppageHH;
	TypeAna		* tsStoppageMM;
	TypeAna		* tsStoppageSS;
	TypeAna		* idStoppageCause;
	TypeAna		* qteEauREnsach;
	TypeBool	* infPalettePleine;
	TypeAna		* plcBatchReqStatus;
	TypeBool	* mesBatchReq;
	TypeAna		* mesSyncTime;
	TypeAna		* plcSyncTime;
	TypeBool	* plcConfirmFinProd;
	TypeAna		* numStoppageCause;
	TypeAna		* numStoppageStation;
	//HU Input
	TypeAna		* mesSyncHU;
	TypeAna		* spvSyncHU;
	TypeAna		* huHH;
	TypeAna		* huMM;
	TypeAna		* huSS;
	TypeAna		* huContID;
	TypeString		* huBatchID;
	TypeString		* huID;
	TypeAna		* huQte;
	TypeAna		* batchGoodPartQty;
};


//Structor pour variables Equipement tronc commun
struct sNameCommun {
	CString		demandeDistant;
	CString		modeDistant;
	CString		alarmeNonBloquante;
	CString		alarmeBloquante;
	CString		numAlarme;
	CString		etatAnimation;
	CString		etatAPI;
	CString		mdvSPV;
	CString		mdvAPI;
	CString		timeoutComAPI;
	CString		timeoutMarcheAPI;
};


struct sNameRobotCaisses {
	CString		demandeDistant;
	CString		modeDistant;
	CString		alarmeNonBloquante;
	CString		alarmeBloquante;
	CString		numAlarme;
	CString		etatAnimation;
	CString		etatAPI;

	CString		configureL1;
	CString		configureL2;
	CString		circuitPropreteL1;
	CString		circuitPropreteL2;
	CString		paramPropreteL1;
	CString		paramPropreteL2;
	CString		codeArticleL1;
	CString		codeArticleL2;
	CString		RnumeroBacL1;
	CString		RnumeroBacL2;
	CString		RcodeArticleL1;
	CString		RcodeArticleL2;
	CString		RqtePiecesL1;
	CString		RqtePiecesL2;
	CString		demandePropreteL1;
	CString		demandePropreteL2;
	CString		etatPropreteL1;
	CString		etatPropreteL2;
	CString		demandeConfigL1;
	CString		demandeConfigL2;
	CString		timeoutConfigL1;
	CString		timeoutConfigL2;
	CString		demandeVidangeL1;
	CString		modeVidangeL1;
	CString		demandeVidangeL2;
	CString		modeVidangeL2;
	CString		etatAnimationL1;
	CString		etatAnimationL2;
	CString		etatAPIL1;
	CString		etatAPIL2;
	CString		mdvSPV;
	CString		mdvAPI;
	CString		timeoutComAPI;
	CString		timeoutMarcheAPI;
	CString		topBacVerseL1;
	CString		ackBacVerseL1;
	CString		topBacVerseL2;
	CString		ackBacVerseL2;
	CString		HUcontIDL1;
	CString		HUbatchIDL1;
	CString		HUIDL1;
	CString		HUqtyL1;
	CString		HUcontIDL2;
	CString		HUbatchIDL2;
	CString		HUIDL2;
	CString		HUqtyL2;
	CString		demandeMarcheL1;
	CString		demandeMarcheL2;
	CString		demandeMarche;
	CString		CampagneL1; //Ajout 19/03/2020
	CString		CampagneL2; //Ajout 19/03/2020
	CString		DemandeArticleL1; //Ajout 24/03/2020
	CString		DemandeArticleL2; //Ajout 24/03/2020
};

extern sNameRobotCaisses sVarEqpRcb;

struct sNameRobotCouvercle {
	CString		demandeDistant;
	CString		modeDistant;
	CString		alarmeNonBloquante;
	CString		alarmeBloquante;
	CString		numAlarme;
	CString		etatAnimation;
	CString		etatAPI;

	CString		configureL1;
	CString		configureL2;
	CString		circuitPropreteL1;
	CString		circuitPropreteL2;
	CString		paramPropreteL1;
	CString		paramPropreteL2;
	CString		magasinCouvercleNivBas;
	CString		demandePropreteL1;
	CString		demandePropreteL2;
	CString		etatPropreteL1;
	CString		etatPropreteL2;
	CString		demandeVidangeL1;
	CString		modeVidangeL1;
	CString		demandeVidangeL2;
	CString		modeVidangeL2;
	CString		etatAnimationL1;
	CString		etatAnimationL2;
	CString		etatAPIL1;
	CString		etatAPIL2;
	CString		mdvSPV;
	CString		mdvAPI;
	CString		timeoutComAPI;
	CString		timeoutMarcheAPI;
	CString		demandeConfigL1;
	CString		demandeConfigL2;
	CString		demandeMarcheL1;
	CString		demandeMarcheL2;
	CString		magEtiquetteNivBasL1;
	CString		magEtiquetteNivBasL2;
	CString		demandeMarche;
	CString		CampagneL1; //Ajout 19/03/2020
	CString		CampagneL2; //Ajout 19/03/2020
};

extern sNameRobotCouvercle sVarEqpRc;

struct sNameEnsacheuse {
	CString		demandeMarche;
	CString		demandeDistant;
	CString		modeDistant;
	CString		alarmeNonBloquante;
	CString		alarmeBloquante;
	CString		numAlarme;
	CString		etatAnimation;
	CString		etatAPI;

	CString		configure;
	CString		finRouleauFilm;
	CString		hauteurSachet;
	CString		adjonctionEau;
	CString		qteEauReelle;
	CString		circuitProprete;
	CString		paramProprete;
	CString		demandeProprete;
	CString		etatProprete;
	CString		demandeConfig;
	CString		timeoutConfig;
	CString		demandeVidange;
	CString		modeVidange;
	CString		mdvSPV;
	CString		mdvAPI;
	CString		timeoutComAPI;
	CString		timeoutMarcheAPI;
	CString		animationIcone;
	CString		Campagne; //Ajout 19/03/2020

};

extern sNameEnsacheuse sVarEnsL1;
extern sNameEnsacheuse sVarEnsL2;


struct sNamePeseuse {
	CString		demandeMarche;
	CString		demandeDistant;
	CString		modeDistant;
	CString		alarmeNonBloquante;
	CString		alarmeBloquante;
	CString		numAlarme;
	CString		etatAnimation;
	CString		etatAPI;

	CString		configure;
	CString		circuitProprete;
	CString		paramProprete;
	CString		demandeProprete;
	CString		etatProprete;
	CString		demandeConfig;
	CString		cheminFichier;
	CString		timeoutConfig;
	CString		demandeVidange;
	CString		modeVidange;
	CString		mdvSPV;
	CString		mdvAPI;
	CString		timeoutComAPI;
	CString		timeoutMarcheAPI;
	CString		animationIcone;
	CString		coherenceSetup;
	CString		Campagne; //Ajout 19/03/2020

};

extern sNamePeseuse sVarPesL1;
extern sNamePeseuse sVarPesL2;

struct sNameTrans {
	CString		demandeDistant;
	CString		modeDistant;
	CString		alarmeNonBloquante;
	CString		alarmeBloquante;
	CString		numAlarme;
	CString		etatAnimation;
	CString		etatAPI;

	CString		configureL1;
	CString		configureL2;
	CString		typeContenantL1;
	CString		typeContenantL2;
	CString		circuitPropreteL1;
	CString		circuitPropreteL2;
	CString		paramPropreteL1;
	CString		paramPropreteL2;
	CString		demandePropreteL1;
	CString		demandePropreteL2;
	CString		etatPropreteL1;
	CString		etatPropreteL2;
	CString		demandeConfigL1;
	CString		demandeConfigL2;
	CString		timeoutConfigL1;
	CString		timeoutConfigL2;
	CString		demandeVidangeL1;
	CString		modeVidangeL1;
	CString		demandeVidangeL2;
	CString		modeVidangeL2;
	CString		etatAnimationL1;
	CString		etatAnimationL2;
	CString		etatAPIL1;
	CString		etatAPIL2;
	CString		mdvSPV;
	CString		mdvAPI;
	CString		timeoutComAPI;
	CString		timeoutMarcheAPI;
	CString		ligneF1;
	CString		ligneF2;
	CString		numOFL1;
	CString		numOFL2;
	CString		demandeMarcheL1;
	CString		demandeMarcheL2;
	CString		vracSacL1;
	CString		vracSacL2;
	CString		qteParColisL1;
	CString		qteParColisL2;
	CString		designationL1;
	CString		designationL2;
	CString		demandeMarche;
	CString		numLotL1;
	CString		numLotL2;
	CString		codeArticleL1;
	CString		codeArticleL2;
	CString		Provenance_Piece_L1;
	CString		derogationIntroductionManuelL1; //Ajout 06/02/2020
	CString		derogationIntroductionManuelL2; //Ajout 06/02/2020
	CString		CampagneL1; //Ajout 19/03/2020
	CString		CampagneL2; //Ajout 19/03/2020
};

extern sNameTrans sVarTrans;

struct sNameBal {
	CString		demandeDistant;
	CString		modeDistant;
	CString		alarmeNonBloquante;
	CString		alarmeBloquante;
	CString		numAlarme;
	CString		etatAnimation;
	CString		etatAPI;

	CString		validationTarreL1;
	CString		validationTarreL2;
	CString		poidsLuTarreL1;
	CString		poidsLuTarreL2;
	CString		poidsLuRompu;
	CString		bacRompuValidL1;
	CString		bacRompuValidL2;
	CString		voyantBalanceTarreL1;
	CString		voyantBalanceTarreL2;
	CString		derogationTarreL1; //Ajout 26/12/19
	CString		derogationTarreL2; //Ajout 26/12/19
	CString		validationColisL1; //Ajout 26/12/19
	CString		validationColisL2; //Ajout 26/12/19
	CString		derogationColisL1; //Ajout 26/12/19
	CString		derogationColisL2; //Ajout 26/12/19
	CString		DLLVoyantTarreL1; //Ajout 24/03/2020
	CString		DLLVoyantTarreL2; //Ajout 24/03/2020

};

extern sNameBal sVarBal;

struct sNamePal {
	CString		demandeDistant;
	CString		modeDistant;
	CString		alarmeNonBloquante;
	CString		alarmeBloquante;
	CString		numAlarme;
	CString		etatAnimation;
	CString		etatAPI;

	CString		configureL1;
	CString		configureL2;
	CString		circuitPropreteL1;
	CString		circuitPropreteL2;
	CString		paramPropreteL1;
	CString		paramPropreteL2;
	CString		demandePropreteL1;
	CString		demandePropreteL2;
	CString		etatPropreteL1;
	CString		etatPropreteL2;
	CString		demandeVidangeL1;
	CString		modeVidangeL1;
	CString		demandeVidangeL2;
	CString		modeVidangeL2;
	CString		circuitPropreteT1;
	CString		circuitPropreteT2;
	CString		paramPropreteT1;
	CString		paramPropreteT2;
	CString		demandePropreteT1;
	CString		demandePropreteT2;
	CString		etatPropreteT1;
	CString		etatPropreteT2;
	CString		demandeVidangeT1;
	CString		modeVidangeT1;
	CString		demandeVidangeT2;
	CString		modeVidangeT2;
	CString		etatAnimationL1;
	CString		etatAnimationL2;
	CString		etatAPIL1;
	CString		etatAPIL2;
	CString		etatAnimationT1;
	CString		etatAnimationT2;
	CString		etatAPIT1;
	CString		etatAPIT2;
	CString		mdvSPV;
	CString		mdvAPI;
	CString		timeoutComAPI;
	CString		timeoutMarcheAPI;
	CString		demandeConfigL1;
	CString		demandeConfigL2;
	CString		nbPPL1;
	CString		nbPPL2;
	CString		demandeMarcheL1;
	CString		demandeMarcheL2;
	CString		magPalettesVidesNivBas;
	CString		demandeMarche;
	CString		CampagneL1; //Ajout 19/03/2020
	CString		CampagneL2; //Ajout 19/03/2020
};

extern sNamePal sVarPal;

struct sNameEtq {
	CString		demandeDistant;
	CString		modeDistant;
	CString		alarmeNonBloquante;
	CString		alarmeBloquante;
	CString		numAlarme;
	CString		etatAnimation;
	CString		etatAPI;

	CString		configureL1;
	CString		configureL2;
	CString		rouleauEtiquetteNivBas;
	CString		demandeConfigL1;
	CString		demandeConfigL2;
	CString		timeoutConfigL1;
	CString		timeoutConfigL2;
	CString		mdvSPV;
	CString		mdvAPI;
	CString		timeoutComAPI;
	CString		timeoutMarcheAPI;
	CString		demandePropreteL1;
	CString		demandePropreteL2;
	CString		etatAPIL1;
	CString		etatAPIL2;
	CString		etatAnimationL1;
	CString		etatAnimationL2;
	CString		demandeVidangeL1;
	CString		demandeVidangeL2;
	CString		demandeMarcheL1;
	CString		demandeMarcheL2;
	CString		demandeMarche;
	CString		CampagneL1; //Ajout 19/03/2020
	CString		CampagneL2; //Ajout 19/03/2020
};

extern sNameEtq sVarEtq;

//Formeuse
struct sNameFormeuse {
	CString		demandeMarche;
	CString		demandeDistant;
	CString		modeDistant;
	CString		alarmeNonBloquante;
	CString		alarmeBloquante;
	CString		numAlarme;
	CString		etatAnimation;
	CString		etatAPI;

	CString		typeContenant;
	CString		typeContenantPre;
	CString		demandeConfig;
	CString		timeoutConfig;
	CString		mdvSPV;
	CString		mdvAPI;
	CString		timeoutComAPI;
	CString		timeoutMarcheAPI;
	CString		demandeProprete;
	CString		circuitProprete;
	CString		paramProprete;
	CString		demandeVidange;
	CString		magasinCartonNivBas;
	CString		magasinAdhesifNivBas;
	CString		animationIcone;
	CString		CampagneL1; //Ajout 27/03/2020
	CString		CampagneL2; //Ajout 27/03/2020
};

extern sNameFormeuse sVarForm1;
extern sNameFormeuse sVarForm2;


//

struct sNameImp {
	CString		demandeDistant;
	CString		modeDistant;
	CString		alarmeNonBloquante;
	CString		alarmeBloquante;
	CString		numAlarme;
	CString		etatAnimation;
	CString		etatAPI;

	CString		configureL1;
	CString		configureL2;
	CString		encreNivBas;
	CString		demandeConfigL1;
	CString		demandeConfigL2;
	CString		timeoutConfigL1;
	CString		timeoutConfigL2;
	CString		mdvSPV;
	CString		mdvAPI;
	CString		timeoutComAPI;
	CString		timeoutMarcheAPI;
	CString		demandePropreteL1;
	CString		demandePropreteL2;

	CString		etatAPIL1;
	CString		etatAPIL2;
	CString		etatAnimationL1;
	CString		etatAnimationL2;
	CString		demandeVidangeL1;
	CString		demandeVidangeL2;
	CString		demandeMarcheL1;
	CString		demandeMarcheL2;
	CString		demandeMarche;
	CString		CampagneL1; //Ajout 19/03/2020
	CString		CampagneL2; //Ajout 19/03/2020
};

extern sNameImp sVarImp;

struct sNameFerm {
	CString		demandeDistant;
	CString		modeDistant;
	CString		alarmeNonBloquante;
	CString		alarmeBloquante;
	CString		numAlarme;
	CString		etatAnimation;
	CString		etatAPI;

	CString		configureL1;
	CString		configureL2;
	CString		rouleauAdhesifNivBas;
	CString		mdvSPV;
	CString		mdvAPI;
	CString		timeoutComAPI;
	CString		timeoutMarcheAPI;
	CString		demandePropreteL1;
	CString		demandePropreteL2;
	CString		demandeConfigL1;
	CString		demandeConfigL2;
	CString		etatAPIL1;
	CString		etatAPIL2;
	CString		etatAnimationL1;
	CString		etatAnimationL2;
	CString		demandeVidangeL1;
	CString		demandeVidangeL2;
	CString		demandeMarcheL1;
	CString		demandeMarcheL2;
	CString		demandeMarche;
	CString		CampagneL1; //Ajout 19/03/2020
	CString		CampagneL2; //Ajout 19/03/2020
};

extern sNameFerm sVarFerm;

//MES
struct sNameMES {
	struct sNameSetup {
		CString		 refPieces;
		CString		 descProduit;
		CString		 idOperateur;
		CString		 numLot;
		CString		 numOF;
		CString		 nbPieces;
		CString		 nbPiecesPPalette;
		CString		 nbPiecesPContenant;
		CString		 typeConditionnement;
		CString		 nbPiecesPSachet;
		CString		 hauteurSachet;
		CString		 poidsPiece;
		CString		 adjonctionEau;
		CString		 provenance;
		CString		 handlingUnit0;
		CString		 handlingUnit1;
		CString		 handlingUnit2;
		CString		 handlingUnitSize0;
		CString		 handlingUnitSize1;
		CString		 handlingUnitSize2;
	};
	sNameSetup setupL1;
	sNameSetup setupL2;
	struct sNameSync {
		CString		 spvAttLot;
		CString		 spvAttConfirmLancProd;
		CString		 mesConfirmLancProd;
		CString		 mesDemFinProd;
		CString		 plcSyncSetup;
		CString		 mesSyncSetup;
		CString		 mesStartReq;
		CString		 spvStartReq;
		CString		 mestoppageSync;
		CString		 spvtoppageSync;
		CString		 tsStoppageHH;
		CString		 tsStoppageMM;
		CString		 tsStoppageSS;
		CString		 idStoppageCause;
		CString		 qteEauREnsach;
		CString		 infPalettePleine;
		CString		 plcBatchReqStatus;
		CString		 mesBatchReq;
		CString		 mesSyncTime;
		CString		 plcSyncTime;
		CString		 plcConfirmFinProd;
		CString		 numStoppageCause;
		CString		 numStoppageStation;
		//HU Input
		CString		 mesSyncHU;
		CString		 spvSyncHU;
		CString		 huHH;
		CString		 huMM;
		CString		 huSS;
		CString		 huContID;
		CString		 huBatchID;
		CString		 huID;
		CString		 huQte;
		CString		 batchGoodPartQty;
	};
	sNameSync syncL1;
	sNameSync syncL2;
	CString		 rayproCounterL1;
	CString		 rayproCounterL2;
	CString		 plcCounterL1;
	CString		 plcCounterL2;
	CString		 commEnsL1;
	CString		 commEnsL2;
	CString		 commEtq;
	CString		 commFerm;
	CString		 commForm1;
	CString		 commForm2;
	CString		 commImp;
	CString		 commPesL1;
	CString		 commPesL2;
	CString		 commRbb;
	CString		 commRe;
	CString		 commRp;
	CString		 commTr;
};

extern sNameMES sVarMES;

struct sNameSPV {
	CString		modeDegradeL1;
	CString		modeDegradeL2;
	CString		validSetupL1;
	CString		validSetupL2;
	CString		confirmLancProdL1;
	CString		confirmLancProdL2;
	CString		plcDemFinProdL1;
	CString		plcDemFinProdL2;
	CString		plcConfirmFinProdL1;
	CString		plcConfirmFinProdL2;
	CString		annulerProdL1;
	CString		annulerProdL2;
	CString		cptBacVersesL1;
	CString		cptBacVersesL2;
	CString		fatBtnManu;
	CString		affF1;
	CString		affF2;
	CString		validAffForm1;
	CString		validAffForm2;
	CString		passageDegradeL1;
	CString		passageDegradeL2;
	CString		annulerCampReglageL1;
	CString		annulerCampReglageL2;
	CString		campagneDegradeL1;
	CString		campagneDegradeL2;
	CString		bitPopupProvenanceL1;
	CString		bitPopupProvenanceL2;
	CString		choixProvenanceL1;
	CString		choixProvenanceL2;
	CString		HUcontIDL1;
	CString		HUbatchIDL1;
	CString		HUIDL1;
	CString		HUqtyL1;
	CString		validHUHumidification;
	CString		passageDistantL1;
	CString		passageDistantL2;
	CString		bitPopupChoixFormeuseL1; //Ajout 03/02/20
	CString		bitPopupChoixFormeuseL2; //Ajout 03/02/20
	CString		choixFormeuseL1; //Ajout 03/02/20
	CString		choixFormeuseL2; //Ajout 03/02/20
	CString		provenancePieceL1; //Ajout 11/02/2020
	CString		provenancePieceL2; //Ajout 11/02/2020
	CString		rompuEnCours; //Ajout 11/02/2020
	CString		spvNouveauSetupL1; //Ajout 17/03/2020
	CString		spvNouveauSetupL2; //Ajout 17/03/2020
	CString		etatCampagneL1; //Ajout 18/03/2020
	CString		etatCampagneL2; //Ajout 18/03/2020
	CString		bitPopupNbrPieceValideL1; //Ajout 18/03/2020
	CString		bitPopupNbrPieceValideL2; //Ajout 18/03/2020
	CString		nbrPieceValideL1; //Ajout 18/03/2020
	CString		nbrPieceValideL2; //Ajout 18/03/2020
	CString		actualisationRefConditionnement; //Ajout 19/03/2020

	struct sNameSetup {
		CString		 refPieces;
		CString		 descProduit;
		CString		 idOperateur;
		CString		 numLot;
		CString		 numOF;
		CString		 nbPieces;
		CString		 nbPiecesPPalette;
		CString		 nbPiecesPContenant;
		CString		 typeConditionnement;
		CString		 nbPiecesPSachet;
		CString		 hauteurSachet;
		CString		 poidsPiece;
		CString		 adjonctionEau;
		CString		 provenance;
	};
	sNameSetup setupL1;
	sNameSetup setupL2;
};

extern sNameSPV sVarSPV;




//Struct HU INput

struct huInput {
	double containerID;
	double hour;
	double minute;
	double	second;
	CString	batchID;
	CString	huID;
	double	containerQuantity;
	double	numOF;
};

//Trémie
struct sNameTremie {
	CString		demandeMarche;	
	CString		demandeDistant;
	CString		modeDistant;
	CString		alarmeNonBloquante;
	CString		alarmeBloquante;
	CString		numAlarme;
	CString		etatAnimation;	
	CString		etatAPI;
	CString		mdvSPV;
	CString		mdvAPI;
	CString		timeoutComAPI;
	CString		timeoutMarcheAPI;
};

//Trémie
extern sNameTremie sVarEqpTremie;