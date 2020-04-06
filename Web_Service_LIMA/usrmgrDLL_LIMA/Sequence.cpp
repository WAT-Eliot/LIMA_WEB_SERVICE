#include "StdAfx.h"
#include "Sequence.h"
#include "my_logger.h"

//Constructor
Sequence::Sequence(void) {
	//Var interne
	int tempEtape = 0;
	//1 sec for scrutation
	m_delay = 1000;
	m_countSec = 0;
	vNumEtape.push_back(new TypeAna("DLL.numEtapeL1", 10, 0));
	vNumEtape.push_back(new TypeAna("DLL.numEtapeL2", 11, 0));
	vNumEtape.at(0)->WriteVar(PROD_ATTENTE_SETUP);
	vNumEtape.at(1)->WriteVar(PROD_ATTENTE_SETUP);
	// Seq de comm setup pour les 2 lignes
	vNumSequComSetup.push_back(COM_SETUP_ETAT_INITIAL);
	vNumSequComSetup.push_back(COM_SETUP_ETAT_INITIAL);
	// Seq de comm fin campagne pour les 2 lignes
	vNumSequComFinCampagne.push_back(COM_FIN_CAMPAGNE_ETAT_INITIAL);
	vNumSequComFinCampagne.push_back(COM_FIN_CAMPAGNE_ETAT_INITIAL);
	// Seq de comm alarme
	// Seq de comm setup pour les 2 lignes
	vNumSequComAlarme.push_back(COM_ALARME_ETAT_INITIAL);
	vNumSequComAlarme.push_back(COM_ALARME_ETAT_INITIAL);
	// Seq de comm HU pour les 2 lignes
	vNumSequComHU.push_back(COM_HU_ETAT_INITIAL);
	vNumSequComHU.push_back(COM_HU_ETAT_INITIAL);
	// Seq de comm dummy setup pour les 2 lignes
	vNumSequComDummySetup.push_back(COM_SETUP_ETAT_INITIAL);
	vNumSequComDummySetup.push_back(COM_SETUP_ETAT_INITIAL);
	//m_numSequComAlarme = COM_ALARME_ETAT_INITIAL;
	m_etatComMES.push_back(0);
	m_etatComMES.push_back(0);

	m_setupDegrade.push_back(false);
	m_setupDegrade.push_back(false);

	m_memAlmEnCoursL1 = 0;
	m_memAlmEnCoursL2 = 0;

	//Cpt com setup
	vCptComSetup.push_back(0);
	vCptComSetup.push_back(0);

	//2 mémoires du mot de vie
	m_rayproCounterMem.push_back(0);
	m_rayproCounterMem.push_back(0);

	//Mémoire étape mode dégradé
	vMemEtapeDegrade.push_back(0);
	vMemEtapeDegrade.push_back(0);

	try {
		//Import des enregistrements
		for (int i = 1; i <= 2; i++) {
			tempEtape = ImportFichierEtape(i);
			if (tempEtape > PROD_ATTENTE_SETUP && tempEtape < PROD_CAMPAGNE_TERMINEE) {
				ImportFichierSetup(i);
				ImportFichierVProprete(i);
				ImportFichierVConfig(i);
				ImportFichierVModeDistant(i);
				ImportFichierVScrutation(i);
				
			}
		}
		ImportFichierConditionnement();
	}
	catch(...){}
}

//Start sequence
void Sequence::Start()
{
	m_tG7Main = boost::thread(&Sequence::G7Main, this);

}

//Start sequence Alarme
void Sequence::StartAlarme(int id, double num, bool val) {

	LOG_DEBUG << "GetMemAlmEnCours(1) = " << GetMemAlmEnCours(1) << "";
	LOG_DEBUG << "GetMemAlmEnCours(2) = " << GetMemAlmEnCours(2) << "";
	LOG_DEBUG << "num = " << num << "";
	LOG_DEBUG << "id = " << id << "";
	LOG_DEBUG << "val = " << val << "";
	/******************* AJOUT PRISE EN COMPTE NUM DEFAUT POUR REDIRECTION LIGNE 1 OU 2******************/
	

	//Si ID transitique
	if (id == 4000 && num >= 600) {
		//Etiqueteuse
		if(num < 700){
		
		}
		//Imprimante
		else if (num < 800) {
		
		}
		//Fermeuse
		else if (num < 900) {
		
		}
		//Général cellule
		else if (num < 1000) {
			if ((val == 1 && sVarEqp.g7->GetMemAlmEnCours(1) == 0)
				|| val == 0 && sVarEqp.g7->GetMemAlmEnCours(1) != 0)
			{
				//On change la mémoire alarme
				sVarEqp.g7->SetMemAlmEnCours(1, (int)num);
				if (mutAlarmeL1.try_lock()) {
					mutAlarmeL1.unlock();
					boost::thread* m_tG7ComAlarme1 = new boost::thread(boost::bind(&Sequence::GetComAlarme, this, id, num, val, 1));
				}
				if (val == 0) {
					sVarEqp.g7->SetMemAlmEnCours(1, 0);
				}
			}
			if ((val == 1 && sVarEqp.g7->GetMemAlmEnCours(2) == 0)
				|| val == 0 && sVarEqp.g7->GetMemAlmEnCours(2) != 0)
			{
				//On change la mémoire alarme
				sVarEqp.g7->SetMemAlmEnCours(2, (int)num);
				if (mutAlarmeL2.try_lock()) {
					mutAlarmeL2.unlock();
					boost::thread* m_tG7ComAlarme2 = new boost::thread(boost::bind(&Sequence::GetComAlarme, this, id, num, val, 2));
				}
				if (val == 0) {
					sVarEqp.g7->SetMemAlmEnCours(2, 0);
				}
			}
		}
	}
	//Transitique
	else if (id == 4000 && num <= 400) {
		//Général
		if (num < 270 || num >= 370) {
			if ((val == 1 && sVarEqp.g7->GetMemAlmEnCours(1) == 0)
				|| val == 0 && sVarEqp.g7->GetMemAlmEnCours(1) != 0)
			{
				//On change la mémoire alarme
				sVarEqp.g7->SetMemAlmEnCours(1, (int)num);
				if (mutAlarmeL1.try_lock()) {
					mutAlarmeL1.unlock();
					boost::thread* m_tG7ComAlarme1 = new boost::thread(boost::bind(&Sequence::GetComAlarme, this, id, num, val, 1));
				}
				if (val == 0) {
					sVarEqp.g7->SetMemAlmEnCours(1, 0);
				}
			}
			if ((val == 1 && sVarEqp.g7->GetMemAlmEnCours(2) == 0)
				|| val == 0 && sVarEqp.g7->GetMemAlmEnCours(2) != 0)
			{
				//On change la mémoire alarme
				sVarEqp.g7->SetMemAlmEnCours(2, (int)num);
				if (mutAlarmeL2.try_lock()) {
					mutAlarmeL2.unlock();
					boost::thread* m_tG7ComAlarme2 = new boost::thread(boost::bind(&Sequence::GetComAlarme, this, id, num, val, 2));
				}
				if (val == 0) {
					sVarEqp.g7->SetMemAlmEnCours(2, 0);
				}
			}
		}
		//L1
		if (num >= 270 && num < 320) {
			if ((val == 1 && sVarEqp.g7->GetMemAlmEnCours(1) == 0)
				|| val == 0 && sVarEqp.g7->GetMemAlmEnCours(1) != 0)
			{
				//On change la mémoire alarme
				sVarEqp.g7->SetMemAlmEnCours(1, (int)num);
				if (mutAlarmeL1.try_lock()) {
					mutAlarmeL1.unlock();
					boost::thread* m_tG7ComAlarme1 = new boost::thread(boost::bind(&Sequence::GetComAlarme, this, id, num, val, 1));
				}

				if (val == 0) {
					sVarEqp.g7->SetMemAlmEnCours(1, 0);
				}
			}
		}
		//L2
		if (num >= 320 && num < 370) {
			if ((val == 1 && sVarEqp.g7->GetMemAlmEnCours(2) == 0)
				|| val == 0 && sVarEqp.g7->GetMemAlmEnCours(2) != 0)
			{
				//On change la mémoire alarme
				sVarEqp.g7->SetMemAlmEnCours(2, (int)num);
				if (mutAlarmeL2.try_lock()) {
					mutAlarmeL2.unlock();
					boost::thread* m_tG7ComAlarme2 = new boost::thread(boost::bind(&Sequence::GetComAlarme, this, id, num, val, 2));
				}
				if (val == 0) {
					sVarEqp.g7->SetMemAlmEnCours(2, 0);
				}
			}
		}
	}
	//Sinon
	else {

		//L1
		if (num < 300) {
			if ((val == 1 && sVarEqp.g7->GetMemAlmEnCours(1) == 0)
				|| val == 0 && sVarEqp.g7->GetMemAlmEnCours(1) != 0)
			{
				//On change la mémoire alarme
				sVarEqp.g7->SetMemAlmEnCours(1, (int)num);
				if (mutAlarmeL1.try_lock()) {
					mutAlarmeL1.unlock();
					boost::thread* m_tG7ComAlarme1 = new boost::thread(boost::bind(&Sequence::GetComAlarme, this, id, num, val, 1));
				}

				if (val == 0) {
					sVarEqp.g7->SetMemAlmEnCours(1, 0);
				}
			}
		}
		//L2
		else if (num < 600) {
			if ((val == 1 && sVarEqp.g7->GetMemAlmEnCours(2) == 0)
				|| val == 0 && sVarEqp.g7->GetMemAlmEnCours(2) != 0)
			{
				//On change la mémoire alarme
				sVarEqp.g7->SetMemAlmEnCours(2, (int)num);
				if (mutAlarmeL2.try_lock()) {
					mutAlarmeL2.unlock();
					boost::thread* m_tG7ComAlarme2 = new boost::thread(boost::bind(&Sequence::GetComAlarme, this, id, num, val, 2));
				}
				if (val == 0) {
					sVarEqp.g7->SetMemAlmEnCours(2, 0);
				}
			}
		}
		//L1 & L2
		else {
			if ((val == 1 && sVarEqp.g7->GetMemAlmEnCours(1) == 0)
				|| val == 0 && sVarEqp.g7->GetMemAlmEnCours(1) != 0)
			{
				//On change la mémoire alarme
				sVarEqp.g7->SetMemAlmEnCours(1, (int)num);
				if (mutAlarmeL1.try_lock()) {
					mutAlarmeL1.unlock();
					boost::thread* m_tG7ComAlarme1 = new boost::thread(boost::bind(&Sequence::GetComAlarme, this, id, num, val, 1));
				}
				if (val == 0) {
					sVarEqp.g7->SetMemAlmEnCours(1, 0);
				}
			}
			if ((val == 1 && sVarEqp.g7->GetMemAlmEnCours(2) == 0)
				|| val == 0 && sVarEqp.g7->GetMemAlmEnCours(2) != 0)
			{
				//On change la mémoire alarme
				sVarEqp.g7->SetMemAlmEnCours(2, (int)num);
				if (mutAlarmeL2.try_lock()) {
					mutAlarmeL2.unlock();
					boost::thread* m_tG7ComAlarme2 = new boost::thread(boost::bind(&Sequence::GetComAlarme, this, id, num, val, 2));
				}
				if (val == 0) {
					sVarEqp.g7->SetMemAlmEnCours(2, 0);
				}
			}
		}
	}


	//m_tG7ComAlarme = boost::thread(&Sequence::GetComAlarme, this, id, num, val);
	//boost::thread* m_tG7ComAlarme = new boost::thread(boost::bind(&Sequence::GetComAlarme, this, id, num, val));
	//if (this->m_numSequComAlarme){}
}

//Stop sequence
void Sequence::Stop()
{
	m_tG7Main.interrupt();
	m_tG7Main.join();
}

