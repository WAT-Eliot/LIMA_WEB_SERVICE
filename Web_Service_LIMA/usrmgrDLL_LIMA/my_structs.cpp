#include "my_structs.h"



//Structure de machines

 sNameEquipements sVarEqp{
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

 //Création des setups
  sFlatSetup setupEnCoursL1{
	 NULL,
		 NULL,
		 NULL,
		 NULL,
		 NULL,
		 NULL,
		 NULL,
		 NULL,
		 NULL,
		 NULL,
		 NULL,
		 NULL,
		 NULL,
 };

  sFlatSetup setupEnCoursL2{
   NULL,
	   NULL,
	   NULL,
	   NULL,
	   NULL,
	   NULL,
	   NULL,
	   NULL,
	   NULL,
	   NULL,
	   NULL,
	   NULL,
	   NULL,
  };

  sFlatSetup memSetupL2{
   NULL,
	   NULL,
	   NULL,
	   NULL,
	   NULL,
	   NULL,
	   NULL,
	   NULL,
	   NULL,
	   NULL,
	   NULL,
	   NULL,
	   NULL,
  };

  sFlatSetup memSetupL1{
   NULL,
	   NULL,
	   NULL,
	   NULL,
	   NULL,
	   NULL,
	   NULL,
	   NULL,
	   NULL,
	   NULL,
	   NULL,
	   NULL,
	   NULL,
  };

  std::vector<sFlatSetup> vFlatSetup{ setupEnCoursL1, setupEnCoursL2, memSetupL1, memSetupL2 };

//Ici mettre tous les noms de variables PCVue

//Robot Bacs Bleus
CString tempPrefixe = "Machine.RobotBacBleu.";
sNameRobotCaisses sVarEqpRcb{
	tempPrefixe + "Distant.Demande",
	tempPrefixe + "Distant.Etat",
	tempPrefixe + "Defaut.NonBloquant.Etat",
	tempPrefixe + "Defaut.Bloquant.Etat",
	tempPrefixe + "Defaut.numAlarme",
	tempPrefixe + "Marche.AnimationEtat",
	tempPrefixe + "Marche.Etat",
	tempPrefixe + "Ligne1.Configuration.Etat",
	tempPrefixe + "Ligne2.Configuration.Etat",
	tempPrefixe + "Ligne1.Proprete.etatProprete",
	tempPrefixe + "Ligne2.Proprete.etatProprete",
	tempPrefixe + "Ligne1.Proprete.paramProprete",
	tempPrefixe + "Ligne2.Proprete.paramProprete",
	tempPrefixe + "SetupEnvoye.codeArticleL1",
	tempPrefixe + "SetupEnvoye.codeArticleL2",
	tempPrefixe + "Ligne1.Rompu.numeroBac",
	tempPrefixe + "Ligne2.Rompu.numeroBac",
	tempPrefixe + "Ligne1.Rompu.codeArticle",
	tempPrefixe + "Ligne2.Rompu.codeArticle",
	tempPrefixe + "Ligne1.Rompu.qtePieces",
	tempPrefixe + "Ligne2.Rompu.qtePieces",
	tempPrefixe + "Ligne1.Proprete.Demande",
	tempPrefixe + "Ligne2.Proprete.Demande",
	tempPrefixe + "Ligne1.Proprete.Etat",
	tempPrefixe + "Ligne2.Proprete.Etat",
	tempPrefixe + "Ligne1.Configuration.Demande",
	tempPrefixe + "Ligne2.Configuration.Demande",
	tempPrefixe + "Ligne1.Configuration.timeoutConfig",
	tempPrefixe + "Ligne2.Configuration.timeoutConfig",
	tempPrefixe + "Ligne1.Vidange.Demande",
	tempPrefixe + "Ligne1.Vidange.Etat",
	tempPrefixe + "Ligne2.Vidange.Demande",
	tempPrefixe + "Ligne2.Vidange.Etat",
	tempPrefixe + "Ligne1.Production.AnimationEtat",
	tempPrefixe + "Ligne2.Production.AnimationEtat",
	tempPrefixe + "Ligne1.Production.Etat",
	tempPrefixe + "Ligne2.Production.Etat",
	tempPrefixe + "Marche.mdvSPV",
	tempPrefixe + "Marche.mdvAPI",
	tempPrefixe + "Marche.timeoutMdV",
	tempPrefixe + "Marche.timeoutMarche",
	tempPrefixe + "Ligne1.topBacVerse",
	tempPrefixe + "Ligne1.ackBacVerse",
	tempPrefixe + "Ligne2.topBacVerse",
	tempPrefixe + "Ligne2.ackBacVerse",
	tempPrefixe + "Ligne1.HU.contID",
	tempPrefixe + "Ligne1.HU.batchID",
	tempPrefixe + "Ligne1.HU.huID",
	tempPrefixe + "Ligne1.HU.qty",
	tempPrefixe + "Ligne2.HU.contID",
	tempPrefixe + "Ligne2.HU.batchID",
	tempPrefixe + "Ligne2.HU.huID",
	tempPrefixe + "Ligne2.HU.qty",
	tempPrefixe + "Ligne1.Production.Demande",
	tempPrefixe + "Ligne2.Production.Demande",
	tempPrefixe + "Marche.Demande",
	tempPrefixe + "Ligne1.CampagneEnCours", // Ajout 19/03/2020
	tempPrefixe + "Ligne2.CampagneEnCours", // Ajout 19/03/2020
	tempPrefixe + "Ligne1.DemandeArticle", // Ajout 24/03/2020
	tempPrefixe + "Ligne2.DemandeArticle", // Ajout 24/03/2020
};

//Robot Couvercle
CString tempPrefixe2 = "Machine.RobotEtiquette.";
sNameRobotCouvercle sVarEqpRc{
	tempPrefixe2 + "Distant.Demande",
	tempPrefixe2 + "Distant.Etat",
	tempPrefixe2 + "Defaut.NonBloquant.Etat",
	tempPrefixe2 + "Defaut.Bloquant.Etat",
	tempPrefixe2 + "Defaut.numAlarme",
	tempPrefixe2 + "Marche.AnimationEtat",
	tempPrefixe2 + "Marche.Etat",
	tempPrefixe2 + "Ligne1.Configuration.Etat",
	tempPrefixe2 + "Ligne2.Configuration.Etat",
	tempPrefixe2 + "Ligne1.Proprete.etatProprete",
	tempPrefixe2 + "Ligne2.Proprete.etatProprete",
	tempPrefixe2 + "Ligne1.Proprete.paramProprete",
	tempPrefixe2 + "Ligne2.Proprete.paramProprete",
	tempPrefixe2 + "magasinCouvercleNivBas",
	tempPrefixe2 + "Ligne1.Proprete.Demande",
	tempPrefixe2 + "Ligne2.Proprete.Demande",
	tempPrefixe2 + "Ligne1.Proprete.Etat",
	tempPrefixe2 + "Ligne2.Proprete.Etat",
	tempPrefixe2 + "Ligne1.Vidange.Demande",
	tempPrefixe2 + "Ligne1.Vidange.Etat",
	tempPrefixe2 + "Ligne2.Vidange.Demande",
	tempPrefixe2 + "Ligne2.Vidange.Etat",
	tempPrefixe2 + "Ligne1.Production.AnimationEtat",
	tempPrefixe2 + "Ligne2.Production.AnimationEtat",
	tempPrefixe2 + "Ligne1.Production.Etat",
	tempPrefixe2 + "Ligne2.Production.Etat",
	tempPrefixe2 + "Marche.mdvSPV",
	tempPrefixe2 + "Marche.mdvAPI",
	tempPrefixe2 + "Marche.timeoutMdV",
	tempPrefixe2 + "Marche.timeoutMarche",
	tempPrefixe2 + "Ligne1.Configuration.Demande",
	tempPrefixe2 + "Ligne2.Configuration.Demande",
	tempPrefixe2 + "Ligne1.Production.Demande",
	tempPrefixe2 + "Ligne2.Production.Demande",
	tempPrefixe2 + "Ligne1.magEtiquetteNivBas",
	tempPrefixe2 + "Ligne2.magEtiquetteNivBas",
	tempPrefixe2 + "Marche.Demande",
	tempPrefixe2 + "Ligne1.CampagneEnCours", // Ajout 19/03/2020
	tempPrefixe2 + "Ligne2.CampagneEnCours", // Ajout 19/03/2020
};

//Ensacheuse L1
CString tempPrefixe3 = "Machine.EnsacheuseL1.";
sNameEnsacheuse sVarEnsL1{
	tempPrefixe3 + "Marche.Demande",
	tempPrefixe3 + "Distant.Demande",
	tempPrefixe3 + "Distant.Etat",
	tempPrefixe3 + "Defaut.NonBloquant.Etat",
	tempPrefixe3 + "Defaut.Bloquant.Etat",
	tempPrefixe3 + "Defaut.numAlarme",
	tempPrefixe3 + "Marche.AnimationEtat",
	tempPrefixe3 + "Marche.Etat",
	tempPrefixe3 + "Configuration.Etat",
	tempPrefixe3 + "finRouleauFilm",
	tempPrefixe3 + "Setup.hauteurSachet",
	tempPrefixe3 + "Setup.adjonctionEau",
	tempPrefixe3 + "Setup.qteEauReelle",
	tempPrefixe3 + "Proprete.etatProprete",
	tempPrefixe3 + "Proprete.paramProprete",
	tempPrefixe3 + "Proprete.Demande",
	tempPrefixe3 + "Proprete.Etat",
	tempPrefixe3 + "Configuration.Demande",
	tempPrefixe3 + "Configuration.timeoutConfig",
	tempPrefixe3 + "Vidange.Demande",
	tempPrefixe3 + "Vidange.Etat",
	tempPrefixe3 + "Marche.mdvSPV",
	tempPrefixe3 + "Marche.mdvAPI",
	tempPrefixe3 + "Marche.timeoutMdV",
	tempPrefixe3 + "Marche.timeoutMarche",
	tempPrefixe3 + "Marche.AnimationIcone",
	tempPrefixe3 + "CampagneEnCours", // Ajout 19/03/2020


};

//Ensacheuse L2
CString tempPrefixe4 = "Machine.EnsacheuseL2.";
sNameEnsacheuse sVarEnsL2{
	tempPrefixe4 + "Marche.Demande",
	tempPrefixe4 + "Distant.Demande",
	tempPrefixe4 + "Distant.Etat",
	tempPrefixe4 + "Defaut.NonBloquant.Etat",
	tempPrefixe4 + "Defaut.Bloquant.Etat",
	tempPrefixe4 + "Defaut.numAlarme",
	tempPrefixe4 + "Marche.AnimationEtat",
	tempPrefixe4 + "Marche.Etat",
	tempPrefixe4 + "Configuration.Etat",
	tempPrefixe4 + "finRouleauFilm",
	tempPrefixe4 + "Setup.hauteurSachet",
	tempPrefixe4 + "Setup.adjonctionEau",
	tempPrefixe4 + "Setup.qteEauReelle",
	tempPrefixe4 + "Proprete.etatProprete",
	tempPrefixe4 + "Proprete.paramProprete",
	tempPrefixe4 + "Proprete.Demande",
	tempPrefixe4 + "Proprete.Etat",
	tempPrefixe4 + "Configuration.Demande",
	tempPrefixe4 + "Configuration.timeoutConfig",
	tempPrefixe4 + "Vidange.Demande",
	tempPrefixe4 + "Vidange.Etat",
	tempPrefixe4 + "Marche.mdvSPV",
	tempPrefixe4 + "Marche.mdvAPI",
	tempPrefixe4 + "Marche.timeoutMdV",
	tempPrefixe4 + "Marche.timeoutMarche",
	tempPrefixe4 + "Marche.AnimationIcone",
	tempPrefixe4 + "CampagneEnCours", // Ajout 19/03/2020
};


//Peseuse L1
CString tempPrefixe5 = "Machine.PeseuseL1.";
sNamePeseuse sVarPesL1{
	tempPrefixe5 + "Marche.Demande",
	tempPrefixe5 + "Distant.Demande",
	tempPrefixe5 + "Distant.Etat",
	tempPrefixe5 + "Defaut.NonBloquant.Etat",
	tempPrefixe5 + "Defaut.Bloquant.Etat",
	tempPrefixe5 + "Defaut.numAlarme",
	tempPrefixe5 + "Marche.AnimationEtat",
	tempPrefixe5 + "Marche.Etat",
	tempPrefixe5 + "Configuration.Etat",
	tempPrefixe5 + "Proprete.etatProprete",
	tempPrefixe5 + "Proprete.paramProprete",
	tempPrefixe5 + "Proprete.Demande",
	tempPrefixe5 + "Proprete.Etat",
	tempPrefixe5 + "Configuration.Demande",
	tempPrefixe5 + "cheminFichier",
	tempPrefixe5 + "Configuration.timeoutConfig",
	tempPrefixe5 + "Vidange.Demande",
	tempPrefixe5 + "Vidange.Etat",
	tempPrefixe5 + "Marche.mdvSPV",
	tempPrefixe5 + "Marche.mdvAPI",
	tempPrefixe5 + "Marche.timeoutMdV",
	tempPrefixe5 + "Marche.timeoutMarche",
	tempPrefixe5 + "Marche.AnimationIcone",
	tempPrefixe5 + "Defaut.Bloquant.CoherenceSetup",
	tempPrefixe5 + "CampagneEnCours", // Ajout 19/03/2020
};

//Peseuse L2
CString tempPrefixe6 = "Machine.PeseuseL2.";
sNamePeseuse sVarPesL2{
	tempPrefixe6 + "Marche.Demande",
	tempPrefixe6 + "Distant.Demande",
	tempPrefixe6 + "Distant.Etat",
	tempPrefixe6 + "Defaut.NonBloquant.Etat",
	tempPrefixe6 + "Defaut.Bloquant.Etat",
	tempPrefixe6 + "Defaut.numAlarme",
	tempPrefixe6 + "Marche.AnimationEtat",
	tempPrefixe6 + "Marche.Etat",
	tempPrefixe6 + "Configuration.Etat",
	tempPrefixe6 + "Proprete.etatProprete",
	tempPrefixe6 + "Proprete.paramProprete",
	tempPrefixe6 + "Proprete.Demande",
	tempPrefixe6 + "Proprete.Etat",
	tempPrefixe6 + "Configuration.Demande",
	tempPrefixe6 + "cheminFichier",
	tempPrefixe6 + "Configuration.timeoutConfig",
	tempPrefixe6 + "Vidange.Demande",
	tempPrefixe6 + "Vidange.Etat",
	tempPrefixe6 + "Marche.mdvSPV",
	tempPrefixe6 + "Marche.mdvAPI",
	tempPrefixe6 + "Marche.timeoutMdV",
	tempPrefixe6 + "Marche.timeoutMarche",
	tempPrefixe6 + "Marche.AnimationIcone",
	tempPrefixe6 + "Defaut.Bloquant.CoherenceSetup",
	tempPrefixe6 + "CampagneEnCours", // Ajout 19/03/2020
};

//Transitique
CString tempPrefixe7 = "Machine.Transitique.";
sNameTrans sVarTrans{
	tempPrefixe7 + "Distant.Demande",
	tempPrefixe7 + "Distant.Etat",
	tempPrefixe7 + "Defaut.NonBloquant.Etat",
	tempPrefixe7 + "Defaut.Bloquant.Etat",
	tempPrefixe7 + "Defaut.numAlarme",
	tempPrefixe7 + "Marche.AnimationEtat",
	tempPrefixe7 + "Marche.Etat",
	tempPrefixe7 + "Ligne1.Configuration.Etat",
	tempPrefixe7 + "Ligne2.Configuration.Etat",
	tempPrefixe7 + "Setup.typeContenantL1",
	tempPrefixe7 + "Setup.typeContenantL2",
	tempPrefixe7 + "Ligne1.Proprete.etatProprete",
	tempPrefixe7 + "Ligne2.Proprete.etatProprete",
	tempPrefixe7 + "Ligne1.Proprete.paramProprete",
	tempPrefixe7 + "Ligne2.Proprete.paramProprete",
	tempPrefixe7 + "Ligne1.Proprete.Demande",
	tempPrefixe7 + "Ligne2.Proprete.Demande",
	tempPrefixe7 + "Ligne1.Proprete.Etat",
	tempPrefixe7 + "Ligne2.Proprete.Etat",
	tempPrefixe7 + "Ligne1.Configuration.Demande",
	tempPrefixe7 + "Ligne2.Configuration.Demande",
	tempPrefixe7 + "Ligne1.Configuration.timeoutConfig",
	tempPrefixe7 + "Ligne2.Configuration.timeoutConfig",
	tempPrefixe7 + "Ligne1.Vidange.Demande",
	tempPrefixe7 + "Ligne1.Vidange.Etat",
	tempPrefixe7 + "Ligne2.Vidange.Demande",
	tempPrefixe7 + "Ligne2.Vidange.Etat",
	tempPrefixe7 + "Ligne1.Production.AnimationEtat",
	tempPrefixe7 + "Ligne2.Production.AnimationEtat",
	tempPrefixe7 + "Ligne1.Production.Etat",
	tempPrefixe7 + "Ligne2.Production.Etat",
	tempPrefixe7 + "Marche.mdvSPV",
	tempPrefixe7 + "Marche.mdvAPI",
	tempPrefixe7 + "Marche.timeoutMdV",
	tempPrefixe7 + "Marche.timeoutMarche",
	tempPrefixe7 + "numLigneFormeuse1",
	tempPrefixe7 + "numLigneFormeuse2",
	tempPrefixe7 + "numOFL1",
	tempPrefixe7 + "numOFL2",
	tempPrefixe7 + "Ligne1.Production.Demande",
	tempPrefixe7 + "Ligne2.Production.Demande",
	tempPrefixe7 + "Setup.vracSacL1",
	tempPrefixe7 + "Setup.vracSacL2",
	tempPrefixe7 + "Setup.qteParColisL1",
	tempPrefixe7 + "Setup.qteParColisL2",
	tempPrefixe7 + "Setup.designationL1",
	tempPrefixe7 + "Setup.designationL2",
	tempPrefixe7 + "Marche.Demande",
	tempPrefixe7 + "Setup.numLotL1",
	tempPrefixe7 + "Setup.numLotL2",
	tempPrefixe7 + "Setup.codeArticleL1",
	tempPrefixe7 + "Setup.codeArticleL2",
	tempPrefixe7 + "Setup.Provenance_Setup",
	tempPrefixe7 + "derogationIntroductionManuelL1", //Ajout 06/02/20
	tempPrefixe7 + "derogationIntroductionManuelL2", //Ajout 06/02/20
	tempPrefixe7 + "Ligne1.CampagneEnCours", // Ajout 19/03/2020
	tempPrefixe7 + "Ligne2.CampagneEnCours", // Ajout 19/03/2020
};


//Balance
CString tempPrefixe8 = "Machine.GestionBalance.";
sNameBal sVarBal{
	tempPrefixe8 + "Distant.Demande",
	tempPrefixe8 + "Distant.Etat",
	tempPrefixe8 + "Defaut.NonBloquant.Etat",
	tempPrefixe8 + "Defaut.Bloquant.Etat",
	tempPrefixe8 + "Defaut.numAlarme",
	tempPrefixe8 + "Marche.AnimationEtat",
	tempPrefixe8 + "Marche.Etat",
	tempPrefixe8 + "BalanceTare.Tare.poidsPieceValideL1",
	tempPrefixe8 + "BalanceTare.Tare.poidsPieceValideL2",
	tempPrefixe8 + "PoidsPieceL1",
	tempPrefixe8 + "PoidsPieceL2",
	tempPrefixe8 + "BalanceBacRompu.Tare.poidsLuRompu",
	tempPrefixe8 + "BalanceBacRompu.Tare.BacRompuValidL1",
	tempPrefixe8 + "BalanceBacRompu.Tare.BacRompuValidL2",
	"PCVue.VoyantBalanceTarreL1",
	"PCVue.VoyantBalanceTarreL2",
	tempPrefixe8 + "BalanceTare.Tare.DerogationValideL1",
	tempPrefixe8 + "BalanceTare.Tare.DerogationValideL2",
	tempPrefixe8 + "BalancePremierColi1.Tare.NBRPieceValide",
	tempPrefixe8 + "BalancePremierColi2.Tare.NBRPieceValide",
	tempPrefixe8 + "BalancePremierColi1.Tare.DerogationValide",
	tempPrefixe8 + "BalancePremierColi2.Tare.DerogationValide",
	"DLL.DLLVoyantTarreL1", //Ajout 24/03/2020
	"DLL.DLLVoyantTarreL2", //Ajout 24/03/2020
};

//Palettiseur
CString tempPrefixe9 = "Machine.RobotPalettisation.";
sNamePal sVarPal{
	tempPrefixe9 + "Distant.Demande",
	tempPrefixe9 + "Distant.Etat",
	tempPrefixe9 + "Defaut.NonBloquant.Etat",
	tempPrefixe9 + "Defaut.Bloquant.Etat",
	tempPrefixe9 + "Defaut.numAlarme",
	tempPrefixe9 + "Marche.AnimationEtat",
	tempPrefixe9 + "Marche.Etat",
	tempPrefixe9 + "Ligne1.Configuration.Etat",
	tempPrefixe9 + "Ligne2.Configuration.Etat",
	tempPrefixe9 + "Ligne1.Proprete.etatProprete",
	tempPrefixe9 + "Ligne2.Proprete.etatProprete",
	tempPrefixe9 + "Ligne1.Proprete.paramProprete",
	tempPrefixe9 + "Ligne2.Proprete.paramProprete",
	tempPrefixe9 + "Ligne1.Proprete.Demande",
	tempPrefixe9 + "Ligne2.Proprete.Demande",
	tempPrefixe9 + "Ligne1.Proprete.Etat",
	tempPrefixe9 + "Ligne2.Proprete.Etat",
	tempPrefixe9 + "Ligne1.Vidange.Demande",
	tempPrefixe9 + "Ligne1.Vidange.Etat",
	tempPrefixe9 + "Ligne2.Vidange.Demande",
	tempPrefixe9 + "Ligne2.Vidange.Etat",
	tempPrefixe9 + "Tapis1.Proprete.etatProprete",
	tempPrefixe9 + "Tapis2.Proprete.etatProprete",
	tempPrefixe9 + "Tapis1.Proprete.paramProprete",
	tempPrefixe9 + "Tapis2.Proprete.paramProprete",
	tempPrefixe9 + "Tapis1.Proprete.Demande",
	tempPrefixe9 + "Tapis2.Proprete.Demande",
	tempPrefixe9 + "Tapis1.Proprete.Etat",
	tempPrefixe9 + "Tapis2.Proprete.Etat",
	tempPrefixe9 + "Tapis1.Vidange.Demande",
	tempPrefixe9 + "Tapis1.Vidange.Etat",
	tempPrefixe9 + "Tapis2.Vidange.Demande",
	tempPrefixe9 + "Tapis2.Vidange.Etat",
	tempPrefixe9 + "Ligne1.Production.AnimationEtat",
	tempPrefixe9 + "Ligne2.Production.AnimationEtat",
	tempPrefixe9 + "Ligne1.Production.Etat",
	tempPrefixe9 + "Ligne2.Production.Etat",
	tempPrefixe9 + "Tapis1.Production.AnimationEtat",
	tempPrefixe9 + "Tapis2.Production.AnimationEtat",
	tempPrefixe9 + "Tapis1.Production.Etat",
	tempPrefixe9 + "Tapis2.Production.Etat",
	tempPrefixe9 + "Marche.mdvSPV",
	tempPrefixe9 + "Marche.mdvAPI",
	tempPrefixe9 + "Marche.timeoutMdV",
	tempPrefixe9 + "Marche.timeoutMarche",
	tempPrefixe9 + "Ligne1.Configuration.Demande",
	tempPrefixe9 + "Ligne2.Configuration.Demande",
	tempPrefixe9 + "Ligne1.Compteur.NBRPiecesSortie",
	tempPrefixe9 + "Ligne2.Compteur.NBRPiecesSortie",
	tempPrefixe9 + "Ligne1.Production.Demande",
	tempPrefixe9 + "Ligne2.Production.Demande",
	tempPrefixe9 + "magPaletteVideNivBas",
	tempPrefixe9 + "Marche.Demande",
	tempPrefixe9 + "Ligne1.CampagneEnCours", // Ajout 19/03/2020
	tempPrefixe9 + "Ligne2.CampagneEnCours", // Ajout 19/03/2020
};

//Etiqueteuse
CString tempPrefixe10 = "Machine.Etiqueteuse.";
sNameEtq sVarEtq{
	tempPrefixe10 + "Distant.Demande",
	tempPrefixe10 + "Distant.Etat",
	tempPrefixe10 + "Defaut.NonBloquant.Etat",
	tempPrefixe10 + "Defaut.Bloquant.Etat",
	tempPrefixe10 + "Defaut.numAlarme",
	tempPrefixe10 + "Marche.AnimationEtat",
	tempPrefixe10 + "Marche.Etat",
	tempPrefixe10 + "Ligne1.Configuration.Etat",
	tempPrefixe10 + "Ligne2.Configuration.Etat",
	tempPrefixe10 + "rouleauEtiquetteNivBas",
	tempPrefixe10 + "Ligne1.Configuration.Demande",
	tempPrefixe10 + "Ligne2.Configuration.Demande",
	tempPrefixe10 + "Ligne1.Configuration.timeoutConfig",
	tempPrefixe10 + "Ligne2.Configuration.timeoutConfig",
	tempPrefixe10 + "Marche.mdvSPV",
	tempPrefixe10 + "Marche.mdvAPI",
	tempPrefixe10 + "Marche.timeoutMdV",
	tempPrefixe10 + "Marche.timeoutMarche",
	tempPrefixe10 + "Ligne1.Proprete.Demande",
	tempPrefixe10 + "Ligne2.Proprete.Demande",
	tempPrefixe10 + "Ligne1.Production.Etat",
	tempPrefixe10 + "Ligne2.Production.Etat",
	tempPrefixe10 + "Ligne1.Production.AnimationEtat",
	tempPrefixe10 + "Ligne2.Production.AnimationEtat",
	tempPrefixe10 + "Ligne1.Vidange.Demande",
	tempPrefixe10 + "Ligne2.Vidange.Demande",
	tempPrefixe10 + "Ligne1.Production.Demande",
	tempPrefixe10 + "Ligne2.Production.Demande",
	tempPrefixe10 + "Marche.Demande",
	tempPrefixe10 + "Ligne1.CampagneEnCours", // Ajout 19/03/2020
	tempPrefixe10 + "Ligne2.CampagneEnCours", // Ajout 19/03/2020
};

//Imprimante
CString tempPrefixe11 = "Machine.Imprimante.";
sNameImp sVarImp{

	tempPrefixe11 + "Distant.Demande",
	tempPrefixe11 + "Distant.Etat",
	tempPrefixe11 + "Defaut.NonBloquant.Etat",
	tempPrefixe11 + "Defaut.Bloquant.Etat",
	tempPrefixe11 + "Defaut.numAlarme",
	tempPrefixe11 + "Marche.AnimationEtat",
	tempPrefixe11 + "Marche.Etat",
	tempPrefixe11 + "Ligne1.Configuration.Etat",
	tempPrefixe11 + "Ligne2.Configuration.Etat",
	tempPrefixe11 + "encreNivBas",
	tempPrefixe11 + "Ligne1.Configuration.Demande",
	tempPrefixe11 + "Ligne2.Configuration.Demande",
	tempPrefixe11 + "Ligne1.Configuration.timeoutConfig",
	tempPrefixe11 + "Ligne2.Configuration.timeoutConfig",
	tempPrefixe11 + "Marche.mdvSPV",
	tempPrefixe11 + "Marche.mdvAPI",
	tempPrefixe11 + "Marche.timeoutMdV",
	tempPrefixe11 + "Marche.timeoutMarche",
	tempPrefixe11 + "Ligne1.Proprete.Demande",
	tempPrefixe11 + "Ligne2.Proprete.Demande",
	tempPrefixe11 + "Ligne1.Production.Etat",
	tempPrefixe11 + "Ligne2.Production.Etat",
	tempPrefixe11 + "Ligne1.Production.AnimationEtat",
	tempPrefixe11 + "Ligne2.Production.AnimationEtat",
	tempPrefixe11 + "Ligne1.Vidange.Demande",
	tempPrefixe11 + "Ligne2.Vidange.Demande",
	tempPrefixe11 + "Ligne1.Production.Demande",
	tempPrefixe11 + "Ligne2.Production.Demande",
	tempPrefixe11 + "Marche.Demande",
	tempPrefixe11 + "Ligne1.CampagneEnCours", // Ajout 19/03/2020
	tempPrefixe11 + "Ligne2.CampagneEnCours", // Ajout 19/03/2020
};


//Fermeuse
CString tempPrefixe12 = "Machine.Fermeuse.";
sNameFerm sVarFerm{
	tempPrefixe12 + "Distant.Demande",
	tempPrefixe12 + "Distant.Etat",
	tempPrefixe12 + "Defaut.NonBloquant.Etat",
	tempPrefixe12 + "Defaut.Bloquant.Etat",
	tempPrefixe12 + "Defaut.numAlarme",
	tempPrefixe12 + "Marche.AnimationEtat",
	tempPrefixe12 + "Marche.Etat",
	tempPrefixe12 + "Ligne1.Configuration.Etat",
	tempPrefixe12 + "Ligne2.Configuration.Etat",
	tempPrefixe12 + "rouleauAdhesifNivBas",
	tempPrefixe12 + "Marche.mdvSPV",
	tempPrefixe12 + "Marche.mdvAPI",
	tempPrefixe12 + "Marche.timeoutMdV",
	tempPrefixe12 + "Marche.timeoutMarche",
	tempPrefixe12 + "Ligne1.Proprete.Demande",
	tempPrefixe12 + "Ligne2.Proprete.Demande",
	tempPrefixe12 + "Ligne1.Configuration.Demande",
	tempPrefixe12 + "Ligne2.Configuration.Demande",
	tempPrefixe12 + "Ligne1.Production.Etat",
	tempPrefixe12 + "Ligne2.Production.Etat",
	tempPrefixe12 + "Ligne1.Production.AnimationEtat",
	tempPrefixe12 + "Ligne2.Production.AnimationEtat",
	tempPrefixe12 + "Ligne1.Vidange.Demande",
	tempPrefixe12 + "Ligne2.Vidange.Demande",
	tempPrefixe12 + "Ligne1.Production.Demande",
	tempPrefixe12 + "Ligne2.Production.Demande",
	tempPrefixe12 + "Marche.Demande",
	tempPrefixe12 + "Ligne1.CampagneEnCours", // Ajout 19/03/2020
	tempPrefixe12 + "Ligne2.CampagneEnCours", // Ajout 19/03/2020
};

//Formeuse 1
CString tempPrefixe13 = "Machine.Formeuse1.";
sNameFormeuse sVarForm1{
	tempPrefixe13 + "Marche.Demande",
	tempPrefixe13 + "Distant.Demande",
	tempPrefixe13 + "Distant.Etat",
	tempPrefixe13 + "Defaut.NonBloquant.Etat",
	tempPrefixe13 + "Defaut.Bloquant.Etat",
	tempPrefixe13 + "Defaut.numAlarme",
	tempPrefixe13 + "Marche.AnimationEtat",
	tempPrefixe13 + "Marche.Etat",
	tempPrefixe13 + "typeConditionnement",
	tempPrefixe13 + "typeConditionnementPre",
	tempPrefixe13 + "Configuration.Demande",
	tempPrefixe13 + "Configuration.timeoutConfig",
	tempPrefixe13 + "Marche.mdvSPV",
	tempPrefixe13 + "Marche.mdvAPI",
	tempPrefixe13 + "Marche.timeoutMdV",
	tempPrefixe13 + "Marche.timeoutMarche",
	tempPrefixe13 + "Proprete.Demande",
	tempPrefixe13 + "Proprete.etatProprete",
	tempPrefixe13 + "Proprete.paramProprete",
	tempPrefixe13 + "Vidange.Demande",
	tempPrefixe13 + "magasinCartonNivBas",
	tempPrefixe13 + "magasinAdhesifNivBas",
	tempPrefixe13 + "Marche.AnimationIcone",
	tempPrefixe13 + "CampagneEnCoursL1", // Ajout 27/03/2020
	tempPrefixe13 + "CampagneEnCoursL2", // Ajout 27/03/2020
};

//Formeuse 2
CString tempPrefixe14 = "Machine.Formeuse2.";
sNameFormeuse sVarForm2{
	tempPrefixe14 + "Marche.Demande",
	tempPrefixe14 + "Distant.Demande",
	tempPrefixe14 + "Distant.Etat",
	tempPrefixe14 + "Defaut.NonBloquant.Etat",
	tempPrefixe14 + "Defaut.Bloquant.Etat",
	tempPrefixe14 + "Defaut.numAlarme",
	tempPrefixe14 + "Marche.AnimationEtat",
	tempPrefixe14 + "Marche.Etat",
	tempPrefixe14 + "typeConditionnement",
	tempPrefixe14 + "typeConditionnementPre",
	tempPrefixe14 + "Configuration.Demande",
	tempPrefixe14 + "Configuration.timeoutConfig",
	tempPrefixe14 + "Marche.mdvSPV",
	tempPrefixe14 + "Marche.mdvAPI",
	tempPrefixe14 + "Marche.timeoutMdV",
	tempPrefixe14 + "Marche.timeoutMarche",
	tempPrefixe14 + "Proprete.Demande",
	tempPrefixe14 + "Proprete.etatProprete",
	tempPrefixe14 + "Proprete.paramProprete",
	tempPrefixe14 + "Vidange.Demande",
	tempPrefixe14 + "magasinCartonNivBas",
	tempPrefixe14 + "magasinAdhesifNivBas",
	tempPrefixe14 + "Marche.AnimationIcone",
	tempPrefixe14 + "CampagneEnCoursL1", // Ajout 27/03/2020
	tempPrefixe14 + "CampagneEnCoursL2", // Ajout 27/03/2020
};


//MES
CString tempPrefixe15 = "OPC.";
CString tempPrefixe16 = tempPrefixe15 + "syncL1.";
CString tempPrefixe17 = tempPrefixe15 + "syncL2.";
CString tempPrefixe21 = tempPrefixe15 + "Ligne1.";
CString tempPrefixe22 = tempPrefixe15 + "Ligne2.";


sNameMES::sNameSetup setupL1{
	tempPrefixe21 + "refPieces",
	tempPrefixe21 + "descProduit",
	tempPrefixe21 + "idOperateur",
	tempPrefixe21 + "numLot",
	tempPrefixe21 + "numOF",
	tempPrefixe21 + "nbPieces",
	tempPrefixe21 + "nbPiecesPPalette",
	tempPrefixe21 + "nbPiecesPContenant",
	tempPrefixe21 + "typeConditionnement",
	tempPrefixe21 + "nbPiecesPSachet",
	tempPrefixe21 + "hauteurSachet",
	tempPrefixe21 + "poidsPiece",
	tempPrefixe21 + "adjonctionEau",
	tempPrefixe21 + "provenance",
	tempPrefixe21 + "HandlingUnit0",
	tempPrefixe21 + "HandlingUnit1",
	tempPrefixe21 + "HandlingUnit2",
	tempPrefixe21 + "HandlingUnitSize0",
	tempPrefixe21 + "HandlingUnitSize1",
	tempPrefixe21 + "HandlingUnitSize2",
};

sNameMES::sNameSync syncL1{
	tempPrefixe16 + "spvAttLot",
	tempPrefixe16 + "spvAttConfirmLancProd",
	tempPrefixe16 + "mesConfirmLancProd",
	tempPrefixe16 + "mesDemFinProd",
	tempPrefixe16 + "plcSyncSetup",
	tempPrefixe16 + "mesSyncSetup",
	tempPrefixe16 + "mesStartReq",
	tempPrefixe16 + "spvStartReq",
	tempPrefixe16 + "mestoppageSync",
	tempPrefixe16 + "spvtoppageSync",
	tempPrefixe16 + "tsStoppageHH",
	tempPrefixe16 + "tsStoppageMM",
	tempPrefixe16 + "tsStoppageSS",
	tempPrefixe16 + "idStoppageCause",
	tempPrefixe16 + "qteEauREnsach",
	tempPrefixe16 + "infPalettePleine",
	tempPrefixe16 + "plcBatchReqStatus",
	tempPrefixe16 + "mesBatchReq",
	tempPrefixe16 + "mesSyncTime",
	tempPrefixe16 + "plcSyncTime",
	tempPrefixe16 + "plcConfirmFinProd",
	tempPrefixe16 + "numStoppageCause",
	tempPrefixe16 + "numStoppageStation",
	tempPrefixe16 + "mesSyncHU",
	tempPrefixe16 + "spvSyncHU",
	tempPrefixe16 + "huHH",
	tempPrefixe16 + "huMM",
	tempPrefixe16 + "huSS",
	tempPrefixe16 + "huContID",
	tempPrefixe16 + "huBatchID",
	tempPrefixe16 + "huID",
	tempPrefixe16 + "huQte",
	tempPrefixe16 + "batchGoodPartQty",
};

//MES L2

sNameMES::sNameSetup setupL2{
	tempPrefixe22 + "refPieces",
	tempPrefixe22 + "descProduit",
	tempPrefixe22 + "idOperateur",
	tempPrefixe22 + "numLot",
	tempPrefixe22 + "numOF",
	tempPrefixe22 + "nbPieces",
	tempPrefixe22 + "nbPiecesPPalette",
	tempPrefixe22 + "nbPiecesPContenant",
	tempPrefixe22 + "typeConditionnement",
	tempPrefixe22 + "nbPiecesPSachet",
	tempPrefixe22 + "hauteurSachet",
	tempPrefixe22 + "poidsPiece",
	tempPrefixe22 + "adjonctionEau",
	tempPrefixe22 + "provenance",
	tempPrefixe22 + "HandlingUnit0",
	tempPrefixe22 + "HandlingUnit1",
	tempPrefixe22 + "HandlingUnit2",
	tempPrefixe22 + "HandlingUnitSize0",
	tempPrefixe22 + "HandlingUnitSize1",
	tempPrefixe22 + "HandlingUnitSize2",
};

sNameMES::sNameSync syncL2{
	tempPrefixe17 + "spvAttLot",
	tempPrefixe17 + "spvAttConfirmLancProd",
	tempPrefixe17 + "mesConfirmLancProd",
	tempPrefixe17 + "mesDemFinProd",
	tempPrefixe17 + "plcSyncSetup",
	tempPrefixe17 + "mesSyncSetup",
	tempPrefixe17 + "mesStartReq",
	tempPrefixe17 + "spvStartReq",
	tempPrefixe17 + "mestoppageSync",
	tempPrefixe17 + "spvtoppageSync",
	tempPrefixe17 + "tsStoppageHH",
	tempPrefixe17 + "tsStoppageMM",
	tempPrefixe17 + "tsStoppageSS",
	tempPrefixe17 + "idStoppageCause",
	tempPrefixe17 + "qteEauREnsach",
	tempPrefixe17 + "infPalettePleine",
	tempPrefixe17 + "plcBatchReqStatus",
	tempPrefixe17 + "mesBatchReq",
	tempPrefixe17 + "mesSyncTime",
	tempPrefixe17 + "plcSyncTime",
	tempPrefixe17 + "plcConfirmFinProd",
	tempPrefixe17 + "numStoppageCause",
	tempPrefixe17 + "numStoppageStation",
	tempPrefixe17 + "mesSyncHU",
	tempPrefixe17 + "spvSyncHU",
	tempPrefixe17 + "huHH",
	tempPrefixe17 + "huMM",
	tempPrefixe17 + "huSS",
	tempPrefixe17 + "huContID",
	tempPrefixe17 + "huBatchID",
	tempPrefixe17 + "huID",
	tempPrefixe17 + "huQte",
	tempPrefixe17 + "batchGoodPartQty",
};

sNameMES sVarMES{
		setupL1,
		setupL2,
		syncL1,
		syncL2,
		tempPrefixe15 + "rayproCounterL1",
		tempPrefixe15 + "rayproCounterL2",
		tempPrefixe15 + "plcCounterL1",
		tempPrefixe15 + "plcCounterL2",
		tempPrefixe15 + "AlmCom.EnsL1",
		tempPrefixe15 + "AlmCom.EnsL2",
		tempPrefixe15 + "AlmCom.Etq",
		tempPrefixe15 + "AlmCom.Ferm",
		tempPrefixe15 + "AlmCom.Form1",
		tempPrefixe15 + "AlmCom.Form2",
		tempPrefixe15 + "AlmCom.Imp",
		tempPrefixe15 + "AlmCom.PesL1",
		tempPrefixe15 + "AlmCom.PesL2",
		tempPrefixe15 + "AlmCom.Rbb",
		tempPrefixe15 + "AlmCom.Re",
		tempPrefixe15 + "AlmCom.Rp",
		tempPrefixe15 + "AlmCom.Tr",
};

//SPV

CString tempPrefixe18 ="modeDegrade.";
CString tempPrefixe19 = tempPrefixe18 + "Ligne1.";
CString tempPrefixe20 = tempPrefixe18 + "Ligne2.";


sNameSPV::sNameSetup setupLocalL1{
	tempPrefixe19 + "refPieces",
	tempPrefixe19 + "descProduit",
	tempPrefixe19 + "idOperateur",
	tempPrefixe19 + "numLot",
	tempPrefixe19 + "numOF",
	tempPrefixe19 + "nbPieces",
	tempPrefixe19 + "nbPiecesPPalette",
	tempPrefixe19 + "nbPiecesPContenant",
	tempPrefixe19 + "typeConditionnement",
	tempPrefixe19 + "nbPiecesPSachet",
	tempPrefixe19 + "hauteurSachet",
	tempPrefixe19 + "poidsPiece",
	tempPrefixe19 + "adjonctionEau",
	tempPrefixe19 + "provenance",
};

sNameSPV::sNameSetup setupLocalL2{
	tempPrefixe20 + "refPieces",
	tempPrefixe20 + "descProduit",
	tempPrefixe20 + "idOperateur",
	tempPrefixe20 + "numLot",
	tempPrefixe20 + "numOF",
	tempPrefixe20 + "nbPieces",
	tempPrefixe20 + "nbPiecesPPalette",
	tempPrefixe20 + "nbPiecesPContenant",
	tempPrefixe20 + "typeConditionnement",
	tempPrefixe20 + "nbPiecesPSachet",
	tempPrefixe20 + "hauteurSachet",
	tempPrefixe20 + "poidsPiece",
	tempPrefixe20 + "adjonctionEau",
	tempPrefixe20 + "provenance",
};

sNameSPV sVarSPV{
	tempPrefixe18 + "modeDegradeL1",
	tempPrefixe18 + "modeDegradeL2",
	tempPrefixe18 + "validSetupL1",
	tempPrefixe18 + "validSetupL2",
	tempPrefixe18 + "confirmLancProdL1",
	tempPrefixe18 + "confirmLancProdL2",
	tempPrefixe18 + "plcDemFinProdL1",
	tempPrefixe18 + "plcDemFinProdL2",
	tempPrefixe18 + "ConfirmFinProdL1",
	tempPrefixe18 + "ConfirmFinProdL2",
	tempPrefixe18 + "annulerProdL1",
	tempPrefixe18 + "annulerProdL2",
	tempPrefixe18 + "cptBacVersesL1",
	tempPrefixe18 + "cptBacVersesL2",
	tempPrefixe18 + "FAT_BTN_MANU",
	tempPrefixe18 + "affF1",
	tempPrefixe18 + "affF2",
	tempPrefixe18 + "validAffForm1",
	tempPrefixe18 + "validAffForm2",
	tempPrefixe18 + "PassageDegradeL1",
	tempPrefixe18 + "PassageDegradeL2",
	tempPrefixe18 + "annulerCampReglageL1",
	tempPrefixe18 + "annulerCampReglageL2",
	tempPrefixe18 + "campagneDegradeL1",
	tempPrefixe18 + "campagneDegradeL2",
	tempPrefixe18 + "bitPopupProvenanceL1",
	tempPrefixe18 + "bitPopupProvenanceL2",
	tempPrefixe18 + "choixProvenanceL1",
	tempPrefixe18 + "choixProvenanceL2",
	tempPrefixe18 + "HUHumidification.HUcontIDL1",
	tempPrefixe18 + "HUHumidification.HUbatchIDL1",
	tempPrefixe18 + "HUHumidification.HUIDL1",
	tempPrefixe18 + "HUHumidification.HUqtyL1",
	tempPrefixe18 + "HUHumidification.validHUHumidification",
	tempPrefixe18 + "passageDistantL1",
	tempPrefixe18 + "passageDistantL2",
	tempPrefixe18 + "bitPopupChoixFormeuseL1", //Ajout 03/02/20
	tempPrefixe18 + "bitPopupChoixFormeuseL2", //Ajout 03/02/20
	tempPrefixe18 + "choixFormeuseL1", //Ajout 03/02/20
	tempPrefixe18 + "choixFormeuseL2", //Ajout 03/02/20
	tempPrefixe18 + "ProvenancePieceL1", // Ajout 11/02/2020
	tempPrefixe18 + "ProvenancePieceL2", // Ajout 11/02/2020
	tempPrefixe18 + "RompuEncours", // Ajout 11/02/2020
	tempPrefixe18 + "spvNouveauSetupL1", // Ajout 17/03/2020
	tempPrefixe18 + "spvNouveauSetupL2", // Ajout 17/03/2020
	tempPrefixe18 + "etatCampagneL1", // Ajout 18/03/2020
	tempPrefixe18 + "etatCampagneL2", // Ajout 18/03/2020
	tempPrefixe18 + "bitNbrPieceValideL1", // Ajout 18/03/2020
	tempPrefixe18 + "bitNbrPieceValideL2", // Ajout 18/03/2020
	tempPrefixe18 + "nbrPieceValideL1", // Ajout 18/03/2020
	tempPrefixe18 + "nbrPieceValideL2", // Ajout 18/03/2020
	tempPrefixe18 + "actualisationRefConditionnement", // Ajout 19/03/2020
	setupLocalL1,
	setupLocalL2,
};


//Trémie
CString tempPrefixe25 = "Machine.PeseuseL1.";
sNameTremie sVarEqpTremie{
	tempPrefixe25 + "Tremie.Marche.Demande",
	tempPrefixe25 + "Tremie.Distant.Demande",
	tempPrefixe25 + "Tremie.Distant.Etat",
	tempPrefixe25 + "Tremie.Defaut.NonBloquant.Etat",
	tempPrefixe25 + "Tremie.Defaut.Bloquant.Etat",
	tempPrefixe25 + "Tremie.Defaut.numAlarme",
	tempPrefixe25 + "Tremie.Marche.AnimationEtat",
	tempPrefixe25 + "Tremie.Marche.Etat",
	tempPrefixe25 + "Tremie.mdvSPV",
	tempPrefixe25 + "Tremie.mdvAPI",
	tempPrefixe25 + "Tremie.timeoutComAPI",
	tempPrefixe25 + "Tremie.timeoutMarcheAPI;",
};