//Check datas 
void Sequence::G7Main(){
	//Vars internes
	int iNumConditionnement[2] = { 0,0 }; // Ajout 19/03/2020 // Peermet de connaitre la plce de la reference dans le vecteur

	// Init machine sortie campagne
	CI_ModeDistant(0);
	CI_ModeDistant(1);

	for (;;){
		try {
			const boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
			for (unsigned short ligne = 1; ligne <= 2; ligne++) {

				//G7 PRODUCTION 
				switch ((int)vNumEtape.at(ligne - 1)->GetVarValue())
				{
				case PROD_ATTENTE_SETUP:
					if (m_setupDegrade.at(ligne - 1) == true) { //Si mode sans Raypro
						
						if (sVarEqp.spvWAT->GetValidSetupLocal(ligne)) { //Si la validation d'OF local est présente
							LOG_INFO << "Setup valide, lancement phase Robot Caisses";
							
							vFlatSetup.at(ligne - 1) = sVarEqp.spvWAT->GetSetupLocal(ligne);
							sVarEqp.spvWAT->ResetValidSetupLocal(ligne);

							//Initialisation des demandes pour la ligne corespondante
							/*
							Ajout 26/12/19
							*/
							InitDemande(ligne);

							//Définition du numéro de reference
							iNumConditionnement[ligne - 1] = DefPositionRefConditionnement(ligne);
							
							//Passage en popup provenance
							vNumEtape.at(ligne - 1)->WriteVar(PROD_ATTENTE_PROVENANCE);
							CreateFichierEtape(ligne, PROD_ATTENTE_PROVENANCE);

							//Gestion des choix formeuse et provenance
							/*
							Ajout 03/02/20
							Modif 07/02/2020
							Modif 19/03/2020
							*/
							if (GetBesoinFormeuse(ligne, iNumConditionnement[ligne - 1])) // Definition du besoin formeuse a carton
							{
								sVarEqp.spvWAT->SetBitPopupChoixFormeuse(ligne, true);
							}
							sVarEqp.spvWAT->SetBitPopupProvenance(ligne, true);
						}
					}
					else { //Si mode avec Raypro

						//Mise à jour de la séquence de com pour le setup
						vNumSequComSetup.at(ligne - 1) = Sequence::GetComSetup(ligne);

						if (vNumSequComSetup.at(ligne - 1) == COM_SETUP_RAZ_SPV)
						{
							vNumSequComSetup.at(ligne - 1) = COM_SETUP_ETAT_INITIAL;

							//Initialisation des demandes pour la ligne corespondante
							/*
							Ajout 26/12/19
							*/
							InitDemande(ligne);

							//Définition du numéro de reference
							iNumConditionnement[ligne - 1] = DefPositionRefConditionnement(ligne);
														
							//Passage en popup provenance et choix formeuse
							vNumEtape.at(ligne - 1)->WriteVar(PROD_ATTENTE_PROVENANCE);
							CreateFichierEtape(ligne, PROD_ATTENTE_PROVENANCE);

							//Mise a jour du bit choix provenance a la reception du setup
							sVarEqp.spvWAT->SetChoixProvenance(ligne, sVarEqp.mesRayPro->GetProvenance(ligne));

							//Gestion des choix formeuse et provenance
							/*
							Ajout 03/02/20
							Modif 07/02/2020
							Modif 19/03/2020
							*/
							if (GetBesoinFormeuse(ligne, iNumConditionnement[ligne - 1])) // Definition du besoin formeuse a carton
							{
								sVarEqp.spvWAT->SetBitPopupChoixFormeuse(ligne, true);
							}
							sVarEqp.spvWAT->SetBitPopupProvenance(ligne, true);
						}
					}
					break;

				case PROD_ATTENTE_PROVENANCE:
					if ((!sVarEqp.spvWAT->GetBitPopupProvenance(ligne)) && ((!sVarEqp.spvWAT->GetBitPopupChoixFormeuse(ligne)))) { //Choix provenance réalisé et choix formeuse
						if (!sVarEqp.spvWAT->GetChoixProvenance(ligne)) { //Si l'opérateur choisit Syleps
							LOG_INFO << "L'opérateur à choisi la provenance : Syleps";
							sVarEqp.spvWAT->SetEtatCampagne(ligne, "Setup validé, lancement phase Robot Caisses"); // Ajout 18/03/2020
							
							vFlatSetup.at(ligne - 1).provenance = 0;
							sVarEqp.spvWAT->SetProvenancePiece(ligne, false); // Ajout 11/02/2020

							//Envoie demande de marche cellule robot bacs bleus
							sVarEqp.APIrobotCaisses->EnvoiMarcheCellule();
							sVarEqp.APIrobotCaisses->SetCampagne(ligne, true);
							vConfigCampagne[ligne - 1].push_back(sVarEqp.APIrobotCaisses);

							//On passe à la phase propreté du robot
							vNumEtape.at(ligne - 1)->WriteVar(PROD_PROPRETE_ROBOT);
							CreateFichierEtape(ligne, PROD_PROPRETE_ROBOT);
						}
						else { //Sinon, provenance Trémie
							LOG_INFO << "L'opérateur à choisi la provenance : Trémie";
							sVarEqp.spvWAT->SetEtatCampagne(ligne, "Setup valide, lancement phase configuration générale"); // Ajout 18/03/2020
							//MAJ du setup DLL et SCADA
							
							vFlatSetup.at(ligne - 1).provenance = 1;
							sVarEqp.spvWAT->SetProvenancePiece(ligne, true); // Ajout 11/02/2020

							//On passe directement à config générale
							vNumEtape.at(ligne - 1)->WriteVar(PROD_CONFIG_GENERALE);
							CreateFichierEtape(ligne, PROD_CONFIG_GENERALE);
						}



						//Activation de la Tare
						sVarEqp.APIbalance->SetDLLVoyantTarre(ligne, true);

						/*
						Modification 03/12/19
						Modification 03/02/20
						*/
						//Définir vecteur d'équipents proprete et configuration
						GetVectorProprete(ligne, iNumConditionnement[ligne - 1]);
						GetVectorConfig(ligne, iNumConditionnement[ligne - 1]);

						//Enregistrement des données
						CreateFichierSetup(ligne, vFlatSetup.at(ligne - 1));
						CreateFichierVProprete(ligne, vPropreteCampagne[ligne - 1]);
						CreateFichierVConfig(ligne, vConfigCampagne[ligne - 1]);

						//Initialisation pour scrutation mode distant
						/*
						Ajout 26/03/2020
						*/
						CI_Scrutation(ligne);

						/*
						Ajout 03/02/20
						*/
						GetVectorScrutation(ligne);
						CreateFichierVScrutation(ligne, vScrutation[ligne - 1]);
					}
					break;

				case PROD_PROPRETE_ROBOT:
					if (phaseConfigRobot(vFlatSetup.at(ligne + 1), vFlatSetup.at(ligne - 1), ligne)) {
						LOG_INFO << "Propreté et config robot validée";
						sVarEqp.spvWAT->SetEtatCampagne(ligne, "Robot démarré, lancement phase configuration générale"); // Ajout 18/03/2020

						// RAZ demande configuration et propreté robot 
						sVarEqp.APIrobotCaisses->ResetDemandeProprete(ligne);
						sVarEqp.APIrobotCaisses->ResetDemandeConfiguration(ligne);

						//Passer à l'étape suivante
						vNumEtape.at(ligne - 1)->WriteVar(PROD_CONFIG_GENERALE);
						CreateFichierEtape(ligne, PROD_CONFIG_GENERALE);
					}
					break;
				case PROD_CONFIG_GENERALE:
					if (phaseConfig(ligne)) {

						LOG_INFO << "Configuration validée";
						sVarEqp.spvWAT->SetEtatCampagne(ligne, "Equipements propre et configuré, attente validation RAYPRO"); // Ajout 18/03/2020
						//Passer à l'étape suivante
						vNumEtape.at(ligne - 1)->WriteVar(PROD_ATTENTE_VALIDATION);
						CreateFichierEtape(ligne, PROD_ATTENTE_VALIDATION);
					}
					//Gestion du voyant tare si poids valide
					//Gestion Voyant Balance Tare en SCADA BASIC dans ETATPROL1 et ETATPROL2
					if (sVarEqp.APIbalance->GetValidTarre(ligne)) {
						//Remise a 0 le voyant de la tare
						sVarEqp.APIbalance->SetDLLVoyantTarre(ligne, false);

						/*
						Ajout 12/12/19
						Modif 26/12/19
						*/
						if ((vFlatSetup.at(ligne - 1).provenance == false) && (sVarEqp.APIrobotCaisses->GetFlagModeDistant(ligne) == false)) {
							if (sVarEqp.APIrobotCaisses->GetEtatAPI(ligne) == 2) {
								//Ajout Robot Bacs Bleus pour le mode distant
								LOG_INFO << "Ajout Robot Caisses -> vModeDistant";
								vModeDistant[ligne - 1].push_back(sVarEqp.APIrobotCaisses);
								CreateFichierVModeDistant(ligne, vModeDistant[ligne - 1]);

								sVarEqp.APIrobotCaisses->SetFlagModeDistant(ligne, true);
							}
						}
					}
					break;
				case PROD_ATTENTE_VALIDATION:
					//RAZ demandes config et propreté
					if (m_setupDegrade.at(ligne - 1)) { //Si mode sans Raypro
						if (sVarEqp.spvWAT->GetValidCampagne(ligne) == 1) {
							LOG_INFO << "Campagne validée et lancée";
							sVarEqp.spvWAT->SetEtatCampagne(ligne, "Validation par l'opérateur, démarrage des équipements"); // Ajout 18/03/2020
							//Reset validation campagne
							sVarEqp.spvWAT->ResetValidCampagne(ligne);
							
							//Définir vecteur d'équipents en marche
							/*
							Ajout 03/12/19
							*/
							GetVectorModeDistant(ligne);

							//Enregistrement des données
							CreateFichierVModeDistant(ligne, vModeDistant[ligne - 1]);

							//Passer à l'étape suivante
							vNumEtape.at(ligne - 1)->WriteVar(PROD_ENVOI_DEPART_CYCLE);
							CreateFichierEtape(ligne, PROD_ENVOI_DEPART_CYCLE);
						}
					}
					else { //Si mode avec Raypro
						if (sVarEqp.mesRayPro->GetValidCampagne(ligne) == 1) {
							LOG_INFO << "Campagne validée et lancée";
							sVarEqp.spvWAT->SetEtatCampagne(ligne, "Validation du RAYPRO, démarrage des équipements"); // Ajout 18/03/2020
							//Répondre à le demande de start
							sVarEqp.mesRayPro->SetValidCampagne(ligne);
														
							//Définir vecteur d'équipents en marche
							/*
							Ajout 03/12/19
							*/
							GetVectorModeDistant(ligne);

							//Enregistrement des données
							CreateFichierVModeDistant(ligne, vModeDistant[ligne - 1]);

							//Passer à l'étape suivante
							vNumEtape.at(ligne - 1)->WriteVar(PROD_ENVOI_DEPART_CYCLE);
							CreateFichierEtape(ligne, PROD_ENVOI_DEPART_CYCLE);
						}
					}
					break;
				case PROD_ENVOI_DEPART_CYCLE:
					if (phaseEnvoiMarche(ligne)) {
						LOG_INFO << "Toutes les machines sont en production";
						sVarEqp.spvWAT->SetEtatCampagne(ligne, "Equipements en production"); // Ajout 18/03/2020
						CI_Configuration(ligne);
						CI_Proprete(ligne);
						//Enregistrer le setup
						vFlatSetup.at(ligne + 1) = vFlatSetup.at(ligne - 1);

						//Passer à l'étape suivante
						vNumEtape.at(ligne - 1)->WriteVar(PROD_SUIVI_DE_PROD);
						CreateFichierEtape(ligne, PROD_SUIVI_DE_PROD);
					}
					break;
				case PROD_SUIVI_DE_PROD:
					//Gestion du nombre de colis palettisés
					/*
					Ajout 18/03/2020
					*/
					if ((sVarEqp.spvWAT->GetDmeFinCampagne(ligne) || (sVarEqp.mesRayPro->GetMesBatchReqStatus(ligne) && (!m_setupDegrade.at(ligne - 1)))) && (sVarEqp.APIpalettiseur->GetNbrPiecePalettisee(ligne) == 0)) {
						sVarEqp.spvWAT->SetBitNbrPieceValide(ligne, true);
					}
					else if ((sVarEqp.spvWAT->GetDmeFinCampagne(ligne) || (sVarEqp.mesRayPro->GetMesBatchReqStatus(ligne) && (!m_setupDegrade.at(ligne - 1)))) && (sVarEqp.APIpalettiseur->GetNbrPiecePalettisee(ligne) > 0)) {
						sVarEqp.spvWAT->SetNbrPieceValide(ligne, true);
					}

					if (sVarEqp.spvWAT->GetNbrPieceValide(ligne)){
						
						LOG_INFO << "Fin de campagne demandée";
						sVarEqp.spvWAT->SetEtatCampagne(ligne, "Fin de campagne demandé, vidange des équipements"); // Ajout 18/03/2020

						/*
						Ajout 17/03/2020
						*/
						if (!m_setupDegrade.at(ligne - 1)) { //Si mode avec Raypro
							//ACK demande fin de campagne
							sVarEqp.mesRayPro->SetPlcBatchReqStatus(ligne, 1);
						}
						//Reset demande fin de campagne
						sVarEqp.spvWAT->ResetDmeFinCampagne(ligne);

						//Passer à l'étape suivante
						vNumEtape.at(ligne - 1)->WriteVar(PROD_MODE_VIDANGE_EQUIPEMENT);
						CreateFichierEtape(ligne, PROD_MODE_VIDANGE_EQUIPEMENT);
					}
					break;
				case PROD_MODE_VIDANGE_EQUIPEMENT:
					if (phaseEnvoiVidange(ligne)) { //Fin de vidange équipement
						LOG_INFO << "Vidange des équipements terminée";
						sVarEqp.spvWAT->SetEtatCampagne(ligne, "Equipements vide, attente des rompus"); // Ajout 18/03/2020

						//Passer à l'étape suivante
						sVarEqp.spvWAT->SetRompuEnCours(ligne, true);
						vNumEtape.at(ligne - 1)->WriteVar(PROD_PHASE_ROMPUS);
						CreateFichierEtape(ligne, PROD_PHASE_ROMPUS);
					}
					break;
				case PROD_PHASE_ROMPUS:
					/*
					Modif 12/12/19
					*/
					if (sVarEqp.APIbalance->GetFinRompus(ligne)) {
						LOG_INFO << "Rompus validés";
						sVarEqp.spvWAT->SetEtatCampagne(ligne, "Rompus validé, vidange du robot bacs bleus"); // Ajout 18/03/2020

						sVarEqp.spvWAT->SetRompuEnCours(ligne, false);
						//Passer à l'étape suivante
						vNumEtape.at(ligne - 1)->WriteVar(PROD_MODE_VIDANGE_ROBOTBACSBLEUS);
						CreateFichierEtape(ligne, PROD_MODE_VIDANGE_ROBOTBACSBLEUS);
					}
					break;
				case PROD_MODE_VIDANGE_ROBOTBACSBLEUS:
					/*
					Ajout 12/12/19
					*/
					if (phaseEnvoiVidange(ligne)) { //Fin de vidange robot bacs bleus
						LOG_INFO << "Vidange Robot Bacs Bleus terminée";
						sVarEqp.spvWAT->SetEtatCampagne(ligne, "Robot bacs bleus vide, arret des équipements"); // Ajout 18/03/2020
						//Passer à l'étape suivante
						vNumEtape.at(ligne - 1)->WriteVar(PROD_ARRET_MACHINES);
						CreateFichierEtape(ligne, PROD_ARRET_MACHINES);
					}
					break;
				case PROD_ARRET_MACHINES:
					if (this->phaseArret(ligne)) {
						LOG_INFO << "Arret machine OK";
						sVarEqp.spvWAT->SetEtatCampagne(ligne, "Equipements arretés, Remise en Condition Initiale"); // Ajout 18/03/2020
						sVarEqp.APIbalance->ResetFinRompus(ligne);

						if (!m_setupDegrade.at(ligne - 1)) { //Si mode avec Raypro
							//ACK demande fin : passer à 2
							sVarEqp.mesRayPro->SetPlcBatchReqStatus(ligne, 2);
						}

						//Passer à l'étape suivante
						vNumEtape.at(ligne - 1)->WriteVar(PROD_CAMPAGNE_TERMINEE);
						CreateFichierEtape(ligne, PROD_CAMPAGNE_TERMINEE);
					}
					break;
				case PROD_CAMPAGNE_TERMINEE:
					if (!m_setupDegrade.at(ligne - 1)) { //Si mode avec Raypro
						if (!sVarEqp.mesRayPro->GetMesBatchReqStatus(ligne)) { // Si le RAYPRO est arreté
							
							//ACK demande RAYPRO
							sVarEqp.mesRayPro->SetPlcBatchReqStatus(ligne, 0);

							//RAZ en CI
							CI_Arret(ligne);
							CI_Configuration(ligne);
							CI_Proprete(ligne);
							CI_Vidange(ligne);
							CI_Campagne(ligne);
							CI_Vector(ligne);
							CI_CommMes(ligne);
							CI_Production(ligne);
							CI_SPV(ligne);
							CI_ModeDistant(ligne);

							//Flag mode Distant Robot Bacs Bleus
							sVarEqp.APIrobotCaisses->SetFlagModeDistant(ligne, false);

							/*
							Ajout 17/03/2020
							*/
							//Reset validation tarre
							sVarEqp.APIbalance->ResetValidTarre(ligne);

							//Remise a zero choix formeuse
							sVarEqp.spvWAT->SetChoixFormeuse(ligne, 0);
							sVarEqp.APItransitique->SetDerogationIntroManu(ligne, false); // Ajout 06/02/2020


							/*
							Ajout 26/12/19
							*/
							if (ligne == 1) {
								//Remise a zero du défaut de coherence
								sVarEqp.APIpeseuseL1->ResetCoherenceSetup();
							}
							else
							{
								//Remise a zero du défaut de coherence
								sVarEqp.APIpeseuseL2->ResetCoherenceSetup();
							}
							
							LOG_INFO << "Campagne terminée. En attente de nouvelle campagne";
							sVarEqp.spvWAT->SetEtatCampagne(ligne, "Campagne terminée, attente d'un nouveau setup"); // Ajout 18/03/2020

							//Passer à l'étape suivante
							vNumEtape.at(ligne - 1)->WriteVar(PROD_ATTENTE_SETUP);
							CreateFichierEtape(ligne, PROD_ATTENTE_SETUP);
						}
					}
					else { //Si mode sans Raypro
						sVarEqp.spvWAT->ResetSetupLocal(ligne);
						CI_Arret(ligne);
						CI_Configuration(ligne);
						CI_Proprete(ligne);
						CI_Vidange(ligne);
						CI_Campagne(ligne);
						CI_Vector(ligne);
						CI_CommMes(ligne);
						CI_Production(ligne);
						CI_SPV(ligne);
						CI_ModeDistant(ligne);

						//Flag mode Distant Robot Bacs Bleus
						sVarEqp.APIrobotCaisses->SetFlagModeDistant(ligne, false);

						/*
						Ajout 17/03/2020
						*/
						//Reset validation tarre
						sVarEqp.APIbalance->ResetValidTarre(ligne);
						
						//Remise a zero choix formeuse
						sVarEqp.spvWAT->SetChoixFormeuse(ligne, 0);
						sVarEqp.APItransitique->SetDerogationIntroManu(ligne, false); // Ajout 06/02/2020

						/*
						Ajout 26/12/19
						*/
						if (ligne == 1) {
							//Remise a zero du défaut de coherence
							sVarEqp.APIpeseuseL1->ResetCoherenceSetup();
						}
						else
						{
							//Remise a zero du défaut de coherence
							sVarEqp.APIpeseuseL2->ResetCoherenceSetup();
						}
						LOG_INFO << "Campagne terminée. En attente de nouvelle campagne";
						//Passer à l'étape suivante
						vNumEtape.at(ligne - 1)->WriteVar(PROD_ATTENTE_SETUP);
						CreateFichierEtape(ligne, PROD_ATTENTE_SETUP);
					}
					break;
				default:

					break;
				}
				
				//FIN G7

				//Vérification reset demande start MES
				if (!sVarEqp.mesRayPro->GetValidCampagne(ligne)) {
					//Répondre à le demande de start
					sVarEqp.mesRayPro->ResetValidCampagne(ligne);
				}

				//Vérification synchro time
				sVarEqp.mesRayPro->SetSyncTime(ligne);
								
				//Prise en compte des setups envoyés à la volée	:
				//pas en attente setup + messyncSetup à 1
				//Ajout étape CAMPAGNE_TERMINEE sinon on glisse et tombe dans une boucle infinie
				/*
				Modif 17/03/2020
				Modif 18/03/2020
				*/
				if (vNumEtape.at(ligne - 1)->GetVarValue() != PROD_ATTENTE_SETUP && 
						vNumEtape.at(ligne - 1)->GetVarValue() != PROD_CAMPAGNE_TERMINEE && 
						sVarEqp.mesRayPro->GetMesSyncSetup(ligne) > 0) {
					LOG_TRACE << "Nouveau setup envoyé par RAYPRO, attente -> arret de la ligne " << ligne;
					//Vérification d'un Setup dupliqué du MES : synchro 
					if (isSetupDuplicate(ligne)) {
						LOG_TRACE << "Setup récupéré sur la ligne " << ligne;
						//Lancer échange fictif
						if (ligne == 1) {
							if (mutComDummySetup1.try_lock()) {
								mutComDummySetup1.unlock();
								boost::thread* m_tG7ComDummySetup1 = new boost::thread(boost::bind(&Sequence::ComDummySetup, this, ligne));
							}
						}
						else {
							if (mutComDummySetup2.try_lock()) {
								mutComDummySetup2.unlock();
								boost::thread* m_tG7ComDummySetup2 = new boost::thread(boost::bind(&Sequence::ComDummySetup, this, ligne));
							}
						}
					}
					//Arret campgne en cours sur nouveau setup
					if (!isSetupDuplicate(ligne) && (!sVarEqp.spvWAT->GetspvNouveauSetup(ligne))) {
						LOG_TRACE << "Nouveau setup, arret de la campagne en cours sur la ligne " << ligne;
						sVarEqp.spvWAT->SetEtatCampagne(ligne, "Nouveau setup, arret de la campagne en cours"); // Ajout 18/03/2020

						sVarEqp.spvWAT->SetspvNouveauSetup(ligne, true);

						if (sVarEqp.APIbalance->GetValidTarre(ligne)) {
							JumpVidange(ligne);
						}
						//Si tare pas faite : fin campagne
						else {
							JumpArretMachine(ligne);
						}
					}
				}

				//Vérification demande mode dégradé
				if (sVarEqp.spvWAT->GetPassageDegrade(ligne)) {
					m_setupDegrade.at(ligne - 1) = true;
					//Transfert données Setup distant vers Setup local
					sVarEqp.spvWAT->SetSetup(ligne, vFlatSetup.at(ligne - 1));
					//Reset bouton
					sVarEqp.spvWAT->ResetPassageDegrade(ligne);
					sVarEqp.spvWAT->SetCampagneDegrade(ligne, 1);
				}

				//Vérification demande mode avec RAYPRO si hors prod
				if (vNumEtape.at(ligne - 1)->GetVarValue() == PROD_ATTENTE_SETUP && 
						sVarEqp.spvWAT->GetPassageDistant(ligne)) {
					m_setupDegrade.at(ligne - 1) = false;
					//Reset bouton
					sVarEqp.spvWAT->ResetPassageDistant(ligne);
					sVarEqp.spvWAT->SetCampagneDegrade(ligne, 0);
				}

				//Vérification annulation setup depuis SCADA en dégradé
				if (m_setupDegrade.at(ligne - 1) && (vNumEtape.at(ligne - 1)->GetVarValue() < PROD_ENVOI_DEPART_CYCLE) && sVarEqp.spvWAT->GetAnnulerCampagne(ligne)) {
					LOG_INFO << "Annulation campagne ligne " << ligne << " en dégradé";
					//Si tare faite : rompus
					if (sVarEqp.APIbalance->GetValidTarre(ligne)) {
						JumpVidange(ligne);
					}
					//Si tare pas faite : fin campagne
					else {
						JumpArretMachine(ligne);
					}
					//Reset bouton
					sVarEqp.spvWAT->ResetAnnulerCampagne(ligne);
				}

				//Vérification reset bac versé
				if (sVarEqp.APIrobotCaisses->GetTopBacVerse(ligne) == false && sVarEqp.APIrobotCaisses->GetAckBacVerse(ligne) == true) {
					//Fin de l'échange, reset ACK
					LOG_TRACE << "Reset ACK bac versé";
					sVarEqp.APIrobotCaisses->ResetAckBacVerse(ligne);
				}

				//Verification mode distant
				/*
				Ajout 10/12/19
				*/
				ScrutationModeDistant(ligne);

				VerificationModeDistant(ligne);

				//Passage en introduction manuel
				/*
				Ajout 06/02/2020
				*/
				if (sVarEqp.APItransitique->GetDerogationIntroManu(ligne)) {
					LOG_INFO << "Introduction manuel sur la ligne " << ligne;
					sVarEqp.APItransitique->SetLigneFormeuse(3, ligne);

					//Sortie de la formeuse
					SortieFormeuseProprete(ligne);
					SortieFormeuseConfiguration(ligne);
					SortieFormeuseVidange(ligne);
				}

				//Gestion du voyant de la tarre
				/*
				Ajout24/03/2020
				*/
				if (sVarEqp.APIrobotCaisses->GetModeDistant()) {
					sVarEqp.APIbalance->SetVoyantBalanceTarre(ligne, sVarEqp.APIrobotCaisses->GetDemandeArticle(ligne));
				}
				else {
					sVarEqp.APIbalance->SetVoyantBalanceTarre(ligne, sVarEqp.APIbalance->GetDLLVoyantTarre(ligne));
				}
			}
			//FIN BOUCLE PAR LIGNE


			//Gestion des référence de conditionnement
			/*
			Ajout 19/03/2020
			*/
			if (sVarEqp.spvWAT->GetActualisationRefConditionnement()) {

				sVarEqp.spvWAT->SetActualisationRefConditionnement(false);

				vConditionnement_Ref.clear();
				vConditionnement_Type.clear();
				vConditionnement_Coiffe.clear();

				ImportFichierConditionnement();
			}

			//Lancer thread HU Input
			// HU Ligne 1
			//Si bac présent dans table et pas de com en cours
			if (vHUInputL1.size() > 0 && mutHUL1.try_lock()) {
				//Purger les bacs trop vieux
				for (unsigned int i = vHUInputL1.size(); i > 0; i--) {
					unsigned short j = i - 1;
					if (
						((now.time_of_day().hours() - vHUInputL1.at(j).hour) > 2
							|| (now.time_of_day().hours() - vHUInputL1.at(j).hour) < 0)
						|| (vFlatSetup.at(0).numOF != 0 && (vFlatSetup.at(0).numOF != vHUInputL1.at(j).numOF))
						) {
						vHUInputL1.erase(vHUInputL1.begin() + j); // Suppression HU du vecteur
						MajFichier(1, vHUInputL1); // Mise a jour du fichier
					}
				}

				//Lancer new thread
				LOG_INFO << "Lancement HU L1";
				mutHUL1.unlock();
				if (vHUInputL1.size() > 0) {
					LOG_INFO << "Mutex mutHUL1 unlock pour purge HU";
					boost::thread* m_tG7ComHUInputL1 = new boost::thread(boost::bind(&Sequence::GetComHU, this, 1));
				}

			}
			// HU Ligne 2
			/*
			Modif 21/02/2020
			Modif 25/02/2020
			*/
			//Si bac présent dans table et pas de com en cours
			if (vHUInputL2.size() > 0 && mutHUL2.try_lock()) {
				//Purger les bacs trop vieux
				for (unsigned int i = vHUInputL2.size(); i > 0; i--) {
					unsigned short j = i - 1;
					if (
						((now.time_of_day().hours() - vHUInputL2.at(j).hour) > 2
							|| (now.time_of_day().hours() - vHUInputL2.at(j).hour) < 0)
						|| (vFlatSetup.at(0).numOF != 0 && (vFlatSetup.at(0).numOF != vHUInputL2.at(j).numOF))
						) {
						vHUInputL2.erase(vHUInputL2.begin() + j); // Suppression HU du vecteur
						MajFichier(2, vHUInputL2); // Mise a jour du fichier
					}
				}

				//Lancer new thread
				LOG_INFO << "Lancement HU L2";
				mutHUL2.unlock();
				if (vHUInputL2.size() > 0) {
					LOG_INFO << "Mutex mutHUL2 unlock pour purge HU";
					boost::thread* m_tG7ComHUInputL2 = new boost::thread(boost::bind(&Sequence::GetComHU, this, 2));
				}

			}

			//Sleep entre scrutations
			boost::this_thread::sleep(boost::posix_time::milliseconds(m_delay));
			//incrément du compteur de secondes
			m_countSec = (m_countSec + 1) % 60;
			//On en profite pour mettre à jour de bit de vie toutes les 5 secondes
			if (m_countSec % 5 == 0) {
				for (int ligne = 1; ligne <= 2; ligne++){
					m_etatComMES.at(ligne - 1) = this->BitDeVie(ligne, m_etatComMES.at(ligne - 1));
					//Passage en mode dégradé après timeout
					//22/07 -> pas de mode dégradé automatique.
					if (m_etatComMES.at(ligne - 1) > 5) {
						sVarEqp.spvWAT->SetEtatDegrade(ligne, true);
						/* Mode dégradé simplement pour le défaut SPV ^
						//Je copie aussi direct dans la valeur locale pour passer l'OF en cours en dégradé
						m_setupDegrade.at(ligne - 1) = true;
						*/
						//Copie mémoire pour reprise
						vMemEtapeDegrade.at(ligne - 1) = (int)vNumEtape.at(ligne - 1)->GetVarValue();
					}
				}
			}
			//Action toutes les minutes
			if (m_countSec == 0) {
				sVarEqp.spvWAT->UpdateCalculMoyenneBac();
			}

			//Rompu en cours
			/*
			Ajout 11/02/2020
			*/
			sVarEqp.spvWAT->SetRompuEnCoursResumer(sVarEqp.spvWAT->GetRompuEnCours(1) || sVarEqp.spvWAT->GetRompuEnCours(2));
			
			//Mot de vie équipement
			sVarEqp.APIrobotCaisses->MotDeVieAPI();
			sVarEqp.APIrobotCouvercle->MotDeVieAPI();
			sVarEqp.APIensacheuseL1->MotDeVieAPI();
			sVarEqp.APIensacheuseL2->MotDeVieAPI();
			sVarEqp.APIpeseuseL1->MotDeVieAPI();
			sVarEqp.APIpeseuseL2->MotDeVieAPI();
			sVarEqp.APItransitique->MotDeVieAPI();
			sVarEqp.APIpalettiseur->MotDeVieAPI();
			sVarEqp.APIformeuse1->MotDeVieAPI();
			sVarEqp.APIformeuse2->MotDeVieAPI();
			//Ces mots de vie doivent être liés aux variables de la transitique
			sVarEqp.APIetiqueteuse->MotDeVieAPI();
			sVarEqp.APIimprimante->MotDeVieAPI();
			sVarEqp.APIfermeuse->MotDeVieAPI();

			//MAJ Etat des équipements
			sVarEqp.APIrobotCaisses->SetMotEtat();
			sVarEqp.APIrobotCouvercle->SetMotEtat();
			sVarEqp.APIensacheuseL1->SetMotEtat();
			sVarEqp.APIensacheuseL2->SetMotEtat();
			sVarEqp.APIpeseuseL1->SetMotEtat();
			sVarEqp.APIpeseuseL2->SetMotEtat();
			sVarEqp.APItransitique->SetMotEtat();
			sVarEqp.APIpalettiseur->SetMotEtat();
			sVarEqp.APIetiqueteuse->SetMotEtat();
			sVarEqp.APIimprimante->SetMotEtat();
			sVarEqp.APIfermeuse->SetMotEtat();
			sVarEqp.APIformeuse1->SetMotEtat();
			sVarEqp.APIformeuse2->SetMotEtat();
		}
		catch (boost::thread_interrupted&)
		{
			//Thread interrupted
			return;
		}
		catch (const std::out_of_range&) {
			LOG_ERROR << "Vector out of range";
			//Libérer les mutex sinon on reste bloqué
			mutHUL1.unlock();
			mutHUL2.unlock();
			mutAlarmeL1.unlock();
			mutAlarmeL2.unlock();
			mutComDummySetup1.unlock();
			mutComDummySetup2.unlock();
		}
	}	
}

//Check datas 
int Sequence::GetComSetup(int ligne) {
	if (sVarEqp.mesRayPro->GetspvSyncSetup(ligne) != 0) {
		vCptComSetup[ligne - 1] ++;
	}
	bool passageEtape = false;
	//LOG_DEBUG << "(L" << ligne << ") Lancement cycle COM SETUP";
	switch (vNumSequComSetup.at(ligne - 1))	{
		case COM_SETUP_ETAT_INITIAL:
			//Attente mesSync > 0
			if(sVarEqp.mesRayPro->GetMesSyncSetup(ligne) > 0){
				LOG_TRACE << "(L" << ligne << ") [COM SETUP] mesSyncSetup >0 : lecture du setup";
				//Ack de la spv
				sVarEqp.mesRayPro->SetspvSyncSetup(ligne, -(sVarEqp.mesRayPro->GetMesSyncSetup(ligne)));
				vNumSequComSetup.at(ligne - 1) = COM_SETUP_LECTURE_SETUP;
			}
			break;
		case COM_SETUP_LECTURE_SETUP:
			//lire le setup
			if (sVarEqp.mesRayPro->GetspvSyncSetup(ligne) < 0 && this->GetMesSetup(ligne)){
			//if (sVarEqp.spvWAT->GetFatBtnManu() == 1 && sVarEqp.mesRayPro->GetspvSyncSetup(ligne) < 0 && this->GetMesSetup(ligne)){
			//	sVarEqp.spvWAT->SetFatBtnManu(0);
				LOG_TRACE << "(L" << ligne << ") [COM SETUP] Setup lu : en attente du RAZ mesSync";
				sVarEqp.mesRayPro->SetspvSyncSetup(ligne, -(sVarEqp.mesRayPro->GetspvSyncSetup(ligne)));
				vNumSequComSetup.at(ligne - 1) = COM_SETUP_ATTENTE_RAZ_MES;
			}
			else{
				sVarEqp.mesRayPro->SetspvSyncSetup(ligne, -(sVarEqp.mesRayPro->GetMesSyncSetup(ligne)));
			}
			break;
		case COM_SETUP_ATTENTE_RAZ_MES:
			if (sVarEqp.mesRayPro->GetMesSyncSetup(ligne) == 0) {
				LOG_TRACE << "(L" << ligne << ") [COM SETUP] mesSyncSetup = 0";
				LOG_TRACE << "(L" << ligne << ") [COM SETUP] RAZ du plcSyncSetup";
				sVarEqp.mesRayPro->SetspvSyncSetup(ligne, 0);
				vNumSequComSetup.at(ligne - 1) = COM_SETUP_RAZ_SPV;
				vCptComSetup[ligne - 1] = 0;
			}
			break;
		case COM_SETUP_RAZ_SPV:
			if (sVarEqp.mesRayPro->GetspvSyncSetup(ligne) == 0) {
				LOG_TRACE << "(L" << ligne << ") [COM SETUP] RAZ du plcSyncSetup";
				sVarEqp.mesRayPro->SetspvSyncSetup(ligne, 0);
				
			}
			break;
		default:

			break;
	}
	//Test timeout (Rajouter si besoin)
	if (vCptComSetup[ligne - 1] > 15) {
		LOG_WARN << "Timeout COM Setup : RAZ Synchro";
		vCptComSetup[ligne - 1] = 0;
		vNumSequComSetup.at(ligne - 1) = COM_SETUP_ETAT_INITIAL;
	}
	return vNumSequComSetup.at(ligne - 1);
}

int Sequence::GetComFinCampagne(int ligne)
{
	/*
	NON UTILISEE
	*/
	switch (vNumSequComFinCampagne.at(ligne - 1))
	{
	case COM_FIN_CAMPAGNE_ETAT_INITIAL:
		//Attente mesBatch > 0
		if (sVarEqp.mesRayPro->GetMesBatchReqStatus(ligne)) {
			//Ack de la spv
			this->phaseEnvoiVidange(ligne);
			vNumSequComSetup.at(ligne - 1) = COM_FIN_CAMPAGNE_DME_VIDANGE;
		}
		break;
	case COM_FIN_CAMPAGNE_DME_VIDANGE:
		//Si vidange et rompus terminés
		if (this->phaseEnvoiVidange(ligne) && sVarEqp.APIbalance->GetFinRompus(ligne))
		{
			sVarEqp.mesRayPro->SetValidFinCampagne(ligne);
			vNumSequComSetup.at(ligne - 1) = COM_FIN_CAMPAGNE_RAZ_SEQUENCE;
		}
		break;
	case COM_FIN_CAMPAGNE_RAZ_SEQUENCE:
		if (sVarEqp.mesRayPro->GetMesBatchReqStatus(ligne) == false) {
			sVarEqp.mesRayPro->ResetValidFinCampagne(ligne);
			vNumSequComSetup.at(ligne - 1) = COM_FIN_CAMPAGNE_ETAT_INITIAL;
		}
		break;
	default:

		break;
	}
	return vNumSequComFinCampagne.at(ligne - 1);
}

int Sequence::BitDeVie(int ligne, int cptTries)
{	//Comparer le compteur lu RayPro 

	double tempRaypro = sVarEqp.mesRayPro->GetRayProCounter(ligne);

	if (tempRaypro != this->m_rayproCounterMem.at(ligne - 1)) {
		//MAJ mémoire
		this->m_rayproCounterMem.at(ligne - 1) = tempRaypro;
		//Reset le compteur
		cptTries = 0;
		sVarEqp.spvWAT->SetEtatDegrade(ligne, false);
		/*if (sVarEqp.mesRayPro->GetMesSyncSetup(ligne) == 0 && vMemEtapeDegrade.at(ligne - 1) == vNumEtape.at(ligne - 1)->GetVarValue())
		{
			sVarEqp.spvWAT->SetEtatDegrade(ligne, false);
			//Pour reprendre en mode distant à la volée
			m_setupDegrade.at(ligne - 1) = false;
		}*/
	}
	else {
		cptTries++;
	}
	//incrémenter le compteur plc
	sVarEqp.mesRayPro->IncrementBitDeViePLC(ligne);

	return cptTries;
}

bool Sequence::phaseConfig(int ligne) {
	bool result = false;
	bool lignePropre = true;			//Passe à 0 si un equipement n'est pas propre	
	bool ligneConfiguree = true;		//Passe à 0 si un equipement n'est pas configuré
	unsigned int i = 0;

	std::vector<Equipement*>::iterator it;
	//Pour chaque equipement, lancement de la fonction propreté
	for (i = 0; i < vPropreteCampagne[ligne -1].size(); i++) {
		//Si en mode distant
		if (vPropreteCampagne[ligne - 1].at(i)->GetModeDistant()) {
			lignePropre = vPropreteCampagne[ligne - 1].at(i)->GestionProprete(ligne) && lignePropre;
		}
		else { // Equipement dans la liste propreté en local. 
		}
	}
	//Pour chaque equipement, lancement de la fonction configuration
	for (i = 0; i < vConfigCampagne[ligne -1].size(); i++) {
		//Si en mode distant
		if (vConfigCampagne[ligne - 1].at(i)->GetModeDistant()) {
			//Si demande de propreté pour l'équipement alors attendre que celui ci soit propre
			it = std::find(vPropreteCampagne[ligne - 1].begin(), vPropreteCampagne[ligne - 1].end(), vConfigCampagne[ligne - 1].at(i));
			if (it != vPropreteCampagne[ligne - 1].end()) {
				if ((*it)->GestionProprete(ligne)) {
					ligneConfiguree = vConfigCampagne[ligne - 1].at(i)->GestionConfig(ligne) && ligneConfiguree;
				}
			}
			else {
				//Config de l'équipement en cours
				ligneConfiguree = vConfigCampagne[ligne - 1].at(i)->GestionConfig(ligne) && ligneConfiguree;
			}
		}
		else { // Equipement dans la liste config en local.
		}
	}
	//Vérification de la validation tare
	ligneConfiguree = sVarEqp.APIbalance->GetValidTarre(ligne) && ligneConfiguree;

	//Retourne 1 si tout propre et configuré, sinon 0
	result = lignePropre && ligneConfiguree;
	return result;
}

//Gestion des demandes au robot bacs bleus - Demande de proprete - demande de configuration - demande de marche cellule et ligne
bool Sequence::phaseConfigRobot(sFlatSetup oldSetup, sFlatSetup setup, int ligne) {
	/*
	Modif 11/12/19
	*/
	//Passe à 0 si un equipement n'est pas propre
	bool lignePropre = true;
	//Passe à 0 si un equipement n'est pas configuré
	bool ligneConfiguree = true;
	unsigned int i = 0;

	//Si distant, on demande propreté et config
	if (sVarEqp.APIrobotCaisses->GetModeDistant()) {
		//Demarrage de la cellule Robot Bacs Bleus
		sVarEqp.APIrobotCaisses->EnvoiMarcheCellule();
		LOG_INFO << "Cellule Robot Bacs Bleus demarrée";

		//Si même article, que demande de config config
		if (vFlatSetup.at(ligne + 1).refPieces == vFlatSetup.at(ligne - 1).refPieces) {
			lignePropre = true;
			ligneConfiguree = sVarEqp.APIrobotCaisses->GestionConfig(ligne) && sVarEqp.APIrobotCaisses->EnvoiDemMarche(ligne) && ligneConfiguree;
		}
		else {
			//Lancement de la fonction propreté / configuration
			if (lignePropre = sVarEqp.APIrobotCaisses->GestionProprete(ligne) && lignePropre) {
				ligneConfiguree = sVarEqp.APIrobotCaisses->GestionConfig(ligne) && sVarEqp.APIrobotCaisses->EnvoiDemMarche(ligne) && ligneConfiguree;
			}
		}	
	}
	else
	{
		lignePropre = true;
		ligneConfiguree = true;
	}
	//Retourne 1 si tout propre et configuré, sinon 0
	return lignePropre && ligneConfiguree;
}

//Envoie demande de marche
bool Sequence::phaseEnvoiMarche(int ligne) {
	/*
	Modif 10/12/19
	*/
	bool ligneEnMarche = true;			//Passe à 0 si un equipement n'est pas en marche	
	unsigned int i = 0;

	/* Modif 10/12/19
	for (i = 0; i < vMarcheCampagne[ligne - 1].size(); i++) {
		if (vMarcheCampagne[ligne - 1].at(i)->GetModeDistant()) {
			ligneEnMarche = vMarcheCampagne[ligne - 1].at(i)->EnvoiDemMarche(ligne) && ligneEnMarche;
		}
	}
	*/
	for (i = 0; i < vModeDistant[ligne - 1].size(); i++) {
		ligneEnMarche = vModeDistant[ligne - 1].at(i)->EnvoiDemMarche(ligne) && ligneEnMarche;
	}
	return ligneEnMarche; //Retourne 1 si tout en marche, sinon 0
}

bool Sequence::phaseEnvoiVidange(int ligne) {
	/*
	Modif 10/12/19
	*/

	bool ligneEnVidange = true;			//Passe à 0 si un equipement n'est pas vidangé	
	unsigned int i = 0;
	bool ValideTare = false;

	for (i = 0; i < vModeDistant[ligne - 1].size(); i++) {
		ligneEnVidange = vModeDistant[ligne - 1].at(i)->EnvoiDemVidange(ligne, sVarEqp.APIbalance->GetFinRompus(ligne)) && ligneEnVidange;
	}

	//Retourne 1 si tout vidangé, sinon 0
	return ligneEnVidange;
}

bool Sequence::phaseArret(int ligne) {
	/*
	Modif 10/12/19
	Modif 25/02/2020
	*/
	bool result = true;
	unsigned int i = 0;

	LOG_INFO << "Ligne " << ligne << " en Arret";

	//Pour chaque equipement, lancement arret 
	for (i = 0; i < vModeDistant[ligne - 1].size(); i++) {
		result = vModeDistant[ligne - 1].at(i)->EnvoiDmeArret(ligne) && result;
	}

	return result; //Retourne 1 si ok

}

Sequence::~Sequence(void)
{
}

bool Sequence::GetMesSetup(int ligne) {
	bool retour = false;

	vFlatSetup.at(ligne - 1) = sVarEqp.mesRayPro->GetMesSetup(ligne);
	LOG_INFO << "Setup distant récupéré";

	//Remise à zero de la prise en charge du nouveau setup
	sVarEqp.spvWAT->SetspvNouveauSetup(ligne, false);
	
	LOG_INFO << "Setup distant OK";
	retour = true;

	return retour;
}

void Sequence::GetComAlarme(int id, double num, bool val, int ligne) {
	int cpt = 0;
	int end = 0;
	if (ligne == 1) {
		mutAlarmeL1.lock();
	}
	else
	{
		mutAlarmeL2.lock();
	}
	while (end == 0) {
		cpt++;
		try {
			
			switch (this->vNumSequComAlarme.at(ligne - 1))	{
			case COM_ALARME_ETAT_INITIAL:
				//Attente mesSync = 0
				if (sVarEqp.mesRayPro->GetMesSyncStoppage(ligne) == 0) {
					LOG_DEBUG << "Tech Stop : COM_ALARME_ETAT_INITIAL -> COM_ALARME_ECRITURE";
					//Ack de la spv
					sVarEqp.mesRayPro->SetspvSyncStoppage(ligne, (sVarEqp.mesRayPro->GetspvSyncStoppage(ligne) - 1));
					vNumSequComAlarme.at(ligne - 1) = COM_ALARME_ECRITURE;
				}
				break;
			case COM_ALARME_ECRITURE:
				//Ecrit les données
				if (sVarEqp.mesRayPro->SetStoppageDatas(id, num, val)) {
					LOG_DEBUG << "Tech Stop : COM_ALARME_ECRITURE -> COM_ALARME_RAZ_SEQUENCE";
					sVarEqp.mesRayPro->SetspvSyncStoppage(ligne , -(sVarEqp.mesRayPro->GetspvSyncStoppage(ligne)));
					vNumSequComAlarme.at(ligne - 1) = COM_ALARME_RAZ_SEQUENCE;
				}
				else {
					LOG_DEBUG << "Tech Stop : COM_ALARME_ECRITURE -> Rebouclage";
					sVarEqp.mesRayPro->SetspvSyncStoppage(ligne, (sVarEqp.mesRayPro->GetspvSyncStoppage(ligne) - (1)));
				}
				break;
			case COM_ALARME_RAZ_SEQUENCE:
				if (sVarEqp.mesRayPro->GetMesSyncStoppage(ligne) != 0 && (sVarEqp.mesRayPro->GetspvSyncStoppage(ligne) <= sVarEqp.mesRayPro->GetMesSyncStoppage(ligne))) {
					sVarEqp.mesRayPro->SetspvSyncStoppage(ligne, 0);
					LOG_DEBUG << "Tech Stop : COM_ALARME_RAZ_SEQUENCE -> COM_ALARME_ETAT_INITIAL";
					vNumSequComAlarme.at(ligne - 1) = COM_ALARME_ETAT_INITIAL;
					end = 1;
					//Finir le thread
					if (ligne == 1) {
						mutAlarmeL1.unlock();
					}
					else
					{
						mutAlarmeL2.unlock();
					}
					
				}
				break;

			default:

				break;
			}

			//Sleep entre scrutations
			boost::this_thread::sleep(boost::posix_time::milliseconds(m_delay));

			if (cpt >= 250) {
				//Finir le thread
				//m_tG7ComAlarme.interrupt();
				//m_tG7ComAlarme.join();
				LOG_ERROR << "Thread Tech Stop termine car timeout 10 sec";
				vNumSequComAlarme.at(ligne - 1) = COM_ALARME_ETAT_INITIAL;
				sVarEqp.mesRayPro->SetspvSyncStoppage(ligne, 0);
				end = 1;
				break;
				if (ligne == 1) {
					mutAlarmeL1.unlock();
				}
				else
				{
					mutAlarmeL2.unlock();
				}
			}
		}
		catch (boost::thread_interrupted&)	{
			LOG_ERROR << "Thread interrompu";
			//Thread interrupted
			return;
		}

	}
}

bool Sequence::JumpVidange(int ligne) {
	/*
	Modif 04/12/19
	*/
	bool retour = true;

	//Retomber des demandes de proprete et de configuration
	CI_Proprete(ligne);
	CI_Configuration(ligne);

	LOG_INFO << "Jump Vidange : Ligne = " << ligne;
	vNumEtape.at(ligne - 1)->WriteVar(PROD_MODE_VIDANGE_EQUIPEMENT);
	CreateFichierEtape(ligne, PROD_MODE_VIDANGE_EQUIPEMENT);
	//CI_SPV(ligne);

	return retour;
}

bool Sequence::JumpArretMachine(int ligne) {
	bool retour = true;
	//Retomber des demandes de proprete et de configuration
	CI_Proprete(ligne);
	CI_Configuration(ligne);

	//Retomber du voyant de la demande de tare
	sVarEqp.APIbalance->SetDLLVoyantTarre(ligne, false);

	LOG_INFO << "Jump Arret Machine Ligne : " << ligne;
	vNumEtape.at(ligne - 1)->WriteVar(PROD_ARRET_MACHINES);
	CreateFichierEtape(ligne, PROD_ARRET_MACHINES);
	//CI_SPV(ligne);

	return retour;
}

bool Sequence::CI_Vector(int ligne) {
	/*
	Ajout 19/03/2020
	*/
	bool result = true;

	//RAZ Vecteur
	
	vConfigCampagne[ligne - 1].clear();
	vPropreteCampagne[ligne - 1].clear();
	vScrutation[ligne - 1].clear();
	
	return result;
}

bool Sequence::CI_Arret(int ligne) {
	/*
	Ajout 19/03/2020
	*/
	bool result = true;
	unsigned int i = 0;

	for (i = 0; i < vConfigCampagne[ligne - 1].size(); i++) {
		vConfigCampagne[ligne - 1].at(i)->EnvoiDmeArret(ligne);
		vConfigCampagne[ligne - 1].at(i)->SetMachineSortie(ligne, false);
	}

	return result;
}

bool Sequence::CI_Vidange(int ligne) {
	bool result = true;
	unsigned int i = 0;

	//Pour chaque equipement, lancement de la fonction vidange

	for (i = 0; i < vModeDistant[ligne - 1].size(); i++) {
		result = vModeDistant[ligne - 1].at(i)->ResetDemandeVidange(ligne) && result;
	}

	//Retourne 1 si ok
	return result;
}

bool Sequence::CI_Configuration(int ligne) {
	bool result = true;
	unsigned int i = 0;

	//Pour chaque equipement, reset demande1config (+ def config)
	for (i = 0; i < vConfigCampagne[ligne - 1].size(); i++) {
		result = vConfigCampagne[ligne - 1].at(i)->ResetDemandeConfiguration(ligne) && result;
	}

	return result;
}

bool Sequence::CI_Proprete(int ligne) {
	bool result = true;
	unsigned int i = 0;
	//RAZ du robot bac bleu
	sVarEqp.APIrobotCaisses->ResetDemandeProprete(ligne);
	//Pour chaque equipement, lancement de la fonction propreté
	for (i = 0; i < vPropreteCampagne[ligne - 1].size(); i++) {
		result = vPropreteCampagne[ligne - 1].at(i)->ResetDemandeProprete(ligne) && result;
	}
	return result;
}

bool Sequence::CI_CommMes(int ligne){
	vNumSequComSetup.at(ligne - 1) = COM_SETUP_ETAT_INITIAL;
	//m_numSequComAlarme = COM_ALARME_ETAT_INITIAL;
	//RAZ du mode dégradé
	//m_setupDegrade.at(ligne - 1) = false;
	return 1;
}

bool Sequence::CI_Production(int ligne){
	//vNumEtape.at(ligne - 1)->WriteVar(PROD_ATTENTE_SETUP);
	//RAZ formeuse pour ligne Transitique
	sVarEqp.APItransitique->SetLigneFormeuse(0, ligne);
	//NB pieces sorties
	sVarEqp.mesRayPro->SetGoodPartQty(ligne, 0);

	m_setupDegrade.at(ligne - 1) = false;

	return 1;
}

bool Sequence::CI_SPV(int ligne) {
	sVarEqp.spvWAT->ResetValidFinCampagne(ligne);
	sVarEqp.spvWAT->ResetAnnulerCampagne (ligne);
	sVarEqp.spvWAT->ResetValidCampagne(ligne);
	sVarEqp.spvWAT->ResetValidSetupLocal(ligne);
	sVarEqp.spvWAT->ResetDmeFinCampagne(ligne);
	sVarEqp.APIbalance->ResetPoidsTarre(ligne);
	sVarEqp.APIbalance->ResetValidTarre(ligne);
	sVarEqp.spvWAT->SetCampagneDegrade(ligne, 0);
	
	return true;
}

bool Sequence::CI_Scrutation(int ligne) {
	/*
	Ajout 23/03/2020
	Modif 26/03/2020
	*/
	bool result = true;
	unsigned int i = 0;

	for (i = 0; i < vConfigCampagne[ligne - 1].size(); i++) {
		vConfigCampagne[ligne - 1].at(i)->SetModeDistant_OLD(vConfigCampagne[ligne - 1].at(i)->GetModeDistant());
	}

	return true;
}

bool Sequence::CI_Campagne(int ligne) {
	/*
	Ajout 27/03/2020
	*/
	bool result = true;
	unsigned int i = 0;

	for (i = 0; i < vConfigCampagne[ligne - 1].size(); i++) {
		vConfigCampagne[ligne - 1].at(i)->SetCampagne(ligne, false);
	}

	return true;
}

int Sequence::GetAlrmEtape( int ligne) {
	return this->vNumSequComAlarme.at(ligne - 1);
}

int Sequence::GetMemAlmEnCours(int ligne) {
	if (ligne == 1) {
		return m_memAlmEnCoursL1;
	}
	else {
		return m_memAlmEnCoursL2;
	}
	
}

void Sequence::SetMemAlmEnCours(int ligne, int val) {
	if (ligne == 1) {
		m_memAlmEnCoursL1 = val;
	}
	else {
		m_memAlmEnCoursL2 = val;
	}
	
}

void Sequence::GetComHU(int ligne) {

	int cpt = 0;
	int end = 0;

	if (ligne == 1) {
		mutHUL1.lock();
		LOG_DEBUG << "(L" << ligne << ") [HU Input]Mutex mutHUL1 lock pour séquence COM HU";
	}
	else {
		mutHUL2.lock();
		LOG_DEBUG << "(L" << ligne << ") [HU Input]Mutex mutHUL2 lock pour séquence COM HU";
	}

	while (end == 0) {
		cpt++;
		try {
			switch (this->vNumSequComHU.at(ligne - 1)) {
			case COM_HU_ETAT_INITIAL:
				//Attente mesSync = 0
				if (sVarEqp.mesRayPro->GetMesSyncHU(ligne) == 0) {
					LOG_DEBUG << "HU : COM_HU_ETAT_INITIAL -> COM_HU_ECRITURE";
					//Ack de la spv
					sVarEqp.mesRayPro->SetspvSyncHU(ligne, (sVarEqp.mesRayPro->GetspvSyncHU(ligne) - 1));
					vNumSequComHU.at(ligne - 1) = COM_HU_ECRITURE;
				}
				break;
			case COM_HU_ECRITURE:
				//Ecrit les données
				if (ligne == 1) {
					if (sVarEqp.mesRayPro->GetspvSyncHU(ligne) < 0 && sVarEqp.mesRayPro->SetHUDatas(ligne, vHUInputL1.at(0))) {
					//if (sVarEqp.spvWAT->GetFatBtnManu() == 1 && sVarEqp.mesRayPro->GetspvSyncHU(ligne) < 0 && sVarEqp.mesRayPro->SetHUDatas(ligne, vHUInputL1.at(0))) {
					//	sVarEqp.spvWAT->SetFatBtnManu(0);
						LOG_DEBUG << "HU : COM_HU_ECRITURE -> COM_HU_RAZ_SEQUENCE";
						sVarEqp.mesRayPro->SetspvSyncHU(ligne, -(sVarEqp.mesRayPro->GetspvSyncHU(ligne)));
						vNumSequComHU.at(ligne - 1) = COM_HU_RAZ_SEQUENCE;
					}
					else {
						LOG_DEBUG << "HU : COM_HU_ECRITURE -> Rebouclage";
						sVarEqp.mesRayPro->SetspvSyncHU(ligne, (sVarEqp.mesRayPro->GetspvSyncHU(ligne) - (1)));
					}
				}
				else {
					if (sVarEqp.mesRayPro->GetspvSyncHU(ligne) < 0 && sVarEqp.mesRayPro->SetHUDatas(ligne, vHUInputL2.at(0))) {
						LOG_DEBUG << "HU : COM_HU_ECRITURE -> COM_HU_RAZ_SEQUENCE";
						sVarEqp.mesRayPro->SetspvSyncHU(ligne, -(sVarEqp.mesRayPro->GetspvSyncHU(ligne)));
						vNumSequComHU.at(ligne - 1) = COM_HU_RAZ_SEQUENCE;
					}
					else {
						LOG_DEBUG << "HU : COM_HU_ECRITURE -> Rebouclage";
						sVarEqp.mesRayPro->SetspvSyncHU(ligne, (sVarEqp.mesRayPro->GetspvSyncHU(ligne) - (1)));
					}
				}
				break;
			case COM_HU_RAZ_SEQUENCE:
				if (sVarEqp.mesRayPro->GetspvSyncHU(ligne) <= sVarEqp.mesRayPro->GetMesSyncHU(ligne) && sVarEqp.mesRayPro->GetMesSyncHU(ligne) != 0) {
					sVarEqp.mesRayPro->SetspvSyncHU(ligne, 0);
					LOG_DEBUG << "HU : COM_HU_RAZ_SEQUENCE -> COM_HU_ETAT_INITIAL";
					vNumSequComHU.at(ligne - 1) = COM_HU_ETAT_INITIAL;
					end = 1;
					//Finir le thread
					if (ligne == 1) {
						vHUInputL1.erase(vHUInputL1.begin());
						mutHUL1.unlock();
						LOG_DEBUG << "(L" << ligne << ") [HU Input]Mutex mutHUL1 unlock pour séquence COM HU";
						MajFichier(ligne, vHUInputL1);
					}
					else {
						vHUInputL2.erase(vHUInputL2.begin());
						mutHUL2.unlock();
						LOG_DEBUG << "(L" << ligne << ") [HU Input]Mutex mutHUL2 unlock pour séquence COM HU";
						MajFichier(ligne, vHUInputL2);
					}

				}
				break;

			default:

				break;
			}

			//Sleep entre scrutations
			boost::this_thread::sleep(boost::posix_time::milliseconds(m_delay));
			//LOG_DEBUG << "(L" << ligne << ") [HU Input] Tour de cycle ";

			if (cpt >= 300) {
				//Finir le thread
				LOG_DEBUG << "(L" << ligne << ") [HU Input]Thread HU termine car timeout delai";
				sVarEqp.mesRayPro->SetspvSyncHU(ligne, 0);
				vNumSequComHU.at(ligne - 1) = COM_HU_ETAT_INITIAL;
				end = 1;
				if (ligne == 1) {
					mutHUL1.unlock();
					LOG_DEBUG << "(L" << ligne << ") [HU Input]Mutex mutHUL1 unlock pour séquence COM HU (timeout)";
				}
				else {
					mutHUL2.unlock();
					LOG_DEBUG << "(L" << ligne << ") [HU Input]Mutex mutHUL2 unlock pour séquence COM HU (timeout)";
				}
				break;
			}
		}
		catch (boost::thread_interrupted&) {
			LOG_DEBUG << "(L" << ligne << ") [HU Input]Thread interrompu";
			//Thread interrupted
			return;
		}

	}

}

void Sequence::AddHUTable(int ligne, double contID, CString batchID, CString huID, double qte) {

	const boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();

	huInput temp;
	temp.batchID = batchID;
	temp.containerID = contID;
	temp.containerQuantity = qte;
	temp.huID = huID;
	temp.numOF = vFlatSetup.at(ligne -1).numOF ;
	temp.hour =	double(now.time_of_day().hours());
	temp.minute = double(now.time_of_day().minutes());
	temp.second = double(now.time_of_day().seconds());

	if (ligne == 1) {
		//Controler qu'on aie bien des données différentes
		if (!(vHUInputL1.size() >= 1 && vHUInputL1.back().containerID == contID)) {
			vHUInputL1.push_back(temp);
			MajFichier(ligne, vHUInputL1);
		}
	}
	else
	{
		//Controler qu'on aie bien des données différentes
		if (!(vHUInputL2.size() >= 1 && vHUInputL2.back().containerID == contID)) {
			vHUInputL2.push_back(temp);
			MajFichier(ligne, vHUInputL2);
		}
	}
}

void Sequence::MajFichier(int ligne, std::vector<huInput> vec) {

	//Ecrire dans le fichier
	std::ofstream myfile;
	std::string filename = "HUInput/L";
	filename += std::to_string(ligne);
	filename += ".txt";
	myfile.open(filename);

	for (unsigned short i = 0; i < vec.size(); i++) {
		myfile 
			<< to_string_with_precision(vec.at(i).hour, 0) << ";"
			<< to_string_with_precision(vec.at(i).minute, 0) << ";"
			<< to_string_with_precision(vec.at(i).second, 0) << ";"
			<< to_string_with_precision(vec.at(i).huID, 0) << ";"
			<< to_string_with_precision(vec.at(i).batchID, 0) << ";"
			<< to_string_with_precision(vec.at(i).containerID, 0) << ";"
			<< to_string_with_precision(vec.at(i).containerQuantity, 0) << ";"
			<< to_string_with_precision(vec.at(i).numOF, 0)
			<< "\n"
			;
	}
	
	LOG_DEBUG << "Fichier " << filename << " modifié";
	myfile.close();
}

//Créer fichier Import etape Campagne en cours
void Sequence::CreateFichierEtape(int ligne, int etape) {

	//Ecrire dans le fichier
	std::ofstream myfile;
	std::string filename = "Campagne/L";
	filename += std::to_string(ligne);
	filename += "/etape.txt";
	myfile.open(filename, std::ofstream::out | std::ofstream::trunc);
	myfile << to_string_with_precision(etape, 0) << ";";
	
	//LOG_DEBUG << "Fichier " << filename << " modifié";
	myfile.close();
}

//Créer fichier Import setup Campagne en cours
void Sequence::CreateFichierSetup(int ligne, sFlatSetup setup) {

	//Ecrire dans le fichier
	std::ofstream myfile;
	std::string filename = "Campagne/L";
	filename += std::to_string(ligne);
	filename += "/setup.txt";
	myfile.open(filename, std::ofstream::out | std::ofstream::trunc);
	myfile
		<< to_string_with_precision(setup.adjonctionEau, 0) << ";"
		<< to_string_with_precision(setup.descProduit, 0) << ";"
		<< to_string_with_precision(setup.hauteurSachet, 0) << ";"
		<< to_string_with_precision(setup.idOperateur, 0) << ";"
		<< to_string_with_precision(setup.nbPieces, 0) << ";"
		<< to_string_with_precision(setup.nbPiecesPContenant, 0) << ";"
		<< to_string_with_precision(setup.nbPiecesPPalette, 0) << ";"
		<< to_string_with_precision(setup.nbPiecesPSachet, 0) << ";"
		<< to_string_with_precision(setup.numLot, 0) << ";"
		<< to_string_with_precision(setup.numOF, 0) << ";"
		<< to_string_with_precision(setup.poidsPiece, 0) << ";"
		<< to_string_with_precision(setup.provenance, 0) << ";"
		<< to_string_with_precision(setup.refPieces, 0) << ";"
		<< to_string_with_precision(setup.typeConditionnement, 0) << ";";

	LOG_DEBUG << "Fichier " << filename << " modifié";
	myfile.close();
}

//Créer fichier Import vProprete Campagne en cours
void Sequence::CreateFichierVProprete(int ligne, std::vector<Equipement*> vec) {

	//Ecrire dans le fichier
	std::ofstream myfile;
	std::string filename = "Campagne/L";
	filename += std::to_string(ligne);
	filename += "/vproprete.txt";
	myfile.open(filename, std::ofstream::out | std::ofstream::trunc);
	for (unsigned short i = 0; i < vec.size(); i++) {
		if (vec.at(i) == sVarEqp.APIrobotCaisses) {
			myfile
				<< "APIrobotCaisses;"
				;
		}
		if (vec.at(i) == sVarEqp.APIrobotCouvercle) {
			myfile
				<< "APIrobotCouvercle;"
				;
		}
		if (vec.at(i) == sVarEqp.APIensacheuseL1) {
			myfile
				<< "APIensacheuseL1;"
				;
		}
		if (vec.at(i) == sVarEqp.APIensacheuseL2) {
			myfile
				<< "APIensacheuseL2;"
				;
		}
		if (vec.at(i) == sVarEqp.APIpeseuseL1) {
			myfile
				<< "APIpeseuseL1;"
				;
		}
		if (vec.at(i) == sVarEqp.APIpeseuseL2) {
			myfile
				<< "APIpeseuseL2;"
				;
		}
		if (vec.at(i) == sVarEqp.APItransitique) {
			myfile
				<< "APItransitique;"
				;
		}
		if (vec.at(i) == sVarEqp.APIpalettiseur) {
			myfile
				<< "APIpalettiseur;"
				;
		}
		if (vec.at(i) == sVarEqp.APIetiqueteuse) {
			myfile
				<< "APIetiqueteuse;"
				;
		}
		if (vec.at(i) == sVarEqp.APIimprimante) {
			myfile
				<< "APIimprimante;"
				;
		}
		if (vec.at(i) == sVarEqp.APIfermeuse) {
			myfile
				<< "APIfermeuse;"
				;
		}
		if (vec.at(i) == sVarEqp.APIformeuse1) {
			myfile
				<< "APIformeuse1;"
				;
		}
		if (vec.at(i) == sVarEqp.APIformeuse2) {
			myfile
				<< "APIformeuse2;"
				;
		}
		if (vec.at(i) == sVarEqp.Tremie) {
			myfile
				<< "Tremie;"
				;
		}
	}
	LOG_DEBUG << "Fichier " << filename << " modifié";
	myfile.close();
}

//Créer fichier Import vConfig Campagne en cours
void Sequence::CreateFichierVConfig(int ligne, std::vector<Equipement*> vec) {

	//Ecrire dans le fichier
	std::ofstream myfile;
	std::string filename = "Campagne/L";
	filename += std::to_string(ligne);
	filename += "/vconfig.txt";
	myfile.open(filename, std::ofstream::out | std::ofstream::trunc);
	for (unsigned short i = 0; i < vec.size(); i++) {
		if (vec.at(i) == sVarEqp.APIrobotCaisses) {
			myfile
				<< "APIrobotCaisses;"
				;
		}
		if (vec.at(i) == sVarEqp.APIrobotCouvercle) {
			myfile
				<< "APIrobotCouvercle;"
				;
		}
		if (vec.at(i) == sVarEqp.APIensacheuseL1) {
			myfile
				<< "APIensacheuseL1;"
				;
		}
		if (vec.at(i) == sVarEqp.APIensacheuseL2) {
			myfile
				<< "APIensacheuseL2;"
				;
		}
		if (vec.at(i) == sVarEqp.APIpeseuseL1) {
			myfile
				<< "APIpeseuseL1;"
				;
		}
		if (vec.at(i) == sVarEqp.APIpeseuseL2) {
			myfile
				<< "APIpeseuseL2;"
				;
		}
		if (vec.at(i) == sVarEqp.APItransitique) {
			myfile
				<< "APItransitique;"
				;
		}
		if (vec.at(i) == sVarEqp.APIpalettiseur) {
			myfile
				<< "APIpalettiseur;"
				;
		}
		if (vec.at(i) == sVarEqp.APIetiqueteuse) {
			myfile
				<< "APIetiqueteuse;"
				;
		}
		if (vec.at(i) == sVarEqp.APIimprimante) {
			myfile
				<< "APIimprimante;"
				;
		}
		if (vec.at(i) == sVarEqp.APIfermeuse) {
			myfile
				<< "APIfermeuse;"
				;
		}
		if (vec.at(i) == sVarEqp.APIformeuse1) {
			myfile
				<< "APIformeuse1;"
				;
		}
		if (vec.at(i) == sVarEqp.APIformeuse2) {
			myfile
				<< "APIformeuse2;"
				;
		}
		if (vec.at(i) == sVarEqp.Tremie) {
			myfile
				<< "Tremie;"
				;
		}
	}
	LOG_DEBUG << "Fichier " << filename << " modifié";
	myfile.close();
}

void Sequence::ImportFichierHuInput(int ligne) {

	//Ecrire dans le fichier
	std::ifstream myfile;
	std::string filename = "HUInput/L";
	filename += std::to_string(ligne);
	filename += ".txt";
	myfile.open(filename);
	std::vector<std::string> strs;
	huInput temphu;

	LOG_DEBUG << "Contenu du fichier " << filename << "";

	for (std::string line; std::getline(myfile, line); )
	{
		boost::split(strs, line, boost::is_any_of(";"));
		LOG_DEBUG << "strs.size() = " << strs.size() << "";
		if (strs.size() == 8) {
			temphu.hour = std::stoi(strs.at(0));
			temphu.minute = std::stoi(strs.at(1));
			temphu.second = std::stoi(strs.at(2));
			temphu.huID = strs.at(3).c_str();
			temphu.batchID = strs.at(4).c_str();
			temphu.containerID = std::stoi(strs.at(5));
			temphu.containerQuantity = std::stoi(strs.at(6));
			temphu.numOF = std::stoi(strs.at(7));

			LOG_DEBUG << "Ch1 - " << strs.at(0) << " -> " << std::stoi(strs.at(0)) << "";
			LOG_DEBUG << "Ch2 - " << strs.at(1) << " -> " << std::stoi(strs.at(1)) << "";
			LOG_DEBUG << "Ch3 - " << strs.at(2) << " -> " << std::stoi(strs.at(2)) << "";
			LOG_DEBUG << "Ch4 - " << strs.at(3) << " -> " << (strs.at(3)) << "";
			LOG_DEBUG << "Ch5 - " << strs.at(4) << " -> " << (strs.at(4)) << "";
			LOG_DEBUG << "Ch6 - " << strs.at(5) << " -> " << std::stoi(strs.at(5)) << "";
			LOG_DEBUG << "Ch7 - " << strs.at(6) << " -> " << std::stoi(strs.at(6)) << "";
			LOG_DEBUG << "Ch8 - " << strs.at(7) << " -> " << std::stoi(strs.at(7)) << "";

			if (ligne == 1) {
				vHUInputL1.push_back(temphu);
			}
			else {
				vHUInputL2.push_back(temphu);
				
			}
		}
	}

	LOG_DEBUG << "Fichier " << filename << " importé";
	myfile.close();
}

int Sequence::ImportFichierEtape(int ligne) {

	int retour = 0;
	//Ecrire dans le fichier
	std::ifstream myfile;
	std::string filename = "Campagne/L";
	filename += std::to_string(ligne);
	filename += "/etape.txt";
	myfile.open(filename);
	std::vector<std::string> strs;

	for (std::string line; std::getline(myfile, line); )
	{
		boost::split(strs, line, boost::is_any_of(";"));
		if (std::stoi(strs.at(0)) % 5 == 0) {
			vNumEtape.at(ligne - 1)->WriteVar(std::stoi(strs.at(0)));
			retour = std::stoi(strs.at(0));
		}
	}

	LOG_DEBUG << "Fichier " << filename << " importé";
	myfile.close();

	return retour;
}

void Sequence::ImportFichierSetup(int ligne) {

	//Ecrire dans le fichier
	std::ifstream myfile;
	std::string filename = "Campagne/L";
	filename += std::to_string(ligne);
	filename += "/setup.txt";
	myfile.open(filename);
	std::vector<std::string> strs;
	sFlatSetup tempsetup;

	LOG_DEBUG << "Contenu du fichier " << filename << "";

	for (std::string line; std::getline(myfile, line); )
	{
		boost::split(strs, line, boost::is_any_of(";"));
		LOG_DEBUG << "strs.size() = " << strs.size() << "";
		if (strs.size() == 14) {
			tempsetup.adjonctionEau =	(double)std::stoi(strs.at(0));
			tempsetup.descProduit =		strs.at(1).c_str();
			tempsetup.hauteurSachet = (double)std::stoi(strs.at(2));
			tempsetup.idOperateur = (double)std::stoi(strs.at(3));
			tempsetup.nbPieces = (double)std::stoi(strs.at(4));
			tempsetup.nbPiecesPContenant = (double)std::stoi(strs.at(5));
			tempsetup.nbPiecesPPalette = (double)std::stoi(strs.at(6));
			tempsetup.nbPiecesPSachet = (double)std::stoi(strs.at(7));
			tempsetup.numLot = strs.at(8).c_str();
			tempsetup.numOF = (double)std::stoi(strs.at(9));
			tempsetup.poidsPiece = (double)std::stoi(strs.at(10));
			tempsetup.provenance = (bool)std::stoi(strs.at(11));
			tempsetup.refPieces = (double)std::stoi(strs.at(12));
			tempsetup.typeConditionnement = strs.at(13).c_str();


			vFlatSetup.at(ligne - 1) = tempsetup;	
		}
	}

	LOG_DEBUG << "Fichier " << filename << " importé";
	myfile.close();
}

void Sequence::ImportFichierVProprete(int ligne) {

	//Ecrire dans le fichier
	std::ifstream myfile;
	std::string filename = "Campagne/L";
	filename += std::to_string(ligne);
	filename += "/vproprete.txt";
	myfile.open(filename);
	std::vector<std::string> strs;
	sFlatSetup tempsetup;

	for (std::string line; std::getline(myfile, line); )
	{
		boost::split(strs, line, boost::is_any_of(";"));
		for (unsigned short i = 0; i < strs.size(); i++) {
			if (strs.at(i) == "APIrobotCaisses") { vPropreteCampagne->push_back(sVarEqp.APIrobotCaisses); }
			if (strs.at(i) == "APIrobotCouvercle") { vPropreteCampagne->push_back(sVarEqp.APIrobotCouvercle); }
			if (strs.at(i) == "APIensacheuseL1") { vPropreteCampagne->push_back(sVarEqp.APIensacheuseL1); }
			if (strs.at(i) == "APIensacheuseL2") { vPropreteCampagne->push_back(sVarEqp.APIensacheuseL2); }
			if (strs.at(i) == "APIpeseuseL1") { vPropreteCampagne->push_back(sVarEqp.APIpeseuseL1); }
			if (strs.at(i) == "APIpeseuseL2") { vPropreteCampagne->push_back(sVarEqp.APIpeseuseL2); }
			if (strs.at(i) == "APItransitique") { vPropreteCampagne->push_back(sVarEqp.APItransitique); }
			if (strs.at(i) == "APIpalettiseur") { vPropreteCampagne->push_back(sVarEqp.APIpalettiseur); }
			if (strs.at(i) == "APIetiqueteuse") { vPropreteCampagne->push_back(sVarEqp.APIetiqueteuse); }
			if (strs.at(i) == "APIimprimante") { vPropreteCampagne->push_back(sVarEqp.APIimprimante); }
			if (strs.at(i) == "APIfermeuse") { vPropreteCampagne->push_back(sVarEqp.APIfermeuse); }
			if (strs.at(i) == "APIformeuse1") { vPropreteCampagne->push_back(sVarEqp.APIformeuse1); }
			if (strs.at(i) == "APIformeuse2") { vPropreteCampagne->push_back(sVarEqp.APIformeuse2); }
			if (strs.at(i) == "Tremie") { vPropreteCampagne->push_back(sVarEqp.Tremie); }
		}
	}

	LOG_DEBUG << "Fichier " << filename << " importé";
	myfile.close();
}

void Sequence::ImportFichierVConfig(int ligne) {

	//Ecrire dans le fichier
	std::ifstream myfile;
	std::string filename = "Campagne/L";
	filename += std::to_string(ligne);
	filename += "/vconfig.txt";
	myfile.open(filename);
	std::vector<std::string> strs;
	sFlatSetup tempsetup;

	for (std::string line; std::getline(myfile, line); )
	{
		boost::split(strs, line, boost::is_any_of(";"));
		for (unsigned short i = 0; i < strs.size(); i++) {
			if (strs.at(i) == "APIrobotCaisses") { vConfigCampagne->push_back(sVarEqp.APIrobotCaisses); }
			if (strs.at(i) == "APIrobotCouvercle") { vConfigCampagne->push_back(sVarEqp.APIrobotCouvercle); }
			if (strs.at(i) == "APIensacheuseL1") { vConfigCampagne->push_back(sVarEqp.APIensacheuseL1); }
			if (strs.at(i) == "APIensacheuseL2") { vConfigCampagne->push_back(sVarEqp.APIensacheuseL2); }
			if (strs.at(i) == "APIpeseuseL1") { vConfigCampagne->push_back(sVarEqp.APIpeseuseL1); }
			if (strs.at(i) == "APIpeseuseL2") { vConfigCampagne->push_back(sVarEqp.APIpeseuseL2); }
			if (strs.at(i) == "APItransitique") { vConfigCampagne->push_back(sVarEqp.APItransitique); }
			if (strs.at(i) == "APIpalettiseur") { vConfigCampagne->push_back(sVarEqp.APIpalettiseur); }
			if (strs.at(i) == "APIetiqueteuse") { vConfigCampagne->push_back(sVarEqp.APIetiqueteuse); }
			if (strs.at(i) == "APIimprimante") { vConfigCampagne->push_back(sVarEqp.APIimprimante); }
			if (strs.at(i) == "APIfermeuse") { vConfigCampagne->push_back(sVarEqp.APIfermeuse); }
			if (strs.at(i) == "APIformeuse1") { vConfigCampagne->push_back(sVarEqp.APIformeuse1); }
			if (strs.at(i) == "APIformeuse2") { vConfigCampagne->push_back(sVarEqp.APIformeuse2); }
			if (strs.at(i) == "Tremie") { vConfigCampagne->push_back(sVarEqp.Tremie); }
		}
	}

	LOG_DEBUG << "Fichier " << filename << " importé";
	myfile.close();
}

bool Sequence::GetG7FormAsync(int formeuse)
{
	bool ligneConfiguree = false;

	if (formeuse == 1) {
		//On lance une phase config pour la formeuse
		ligneConfiguree = sVarEqp.APIformeuse1->GestionConfig(1); 
		//Retomber config
		if (ligneConfiguree) {
			sVarEqp.APIformeuse1->ResetDemandeConfiguration(1);
		}
	}
	else {
		ligneConfiguree = sVarEqp.APIformeuse2->GestionConfig(1); 
		//Retomber config
		if (ligneConfiguree) {
			sVarEqp.APIformeuse2->ResetDemandeConfiguration(1);
		}
	}

 ;
	return ligneConfiguree;
}

bool Sequence::isSetupDuplicate(int ligne)
{
	bool retour = false;
	//Si le setup est un duplicata
	if ((vFlatSetup.at(ligne - 1).numLot == sVarEqp.mesRayPro->GetMesSetup(ligne).numLot &&
		vFlatSetup.at(ligne - 1).numOF == sVarEqp.mesRayPro->GetMesSetup(ligne).numOF &&
		vFlatSetup.at(ligne - 1).refPieces == sVarEqp.mesRayPro->GetMesSetup(ligne).refPieces &&
		vFlatSetup.at(ligne - 1).provenance == sVarEqp.mesRayPro->GetMesSetup(ligne).provenance )
		||
		(vFlatSetup.at(ligne + 1).numLot == sVarEqp.mesRayPro->GetMesSetup(ligne).numLot &&
		vFlatSetup.at(ligne + 1).numOF == sVarEqp.mesRayPro->GetMesSetup(ligne).numOF &&
		vFlatSetup.at(ligne + 1).refPieces == sVarEqp.mesRayPro->GetMesSetup(ligne).refPieces &&
		vFlatSetup.at(ligne + 1).provenance == sVarEqp.mesRayPro->GetMesSetup(ligne).provenance)) {
		
		retour = true;
	}
	return retour;
}

void Sequence::ComDummySetup(int ligne)
{
	int cpt = 0;
	int end = 0;

	if (ligne == 1) {
		mutComDummySetup1.lock();
		LOG_DEBUG << "(L" << ligne << ") [Dummy Setup]Mutex mutComDummySetup1 lock pour séquence COM";
	}
	else {
		mutComDummySetup2.lock();
		LOG_DEBUG << "(L" << ligne << ") [Dummy Setup]Mutex mutComDummySetup2 lock pour séquence COM";
	}

	while (end == 0) {
		//LOG_DEBUG << "cpt=" << cpt << "";
		cpt++;
		try {
			switch (this->vNumSequComDummySetup.at(ligne - 1)) {
			case COM_SETUP_ETAT_INITIAL:
				//Attente mesSync = 0
				if ((sVarEqp.mesRayPro->GetMesSyncSetup(ligne) > 0)) {
					LOG_DEBUG << "[Dummy Setup] : COM_SETUP_ETAT_INITIAL ->COM_SETUP_LECTURE_SETUP";
					//Ack de la spv
					sVarEqp.mesRayPro->SetspvSyncSetup(ligne, -(sVarEqp.mesRayPro->GetMesSyncSetup(ligne)));
					vNumSequComDummySetup.at(ligne - 1) = COM_SETUP_LECTURE_SETUP;
				}
				break;
			case COM_SETUP_LECTURE_SETUP:
				//lire le setup
				LOG_DEBUG << "[Dummy Setup] : COM_SETUP_LECTURE_SETUP -> COM_SETUP_RAZ_SEQUENCE";
				sVarEqp.mesRayPro->SetspvSyncSetup(ligne, -(sVarEqp.mesRayPro->GetspvSyncSetup(ligne)));
				vNumSequComDummySetup.at(ligne - 1) = COM_SETUP_ATTENTE_RAZ_MES;
				
				break;
			case COM_SETUP_ATTENTE_RAZ_MES:
				if (sVarEqp.mesRayPro->GetMesSyncSetup(ligne) == 0) {
					sVarEqp.mesRayPro->SetspvSyncSetup(ligne, 0);
					LOG_DEBUG << "[Dummy Setup] : COM_SETUP_RAZ_SEQUENCE -> COM_SETUP_ETAT_INITIAL";
					vNumSequComDummySetup.at(ligne - 1) = COM_SETUP_ETAT_INITIAL;
					end = 1;
					//Finir le thread
					if (ligne == 1) {
						mutComDummySetup1.unlock();
					}
					else {
						mutComDummySetup2.unlock();
					}
				}
				break;

			default:

				break;
			}

			//Sleep entre scrutations
			boost::this_thread::sleep(boost::posix_time::milliseconds(m_delay));
			LOG_DEBUG << "(L" << ligne << ") [Dummy Setup] Tour de cycle ";

			if (cpt >= 300) {
				//Finir le thread
				LOG_DEBUG << "(L" << ligne << ") [Dummy Setup]Thread HU termine car timeout delai";
				sVarEqp.mesRayPro->SetspvSyncHU(ligne, 0);
				vNumSequComDummySetup.at(ligne - 1) = COM_HU_ETAT_INITIAL;
				end = 1;
				if (ligne == 1) {
					mutComDummySetup1.unlock();
					LOG_DEBUG << "(L" << ligne << ") [Dummy Setup]Mutex mutComDummySetup1 unlock pour séquence (timeout)";
				}
				else {
					mutComDummySetup2.unlock();
					LOG_DEBUG << "(L" << ligne << ") [Dummy Setup]Mutex mutComDummySetup2 unlock pour séquence (timeout)";
				}
				break;
			}
		}
		catch (boost::thread_interrupted&) {
			LOG_DEBUG << "(L" << ligne << ") [Dummy Setup]Thread interrompu";
			//Thread interrupted
			return;
		}

	}
}

bool Sequence::GetOFHumidification() {
	return vFlatSetup.at(0).provenance;
}

// Definit les equipements Distant
bool Sequence::GetVectorModeDistant(int ligne) { 
	/*
	Ajout 04/12/19
	Modif 10/12/19
	Modif 11/12/19
	*/
	bool result = true;
	unsigned int i;

	for (i = 0; i < vConfigCampagne[ligne - 1].size(); i++) {
		if (vConfigCampagne[ligne - 1].at(i)->GetModeDistant()) { //Si en mode distant
			vModeDistant[ligne - 1].push_back(vConfigCampagne[ligne - 1].at(i));
		}
	}
	return result;
}

//Créer fichier Import vModeDistant campagne en cours
void Sequence::CreateFichierVModeDistant(int ligne, std::vector<Equipement*> vec) {
	/*
	Ajout 04/12/19
	*/
	//Ecrire dans le fichier
	std::ofstream myfile;
	std::string filename = "Campagne/L";
	filename += std::to_string(ligne);
	filename += "/vmodedistant.txt";
	myfile.open(filename, std::ofstream::out | std::ofstream::trunc);
	for (unsigned short i = 0; i < vec.size(); i++) {
		if (vec.at(i) == sVarEqp.APIrobotCaisses) {
			myfile
				<< "APIrobotCaisses;"
				;
		}
		if (vec.at(i) == sVarEqp.APIrobotCouvercle) {
			myfile
				<< "APIrobotCouvercle;"
				;
		}
		if (vec.at(i) == sVarEqp.APIensacheuseL1) {
			myfile
				<< "APIensacheuseL1;"
				;
		}
		if (vec.at(i) == sVarEqp.APIensacheuseL2) {
			myfile
				<< "APIensacheuseL2;"
				;
		}
		if (vec.at(i) == sVarEqp.APIpeseuseL1) {
			myfile
				<< "APIpeseuseL1;"
				;
		}
		if (vec.at(i) == sVarEqp.APIpeseuseL2) {
			myfile
				<< "APIpeseuseL2;"
				;
		}
		if (vec.at(i) == sVarEqp.APItransitique) {
			myfile
				<< "APItransitique;"
				;
		}
		if (vec.at(i) == sVarEqp.APIpalettiseur) {
			myfile
				<< "APIpalettiseur;"
				;
		}
		if (vec.at(i) == sVarEqp.APIetiqueteuse) {
			myfile
				<< "APIetiqueteuse;"
				;
		}
		if (vec.at(i) == sVarEqp.APIimprimante) {
			myfile
				<< "APIimprimante;"
				;
		}
		if (vec.at(i) == sVarEqp.APIfermeuse) {
			myfile
				<< "APIfermeuse;"
				;
		}
		if (vec.at(i) == sVarEqp.APIformeuse1) {
			myfile
				<< "APIformeuse1;"
				;
		}
		if (vec.at(i) == sVarEqp.APIformeuse2) {
			myfile
				<< "APIformeuse2;"
				;
		}
		if (vec.at(i) == sVarEqp.Tremie) {
			myfile
				<< "Tremie;"
				;
		}
	}
	myfile.close();
	}

//Import le fichier Import vModeDistant
void Sequence::ImportFichierVModeDistant(int ligne) {
	/*
	Ajout 04/12/19
	*/
	//Ecrire dans le fichier
	std::ifstream myfile;
	std::string filename = "Campagne/L";
	filename += std::to_string(ligne);
	filename += "/vmodedistant.txt";
	myfile.open(filename);
	std::vector<std::string> strs;
	sFlatSetup tempsetup;

	for (std::string line; std::getline(myfile, line); )
	{
		boost::split(strs, line, boost::is_any_of(";"));
		for (unsigned short i = 0; i < strs.size(); i++) {
			if (strs.at(i) == "APIrobotCaisses") { vModeDistant->push_back(sVarEqp.APIrobotCaisses); }
			if (strs.at(i) == "APIrobotCouvercle") { vModeDistant->push_back(sVarEqp.APIrobotCouvercle); }
			if (strs.at(i) == "APIensacheuseL1") { vModeDistant->push_back(sVarEqp.APIensacheuseL1); }
			if (strs.at(i) == "APIensacheuseL2") { vModeDistant->push_back(sVarEqp.APIensacheuseL2); }
			if (strs.at(i) == "APIpeseuseL1") { vModeDistant->push_back(sVarEqp.APIpeseuseL1); }
			if (strs.at(i) == "APIpeseuseL2") { vModeDistant->push_back(sVarEqp.APIpeseuseL2); }
			if (strs.at(i) == "APItransitique") { vModeDistant->push_back(sVarEqp.APItransitique); }
			if (strs.at(i) == "APIpalettiseur") { vModeDistant->push_back(sVarEqp.APIpalettiseur); }
			if (strs.at(i) == "APIetiqueteuse") { vModeDistant->push_back(sVarEqp.APIetiqueteuse); }
			if (strs.at(i) == "APIimprimante") { vModeDistant->push_back(sVarEqp.APIimprimante); }
			if (strs.at(i) == "APIfermeuse") { vModeDistant->push_back(sVarEqp.APIfermeuse); }
			if (strs.at(i) == "APIformeuse1") { vModeDistant->push_back(sVarEqp.APIformeuse1); }
			if (strs.at(i) == "APIformeuse2") { vModeDistant->push_back(sVarEqp.APIformeuse2); }
			if (strs.at(i) == "Tremie") { vModeDistant->push_back(sVarEqp.Tremie); }
		}
	}
	LOG_INFO << "Fichier " << filename << " importé";
	myfile.close();
	}

bool Sequence::CI_ModeDistant(int ligne) {
	/*
	Ajout 04/12/19
	modif 11/12/19
	*/
	bool result = true;

	//Reset Machine sortie campagne
	sVarEqp.APIbalance->SetMachineSortie(ligne, false);
	sVarEqp.APIensacheuseL1->SetMachineSortie(ligne, false);
	sVarEqp.APIensacheuseL2->SetMachineSortie(ligne, false);
	sVarEqp.APIetiqueteuse->SetMachineSortie(ligne, false);
	sVarEqp.APIfermeuse->SetMachineSortie(ligne, false);
	sVarEqp.APIformeuse1->SetMachineSortie(ligne, false);
	sVarEqp.APIformeuse2->SetMachineSortie(ligne, false);
	sVarEqp.APIimprimante->SetMachineSortie(ligne, false);
	sVarEqp.APIpalettiseur->SetMachineSortie(ligne, false);
	sVarEqp.APIpeseuseL1->SetMachineSortie(ligne, false);
	sVarEqp.APIpeseuseL2->SetMachineSortie(ligne, false);
	sVarEqp.APIrobotCaisses->SetMachineSortie(ligne, false);
	sVarEqp.APIrobotCouvercle->SetMachineSortie(ligne, false);
	sVarEqp.APItransitique->SetMachineSortie(ligne, false);

	vModeDistant[ligne - 1].clear();
	
	return result;
	}

//Sortir les equipements qui passe en local
bool Sequence::ScrutationModeDistant(int ligne) {
	/*
	Ajout 10/12/19
	*/
	bool result = true;
	unsigned int i;
	
	for (i = 0; i < vModeDistant[ligne - 1].size(); i++) {
		if (vModeDistant[ligne - 1].at(i)->GetModeDistant()) { //Si toujour distant mode distant
			vModeDistant_Temp[ligne - 1].push_back(vModeDistant[ligne - 1].at(i));
		}
		else {
			vModeDistant[ligne - 1].at(i)->SetMachineSortie(ligne, true);
			vModeDistant[ligne - 1].at(i)->EnvoiDmeArret(ligne);
			vModeDistant[ligne - 1].at(i)->ResetDemandeConfiguration(ligne);
			vModeDistant[ligne - 1].at(i)->ResetDemandeProprete(ligne);
			vModeDistant[ligne - 1].at(i)->ResetDemandeVidange(ligne);
		}
	}
	//RAZ vModeDistant
	vModeDistant[ligne - 1].clear();

	//Retranscription
	for (i = 0; i < vModeDistant_Temp[ligne - 1].size(); i++) {
		vModeDistant[ligne - 1].push_back(vModeDistant_Temp[ligne - 1].at(i));
	}
	//Création fichier
	CreateFichierVModeDistant(ligne, vModeDistant[ligne - 1]);
	//RAZ vModeDistant_Temp
	vModeDistant_Temp[ligne - 1].clear();

	return result;
}

bool Sequence::VerificationModeDistant(int ligne) {
	/*
	Ajout 10/12/19
	*/
	bool result = true;
	unsigned int i;

	if (/*(vNumEtape.at(ligne - 1)->GetVarValue() >= PROD_CONFIG_GENERALE)
		&&*/ (vNumEtape.at(ligne - 1)->GetVarValue() <= PROD_ATTENTE_VALIDATION)) {
		for (i = 0; i < vScrutation[ligne - 1].size(); i++) {
			if (vScrutation[ligne - 1].at(i)->GetModeDistant() != vScrutation[ligne - 1].at(i)->GetModeDistant_OLD()) {
				vScrutation[ligne - 1].at(i)->SetModeDistant_OLD(vScrutation[ligne - 1].at(i)->GetModeDistant());
				

				//Si une machine change d'etat distant
				LOG_INFO << "Mode distant différent";
				if ((sVarEqp.APIbalance->GetValidTarre(ligne)) || (sVarEqp.spvWAT->GetChoixProvenance(ligne))) {
					vScrutation[ligne - 1].at(i)->EnvoiDmeArret(ligne);
					vScrutation[ligne - 1].at(i)->ResetDemandeConfiguration(ligne);
					vScrutation[ligne - 1].at(i)->ResetDemandeProprete(ligne);
					vScrutation[ligne - 1].at(i)->ResetDemandeVidange(ligne);
					vNumEtape.at(ligne - 1)->WriteVar(PROD_CONFIG_GENERALE);
					CreateFichierEtape(ligne, PROD_CONFIG_GENERALE);
					LOG_INFO << "Retour en config géneral";
				}
				else {
					vScrutation[ligne - 1].at(i)->EnvoiDmeArret(ligne);
					vScrutation[ligne - 1].at(i)->ResetDemandeConfiguration(ligne);
					vScrutation[ligne - 1].at(i)->ResetDemandeProprete(ligne);
					vScrutation[ligne - 1].at(i)->ResetDemandeVidange(ligne);
					vNumEtape.at(ligne - 1)->WriteVar(PROD_PROPRETE_ROBOT);
					CreateFichierEtape(ligne, PROD_PROPRETE_ROBOT);
					LOG_INFO << "Retour en proprete Robot Bacs Bleus";
					
				}
				break;
			}
		}
	}
	return result;
}

// Definit les equipements à scrut
bool Sequence::GetVectorScrutation(int ligne) {
	/*
	Ajout 11/12/19
	*/
	bool result = true;
	unsigned int i;

	for (i = 0; i < vConfigCampagne[ligne - 1].size(); i++) {
		vScrutation[ligne - 1].push_back(vConfigCampagne[ligne - 1].at(i));
	}

	//Création fichier
	CreateFichierVModeDistant(ligne, vScrutation[ligne - 1]);
	return result;
}

//Créer fichier Import vModeDistant campagne en cours
void Sequence::CreateFichierVScrutation(int ligne, std::vector<Equipement*> vec) {
	/*
	Ajout 11/12/19
	*/
	//Ecrire dans le fichier
	std::ofstream myfile;
	std::string filename = "Campagne/L";
	filename += std::to_string(ligne);
	filename += "/vscrutation.txt";
	myfile.open(filename, std::ofstream::out | std::ofstream::trunc);
	for (unsigned short i = 0; i < vec.size(); i++) {
		if (vec.at(i) == sVarEqp.APIrobotCaisses) {
			myfile
				<< "APIrobotCaisses;"
				;
		}
		if (vec.at(i) == sVarEqp.APIrobotCouvercle) {
			myfile
				<< "APIrobotCouvercle;"
				;
		}
		if (vec.at(i) == sVarEqp.APIensacheuseL1) {
			myfile
				<< "APIensacheuseL1;"
				;
		}
		if (vec.at(i) == sVarEqp.APIensacheuseL2) {
			myfile
				<< "APIensacheuseL2;"
				;
		}
		if (vec.at(i) == sVarEqp.APIpeseuseL1) {
			myfile
				<< "APIpeseuseL1;"
				;
		}
		if (vec.at(i) == sVarEqp.APIpeseuseL2) {
			myfile
				<< "APIpeseuseL2;"
				;
		}
		if (vec.at(i) == sVarEqp.APItransitique) {
			myfile
				<< "APItransitique;"
				;
		}
		if (vec.at(i) == sVarEqp.APIpalettiseur) {
			myfile
				<< "APIpalettiseur;"
				;
		}
		if (vec.at(i) == sVarEqp.APIetiqueteuse) {
			myfile
				<< "APIetiqueteuse;"
				;
		}
		if (vec.at(i) == sVarEqp.APIimprimante) {
			myfile
				<< "APIimprimante;"
				;
		}
		if (vec.at(i) == sVarEqp.APIfermeuse) {
			myfile
				<< "APIfermeuse;"
				;
		}
		if (vec.at(i) == sVarEqp.APIformeuse1) {
			myfile
				<< "APIformeuse1;"
				;
		}
		if (vec.at(i) == sVarEqp.APIformeuse2) {
			myfile
				<< "APIformeuse2;"
				;
		}
		if (vec.at(i) == sVarEqp.Tremie) {
			myfile
				<< "Tremie;"
				;
		}
	}
	LOG_INFO << "Fichier " << filename << " modifié";
	myfile.close();
}

//Import le fichier Import vModeDistant
void Sequence::ImportFichierVScrutation(int ligne) {
	/*
	Ajout 11/12/19
	*/
	//Ecrire dans le fichier
	std::ifstream myfile;
	std::string filename = "Campagne/L";
	filename += std::to_string(ligne);
	filename += "/vScrutation.txt";
	myfile.open(filename);
	std::vector<std::string> strs;
	sFlatSetup tempsetup;

	for (std::string line; std::getline(myfile, line); )
	{
		boost::split(strs, line, boost::is_any_of(";"));
		for (unsigned short i = 0; i < strs.size(); i++) {
			if (strs.at(i) == "APIrobotCaisses") { vScrutation->push_back(sVarEqp.APIrobotCaisses); }
			if (strs.at(i) == "APIrobotCouvercle") { vScrutation->push_back(sVarEqp.APIrobotCouvercle); }
			if (strs.at(i) == "APIensacheuseL1") { vScrutation->push_back(sVarEqp.APIensacheuseL1); }
			if (strs.at(i) == "APIensacheuseL2") { vScrutation->push_back(sVarEqp.APIensacheuseL2); }
			if (strs.at(i) == "APIpeseuseL1") { vScrutation->push_back(sVarEqp.APIpeseuseL1); }
			if (strs.at(i) == "APIpeseuseL2") { vScrutation->push_back(sVarEqp.APIpeseuseL2); }
			if (strs.at(i) == "APItransitique") { vScrutation->push_back(sVarEqp.APItransitique); }
			if (strs.at(i) == "APIpalettiseur") { vScrutation->push_back(sVarEqp.APIpalettiseur); }
			if (strs.at(i) == "APIetiqueteuse") { vScrutation->push_back(sVarEqp.APIetiqueteuse); }
			if (strs.at(i) == "APIimprimante") { vScrutation->push_back(sVarEqp.APIimprimante); }
			if (strs.at(i) == "APIfermeuse") { vScrutation->push_back(sVarEqp.APIfermeuse); }
			if (strs.at(i) == "APIformeuse1") { vScrutation->push_back(sVarEqp.APIformeuse1); }
			if (strs.at(i) == "APIformeuse2") { vScrutation->push_back(sVarEqp.APIformeuse2); }
			if (strs.at(i) == "Tremie") { vScrutation->push_back(sVarEqp.Tremie); }
		}
	}
	LOG_INFO << "Fichier " << filename << " importé";
	myfile.close();
}

//Initialisation des demandes
bool Sequence::InitDemande(int ligne) {
	/*
	Ajout 26/125/19
	Modif 03/02/20
	*/
	bool retour = true;
	
	//Tare - Validation poids Piece
	sVarEqp.APIbalance->ResetValidTarre(ligne);
	sVarEqp.APIbalance->ResetDerogationTarre(ligne);

	//Colis - Validation colis
	sVarEqp.APIbalance->ResetValidColis(ligne);
	sVarEqp.APIbalance->ResetDerogationColis(ligne);

	//Rompus - Validation piece rompu
	sVarEqp.APIbalance->ResetFinRompus(ligne);
	sVarEqp.spvWAT->SetRompuEnCours(ligne, false);

	//Etiqueteuse
	sVarEqp.APIetiqueteuse->EnvoiDmeArret(ligne);
	sVarEqp.APIetiqueteuse->ResetDemandeConfiguration(ligne);
	sVarEqp.APIetiqueteuse->ResetDemandeProprete(ligne);
	sVarEqp.APIetiqueteuse->ResetDemandeVidange(ligne);

	//Fermeuse
	sVarEqp.APIfermeuse->EnvoiDmeArret(ligne);
	sVarEqp.APIfermeuse->ResetDemandeConfiguration(ligne);
	sVarEqp.APIfermeuse->ResetDemandeProprete(ligne);
	sVarEqp.APIfermeuse->ResetDemandeVidange(ligne);

	//Imprimante
	sVarEqp.APIimprimante->EnvoiDmeArret(ligne);
	sVarEqp.APIimprimante->ResetDemandeConfiguration(ligne);
	sVarEqp.APIimprimante->ResetDemandeProprete(ligne);
	sVarEqp.APIimprimante->ResetDemandeVidange(ligne);

	//Tremie
	sVarEqp.Tremie->EnvoiDmeArret(ligne);

	//RobotBacBleu
	sVarEqp.APIrobotCaisses->EnvoiDmeArret(ligne);
	sVarEqp.APIrobotCaisses->ResetDemandeConfiguration(ligne);
	sVarEqp.APIrobotCaisses->ResetDemandeProprete(ligne);
	sVarEqp.APIrobotCaisses->ResetDemandeVidange(ligne);

	//RobotCouvercle
	sVarEqp.APIrobotCouvercle->EnvoiDmeArret(ligne);
	sVarEqp.APIrobotCouvercle->ResetDemandeConfiguration(ligne);
	sVarEqp.APIrobotCouvercle->ResetDemandeProprete(ligne);
	sVarEqp.APIrobotCouvercle->ResetDemandeVidange(ligne);

	//RobotEtiquette
	sVarEqp.APIetiqueteuse->EnvoiDmeArret(ligne);
	sVarEqp.APIetiqueteuse->ResetDemandeConfiguration(ligne);
	sVarEqp.APIetiqueteuse->ResetDemandeProprete(ligne);
	sVarEqp.APIetiqueteuse->ResetDemandeVidange(ligne);

	//RobotPalettisation
	sVarEqp.APIpalettiseur->EnvoiDmeArret(ligne);
	sVarEqp.APIpalettiseur->ResetDemandeConfiguration(ligne);
	sVarEqp.APIpalettiseur->ResetDemandeProprete(ligne);
	sVarEqp.APIpalettiseur->ResetDemandeVidange(ligne);
	//NB pieces sorties
	sVarEqp.mesRayPro->SetGoodPartQty(ligne, 0);

	//Transitique
	sVarEqp.APItransitique->EnvoiDmeArret(ligne);
	sVarEqp.APItransitique->ResetDemandeConfiguration(ligne);
	sVarEqp.APItransitique->ResetDemandeProprete(ligne);
	sVarEqp.APItransitique->ResetDemandeVidange(ligne);
	sVarEqp.APItransitique->SetLigneFormeuse(0, ligne);

	if (ligne == 1) {
		//Ensacheuse L1
		sVarEqp.APIensacheuseL1->EnvoiDmeArret(ligne);
		sVarEqp.APIensacheuseL1->ResetDemandeConfiguration(ligne);
		sVarEqp.APIensacheuseL1->ResetDemandeProprete(ligne);
		sVarEqp.APIensacheuseL1->ResetDemandeVidange(ligne);

		//Peseuse L1
		sVarEqp.APIpeseuseL1->EnvoiDmeArret(ligne);
		sVarEqp.APIpeseuseL1->ResetDemandeConfiguration(ligne);
		sVarEqp.APIpeseuseL1->ResetDemandeProprete(ligne);
		sVarEqp.APIpeseuseL1->ResetDemandeVidange(ligne);
		
	}
	else {
		//Ensacheuse L2
		sVarEqp.APIensacheuseL2->EnvoiDmeArret(ligne);
		sVarEqp.APIensacheuseL2->ResetDemandeConfiguration(ligne);
		sVarEqp.APIensacheuseL2->ResetDemandeProprete(ligne);
		sVarEqp.APIensacheuseL2->ResetDemandeVidange(ligne);

		//Peseuse L2
		sVarEqp.APIpeseuseL2->EnvoiDmeArret(ligne);
		sVarEqp.APIpeseuseL2->ResetDemandeConfiguration(ligne);
		sVarEqp.APIpeseuseL2->ResetDemandeProprete(ligne);
		sVarEqp.APIpeseuseL2->ResetDemandeVidange(ligne);
	}

	sVarEqp.spvWAT->SetNbrPieceValide(ligne, false);
	sVarEqp.spvWAT->SetBitNbrPieceValide(ligne, false);

	//Cellule
	/*
	@Machine.Etiqueteuse.Marche.Demande = @Machine.Etiqueteuse.Ligne1.Production.Demande || @Machine.Etiqueteuse.Ligne2.Production.Demande;
	@Machine.Fermeuse.Marche.Demande = @Machine.Fermeuse.Ligne1.Production.Demande || @Machine.Fermeuse.Ligne2.Production.Demande;
	@Machine.Imprimante.Marche.Demande = @Machine.Imprimante.Ligne1.Production.Demande || @Machine.Imprimante.Ligne2.Production.Demande;
	@Machine.RobotBacBleu.Marche.Demande = @Machine.RobotBacBleu.Ligne1.Production.Demande || @Machine.RobotBacBleu.Ligne2.Production.Demande;
	@Machine.RobotEtiquette.Marche.Demande = @Machine.RobotEtiquette.Ligne1.Production.Demande || @Machine.RobotEtiquette.Ligne2.Production.Demande;
	@Machine.RobotPalettisation.Marche.Demande = @Machine.RobotPalettisation.Ligne1.Production.Demande || @Machine.RobotPalettisation.Ligne2.Production.Demande;
	@Machine.Transitique.Marche.Demande = @Machine.Transitique.Ligne1.Production.Demande || @Machine.Transitique.Ligne2.Production.Demande;
	*/
	return retour;
}

//Sortir de la formeuse quand passage en intoduction manuelle
bool Sequence::SortieFormeuseProprete(int ligne) {
	/*
	Ajout 06/02/2020
	*/
	bool result = true;
	unsigned int i;

	for (i = 0; i < vPropreteCampagne[ligne - 1].size(); i++) {
		if ((vPropreteCampagne[ligne - 1].at(i) != sVarEqp.APIformeuse1) && (vPropreteCampagne[ligne - 1].at(i) != sVarEqp.APIformeuse2)) {
			vPropreteCampagne_Temp[ligne - 1].push_back(vPropreteCampagne[ligne - 1].at(i));
		}
		else {
			vPropreteCampagne[ligne - 1].at(i)->SetMachineSortie(ligne, true);
			vPropreteCampagne[ligne - 1].at(i)->EnvoiDmeArret(ligne);
			vPropreteCampagne[ligne - 1].at(i)->ResetDemandeConfiguration(ligne);
			vPropreteCampagne[ligne - 1].at(i)->ResetDemandeProprete(ligne);
			vPropreteCampagne[ligne - 1].at(i)->ResetDemandeVidange(ligne);
		}
	}
	//RAZ vModeDistant
	vPropreteCampagne[ligne - 1].clear();

	//Retranscription
	for (i = 0; i < vPropreteCampagne_Temp[ligne - 1].size(); i++) {
		vPropreteCampagne[ligne - 1].push_back(vPropreteCampagne_Temp[ligne - 1].at(i));
	}
	//Création fichier
	CreateFichierVProprete(ligne, vPropreteCampagne[ligne - 1]);
	//RAZ vModeDistant_Temp
	vPropreteCampagne_Temp[ligne - 1].clear();

	return result;
}

//Sortir de la formeuse quand passage en intoduction manuelle
bool Sequence::SortieFormeuseConfiguration(int ligne) {
	/*
	Ajout 06/02/2020
	*/
	bool result = true;
	unsigned int i;

	for (i = 0; i < vConfigCampagne[ligne - 1].size(); i++) {
		if ((vConfigCampagne[ligne - 1].at(i) != sVarEqp.APIformeuse1) && (vConfigCampagne[ligne - 1].at(i) != sVarEqp.APIformeuse2)) {
			vConfigCampagne_Temp[ligne - 1].push_back(vConfigCampagne[ligne - 1].at(i));
		}
		else {
			vConfigCampagne[ligne - 1].at(i)->SetMachineSortie(ligne, true);
			vConfigCampagne[ligne - 1].at(i)->EnvoiDmeArret(ligne);
			vConfigCampagne[ligne - 1].at(i)->ResetDemandeConfiguration(ligne);
			vConfigCampagne[ligne - 1].at(i)->ResetDemandeProprete(ligne);
			vConfigCampagne[ligne - 1].at(i)->ResetDemandeVidange(ligne);
		}
	}
	//RAZ vModeDistant
	vConfigCampagne[ligne - 1].clear();

	//Retranscription
	for (i = 0; i < vConfigCampagne_Temp[ligne - 1].size(); i++) {
		vConfigCampagne[ligne - 1].push_back(vConfigCampagne_Temp[ligne - 1].at(i));
	}
	//Création fichier
	CreateFichierVConfig(ligne, vConfigCampagne[ligne - 1]);
	//RAZ vModeDistant_Temp
	vConfigCampagne_Temp[ligne - 1].clear();

	return result;
}

//Sortir de la formeuse quand passage en intoduction manuelle
bool Sequence::SortieFormeuseVidange(int ligne) {
	/*
	Ajout 06/02/2020
	*/	
	bool result = true;
	unsigned int i;

	for (i = 0; i < vModeDistant[ligne - 1].size(); i++) {
		if ((vModeDistant[ligne - 1].at(i) != sVarEqp.APIformeuse1) && (vModeDistant[ligne - 1].at(i) != sVarEqp.APIformeuse2)) {
			vModeDistant_Temp[ligne - 1].push_back(vModeDistant[ligne - 1].at(i));
		}
		else {
			vModeDistant[ligne - 1].at(i)->SetMachineSortie(ligne, true);
			vModeDistant[ligne - 1].at(i)->EnvoiDmeArret(ligne);
			vModeDistant[ligne - 1].at(i)->ResetDemandeConfiguration(ligne);
			vModeDistant[ligne - 1].at(i)->ResetDemandeProprete(ligne);
			vModeDistant[ligne - 1].at(i)->ResetDemandeVidange(ligne);
		}
	}
	//RAZ vModeDistant
	vModeDistant[ligne - 1].clear();

	//Retranscription
	for (i = 0; i < vModeDistant_Temp[ligne - 1].size(); i++) {
		vModeDistant[ligne - 1].push_back(vModeDistant_Temp[ligne - 1].at(i));
	}
	//Création fichier
	CreateFichierVModeDistant(ligne, vModeDistant[ligne - 1]);
	//RAZ vModeDistant_Temp
	vModeDistant_Temp[ligne - 1].clear();

	return result;
}

//Définir la liste des équipements qui ont besoin d'être configurés
bool Sequence::GetBesoinFormeuse(int ligne, int iPosition) {
	/*
	Ajout 07/02/2020
	Modif 19/03/2020
	*/
	bool Retour = false;

	//Si pas caisses gefbox, mettre formeuse
	if (vConditionnement_Type.at(iPosition) == "Carton") {
		
		Retour = true;
	}
	//Si gefbox
	else if (vConditionnement_Type.at(iPosition) == "GefBox") {
		Retour = false;
	}
	return Retour;
}

void Sequence::ImportFichierConditionnement() {
	/*
	Ajout 19/03/2020
	*/
	int retour = 0;
	std::ifstream myfile;
	std::string filename = "Campagne/RefConditionnement.txt"; // Chemin du fichier
	myfile.open(filename); // Ouverture du fichier
	std::vector<std::string> strs;
	std::vector<std::string> strs2;

	//Numero 0 vide, a utilisé si un défaut
	vConditionnement_Ref.push_back("");
	vConditionnement_Type.push_back("");
	vConditionnement_Coiffe.push_back("");

	for (std::string line; std::getline(myfile, line); )
	{
		boost::split(strs, line, boost::is_any_of(";"));
		
		for (unsigned short a = 0; a < (strs.size()-1); a++) {
			
			boost::split(strs2, strs.at(a), boost::is_any_of(","));
	
			for (unsigned short i = 0; i < strs2.size(); i++) {
								
				if (i == 0) {
					vConditionnement_Ref.push_back(strs2.at(i));
				}
				else if (i == 1) {
					vConditionnement_Type.push_back(strs2.at(i));
				}
				else if (i == 2) {
					vConditionnement_Coiffe.push_back(strs2.at(i));
				}
			}
		}
	}

	LOG_INFO << "Fichier " << filename << " importé";
	myfile.close(); // Fermeture du fichier
}

//Définir la liste des équipements qui ont besoin d'être configurés
bool Sequence::GetVectorConfig(int ligne, int iPosition) {
	/*
	Ajout 19/03/2020
	*/

	LOG_TRACE << "***** Liste des machines à configurer pour l'OF L" << ligne << " *****";

	//Machine cummune
	vConfigCampagne[ligne - 1].push_back((sVarEqp.APItransitique));
	sVarEqp.APItransitique->SetCampagne(ligne, true);
	LOG_TRACE << "Transitique L" << ligne;

	vConfigCampagne[ligne - 1].push_back((sVarEqp.APIpalettiseur));
	sVarEqp.APIpalettiseur->SetCampagne(ligne, true);
	LOG_TRACE << "Palettisation L" << ligne;

	if (ligne == 1) {
		//Ligne 1, avec sachet
		if (vFlatSetup.at(ligne - 1).nbPiecesPSachet != 0)
		{
			vConfigCampagne[ligne - 1].push_back(sVarEqp.APIensacheuseL1);
			sVarEqp.APIensacheuseL1->SetCampagne(ligne, true);
			LOG_TRACE << "Ensacheuse L1";
		}
		vConfigCampagne[ligne - 1].push_back(sVarEqp.APIpeseuseL1);
		sVarEqp.APIpeseuseL1->SetCampagne(ligne, true);
		LOG_TRACE << "Peseuse L1";
	}
	else {
		//Ligne 2, avec sachet
		if (vFlatSetup.at(ligne - 1).nbPiecesPSachet != 0) {
			vConfigCampagne[ligne - 1].push_back(sVarEqp.APIensacheuseL2);
			sVarEqp.APIensacheuseL2->SetCampagne(ligne, true);
			LOG_TRACE << "Ensacheuse L2";
		}
		vConfigCampagne[ligne - 1].push_back(sVarEqp.APIpeseuseL2);
		sVarEqp.APIpeseuseL2->SetCampagne(ligne, true);
		LOG_TRACE << "Peseuse L2";
	}

	//Si pas carton plat - Coiffe
	if (vConditionnement_Coiffe.at(iPosition) == "Coiffe") {
		vConfigCampagne[ligne - 1].push_back((sVarEqp.APIrobotCouvercle));
		sVarEqp.APIrobotCouvercle->SetCampagne(ligne, true);
		LOG_TRACE << "Couvercle L" << ligne;
	}
	else if (vConditionnement_Coiffe.at(iPosition) == "Sans Coiffe") {
		vConfigCampagne[ligne - 1].push_back((sVarEqp.APIfermeuse));
		sVarEqp.APIfermeuse->SetCampagne(ligne, true);
		LOG_TRACE << "Fermeuse L" << ligne;
	}
	
	//Si pas caisses gefbox, mettre formeuse ET IMPRIMANTE  - Carton
	if (vConditionnement_Type.at(iPosition) == "Carton") {
		
		vConfigCampagne[ligne - 1].push_back((sVarEqp.APIimprimante));
		sVarEqp.APIimprimante->SetCampagne(ligne, true);
		LOG_TRACE << "Imprimante L" << ligne;

		vConfigCampagne[ligne - 1].push_back((sVarEqp.APIetiqueteuse));
		sVarEqp.APIetiqueteuse->SetCampagne(ligne, true);
		LOG_TRACE << "Etiqueteuse L" << ligne;

		//Gestion formeuse - Affectation en fonction du choix de l'opérateur.
		if (sVarEqp.spvWAT->GetChoixFormeuse(ligne) == 1) {
			vConfigCampagne[ligne - 1].push_back(sVarEqp.APIformeuse1);
			sVarEqp.APItransitique->SetLigneFormeuse(1, ligne);
			sVarEqp.APIformeuse1->SetCampagne(ligne, true);
			LOG_TRACE << "Formeuse 1, choix opérateur";
		}
		else if (sVarEqp.spvWAT->GetChoixFormeuse(ligne) == 2) {
			vConfigCampagne[ligne - 1].push_back(sVarEqp.APIformeuse2);
			sVarEqp.APItransitique->SetLigneFormeuse(2, ligne);
			sVarEqp.APIformeuse2->SetCampagne(ligne, true);
			LOG_TRACE << "Formeuse 2, choix opérateur";
		}
		else if (sVarEqp.spvWAT->GetChoixFormeuse(ligne) == 3) { // Introduction manuel
			sVarEqp.APItransitique->SetLigneFormeuse(3, ligne);
			LOG_TRACE << "Choix opérateur, introduction manuel";
		}
	}
	else if (vConditionnement_Type.at(iPosition) == "GefBox") {
		sVarEqp.APItransitique->SetLigneFormeuse(4, ligne);
	}

	/*
	if ((vFlatSetup.at(ligne - 1).typeConditionnement != "SC4"
		&& vFlatSetup.at(ligne - 1).typeConditionnement != "RC2"
		&& vFlatSetup.at(ligne - 1).typeConditionnement != "RC3"
		&& vFlatSetup.at(ligne - 1).typeConditionnement != "MN6"
		&& vFlatSetup.at(ligne - 1).typeConditionnement != "MN7"
		&& vFlatSetup.at(ligne - 1).typeConditionnement != "MN9"
		&& vFlatSetup.at(ligne - 1).typeConditionnement != "OP1"
		&& vFlatSetup.at(ligne - 1).typeConditionnement != "OP2"
		&& vFlatSetup.at(ligne - 1).typeConditionnement != "OP3"
		&& vFlatSetup.at(ligne - 1).typeConditionnement != "OP4"
		&& vFlatSetup.at(ligne - 1).typeConditionnement != "PC1")) {
		//Imprimante ici car multiligne
		vConfigCampagne[ligne - 1].push_back((sVarEqp.APIimprimante));
		sVarEqp.APIimprimante->EnvoiMarcheCellule();
		LOG_TRACE << "Imprimante";

		vConfigCampagne[ligne - 1].push_back((sVarEqp.APIetiqueteuse));
		sVarEqp.APIetiqueteuse->EnvoiMarcheCellule();
		LOG_TRACE << "Etiqueteuse";


		//Gestion formeuse - Affectation en fonction du choix de l'opérateur.
		if (sVarEqp.spvWAT->GetChoixFormeuse(ligne) == 1) {
			vConfigCampagne[ligne - 1].push_back(sVarEqp.APIformeuse1);
			sVarEqp.APItransitique->SetLigneFormeuse(1, ligne);
			LOG_TRACE << "Formeuse 1, choix opérateur";
		}
		else if (sVarEqp.spvWAT->GetChoixFormeuse(ligne) == 2) {
			vConfigCampagne[ligne - 1].push_back(sVarEqp.APIformeuse2);
			sVarEqp.APItransitique->SetLigneFormeuse(2, ligne);
			LOG_TRACE << "Formeuse 2, choix opérateur";

		}
		else if (sVarEqp.spvWAT->GetChoixFormeuse(ligne) == 3) { // Introduction manuel
			sVarEqp.APItransitique->SetLigneFormeuse(3, ligne);
			LOG_TRACE << "Choix opérateur, introduction manuel";
		}
	}
	//Si gefbox : dire à la transitique qu'on fait pas de carton
	else { sVarEqp.APItransitique->SetLigneFormeuse(0, ligne); }
	*/
	LOG_TRACE << "***** Fin *****";
	return 1;
}

//Définir la liste des équipements qui ont besoin d'être propres
bool Sequence::GetVectorProprete(int ligne, int iPosition) {
	/*
	Ajout 19/03/2020
	*/
	
	LOG_TRACE << "***** Liste des machines à nettoyer pour l'OF L" << ligne << " *****";

	//Machine commune
	vPropreteCampagne[ligne - 1].push_back((sVarEqp.APItransitique));
	vPropreteCampagne[ligne - 1].push_back((sVarEqp.APIpalettiseur));
	LOG_TRACE << "Transitique";
	LOG_TRACE << "Robot palettiseur";

	//Si couvercle - Coiffe
	if (vConditionnement_Coiffe.at(iPosition) == "Coiffe") {
		vPropreteCampagne[ligne - 1].push_back((sVarEqp.APIrobotCouvercle));
		LOG_TRACE << "Robot Couvercle";
	}
	//Si  pas couvercle
	else if (vConditionnement_Coiffe.at(iPosition) == "Sans Coiffe") {
		vPropreteCampagne[ligne - 1].push_back((sVarEqp.APIfermeuse));
		LOG_TRACE << "Fermeuse";
	}

	//Si pas caisses gefbox, mettre formeuse ET IMPRIMANTE
	if (vConditionnement_Type.at(iPosition) == "Carton") {
			
		vPropreteCampagne[ligne - 1].push_back((sVarEqp.APIimprimante));
		vPropreteCampagne[ligne - 1].push_back((sVarEqp.APIetiqueteuse));
		LOG_TRACE << "Imprimante";
		LOG_TRACE << "Etiqueteuse";

		// Gestion Formeuse
		if (sVarEqp.spvWAT->GetChoixFormeuse(ligne) == 1) {
			vPropreteCampagne[ligne - 1].push_back(sVarEqp.APIformeuse1);
			sVarEqp.APItransitique->SetLigneFormeuse(1, ligne);
			LOG_TRACE << "Formeuse 1, choix opérateur";
		}
		else if (sVarEqp.spvWAT->GetChoixFormeuse(ligne) == 2) {
			vPropreteCampagne[ligne - 1].push_back(sVarEqp.APIformeuse2);
			sVarEqp.APItransitique->SetLigneFormeuse(2, ligne);
			LOG_TRACE << "Formeuse 2, choix opérateur";
		}
		else if (sVarEqp.spvWAT->GetChoixFormeuse(ligne) == 3) {
			sVarEqp.APItransitique->SetLigneFormeuse(3, ligne);
			LOG_TRACE << "Choix opérateur, introduction manuel";
		}
			
	}
	
	//Si article différent de la campagne precedande
	if (vFlatSetup.at(ligne + 1).refPieces != vFlatSetup.at(ligne - 1).refPieces) {

		if (ligne == 1) {
			if (vFlatSetup.at(ligne - 1).nbPiecesPSachet != 0)
			{
				vPropreteCampagne[ligne - 1].push_back((sVarEqp.APIensacheuseL1));
				LOG_TRACE << "Ensacheuse 1";
			}
			vPropreteCampagne[ligne - 1].push_back((sVarEqp.APIpeseuseL1));
			LOG_TRACE << "Peseuse 1";
		}
		else {
			if (vFlatSetup.at(ligne - 1).nbPiecesPSachet != 0)
			{
				vPropreteCampagne[ligne - 1].push_back((sVarEqp.APIensacheuseL2));
				LOG_TRACE << "Ensacheuse 2";
			}
			vPropreteCampagne[ligne - 1].push_back((sVarEqp.APIpeseuseL2));
			LOG_TRACE << "Peseuse 2";
		}
	}

	LOG_TRACE << "***** Fin *****";
	return true;
}

int Sequence::DefPositionRefConditionnement(int ligne) {
	/*
	Ajout 19/03/2020
	*/
	int iPosition = 0;

	for (unsigned short i = 0; i < vConditionnement_Ref.size(); i++) {

		if (std::string(vFlatSetup.at(ligne - 1).typeConditionnement) == vConditionnement_Ref.at(i)) {
			return iPosition = i;
		}
	}
	return 0;
}